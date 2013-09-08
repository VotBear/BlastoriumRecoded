#include "Select.h"

#include <vector>
#include <cfloat>
#include <string>
#include <fstream>
#include <ctime>
#include <stdlib.h>

#include "Interface.h"
#include "Tilelist.h"
#include "TextureManager.h"
#include "Data.h"

#include "rapidxml.hpp"

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

using namespace std;
using namespace rapidxml;

#define Width 6
#define Heigt 2
#define Dist 100
#define Stageamt 7

SelectManager::SelectManager(){
	InitSelect();
	Ret=vector<int>(6,-1);
	Init();
	return;
}

SelectManager::~SelectManager(){
	return;
}

//initialize
void SelectManager::InitSelect(){
	WpnS1=0; 
	WpnS2=0; 
	SelS1=0; 
	SelS2=5; 
	StgSel=0;
	SelectTextureManager.ClearTexture();
	Ret.clear();
	Ret=vector<int>(6,-1);
	/////////////////////////
	//XML Read & Data load //
	/////////////////////////
	std::string filename="Data/stageselect.xml";
	std::ifstream DataFile(filename);  
	std::string DataContent="",temp;
	while(std::getline(DataFile,temp)) DataContent+=temp;
	std::vector<char> XMLData = std::vector<char>(DataContent.begin(),DataContent.end());
	XMLData.push_back('\0'); 
	xml_document<> Data;	Data.parse<parse_no_data_nodes>(&XMLData[0]);
	
	xml_node<>* root=Data.first_node(); 
	xml_node<>* tiles; 
	std::string imagedir; sf::Texture imagetext; 

	tiles=root->first_node("imagefile")->first_node(); 
	while (tiles) {
		imagedir=tiles->first_attribute("dir")->value();
		imagetext.loadFromFile(imagedir); 
		SelectTextureManager.AddTexture(imagetext); 
		tiles=tiles->next_sibling();
	}
	//get tiles dir
	root=root->first_node("tiles");
	int texture_no=0;
	while(root){
		tiles = root->first_node("tile");
		while(tiles){
			int x,y,xsiz,ysiz;
			x		 =atoi(tiles->first_attribute("x")->value());
			y		 =atoi(tiles->first_attribute("y")->value());
			xsiz	 =atoi(tiles->first_attribute("xsiz")->value());
			ysiz	 =atoi(tiles->first_attribute("ysiz")->value());  

			SelectTilelist[texture_no].AddTile(SelectTextureManager.GetTexture(texture_no),x,y,xsiz,ysiz); 
			tiles=tiles->next_sibling();
		}
		++texture_no;
		root=root->next_sibling();
	}
	return;

	//////////////////////
	//end of xml read	//
	//////////////////////
}

//Checks if weapon is available based on prequisites and statistics
void SelectManager::CheckValid(){
	unique_ptr<DataManager> Dat(new DataManager()); 
	IsValid[0]=true;									//bomb
	IsValid[1]=true;									//mine
	IsValid[2]=true;									//rocketLauncher
	IsValid[3]=true;		//medikit
	IsValid[4]=false;		//shuriken
	IsValid[5]=false;		//blade
	IsValid[6]=false;		//remoteBomb
	IsValid[7]=false;		//statisTrap
	IsValid[8]=false;		//turret
	IsValid[9]=false;		//teleportOrb
	IsValid[10]=false;		//flameThrower
	IsValid[11]=false;		//blazeCloak
	/*
	IsValid[3]=Dat->MainData.PowerupsTaken>=750;		//medikit
	IsValid[4]=Dat->MainData.DamageDealt>=10000;		//shuriken
	IsValid[5]=Dat->MainData.MatchesPlayed>=200;		//blade
	IsValid[6]=Dat->MainData.BlocksDestroyed>=1500;		//remoteBomb
	IsValid[7]=Dat->MainData.WeaponsStat[1]>=200;		//statisTrap
	IsValid[8]=Dat->MainData.WeaponsStat[2]>=100;		//turret
	IsValid[9]=Dat->MainData.WeaponsStat[3]>=12000;		//teleportOrb
	IsValid[10]=Dat->MainData.WeaponsStat[4]>=400;		//flameThrower
	IsValid[11]=Dat->MainData.WeaponsStat[5]>=300;		//blazeCloak
	*/
	return;
}

