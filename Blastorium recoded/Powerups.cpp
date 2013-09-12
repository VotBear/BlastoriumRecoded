#include "Powerups.h"

#include <vector>
#include <string>
#include <fstream>
#include <ctime>

#include "Globals.h"
#include "Player.h"
#include "WeaponManager.h"
#include "WpnBomb.h"
#include "Level.h"

#include "rapidxml.hpp"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace rapidxml; 

const int U_MAP_LENGTH=17;
const int U_MAP_HEIGHT=15;

PowerupManager::PowerupManager(){
	
}

PowerupManager::~PowerupManager(){

}

void PowerupManager::Construct(shared_ptr<GlobalManager> Glo){
	Globals		=	Glo;
	MainWindow	=	Glo->GlobalWindow; 
	LoadPowerup();
}

void PowerupManager::Init(){
	memset(Powerup,-1,sizeof Powerup);
	memset(Active,false,sizeof Active);
	Covered.clear();

	vector<int> tmp;

	SetPowerupAmount(tmp);
	GeneratePowerup();
	
}

void PowerupManager::SetPowerupAmount(vector<int> amt){
	PowerupCount[0]=5;
	PowerupCount[1]=5;
	PowerupCount[2]=5;
	PowerupCount[3]=10;
	PowerupCount[4]=10;
	
}

void PowerupManager::GeneratePowerup(){ 
	srand(time(0));
	int cnt=0,amt=0;
	for(int i=0;i<5;++i) amt+=PowerupCount[i];
	while(cnt<amt){
		for(int row=0;row<U_MAP_HEIGHT;++row){
			for(int col=0;col<U_MAP_LENGTH;++col){
				if (Powerup[row][col]==-1&&Globals->GlobalLevel->SoftMap[row][col]==1){
					int now=rand()%15; 
					if (now<5) if (PowerupCount[now]>0) {
						Powerup[row][col]=now;
						--PowerupCount[now];
						Covered.push_back(make_pair(row,col));
						++cnt;
						if(cnt>=amt) return;
					}
				}
			}
		}
	}
	
}
 
void PowerupManager::GetPowerup(int id,int powerupno){
	Globals->GlobalDataManager->TakePowerup(id);
	Globals->GlobalMediManager->UsePowerup(id);
	if (powerupno<=2){			//wpn upgrade
		Globals->GlobalWeaponManager->UpgradeWeapon(id,powerupno);
	} else if (powerupno==3){	//speed
		Globals->GlobalPlayerManager->ChgMaxSpeed(id,0.5f);
		Globals->GlobalPlayerManager->ChgTmpSpeed(id,1.15f,120);
	} else if (powerupno==4){	//heal
		Globals->GlobalPlayerManager->Damage(id,-60);
		Globals->GlobalPlayerManager->Debuff(id);
	}
	
}

void PowerupManager::CalculateLogic(){
	int ite=0;
	while (ite<Covered.size()){
		int r=Covered[ite].first,c=Covered[ite].second;
		if (Globals->GlobalLevel->SoftMap[r][c]==0&&Globals->GlobalBombManager->FireMap[r][c]==0){
			Active[r][c]=true;
			LoopTimer[r][c]=0;
			Covered.erase(Covered.begin()+ite);
			//printf("UNCOVERED POWERUP %d AT %d %d\n",Powerup[r][c],r,c);
		} else {
			++ite;
		}
	}
	for(int row=0;row<U_MAP_HEIGHT;++row)	for(int col=0;col<U_MAP_LENGTH;++col) if (Active[row][col]){
		if (Globals->GlobalBombManager->FireMap[row][col]>0) {
			Active[row][col]=false;
			Powerup[row][col]=-1;
		}
		LoopTimer[row][col]=(LoopTimer[row][col]+1)%120;
	}
	for(int id=0;id<2;++id){
		pair<int,int> pos=Globals->GlobalPlayerManager->GetPosition(id); 
		int rnow=(pos.first+16)/32, cnow=(pos.second-160+16)/32;
		if (Active[rnow][cnow]){
			GetPowerup(id,Powerup[rnow][cnow]);
			Active[rnow][cnow]=false;
			Powerup[rnow][cnow]=-1;
		}
	} 
}

void PowerupManager::RenderPowerup(){
	for(int row=0;row<U_MAP_HEIGHT;++row)	for(int col=0;col<U_MAP_LENGTH;++col) if (Active[row][col]){
		IconTileList.DrawTile(Powerup[row][col],160+32*col,32*row,MainWindow);
		if (LoopTimer[row][col]<24){
			AnimTileList.DrawTile(LoopTimer[row][col]/3,160+32*col,32*row,MainWindow);
		}
	} 
}

void PowerupManager::LoadPowerup(){ 
	std::string filename="Data/powerup.xml";
	std::ifstream DataFile(filename); 
	//if (!DataFile) throw "Could not load tileset: " +filename;  
	std::string DataContent="",temp;
	while(std::getline(DataFile,temp)) DataContent+=temp;
	std::vector<char> XMLData = std::vector<char>(DataContent.begin(),DataContent.end());
	XMLData.push_back('\0'); 
	
	xml_document<> Data;
	Data.parse<parse_no_data_nodes>(&XMLData[0]);

	xml_node<>* root=Data.first_node();
	xml_node<>* son=root->first_node("icon"); 
	xml_node<>* tiles;
	//get texture dir  
	std::string imagedir; sf::Texture imagetext; 
	imagedir=root->first_attribute("path")->value(); imagetext.loadFromFile(imagedir); 
	PowerupTextureManager.AddTexture(imagetext);  
	//get tiles dir
	
	tiles = son->first_node("tile");
	while(tiles){
		int x,y,xsiz,ysiz;
		x=atoi(tiles->first_attribute("x")->value());
		y=atoi(tiles->first_attribute("y")->value());
		xsiz=atoi(tiles->first_attribute("xsiz")->value());
		ysiz=atoi(tiles->first_attribute("ysiz")->value());
		IconTileList.AddTile(PowerupTextureManager.GetTexture(0),x,y,xsiz,ysiz); 
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
		AnimTileList.AddTile(PowerupTextureManager.GetTexture(0),x,y,xsiz,ysiz); 
		tiles=tiles->next_sibling();
	}  
	
}
