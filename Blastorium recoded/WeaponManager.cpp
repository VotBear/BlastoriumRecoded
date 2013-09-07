#include "WeaponManager.h"
#include <vector>
#include <cfloat>
#include <string>
#include <fstream>
#include <ctime>

#include "Globals.h"
#include "Player.h"
#include "Level.h"
#include "WpnBomb.h"

#include "SFML\Window.hpp"
#include "SFML\Graphics.hpp"
#include "SFML\Audio.hpp"

using namespace std;
using namespace rapidxml;

WeaponManager::WeaponManager(){

}

WeaponManager::~WeaponManager(){

}

void WeaponManager::Construct(shared_ptr<GlobalManager> Glo){
	Globals		=	Glo; 
	MainWindow	=	Glo->GlobalWindow; 
	LoadWeapon(); 
}

void WeaponManager::Init(){  
	Globals->GlobalBombManager->Construct(Globals);
	Globals->GlobalMineManager->Construct(Globals);
	Globals->GlobalRocketManager->Construct(Globals); 
	return;
}

void WeaponManager::SetWeapon(int id,int w1,int w2,int w3){
	//set the player's equipments. Main equipment is always the strongest.
	PlayerEquip[id][0]=w1; NowLevel[id][0]=1; MaxLevel[id][0]=5;
	PlayerEquip[id][1]=w2; NowLevel[id][1]=1; MaxLevel[id][1]=5;
	PlayerEquip[id][2]=w3; NowLevel[id][2]=1; MaxLevel[id][2]=5;
	PlayerActiveWeapon[id]=0;
	return;
}

void WeaponManager::UseWeapon(int id){
	if (Globals->GlobalPlayerManager->IsDisabled(id)) return;
	int wpn_no=PlayerActiveWeapon[id]; 
	pair<int,int> Pos=Globals->GlobalPlayerManager->GetPosition(id);
	int now=PlayerEquip[id][wpn_no], rowpos=Pos.first, colpos=Pos.second;  
	if (now==1){
		//use bomb; 
		Globals->GlobalBombManager->PutBomb(id,NowLevel[id][wpn_no],rowpos,colpos);
	} else if (now==2){
		//use mine
		Globals->GlobalMineManager->PutMine(id,NowLevel[id][wpn_no],rowpos,colpos);
	} else {
		//use rocket
		Globals->GlobalRocketManager->UseRocket(id,NowLevel[id][wpn_no]);
	}
	return;
}

void WeaponManager::ChangeActiveWeapon(int id){
	PlayerActiveWeapon[id]=(PlayerActiveWeapon[id]+1)%3;
	return;
}

void WeaponManager::UpgradeWeapon(int id,int wpn_no){
	NowLevel[id][wpn_no]=min(NowLevel[id][wpn_no]+1,MaxLevel[id][wpn_no]);
	return;
}

void WeaponManager::CalculateLogic(){
	Globals->GlobalRocketManager->RocketLogic();
	Globals->GlobalMineManager->MineLogic();
	Globals->GlobalBombManager->BombLogic(); 
	//andeverythingelse
	return;
}