//Get weapon title for printing
string SelectManager::GetTitle(int WpnNo){
	if (WpnNo==-1) return "?";
	if (WpnNo==0)  return "Bomb";
	if (WpnNo==1)  return "Mine";
	if (WpnNo==2)  return "Rocket Launcher";
	if (WpnNo==3)  return "Medikit";	//Medikit
	if (WpnNo==4)  return "?";	//Shurikens
	if (WpnNo==5)  return "?";	//Blade
	if (WpnNo==6)  return "?";	//Remote Bomb
	if (WpnNo==7)  return "?";	//Stasis Trap
	if (WpnNo==8)  return "?";	//Turret
	if (WpnNo==9)  return "?";	//Teleport Orb
	if (WpnNo==10) return "?";	//Flamethrower
	if (WpnNo==11) return "?";	//Blaze Cloak
}

//Get weapon description (unused)
string SelectManager::GetDescr(int WpnNo,int Done){
	if (Done==1){
		if (WpnNo==0)  return "Plants a bomb that explodes in ...\n2.5 secs, dealing heavy damage over \ntime on a large radius.            \nLevel 5 pierces";	//bomb
		if (WpnNo==1)  return "Places a mine on the ground that  \nturns invisible and activates over  \ntime, dealing damage on contact.   \nLevel 5 reduces friendly fire damage";	//mine
		if (WpnNo==2)  return "Fires a rocket at enemy location, \ndealing mediocre damage and heavily \ncrippling the target heavily on hit\nLevel 5 stuns target"; //rocket
		if (WpnNo==3)  return "?";	//Medikit
		if (WpnNo==4)  return "?";	//Shurikens
		if (WpnNo==5)  return "?";	//Blade
		if (WpnNo==6)  return "?";	//Remote Bomb
		if (WpnNo==7)  return "?";	//Stasis Trap
		if (WpnNo==8)  return "?";	//Turret
		if (WpnNo==9)  return "?";	//Teleport Orb
		if (WpnNo==10) return "?";	//Flamethrower
		if (WpnNo==11) return "?";	//Blaze Cloak
	} else {
		if (WpnNo==3)  return "Pick 750 Powerups to unlock";	//Medikit
		if (WpnNo==4)  return "Deal 100000 Damage to unlock";	//Shurikens
		if (WpnNo==5)  return "Play 150 Rounds to unlock";		//Blade
		if (WpnNo==6)  return "Destroy 1500 Blocks to unlock";	//Remote Bomb
		if (WpnNo==7)  return "Hit 160 Mines to unlock";		//Stasis Trap
		if (WpnNo==8)  return "Hit 80 Rockets to unlock";		//Turret
		if (WpnNo==9)  return "Heal 12000 Damage with Medikit to unlock";	//Teleport Orb
		if (WpnNo==10) return "Hit 400 Shurikens to unlock";				//Flamethrower
		if (WpnNo==11) return "Hit a slash 240 times to unlock";			//Blaze Cloak
	}
}
 
