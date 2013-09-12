#include "WpnBomb.h"

#include <vector>
#include <cfloat>
#include <string>
#include <fstream>
#include <ctime>

#include "Globals.h"
#include "Data.h"
#include "Level.h"
#include "Player.h"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include "rapidxml.hpp"

const int B_MAP_LENGTH=17;	//self
const int B_MAP_HEIGHT=15;	//explanatory

const int BMBDUR=75;		//2.5 seconds	- pre-explosion duration. 
const int BMBEXP=30;		//1 second		- explosion duration.
const int BMBDMG=6;			//damage per tick (30 ticks per second) total of 180 damage

using namespace rapidxml;

//////////////////////////////////////////////////////////////////////
//BOMBS (ID 1)
//
//PRIMARY OFFENSIVE WEAPON AND BLOCK CLEARER
//WHEN PUT, EXPLODES AFTER SEVERAL SECONDS DELAY
//DEALS DAMAGE OVER TIME. CLEARS BLOCKS
//
//WITH MINES: MINES CAN BE PUT BEFORE BOMBS, NOT AFTER. NO TRIGGER BETWEEN THEM
//WITH ROCKET: ROCKETS WILL PREMATURELY TRIGGER BOMBS
//
//INCREASING LEVELS INCREASE BOMB STOCK AND EXPLOSION SIZE
//AT LEVEL 5, BOMBS CAN PIERCE SOFT BLOCKS. LEVEL 5 DOESNT GRANT EXTRA BOMB OR EXPLOSION SIZE
//STOCK		= 1 + (MIN(LEVEL,4))
//EXPLOSION	= 2*(MIN(LEVEL,4))
//////////////////////////////////////////////////////////////////////

Bomb::Bomb(int id,int lvl,int row,int col){
	owner=id;
	level=lvl;
	timer=BMBDUR;
	row_pos=row;
	col_pos=col;
}

Bomb::~Bomb(){

}

bool Bomb::Logic(){
	--timer;
	return(timer<=0);
}
 

BombManager::BombManager(){
	LoadBomb(); 
}

BombManager::~BombManager(){ 

}

void BombManager::Construct(shared_ptr<GlobalManager> Glo){  
	Globals		=	Glo;
	MainWindow	=	Glo->GlobalWindow; 
	for(int i=0; i<5; ++i){
		BombList[i].clear();
	}
	ExplosionList.clear();
	memset(BombMap,false,sizeof BombMap);
	memset(JustExploded,0,sizeof JustExploded);
	memset(FireMap,0,sizeof FireMap);
}
 
void BombManager::PutBomb(int id,int bomblevel,int row,int col){
	int nrow=(row+16)/32,ncol=(col-160+16)/32;
	if (BombMap[nrow][ncol]) return;

	//////printf("put a bomb on %d %d, results in %d %d\n",row,col,nrow,ncol);
	 
	int limit=min(bomblevel,4)+1;  
	if (BombList[id].size()<limit) {
		Globals->GlobalDataManager->UseWeapon(id);
		BombList[id].push_back(Bomb(id,bomblevel,nrow,ncol));
		BombMap[nrow][ncol]=true;
	}
}	 

bool BombManager::IsAvailable(int id,int bomblevel){
	return (BombList[id].size()<min(bomblevel,4)+1);
}

int BombManager::GetAvailability(int id,int bomblevel){ 
	return (min(bomblevel,4)+1-BombList[id].size());
}