void WeaponManager::RenderWeapons(){
	Globals->GlobalMineManager->RenderMine();
	Globals->GlobalRocketManager->RenderRocket();
	Globals->GlobalBombManager->RenderBomb();
	for (int id=0;id<2;++id){
		int xpos,ipos,ypos,xnum,xlvl; //xpos=x, ypos=y, ipos=second x for icon, etc
		double scl;
		if (id==0)	{ 
			xpos=24;		ipos=24;	 scl=1;
			xnum=ipos+22;	xlvl=ipos;
		}	else	{ 
			xpos=840;		ipos=840-32; scl=-1;
			xnum=ipos+2;  	xlvl=ipos+17;
		} 
		ypos=24;
		for (int ite=0;ite<3;++ite){
			//draw frame
			if (PlayerActiveWeapon[id]==ite) 
				FrameList.DrawTile(0,xpos,ypos,scl,1,MainWindow);
			else 
				FrameList.DrawTile(1,xpos,ypos,scl,1,MainWindow);

			//draw icon
			int temp;
			IconList.DrawTile(PlayerEquip[id][ite]-1,xpos,ypos,scl,1,MainWindow);
			LevelList.DrawTile(NowLevel[id][ite]-1,xlvl,ypos+22,MainWindow);

			if (PlayerEquip[id][ite]==1) 
				temp=Globals->GlobalBombManager->GetAvailability(id,NowLevel[id][ite]);
			
			else if (PlayerEquip[id][ite]==2) 
				temp=Globals->GlobalMineManager->GetAvailability(id,NowLevel[id][ite]);
			
			else if (PlayerEquip[id][ite]==3) 
				temp=Globals->GlobalRocketManager->GetAvailability(id,NowLevel[id][ite]); 
			
			if (temp>=0) {
				if (temp>=10&&id==1) NumberList.DrawTile(temp,xnum+2,ypos+22,MainWindow);
				else NumberList.DrawTile(temp,xnum,ypos+22,MainWindow);
			}

			//draw blur/disable 
			if (Globals->GlobalPlayerManager->IsDisabled(id)){
				FrameList.DrawTile(2,xpos,ypos,scl,1,MainWindow);
				//FrameList.DrawTile(3,xpos,ypos,scl,1,MainWindow);
		
			} else {
				bool can;
					 if (PlayerEquip[id][ite]==1) can=Globals->GlobalBombManager->IsAvailable(id,NowLevel[id][ite]);
				else if (PlayerEquip[id][ite]==2) can=Globals->GlobalMineManager->IsAvailable(id,NowLevel[id][ite]);
				else if (PlayerEquip[id][ite]==3) can=Globals->GlobalRocketManager->IsAvailable(id,NowLevel[id][ite]); 
				if (!can) FrameList.DrawTile(2,xpos,ypos,scl,1,MainWindow);
			}
			ypos+=32;
		}
	}
	return;
}

void WeaponManager::LoadWeapon(){
	std::string filename="Data/weapon.xml";
	std::ifstream DataFile(filename);   
	std::string DataContent="",temp;
	while(std::getline(DataFile,temp)) DataContent+=temp;
	std::vector<char> XMLData = std::vector<char>(DataContent.begin(),DataContent.end());
	XMLData.push_back('\0'); 
	
	xml_document<> Data;
	Data.parse<parse_no_data_nodes>(&XMLData[0]);

	xml_node<>* root=Data.first_node();
	xml_node<>* son=root->first_node("GUI"); 
	xml_node<>* tiles;
	//get texture dir  
	std::string imagedir; sf::Texture imagetext; 
	imagedir=root->first_attribute("path")->value(); imagetext.loadFromFile(imagedir); 
	WeaponTextureManager.AddTexture(imagetext);  
	//get tiles dir
	for (int ite=0;ite<4;++ite){
		tiles = son->first_node("tile");
		while(tiles){
			int x,y,xsiz,ysiz;
			x=atoi(tiles->first_attribute("x")->value());
			y=atoi(tiles->first_attribute("y")->value());
			xsiz=atoi(tiles->first_attribute("xsiz")->value());
			ysiz=atoi(tiles->first_attribute("ysiz")->value());
				 if (ite==0) FrameList.AddTile(WeaponTextureManager.GetTexture(0),x,y,xsiz,ysiz); 
			else if (ite==1) IconList.AddTile(WeaponTextureManager.GetTexture(0),x,y,xsiz,ysiz); 
			else if (ite==2) NumberList.AddTile(WeaponTextureManager.GetTexture(0),x,y,xsiz,ysiz); 
			else if (ite==3) LevelList.AddTile(WeaponTextureManager.GetTexture(0),x,y,xsiz,ysiz); 
			tiles=tiles->next_sibling();
		}
		son=son->next_sibling();
	} 
	return;
}