//Renders the stage select (part of RenderSelect, but separated due to size)
void SelectManager::RenderStage (shared_ptr<sf::RenderWindow> Win){	
	int WinX=Win->getSize().x;

	//draw arrow keys
	SelectTilelist[2].SetRotation(1,90);
	SelectTilelist[2].SetOriginCenter(1);
	if (CurPos%100==0) SelectTilelist[2].DrawTile(1,WinX/2+140,310,Win); 
	else			   SelectTilelist[2].DrawTile(1,WinX/2+140,310,80,Win);  
	SelectTilelist[2].SetRotation(1,270);
	SelectTilelist[2].SetOriginCenter(1);
	if (CurPos%100==0) SelectTilelist[2].DrawTile(1,WinX/2-140,310,Win); 
	else			   SelectTilelist[2].DrawTile(1,WinX/2-140,310,80,Win);

	//draw the stage minimaps
	for(int stg=0;stg<Stageamt;++stg){
		int StgPos=stg*Dist;
		int toLef=CurPos-StgPos; if (toLef<0) toLef+=Dist*Stageamt;	//How far it is to the left of CurPos
		int toRgt=StgPos-CurPos; if (toRgt<0) toRgt+=Dist*Stageamt;	//How far it is to the right of CurPos

		int Distance=min(toLef,toRgt);

		SelectTilelist[0].SetScale(stg,float(200.0f-Distance)/200.0f,float(200.0f-Distance)/200.0f);
		SelectTilelist[0].SetColor(stg,255*float(200.0f-Distance)/200.0f);
		SelectTilelist[0].SetOriginCenter(stg);
		if (toLef<toRgt) {
			SelectTilelist[0].DrawTile(stg,WinX/2-Distance*3,310,Win);
		} else {
			SelectTilelist[0].DrawTile(stg,WinX/2+Distance*3,310,Win);
		}
	}  
	return;
}

//Renders the select screen
void SelectManager::RenderSelect (shared_ptr<sf::RenderWindow> Win){
	sf::Font Fnt1,Fnt2;
	sf::Text Titl1,Titl2,Help[6],Warnin;
	Fnt1.loadFromFile("Fonts/bankgthd.ttf");
	Fnt2.loadFromFile("Fonts/consola.ttf");
	Titl1.setFont(Fnt1); Titl1.setCharacterSize(23);	
	Titl2.setFont(Fnt1); Titl2.setCharacterSize(23);	

	Win->clear();
	Win->draw(BG); 
	
	//draw selection pointers
	int stx=(int)(Win->getSize().x/2) - 3*48, sty = 70;
	SelectTilelist[1].DrawTile(14   , stx + 48 * (SelS1 % Width), sty + 48 * (SelS1 / Width), Win);
	SelectTilelist[1].DrawTile(15   , stx + 48 * (SelS2 % Width), sty + 48 * (SelS2 / Width), Win); 
	
	//draw selected weapons
	for(int i=0;i<WpnS1;++i){
		SelectTilelist[1].DrawTile(	17	  ,	0				   ,	sty+48*i  ,	Win );
		SelectTilelist[1].DrawTile(	Ret[i],	8				   ,	sty+48*i+8,	Win );
		Titl1.setString(GetTitle(Ret[i]));	
		Titl1.setOrigin(0								,Titl1.getGlobalBounds().height/2);	
		Titl1.setPosition(48.0f,82.0f+48.0f*i); 
		Win->draw(Titl1);
	}
	for(int i=0;i<WpnS2;++i){
		SelectTilelist[1].DrawTile(	17		,	Win->getSize().x-48,	sty+48*i  ,	Win );
		SelectTilelist[1].DrawTile(	Ret[i+3],	Win->getSize().x-40,	sty+48*i+8,	Win );
		Titl2.setString(GetTitle(Ret[i+3]));
		Titl2.setOrigin(Titl2.getGlobalBounds().width	,Titl2.getGlobalBounds().height/2);
		Titl2.setPosition(Win->getSize().x-50,82+48*i); 
		Win->draw(Titl2);  
	}
	
	//draw currently selected weapon
	if (WpnS1>=0&&WpnS1<3) {
		SelectTilelist[1].DrawTile( SelS1,	8				   ,	sty+48*WpnS1+8,	140, Win);
		Titl1.setColor(sf::Color(255,255,255,140));
		Titl1.setString(GetTitle(SelS1));	
		Titl1.setOrigin(0								,Titl1.getGlobalBounds().height/2);	
		Titl1.setPosition(48,82+48*WpnS1); 
		Win->draw(Titl1);
	}
	if (WpnS2>=0&&WpnS2<3) {
		SelectTilelist[1].DrawTile( SelS2,	Win->getSize().x-40,	sty+48*WpnS2+8,	140, Win);
		Titl2.setColor(sf::Color(255,255,255,140));
		Titl2.setString(GetTitle(SelS2));
		Titl2.setOrigin(Titl2.getGlobalBounds().width	,Titl2.getGlobalBounds().height/2);
		Titl2.setPosition(Win->getSize().x-50,82+48*WpnS2); 
		Win->draw(Titl2);
	}

	RenderStage(Win);

	//shade based on current stage
	sf::RectangleShape Shade;
	Shade.setFillColor(sf::Color(0,0,0,170));
	if (WpnS1<3||WpnS2<3){
		Shade.setSize(sf::Vector2f(Win->getSize().x,270));
		Shade.setPosition(0,215);
	} else {
		Shade.setSize(sf::Vector2f(Win->getSize().x,146));
		Shade.setPosition(0,70); 
	}
	Win->draw(Shade);
	
	//draw help
	Help[0].setString("WASD : move");
	Help[1].setString("E    : select");
	Help[2].setString("Q    : cancel");
	Help[3].setString("Arrow Keys  : move");
	Help[4].setString("Right Shift : select");
	Help[5].setString("Right Ctrl  : cancel");
	for(int i=0;i<6;++i){
		Help[i].setFont(Fnt1);
		Help[i].setCharacterSize(12);
		if (i>=3) {
			Help[i].setOrigin(Help[i].getGlobalBounds().width,0);
			Help[i].setPosition(Win->getSize().x-2,Win->getSize().y-42+14*(i-3));
		}
		else {
			Help[i].setPosition(2				  ,Win->getSize().y-42+14*(i));
		}
		Win->draw(Help[i]);
	}

	//draw warning
	Warnin.setFont(Fnt1);
	Warnin.setCharacterSize(13);
	Warnin.setString("You have to choose at least 1 Block Destroying Weapon ");
	for(int i=0;i<2;++i) if (Warn[i]>0){
		Warnin.setOrigin(i*Warnin.getGlobalBounds().width,Warnin.getGlobalBounds().height);
		Warnin.setPosition(2+i*(Win->getSize().x-4),Win->getSize().y-46);
		Warnin.setColor(sf::Color(255,0,0,std::min(255,Warn[i]*10)));
		Win->draw(Warnin);
		--Warn[i];
	}
	return;
} 

