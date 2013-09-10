#include "Player.h"
#include <vector>
#include <cfloat>
#include <string>
#include <fstream>
#include <ctime>

#include "Globals.h"

#include "SFML/Graphics.hpp"

#include "rapidxml.hpp"

using namespace rapidxml;
using namespace std;

#define MAP_LENGTH 17
#define MAP_HEIGHT 15

using namespace std;

Player::Player(int colpos,int rowpos,int color){
	col=color;
	max_hp=now_hp=tmp_hp=500; 
	row_pos=rowpos; col_pos=colpos;
	cur_row=nex_row=(int)rowpos/32; cur_col=nex_col=(int)(colpos-160)/32; 

	disabled=0;
	stunned=0;
	invul=0;
	octimer=0;

	overclock=false;

	max_move_speed=move_speed=4.5; //max is 10
	speed_modifier.clear();
	effect_list.clear();
}

Player::~Player(){
}

void Player::Move(){  
	double ms=move_speed; 
	double row_tar=nex_row*32, col_tar=160+nex_col*32;
	if (row_pos>row_tar){
		row_pos-=ms;
		if (row_pos<=row_tar) { row_pos=row_tar; cur_row=nex_row; }
	} else if (row_pos<row_tar){
		row_pos+=ms;
		if (row_pos>=row_tar) { row_pos=row_tar; cur_row=nex_row; }
	}
	if (col_pos>col_tar){
		col_pos-=ms;
		if (col_pos<=col_tar) { col_pos=col_tar; cur_col=nex_col; }
	} else if (col_pos<col_tar){
		col_pos+=ms;
		if (col_pos>=col_tar) { col_pos=col_tar; cur_col=nex_col; }
	}
} 

