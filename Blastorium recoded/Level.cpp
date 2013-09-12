#include "Level.h" 

#include <vector>
#include <string>
#include <fstream>
#include <ctime>

#include "TextureManager.h"
#include "Tilelist.h"
#include "Globals.h"

#include "rapidxml.hpp"

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

using namespace std;
using namespace rapidxml;

const int L_MAP_LENGTH=17;
const int L_MAP_HEIGHT=15;


Level::Level(){
	
}

Level::~Level(){
	
}

void Level::Construct(shared_ptr<GlobalManager> Glo){
	Globals			  = Glo;
	MainWindow		  = Glo->GlobalWindow; 
	usedpos=0; 
	
}

void Level::Init(){ 
	usedpos=0; 
	for(int row=0;row<L_MAP_HEIGHT;++row){
		for(int col=0;col<L_MAP_LENGTH;++col){
			if (LevelMap[row][col]==9) SoftMap[row][col]=2; else SoftMap[row][col]=0; 
		}
	}
	GenerateSoftTiles(softamt);
	for(int row=0;row<L_MAP_HEIGHT;++row){
		for(int col=0;col<L_MAP_LENGTH;++col){
				 if (SoftMap[row][col]==1) SoftHP[row][col]=180;
			else if (SoftMap[row][col]==2) SoftHP[row][col]=-1;
			else SoftHP[row][col]=0;
		}
	}
	
}
 
void Level::SetLevel(int ID){ 
	LevelID=ID;
	LevelTextureManager.ClearTexture();
	LevelTilelist.ClearTile();
	StartPosition.clear();
	LoadLevel();
	Init();
	
}

void Level::GenerateSoftTiles(int amt){
	srand(time(0));
	int cnt=0;
	while(cnt<amt){
		for(int row=0;row<L_MAP_HEIGHT;++row){
			for(int col=0;col<L_MAP_LENGTH;++col){
				if (LevelMap[row][col]==1&&SoftMap[row][col]==0){
					int now=rand()%10; 
					if (now<3) {
						SoftMap[row][col]=1; 
						++cnt;
						if(cnt>=amt) return;
					}
				}
			}
		}
	}
	
}

void Level::RenderLevel(){
	LevelTilelist.DrawTile(5,0,0,MainWindow);
	LevelTilelist.DrawTile(6,704,0,MainWindow);
	for(int row=0;row<L_MAP_HEIGHT;++row){
		LevelTilelist.DrawTile(3,128,row*32,MainWindow);
		LevelTilelist.DrawTile(4,160+L_MAP_LENGTH*32,row*32,MainWindow);
		for(int col=0;col<L_MAP_LENGTH;++col){
			LevelTilelist.DrawTile(0,160+col*32,row*32,MainWindow);
			if (SoftMap[row][col]==2) LevelTilelist.DrawTile(1,160+col*32,row*32,MainWindow);
			else if (SoftMap[row][col]==1) LevelTilelist.DrawTile(2,160+col*32,row*32,MainWindow);
		}
	} 
	
}

std::pair<int,int> Level::GetStartPos(){
	++usedpos;
	return StartPosition[usedpos-1];
}

void Level::LoadLevel(){
	std::string filename="Data/level"; filename+=('0'+LevelID); filename+=".xml";
	std::ifstream DataFile(filename); 
	//if (!DataFile) throw "Could not load tileset: " +filename; 
	
	std::string DataContent="",temp;
	while(std::getline(DataFile,temp)) DataContent+=temp;
	std::vector<char> XMLData = std::vector<char>(DataContent.begin(),DataContent.end());
	XMLData.push_back('\0'); 
	
	xml_document<> Data;
	Data.parse<parse_no_data_nodes>(&XMLData[0]);

	xml_node<>* root=Data.first_node();
	xml_node<>* imagefile=root->first_node("imagefile");
	int texid=0; 
	//get texture dir
	std::string imagedir=imagefile->first_attribute("path")->value();
	sf::Texture imagetext; imagetext.loadFromFile(imagedir);
	LevelTextureManager.AddTexture(imagetext);
				imagedir=imagefile->next_sibling()->first_attribute("path")->value();
				imagetext; imagetext.loadFromFile(imagedir);
	LevelTextureManager.AddTexture(imagetext);
		
	//get tiles dir
	xml_node<>* tiles = imagefile->first_node("tile");
	for(int i=0;i<2;++i){
		while(tiles){
			int x,y,xsiz,ysiz;
			x=atoi(tiles->first_attribute("x")->value());
			y=atoi(tiles->first_attribute("y")->value());
			xsiz=atoi(tiles->first_attribute("xsiz")->value());
			ysiz=atoi(tiles->first_attribute("ysiz")->value());
			LevelTilelist.AddTile(LevelTextureManager.GetTexture(texid),x,y,xsiz,ysiz); 
			tiles=tiles->next_sibling();
		}
		++texid; 
		tiles = imagefile->next_sibling()->first_node("tile");
	}
	xml_node<>* wall=root->first_node("map"); softamt=atoi(wall->first_attribute("softamt")->value());
	xml_node<>* rows=wall->first_node("row");
	while (rows){
		int row=atoi(rows->first_attribute("no")->value());
		std::string val=rows->first_attribute("id")->value();

		for(int i=0;i<L_MAP_LENGTH;++i){
			LevelMap[row][i]=val[i]-'0'; 
			if (LevelMap[row][i]==9) SoftMap[row][i]=2; else SoftMap[row][i]=0; 
		}

		rows=rows->next_sibling();
	}
	xml_node<>* stpos=root->first_node("startpositions");
	xml_node<>* pos=stpos->first_node("pos");
	while (pos){
		int xpos=atoi(pos->first_attribute("x")->value()),
			ypos=atoi(pos->first_attribute("y")->value());
		StartPosition.push_back(std::make_pair(xpos,ypos)); //col, row
		pos=pos->next_sibling();
	} 
	
}