//moves stage selection to direction Dir
void SelectManager::MoveStage(int Dir,shared_ptr<sf::RenderWindow> Win){
	//20 loops to move directions.
	for(int i=1;i<=12;++i){
		int Spd;
		if		(i>=5&&i<=8)  Spd=15;
		else if (i>=4&&i<=9)  Spd=10;
		else if (i>=3&&i<=10) Spd=6;
		else if (i>=2&&i<=11) Spd=3;
		else				  Spd=1;

		int Mov=Dir*Spd;
		CurPos=(CurPos+Mov+(Stageamt*Dist))%(Stageamt*Dist); 
		sf::Event Evt;
		while (Win->pollEvent(Evt));
		RenderSelect(Win);
		Win->display();
	} 
	StgSel=(StgSel+Stageamt+Dir)%Stageamt;	//record movement of selection
	return;
}

//back to main menu, launch end animation
void SelectManager::CancelSelection (shared_ptr<sf::RenderWindow> Win){  
	RenderSelect(Win);
	sf::RenderWindow &TRender=*Win;   
	sf::Texture Temp;
	Temp.create(Win->getSize().x,Win->getSize().y); 
	Temp.update(TRender); 

	EndScreen(Win,Temp);
	return;
}

//restarts everything
void SelectManager::Init(){
	for(int ite=0;ite<6;++ite) Ret[ite]=-1;
	memset(IsValid,false,sizeof IsValid);
	Fin=false;
	WpnS1=0; 
	WpnS2=0; 
	SelS1=0; 
	SelS2=5; 
	StgSel=0;
	CurPos=0;  
}