bool Player::Logic(){ //if the player is dead, returns false
	//now hp is the green bar, indicating the HP at the moment
	//tmp hp is the red bar, animation
	//max hp is max hp, duh
	if (now_hp>max_hp) now_hp=max_hp; if (now_hp<0) now_hp=0;
	if (tmp_hp<now_hp) tmp_hp=now_hp; if (tmp_hp>now_hp) tmp_hp-=5;	if (tmp_hp<0) tmp_hp=0;
	move_speed=max_move_speed;
	int ite=0;
	while(ite<speed_modifier.size()){
		move_speed*=speed_modifier[ite].first;
		--speed_modifier[ite].second;
		if (speed_modifier[ite].second<=0){
			speed_modifier.erase(speed_modifier.begin()+ite);
		} else {
			++ite;
		}
	}
	if (stunned>0){
		--stunned; 
		move_speed=0;
	}
	return now_hp>0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////


PlayerManager::PlayerManager(){

}

PlayerManager::~PlayerManager(){

}

void PlayerManager::Construct(shared_ptr<GlobalManager> Glo){
	Globals		=	Glo;
	MainWindow	=	Glo->GlobalWindow;
	LoadPlayer(); 
}

void PlayerManager::Init(){ 
	PlayerList.clear();
	std::pair<int,int> stpos;
	stpos=Globals->GlobalLevel->GetStartPos(); AddPlayer(stpos.first*32+160,stpos.second*32);
	stpos=Globals->GlobalLevel->GetStartPos(); AddPlayer(stpos.first*32+160,stpos.second*32);
}

void PlayerManager::AddPlayer(int rowpos,int colpos){
	PlayerList.push_back(Player(rowpos,colpos,PlayerList.size()));
}

bool PlayerManager::check(int row,int col){ 
	if (row<0||col<0||row>=MAP_HEIGHT||col>=MAP_LENGTH) return false;
	return (Globals->GlobalLevel->SoftMap[row][col]==0);
}

void PlayerManager::MovePlayer(int id,int dir){	//clockwise, 1-4 = north, east, south, west respectively
	//check if the move is legitimate
	int CurRow=PlayerList[id].cur_row,
		NexRow=PlayerList[id].nex_row,
		CurCol=PlayerList[id].cur_col,
		NexCol=PlayerList[id].nex_col;
	if (dir==1){ //^
		if (NexRow>CurRow) {
			--PlayerList[id].nex_row;
			++PlayerList[id].cur_row;
		}
		else if (NexRow==CurRow) if (check(CurRow-1,CurCol)&&check(CurRow-1,NexCol)) {
			 --PlayerList[id].nex_row;
		}
	} else if (dir==2){ //>
		if (NexCol<CurCol) {
			++PlayerList[id].nex_col;
			--PlayerList[id].cur_col;
		}
		else if (NexCol==CurCol) if (check(CurRow,CurCol+1)&&check(NexRow,CurCol+1)) {
			 ++PlayerList[id].nex_col;
		}
	} else if (dir==3){ //V
		if (NexRow<CurRow) {
			++PlayerList[id].nex_row;
			--PlayerList[id].cur_row;
		}
		else if (NexRow==CurRow) if (check(CurRow+1,CurCol)&&check(CurRow+1,NexCol)) {
			 ++PlayerList[id].nex_row;
		}
	} else if (dir==4){ //<
		if (NexCol>CurCol) {
			--PlayerList[id].nex_col;
			++PlayerList[id].cur_col;
		}
		else if (NexCol==CurCol) if (check(CurRow,CurCol-1)&&check(NexRow,CurCol-1)) {
			 --PlayerList[id].nex_col;
		}
	} 
}

void PlayerManager::DrawNum(int id){//draws the numbers.
	int hp=(int)floor(PlayerList[id].now_hp);
	int xpos,ypos;
	if (id==0) xpos=48; else xpos=864-24-8;
	ypos=6;
	NumberList.DrawTile(hp%10,xpos,ypos,MainWindow);
	hp/=10;
	while (hp>0){ 
		xpos-=9;
		NumberList.DrawTile(hp%10,xpos,ypos,MainWindow); 
		hp/=10;
	}
}

void PlayerManager::RenderPlayer(){	//renders all player-related stuff at once
	for(int i=0; i<PlayerList.size(); ++i){ 
		int hgt; 
		double xpos,scale=468.0f/24.0f,flip;
		if (i==0) xpos=0; else xpos=864;
		if (i==0) flip=1; else flip=-1;
		//draw the HP bar
		HPBarList.DrawTile(0,xpos,6,flip,scale,MainWindow);
		HPBarList.DrawTile(1,xpos,6,flip,scale*(PlayerList[i].tmp_hp/PlayerList[i].max_hp),MainWindow);
		HPBarList.DrawTile(2,xpos,6,flip,scale*(PlayerList[i].now_hp/PlayerList[i].max_hp),MainWindow);
		HPBarList.DrawTile(3,xpos,0,flip,1,MainWindow);
		HPBarList.DrawTile(4,xpos,474,flip,1,MainWindow);
		//the top HP bar. This is tricky because simple scaling won't work.
		//it must be resized based on height
			hgt=int(floor(468*PlayerList[i].tmp_hp/PlayerList[i].max_hp));
			HPBarList.SetSize(5,80,min(hgt,18));
			hgt=int(floor(468*PlayerList[i].now_hp/PlayerList[i].max_hp));
			HPBarList.SetSize(6,80,min(hgt,18));

		HPBarList.DrawTile(5,xpos,6,flip,1,MainWindow);
		HPBarList.DrawTile(6,xpos,6,flip,1,MainWindow);

		//draw the numbers
		DrawNum(i);

		//draw player
		PlayerSprList.DrawTile(PlayerList[i].col,PlayerList[i].col_pos,PlayerList[i].row_pos,MainWindow);
		PlayerSprList.DrawTile(4+2*i,PlayerList[i].col_pos,PlayerList[i].row_pos,MainWindow);

		if (PlayerList[i].invul>0){ 
			PlayerSprList.DrawTile(14+((PlayerList[i].invul/2)%4),PlayerList[i].col_pos,PlayerList[i].row_pos,MainWindow);
			--PlayerList[i].invul;
		}

		if (PlayerList[i].overclock){
			PlayerSprList.DrawTile(18+(PlayerList[i].octimer),PlayerList[i].col_pos,PlayerList[i].row_pos,MainWindow);
			PlayerList[i].octimer=(PlayerList[i].octimer+1)%4;
		}

		//draw special effects
		int ite=0;
		while(ite<PlayerList[i].effect_list.size()){
			if (PlayerList[i].effect_list[ite].first==0) {
				PlayerSprList.DrawTile(14-((PlayerList[i].effect_list[ite].second+1)/2),PlayerList[i].col_pos,PlayerList[i].row_pos,MainWindow);
			}
			--PlayerList[i].effect_list[ite].second;
			if (PlayerList[i].effect_list[ite].second==0) PlayerList[i].effect_list.erase(PlayerList[i].effect_list.begin()+ite);
			else ++ite;
		}
	}
}

void PlayerManager::Damage(int id,int amt){
	if (amt>0){
		if (PlayerList[id].invul>0) return;
		int tamt=amt;
		if (amt>PlayerList[id].now_hp) tamt=PlayerList[id].now_hp;
		Globals->GlobalDataManager->DealDamage(id,tamt);
		
	}
	if (amt<0){
		PlayerList[id].effect_list.push_back(std::make_pair(0,12));
	}
	PlayerList[id].now_hp-=amt;
	if (PlayerList[id].now_hp>PlayerList[id].max_hp) PlayerList[id].now_hp=PlayerList[id].max_hp;
	if (PlayerList[id].now_hp<0) PlayerList[id].now_hp=0;
}

void PlayerManager::Disable(int id,int amt){
	if (PlayerList[id].invul>0) return;
	PlayerList[id].disabled=std::max(PlayerList[id].disabled,amt);
}

void PlayerManager::Invul(int id,int amt){
	PlayerList[id].invul=std::max(PlayerList[id].invul,amt);
}

void PlayerManager::Stun(int id,int amt){
	if (PlayerList[id].invul>0) return;
	PlayerList[id].stunned=std::max(PlayerList[id].stunned,amt);
}

void PlayerManager::ChgMaxSpeed(int id,double amt){
	PlayerList[id].max_move_speed+=amt;
	if (PlayerList[id].max_move_speed<2.0f) PlayerList[id].max_move_speed=2;
	if (PlayerList[id].max_move_speed>10.0f) PlayerList[id].max_move_speed=10;
}

void PlayerManager::ChgTmpSpeed(int id,double mul,int dur){
	if (PlayerList[id].invul>0&&mul<1.0f) return;
	if (dur>0) PlayerList[id].speed_modifier.push_back(make_pair(mul,dur));
}

void PlayerManager::Debuff(int id){
	PlayerList[id].speed_modifier.clear();
	PlayerList[id].disabled=0;
	PlayerList[id].stunned=0;
}

bool PlayerManager::CalculateLogic(){ //if a player is dead, returns false
	bool ret=true;
	for(int i=0; i<PlayerList.size(); ++i){
		ret=ret&PlayerList[i].Logic();
		--PlayerList[i].disabled;
		PlayerList[i].Move();
	}
	return ret;
}

bool PlayerManager::IsDisabled(int id){ //if the player is disabled or stunned, return true (incapable of using weapons)
	return (PlayerList[id].disabled>0||PlayerList[id].stunned);
}

bool PlayerManager::IsDead(int id){					//checks if player is dead.
	return (PlayerList[id].now_hp<=0);
}

std::pair<int,int> PlayerManager::GetPosition(int id){
	return std::make_pair(PlayerList[id].row_pos,PlayerList[id].col_pos);
}

std::pair<int,int> PlayerManager::GetCurPos(int id){
	return std::make_pair(PlayerList[id].cur_row,PlayerList[id].cur_col);
}

std::pair<int,int> PlayerManager::GetNexPos(int id){
	return std::make_pair(PlayerList[id].nex_row,PlayerList[id].nex_col);
}

void PlayerManager::LoadPlayer(){ 
	std::string filename="Data/player.xml";
	std::ifstream DataFile(filename); 
	//if (!DataFile) throw "Could not load tileset: " +filename;  
	std::string DataContent="",temp;
	while(std::getline(DataFile,temp)) DataContent+=temp;
	std::vector<char> XMLData = std::vector<char>(DataContent.begin(),DataContent.end());
	XMLData.push_back('\0'); 
	
	xml_document<> Data;
	Data.parse<parse_no_data_nodes>(&XMLData[0]);

	xml_node<>* root=Data.first_node();
	xml_node<>* colors=root->first_node("colors");  
	xml_node<>* hpbar=root->first_node("hpbar"); 
	xml_node<>* tiles;
	//get texture dir
	colamt=atoi(colors->first_attribute("amount")->value());

	std::string imagedir; sf::Texture imagetext; 
	imagedir=colors->first_attribute("path")->value(); imagetext.loadFromFile(imagedir); 
	PlayerTextureManager.AddTexture(imagetext); 
	imagedir=hpbar->first_attribute("path")->value();imagetext.loadFromFile(imagedir); 
	PlayerTextureManager.AddTexture(imagetext); 
	//get tiles dir
	
	tiles = colors->first_node("tile");
	while(tiles){
		int x,y,xsiz,ysiz;
		x=atoi(tiles->first_attribute("x")->value());
		y=atoi(tiles->first_attribute("y")->value());
		xsiz=atoi(tiles->first_attribute("xsiz")->value());
		ysiz=atoi(tiles->first_attribute("ysiz")->value());
		PlayerSprList.AddTile(PlayerTextureManager.GetTexture(0),x,y,xsiz,ysiz); 
		tiles=tiles->next_sibling(); 
	}
	 
	tiles = hpbar->first_node("tile");
	while(tiles){
		int x,y,xsiz,ysiz;
		x=atoi(tiles->first_attribute("x")->value());
		y=atoi(tiles->first_attribute("y")->value());
		xsiz=atoi(tiles->first_attribute("xsiz")->value());
		ysiz=atoi(tiles->first_attribute("ysiz")->value());
		HPBarList.AddTile(PlayerTextureManager.GetTexture(1),x,y,xsiz,ysiz); 
		tiles=tiles->next_sibling();
	}

	hpbar=hpbar->next_sibling();
	tiles = hpbar->first_node("tile");
	while(tiles){
		int x,y,xsiz,ysiz;
		x=atoi(tiles->first_attribute("x")->value());
		y=atoi(tiles->first_attribute("y")->value());
		xsiz=atoi(tiles->first_attribute("xsiz")->value());
		ysiz=atoi(tiles->first_attribute("ysiz")->value());
		NumberList.AddTile(PlayerTextureManager.GetTexture(1),x,y,xsiz,ysiz); 
		tiles=tiles->next_sibling();
	}
}
