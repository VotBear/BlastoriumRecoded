#include "WpnMine.h"

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

const int MAP_LENGTH=17;	//self
const int MAP_HEIGHT=15;	//explanatoryv
const int MNEDMG=50;		//base damage
const int MNEDMD=10;		//damage increase/level

using namespace rapidxml;
using namespace std;
  
//////////////////////////////////////////////////////////////////////
//MINES (ID 2)
//
//PASSIVE WEAPONS, USED TO BLOCK PASSAGES AND HINDER ENEMY MOVEMENT
//WHEN PUT, SPENDS SEVERAL SECONDS FADE TIME BEFORE IT IS ACTIVE
//DEALS FULL DAMAGE IMMEDIATELY WHEN STEPPED. EXPLOSION JUST FOR VISUALS
//
//WITH BOMB: MINES CAN BE PUT BEFORE BOMBS. NO TRIGGER BETWEEN THEM
//WITH ROCKET: ROCKETS WILL PREMATURELY TRIGGER MINES
//
//INCREASING LEVELS INCREASE MINE STOCK, DAMAGE, AND FASTER FADE TIME
//AT LEVEL 5, MINES DEAL LESS DAMAGE TO ITS OWNER (LESS FRIENDLY FIRE)
//MINE STOCK = 2*(LEVEL)
//DAMAGE	 = 50 + 10*(LEVEL). LEVEL 5 FRIENDLY FIRE DAMAGE IS HALVED
//FADETIME(s)= 3.5 - 0.5*(LEVEL)
//////////////////////////////////////////////////////////////////////

MineManager::MineManager(){ 
	LoadMine();
}

MineManager::~MineManager(){

}

void MineManager::Construct(shared_ptr<GlobalManager> Glo) {
	Globals		=	Glo; 
	MainWindow	=	Glo->GlobalWindow;
	memset(IsMined,false,sizeof IsMined);
	memset(MineAmt,0,sizeof MineAmt);
	ExplosionList.clear();
}

void MineManager::PutMine(int id,int minelevel,int row,int col){ 
	//printf("TRYPUT");
	int nrow=(row+16)/32,ncol=(col-160+16)/32; 
	if (MineAmt[id]>=2*minelevel) return;
	if (IsMined[nrow][ncol]||Globals->GlobalBombManager->BombMap[nrow][ncol]) return; 

	Globals->GlobalDataManager->UseWeapon(id);
	IsMined[nrow][ncol]=true;
	MineMap[nrow][ncol]=Mine(id,minelevel);
	++MineAmt[id];
	//printf("SUCCESS");
}

bool MineManager::IsAvailable(int id,int minelevel){
	return (MineAmt[id]<2*minelevel);
}

int MineManager::GetAvailability(int id,int minelevel){
	return (2*minelevel-MineAmt[id]);
}

void MineManager::TriggerMine(int row,int col){
	if (IsMined[row][col]){ 
		for (int id=0; id<2; ++id){
			pair<int,int> pos = Globals->GlobalPlayerManager->GetPosition(id); 
			int nrow=(pos.first+16)/32, 
				ncol=(pos.second-160+16)/32;
		
			if (row==nrow&&col==ncol) {
				int damage=MNEDMG + MineMap[nrow][ncol].level*MNEDMD;
				if (id==MineMap[nrow][ncol].owner && MineMap[nrow][ncol].level==5) damage/=4;
				Globals->GlobalPlayerManager->Damage(id,damage);  
				Globals->GlobalDataManager->UpdateWeapon(2,1);
			}
		}
		ExplosionList.push_back(Explosion(row,col));
		IsMined[row][col]=false;
		--MineAmt[MineMap[row][col].owner];
	}
}

void MineManager::MineLogic(){
	for (int id=0; id<2; ++id){
		pair<int,int> pos = Globals->GlobalPlayerManager->GetPosition(id); 
		int nrow=(pos.first+16)/32, 
			ncol=(pos.second-160+16)/32;
		
		if (IsMined[nrow][ncol] && MineMap[nrow][ncol].timer>=105-15*MineMap[nrow][ncol].level) {
			TriggerMine(nrow,ncol);
		}
	}
	for (int ite=0;ite<ExplosionList.size();++ite){
		++ExplosionList[ite].timer;
	}
}

void MineManager::RenderMine(){ 
	for (int row=0; row<MAP_HEIGHT; ++row)
		for (int col=0; col<MAP_LENGTH; ++col)
			if (IsMined[row][col]) {
				int time=MineMap[row][col].timer;
				int limit=105-15*MineMap[row][col].level;
				double t=255*(limit-time); t/=limit; 
				MineTileList.SetColor((time/10)%2,t);
				MineTileList.DrawTile((time/10)%2,160+col*32,row*32,MainWindow);

				MineMap[row][col].timer=min(MineMap[row][col].timer+1,limit);
			}

	int ite=0; 
	for (ite;ite<ExplosionList.size();){
		Explosion now=ExplosionList[ite];
		int row=now.coor.first, col=now.coor.second;
		double time=now.timer;
		MineTileList.DrawTile(2+time/2,160+col*32,row*32,MainWindow);
		if (now.timer==24){
			ExplosionList.erase(ExplosionList.begin()+ite);
		} else {
			++ite;
		}
	}
}

void MineManager::LoadMine(){
	
	std::string filename="Data/mine.xml";
	std::ifstream DataFile(filename); 
	//if (!DataFile) throw "Could not load tileset: " +filename;  
	std::string DataContent="",temp;
	while(std::getline(DataFile,temp)) DataContent+=temp;
	std::vector<char> XMLData = std::vector<char>(DataContent.begin(),DataContent.end());
	XMLData.push_back('\0'); 
	
	xml_document<> Data;
	Data.parse<parse_no_data_nodes>(&XMLData[0]);

	xml_node<>* root=Data.first_node();
	xml_node<>* son=root->first_node("minespr"); 
	xml_node<>* tiles;
	//get texture dir  
	std::string imagedir; sf::Texture imagetext; 
	imagedir=root->first_attribute("path")->value(); imagetext.loadFromFile(imagedir); 
	MineTextureManager.AddTexture(imagetext);  
	//get tiles dir
	
	tiles = son->first_node("tile");
	while(tiles){
		int x,y,xsiz,ysiz;
		x=atoi(tiles->first_attribute("x")->value());
		y=atoi(tiles->first_attribute("y")->value());
		xsiz=atoi(tiles->first_attribute("xsiz")->value());
		ysiz=atoi(tiles->first_attribute("ysiz")->value());
		MineTileList.AddTile(MineTextureManager.GetTexture(0),x,y,xsiz,ysiz); 
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
		MineTileList.AddTile(MineTextureManager.GetTexture(0),x,y,xsiz,ysiz); 
		tiles=tiles->next_sibling();
	} 
}