void BombManager::BombLogic(){
	memset(JustExploded,false,sizeof JustExploded);
	for(int id=0;id<2;++id){
		int ite=0;
		while (ite<BombList[id].size()){
			std::pair<int,int> now=std::make_pair(BombList[id][ite].row_pos,BombList[id][ite].col_pos);
			if(BombList[id][ite].Logic()){ 
				BombMap[now.first][now.second]=false;
				Globals->GlobalLevel->SoftMap[now.first][now.second]=0;

				int lvl=BombList[id][ite].level;
				BombList[id].erase(BombList[id].begin()+ite);
				Explode(now.first,now.second,0,lvl);
			} else {
				bool collision=true;
				std::pair<int,int> check;
				for(int ply=0;ply<2;++ply){
					check=Globals->GlobalPlayerManager->GetCurPos(ply); if (check==now) collision=false;
					check=Globals->GlobalPlayerManager->GetNexPos(ply); if (check==now) collision=false;
				}
				if (collision) Globals->GlobalLevel->SoftMap[now.first][now.second] = 3;
				++ite;
			}
		}
	}
	//check damage
	for (int id=0;id<2;++id){
		pair<int,int> now=Globals->GlobalPlayerManager->GetPosition(id);
		int rnow=(now.first+16)/32, cnow=(now.second-160+16)/32;
		if (FireMap[rnow][cnow]){
			Globals->GlobalPlayerManager->Damage(id,FireMap[rnow][cnow]);
			Globals->GlobalPlayerManager->Disable(id,15);
		}
	}
	//check fires, destroys blocks
	int ite=0; while (ite<ExplosionList.size()){
		int row=ExplosionList[ite].row, col=ExplosionList[ite].col;
		int time=ExplosionList[ite].timer,spr=ExplosionList[ite].sprite;  
		if (Globals->GlobalLevel->SoftHP[row][col]>0) {
			Globals->GlobalLevel->SoftHP[row][col]=max(0,Globals->GlobalLevel->SoftHP[row][col]-FireMap[row][col]);
			if (Globals->GlobalLevel->SoftHP[row][col]==0) {
				Globals->GlobalLevel->SoftMap[row][col]=0;
				Globals->GlobalDataManager->DestroyBlock();
			}
		}
		--ExplosionList[ite].timer;
		if (ExplosionList[ite].timer==0){
			FireMap[ExplosionList[ite].row][ExplosionList[ite].col]-=ExplosionList[ite].dmg;
			ExplosionList.erase(ExplosionList.begin()+ite);
			//if (ExplosionList.empty()) printf("Explosion Duration: %d milliseconds\n",DEBUGGER_COUNTER.getElapsedTime().asMilliseconds());
		} else ++ite;
	}
}
   
  
void BombManager::Explode(int row,int col,int srcdir,int level){
	//spreads fire starting in [row][col] to all directions instead of srcdir
	//fire distance depends on level
	//srcdir -> 1=up, 2=right, 3=down, 4=left, 0=null(all directions ok)
	//remember fire shapes!
	//from 0..6 respectively: +,|,-,^,>,V,< 
	int drow[5]={0,-1,0,1,0},dcol[5]={0,0,1,0,-1};

	Globals->GlobalDataManager->UpdateWeapon(1,1);

	//if (ExplosionList.empty()) DEBUGGER_COUNTER.restart();
	ExplosionList.push_back(Explosion(BMBEXP,BMBDMG,row,col,0));

	FireMap[row][col]+=BMBDMG;
	JustExploded[row][col]=true;

	for (int dir=1;dir<=4;++dir) if (dir!=srcdir){						//determine direction
		for (int len=1;len<=2*(min(4,level));++len){					//determine length
			int newrow=row+(len*drow[dir]), newcol=col+(len*dcol[dir]); //new positions 
			
			if (newrow<0||newrow>=B_MAP_HEIGHT||newcol<0||newcol>=B_MAP_LENGTH||JustExploded[newrow][newcol]) break;
			int spritenum;
			
			if (BombMap[newrow][newcol]){								//if there's a bomb, explode that one instead
				for(int id=0;id<2;++id) {
					int ite=0;
					while (ite<BombList[id].size()){
						std::pair<int,int> now=std::make_pair(BombList[id][ite].row_pos,BombList[id][ite].col_pos);
						if (now.first==newrow && now.second==newcol){
							BombMap[now.first][now.second]=false;
							Globals->GlobalLevel->SoftMap[now.first][now.second]=0;
							int nexlvl=BombList[id][ite].level;
							BombList[id].erase(BombList[id].begin()+ite);
							Explode(now.first,now.second,1+((dir+1)%4),nexlvl);
						} else ++ite;
					}
				}
				break;
			} else if (Globals->GlobalLevel->SoftMap[newrow][newcol]==1) {			//if there's a soft tile, end explosion
				spritenum=dir+2;	   //^,>,v,< respectively
				FireMap[newrow][newcol]+=BMBDMG;
				ExplosionList.push_back(Explosion(BMBEXP,BMBDMG,newrow,newcol,spritenum));
				break;
			} else if (Globals->GlobalLevel->SoftMap[newrow][newcol]==0) {			//if it's empty, continue explosion
				if (len==2*(min(4,level))) spritenum=dir+2;				//if it's the fire's limit, same as soft tile
				else					   spritenum=1+(dir+1)%2;		//else 1 if vertical, 2 if horizontal
				FireMap[newrow][newcol]+=BMBDMG;
				ExplosionList.push_back(Explosion(BMBEXP,BMBDMG,newrow,newcol,spritenum));
			} else break; 
		}
	}
}
 
void BombManager::TriggerBomb(int row,int col){
	if (BombMap[row][col]){								//If there's a bomb there, explode it.
		for(int id=0;id<2;++id) {
			int ite=0;
			while (ite<BombList[id].size()){
				std::pair<int,int> now=std::make_pair(BombList[id][ite].row_pos,BombList[id][ite].col_pos);
				if (now.first==row && now.second==col){
					BombMap[now.first][now.second]=false;
					Globals->GlobalLevel->SoftMap[now.first][now.second]=0;
					int nexlvl=BombList[id][ite].level;
					BombList[id].erase(BombList[id].begin()+ite);
					Explode(now.first,now.second,0,nexlvl);
				} else ++ite;
			}
		}
	} 
}

