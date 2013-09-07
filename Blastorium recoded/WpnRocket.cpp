#include "WpnRocket.h"

#include <vector>
#include <cfloat>
#include <string>
#include <fstream>
#include <ctime>

#include "Globals.h"
#include "Level.h"
#include "Player.h"
#include "SFML\Graphics.hpp"
#include "SFML\Window.hpp" 

#include "rapidxml.hpp"

#define MAP_LENGTH 17
#define MAP_HEIGHT 15
#define DUR 30		//1 second
#define DMG 90

using namespace rapidxml;
using namespace std;

//////////////////////////////////////////////////////////////////////
//ROCKETS (ID 3)
//
//SECONDARY OFFENSIVE WEAPON TO HINDER AND/OR DISABLE ENEMIES
//WHEN USED, LAUNCHES A ROCKET AT CURRENT ENEMY POSITION.
//DEALS IMMEDIATE EFFECT AFTER SEVERAL SECS DELAY. EXPLOSION JUST FOR VISUALS
//SIGNIFICANTLY DISABLES AND SLOWS TARGET ON HIT
//HAS A COOLDOWN.
//
//WITH BOMB: ROCKETS WILL PREMATURELY TRIGGER BOMBS
//WITH MINES: ROCKETS WILL PREMATURELY TRIGGER MINES
//
//INCREASING LEVELS REDUCES COOLDOWN AND HIT DELAY
//AT LEVEL 5, ROCKETS STUNS
//COOLDOWN(s)  = 5.50 - 0.50*(LEVEL)
//HIT DELAY(s) = 2.50 - 0.25*(LEVEL)
//DISABLE(s)   = 1.00 + 0.25*(LEVEL)
//SLOW(s)	   = 1.00 + 0.25*(LEVEL)
//SLOW(magn)   = 50%
//STUN(lv5)	   = 1.00
//DAMAGE	   = 75	
//////////////////////////////////////////////////////////////////////

RocketManager::RocketManager(){
	LoadRocket(); 
}

RocketManager::~RocketManager(){

}

void RocketManager::Construct(shared_ptr<GlobalManager> Glo){ 
	Globals		=	Glo;
	MainWindow	=	Glo->GlobalWindow;
	memset(Cooldown,0,sizeof Cooldown);
	ExplosionList.clear();
	RocketList.clear();
}

void RocketManager::UseRocket(int id,int rocketlevel){

	if (Cooldown[id]>0) return;
	Globals->GlobalDataManager->UseWeapon(id);

	for(int ot=0;ot<2;++ot) if (ot!=id){
		pair<int,int> Pos=Globals->GlobalPlayerManager->GetPosition(ot);
		int rowpos=(Pos.first+16)/32, colpos=(Pos.second+16-160)/32;
		RocketList.push_back(Rocket(rowpos,colpos,rocketlevel));
	}
	Cooldown[id]=165-rocketlevel*15;
}

bool RocketManager::IsAvailable(int id,int rocketlevel){
	return (Cooldown[id]<=0);
}

int RocketManager::GetAvailability(int id,int rocketlevel){
	if (Cooldown[id]<=0) return -1;
	else return ((Cooldown[id]+29)/30);
}

void RocketManager::RocketLogic(){
	for (int id=0 ; id<2 ; ++id) --Cooldown[id];
	int ite=0;
	for(ite;ite<RocketList.size();){

		--RocketList[ite].timer;

		if (RocketList[ite].timer==0){

			for(int id=0;id<2;++id){

				pair<int,int> Pos=Globals->GlobalPlayerManager->GetPosition(id);
				Pos.first=(Pos.first+16)/32; Pos.second=(Pos.second+16-160)/32;
				Globals->GlobalBombManager->TriggerBomb(RocketList[ite].coor.first,RocketList[ite].coor.second);
				Globals->GlobalMineManager->TriggerMine(RocketList[ite].coor.first,RocketList[ite].coor.second);

				if (RocketList[ite].coor==Pos){ 
					Globals->GlobalDataManager->UpdateWeapon(3,1);
					Globals->GlobalPlayerManager->Damage(id,75);										//dmg
					Globals->GlobalPlayerManager->Disable(id,60+15*RocketList[ite].level);				//disable
					Globals->GlobalPlayerManager->ChgTmpSpeed(id,0.40f,60+15*RocketList[ite].level);	//slow
					if (RocketList[ite].level>=5) Globals->GlobalPlayerManager->Stun(id,60);			//stun
				}

			}
			ExplosionList.push_back(Explosion(RocketList[ite].coor));
			RocketList.erase(RocketList.begin()+ite);
		} else {
			++ite;
		}
	}
	for (int ite=0;ite<ExplosionList.size();++ite){
		++ExplosionList[ite].timer;
	}
}
										
void RocketManager::RenderRocket(){
	//Math
	//not even once
	for(int ite=0;ite<RocketList.size();++ite){
		int time=RocketList[ite].timer, row=RocketList[ite].coor.first*32, col=RocketList[ite].coor.second*32+160, no;
		no=0; if (time>10) ++no; if (time>25) ++no;
		RocketTileList.DrawTile(no,col,row-(time*35),MainWindow);
		no=0; if ((time-1)%40<16) no=((time-1)%40)%8;
		TargetTileList.DrawTile(no,col,row,MainWindow);
	}
	for(int ite=0;ite<ExplosionList.size();){
		int time=ExplosionList[ite].timer, row=ExplosionList[ite].coor.first*32, col=ExplosionList[ite].coor.second*32+160;
		ExplosionTileList.DrawTile(time/2,col,row,MainWindow); 
		if (ExplosionList[ite].timer>=30){
			ExplosionList.erase(ExplosionList.begin()+ite);
		} else {
			++ite;
		}
	}
} 

void RocketManager::LoadRocket(){ 
	std::string filename="Data/rocket.xml";
	std::ifstream DataFile(filename); 
	//if (!DataFile) throw "Could not load tileset: " +filename;  
	std::string DataContent="",temp;
	while(std::getline(DataFile,temp)) DataContent+=temp;
	std::vector<char> XMLData = std::vector<char>(DataContent.begin(),DataContent.end());
	XMLData.push_back('\0'); 
	
	xml_document<> Data;
	Data.parse<parse_no_data_nodes>(&XMLData[0]);

	xml_node<>* root=Data.first_node();
	xml_node<>* son=root->first_node("rocketspr"); 
	xml_node<>* tiles;
	//get texture dir  
	std::string imagedir; sf::Texture imagetext; 
	imagedir=root->first_attribute("path")->value(); imagetext.loadFromFile(imagedir); 
	RocketTextureManager.AddTexture(imagetext);  
	//get tiles dir
	
	tiles = son->first_node("tile");
	while(tiles){
		int x,y,xsiz,ysiz;
		x=atoi(tiles->first_attribute("x")->value());
		y=atoi(tiles->first_attribute("y")->value());
		xsiz=atoi(tiles->first_attribute("xsiz")->value());
		ysiz=atoi(tiles->first_attribute("ysiz")->value());
		RocketTileList.AddTile(RocketTextureManager.GetTexture(0),x,y,xsiz,ysiz); 
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
		TargetTileList.AddTile(RocketTextureManager.GetTexture(0),x,y,xsiz,ysiz); 
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
		ExplosionTileList.AddTile(RocketTextureManager.GetTexture(0),x,y,xsiz,ysiz); 
		tiles=tiles->next_sibling();
	} 
}