//The main function, called from Main()
//Initializes background, handles input, calls the other functions
//Returns a pair of int (stage no, 0..7) and a vector of int (player 1 and 2's weapons, 0..weapon id)
pair < int, vector<int> > SelectManager::SelectScreen(shared_ptr<sf::RenderWindow> Win){ 
	
	//Initialization//
	Init();
	CheckValid();

	//Initial render//
	sf::Font Fnt; 
	Fnt.loadFromFile("Fonts/bankgthd.ttf");

	sf::Text Title,Nm1,Nm2,Desc1,Desc2; 
	
	sf::RenderTexture SelectWeapon;
	SelectWeapon.create(Win->getSize().x,Win->getSize().y);	SelectWeapon.clear(sf::Color::Black);  
	
	Title.setFont(Fnt); 
	Title.setCharacterSize(40);
	Title.setString("Weapon Select");
	Title.setOrigin(Title.getGlobalBounds().width/2,Title.getGlobalBounds().height/2);
	Title.setPosition(float(Win->getSize().x)/2,10.0f);
	SelectWeapon.draw(Title);

	int stx=(int)(Win->getSize().x/2) - 3*48, sty = 70;
	for(int i=0;i<6;++i) for(int j=0;j<2;++j) {
		SelectTilelist[1].DrawTile(	16   ,	stx+48*i	,		sty+48*j	,	&SelectWeapon );
		SelectTilelist[1].DrawTile(	6*j+i,	stx+48*i+8	,		sty+48*j+8	,	&SelectWeapon );
	}
	for(int j=0;j<3;++j) {
		SelectTilelist[1].DrawTile(	16	,	0				   ,	sty+48*j,	&SelectWeapon );
		SelectTilelist[1].DrawTile(	16	,	Win->getSize().x-48,	sty+48*j,	&SelectWeapon );
	}
	SelectTilelist[1].DrawTile(12,0					 ,	0,	&SelectWeapon);
	SelectTilelist[1].DrawTile(13,Win->getSize().x-64,	0,	&SelectWeapon);

	SelectWeapon.display();
	sf::Texture Temp;

	BGTex=SelectWeapon.getTexture();	BG.setTexture(BGTex);

	RenderSelect(Win); 
	sf::RenderWindow &TRender=*Win;   
	Temp.create(Win->getSize().x,Win->getSize().y); 
	Temp.update(TRender); 

	StartScreen(Win,Temp);

	//Main loop//

	sf::Event Evt;  
	while(Win->pollEvent(Evt));
	while(!Fin){   
		//first part: Weapon selection
		if (WpnS1<3||WpnS2<3) while(Win->pollEvent(Evt)){	
			if (Evt.type==sf::Event::Resized){
				Win->setSize(sf::Vector2u(864,480));
			} else if (Evt.type==sf::Event::Closed){
				Win->close();
				return make_pair(0,Ret); 

			} else if (Evt.type==sf::Event::KeyReleased&&Evt.key.code==sf::Keyboard::Tilde){
				Win->capture().saveToFile("Images/Screenshots/Screen.png");

			} else if (Evt.type==sf::Event::KeyReleased){
					 if (Evt.key.code==sf::Keyboard::W)		SelS1=(SelS1+Width) % (Width*Heigt);
				else if (Evt.key.code==sf::Keyboard::S)		SelS1=(SelS1+Width*Heigt-Width) % (Width*Heigt);
				else if (Evt.key.code==sf::Keyboard::D)		SelS1=Width*(SelS1/Width) + ((SelS1+1) % Width);
				else if (Evt.key.code==sf::Keyboard::A)		SelS1=Width*(SelS1/Width) + ((SelS1+Width-1) % Width);
				else if (Evt.key.code==sf::Keyboard::E)	{ //select
					if (WpnS1>=3) continue;		if (!IsValid[SelS1]) continue;
					bool Same=false,Fnd=(SelS1==0);	
					for (int i=0;i<WpnS1;++i) {
						if (Ret[i]==SelS1) Same=true;
						if (Ret[i]==0)     Fnd=true;
					}
					if (!Fnd&&WpnS1==2){
						Warn[0]=75;
						continue;
					}
					if (Same) continue;
					Ret[0+WpnS1]=SelS1;
					++WpnS1;
				} 
				else if (Evt.key.code==sf::Keyboard::Q)	{ //deselect
					--WpnS1;
					if (WpnS1<0){
						CancelSelection(Win);
						return make_pair(-1,Ret);
					}
					Ret[0+WpnS1]=-1;
				}

				else if (Evt.key.code==sf::Keyboard::Up)	SelS2=(SelS2+Width) % (Width*Heigt);
				else if (Evt.key.code==sf::Keyboard::Down)	SelS2=(SelS2+Width*Heigt-Width) % (Width*Heigt);
				else if (Evt.key.code==sf::Keyboard::Right)	SelS2=Width*(SelS2/Width) + ((SelS2+1) % Width);
				else if (Evt.key.code==sf::Keyboard::Left)  SelS2=Width*(SelS2/Width) + ((SelS2+Width-1) % Width);
				else if (Evt.key.code==sf::Keyboard::RShift)	{ //select
					if (WpnS2>=3) continue;		if (!IsValid[SelS2]) continue;
					bool Same=false,Fnd=(SelS2==0);	
					for (int i=0;i<WpnS2;++i) {
						if (Ret[3+i]==SelS2) Same=true;
						if (Ret[3+i]==0)     Fnd=true;
					}
					if (!Fnd&&WpnS2==2){
						Warn[1]=75;
						continue;
					}
					if (Same) continue;
					Ret[3+WpnS2]=SelS2;
					++WpnS2;
				} 
				else if (Evt.key.code==sf::Keyboard::RControl)	{ //deselect
					--WpnS2;
					if (WpnS2<0){
						CancelSelection(Win);
						return make_pair(-1,Ret);
					}
					Ret[3+WpnS2]=-1;
				}

				else if (Evt.key.code==sf::Keyboard::Space||Evt.key.code==sf::Keyboard::Return){

				} else if (Evt.key.code==sf::Keyboard::Escape){
					return make_pair(-1,Ret);
				}  
			} 
		}
		//Second part: Select stage
		else while (Win->pollEvent(Evt)){  
			if (Evt.type==sf::Event::Resized){
				Win->setSize(sf::Vector2u(864,480));
			} else if (Evt.type==sf::Event::Closed){
				Win->close();
				return make_pair(0,Ret); 

			} else if (Evt.type==sf::Event::KeyReleased&&Evt.key.code==sf::Keyboard::Tilde){
				Win->capture().saveToFile("Images/Screenshots/Screen.png");

			} else if (Evt.type==sf::Event::KeyReleased){  
				if (Evt.key.code==sf::Keyboard::A	||	Evt.key.code==sf::Keyboard::Left){ 
					MoveStage(-1,Win); 
				
				} else if (Evt.key.code==sf::Keyboard::D	||	Evt.key.code==sf::Keyboard::Right){ 
					MoveStage(1,Win); 
				
				} else if (Evt.key.code==sf::Keyboard::E		||	Evt.key.code==sf::Keyboard::RShift	||
						   Evt.key.code==sf::Keyboard::Return	||	Evt.key.code==sf::Keyboard::Space )	{ //select	 
					RenderSelect(Win); 
					sf::RenderWindow &TRender=*Win;   
					Temp.create(Win->getSize().x,Win->getSize().y); 
					Temp.update(TRender); 

					EndScreen(Win,Temp);
					return make_pair(StgSel+1,Ret);

				} 

				else if (Evt.key.code==sf::Keyboard::Q)	{ //deselect 
					--WpnS1;
					if (WpnS1<0) {					 
						CancelSelection(Win);
						return make_pair(-1,Ret);
					}
					Ret[0+WpnS1]=-1;
				
				}  
				else if (Evt.key.code==sf::Keyboard::RControl)	{ //deselect
					--WpnS2;
					if (WpnS2<0) {
						CancelSelection(Win);
						return make_pair(-1,Ret);
					}
					Ret[3+WpnS2]=-1;
				
				} else if (Evt.key.code==sf::Keyboard::Escape){
					CancelSelection(Win);
					return make_pair(-1,Ret);
					printf("If you're reading this something is TERRIBLY wrong\n");
				
				}  
			} 
		}  
		RenderSelect(Win);
		Win->display();  
	} 
	return make_pair(-1,Ret);
}