void BombManager::RenderBomb(){ //and also a bit of explosion logic. whaaat
	for(int id=0;id<2;++id){ 
		for(int ite=0;ite<BombList[id].size();++ite) {
			int time=BombList[id][ite].timer,col=BombList[id][ite].col_pos,row=BombList[id][ite].row_pos;
			int now=((time/4)%2)+2*((75-time)/26); 
			BombTileList.DrawTile(now,160+(32*col),32*row,MainWindow);
		}
	}
	int ite=0; while (ite<ExplosionList.size()){
		int row=ExplosionList[ite].row, col=ExplosionList[ite].col;
		int time=ExplosionList[ite].timer,spr=ExplosionList[ite].sprite; 
		//8 blocks, dimmest to brightest=0..4
			 if (time<=3 ||time>27) FireTileList.DrawTile(   spr,160+32*col,32*row,MainWindow);
		else if (time<=6 ||time>24) FireTileList.DrawTile( 7+spr,160+32*col,32*row,MainWindow);
		else if (time<=10||time>20) FireTileList.DrawTile(14+spr,160+32*col,32*row,MainWindow);
		else						FireTileList.DrawTile(21+spr,160+32*col,32*row,MainWindow);
		 
		++ite;
	}
	for(int row=0;row<B_MAP_HEIGHT;++row) for(int col=0;col<B_MAP_LENGTH;++col) if (FireMap[row][col]&&Globals->GlobalLevel->SoftHP[row][col]>0) {  
		//debris tiles= 0..5, 0 thickest, 5 weakest
		int now=Globals->GlobalLevel->SoftHP[row][col];
		now=5-(now/35);
		DebrisTileList.DrawTile(now,160+32*col,32*row,MainWindow); 
	}
}
 
void BombManager::LoadBomb(){ 
	std::string filename="Data/bomb.xml";
	std::ifstream DataFile(filename); 
	//if (!DataFile) throw "Could not load tileset: " +filename;  
	std::string DataContent="",temp;
	while(std::getline(DataFile,temp)) DataContent+=temp;
	std::vector<char> XMLData = std::vector<char>(DataContent.begin(),DataContent.end());
	XMLData.push_back('\0'); 
	
	xml_document<> Data;
	Data.parse<parse_no_data_nodes>(&XMLData[0]);

	xml_node<>* root=Data.first_node();
	xml_node<>* bomb=root->first_node("bombimg");
	xml_node<>* son=bomb->first_node("bombspr");
	xml_node<>* tiles;
	//get texture dir  
	std::string imagedir; sf::Texture imagetext; 
	imagedir=bomb->first_attribute("path")->value(); imagetext.loadFromFile(imagedir); 
	BombTextureManager.AddTexture(imagetext);  
	//get tiles dir
	
	tiles = son->first_node("tile");
	while(tiles){
		int x,y,xsiz,ysiz;
		x=atoi(tiles->first_attribute("x")->value());
		y=atoi(tiles->first_attribute("y")->value());
		xsiz=atoi(tiles->first_attribute("xsiz")->value());
		ysiz=atoi(tiles->first_attribute("ysiz")->value());
		BombTileList.AddTile(BombTextureManager.GetTexture(0),x,y,xsiz,ysiz); 
		tiles=tiles->next_sibling();
	}

	son=son->next_sibling();
	tiles = son->first_node("tile");
	while(tiles){
		int x,y,xsiz,ysiz;
		x=atoi(tiles->first_attribute("x")->value());
		y=atoi(tiles->first_attribute("y")->value());
		xsiz=atoi(tiles->first_attribute("xsiz")->value());
		ysiz=atoi(tiles->first_attribute("ysiz")->value());
		FireTileList.AddTile(BombTextureManager.GetTexture(0),x,y,xsiz,ysiz); 
		tiles=tiles->next_sibling();
	}

	son=son->next_sibling();
	tiles = son->first_node("tile");
	while(tiles){
		int x,y,xsiz,ysiz;
		x=atoi(tiles->first_attribute("x")->value());
		y=atoi(tiles->first_attribute("y")->value());
		xsiz=atoi(tiles->first_attribute("xsiz")->value());
		ysiz=atoi(tiles->first_attribute("ysiz")->value());
		DebrisTileList.AddTile(BombTextureManager.GetTexture(0),x,y,xsiz,ysiz); 
		tiles=tiles->next_sibling();
	}
}
