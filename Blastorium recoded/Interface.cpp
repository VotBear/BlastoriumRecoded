#include "Interface.h"

#include <vector>
#include <cfloat>
#include <string>
#include <fstream>
#include <ctime>
#include <stdlib.h>
#include "Tilelist.h"
#include "TextureManager.h"
#include "Data.h"
#include "rapidxml.hpp"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace rapidxml;


/*HOW TO CAPTURE THE WINDOW TO TEXTURE
	sf::RenderWindow &temp=*MainWindow;
		--> converts window pointer to reference for function update(&window)
	sf::Texture Back; 
		--> create a new texture
	Back.create(MainWindow->getSize().x,MainWindow->getSize().y);
		--> IMPORTANT: set the texture size first, Update() wont check size so it has to be done manually
	Back.update(temp);
		--> update with the window reference
	EndScreen(MainWindow,Back); 
	StartScreen(MainWindow,Back);
		--> usage samples
	
	//Half hour well spent on learning pointers, references, sf::texture and shit//
*/


void EndScreen(shared_ptr<sf::RenderWindow> Win, sf::Texture &bg){ 
	sf::Sprite Back; Back.setTexture(bg);

	for (int i=0;i<=116;i+=4){
		Win->clear();
		Win->draw(Back);
		for (int row=0;row<15;++row){
			int hgt; //min 0, max 32
			hgt=i-6*row;
			if (hgt<0) hgt=0; if (hgt>32) hgt=32;
			sf::RectangleShape Shade(sf::Vector2f(864,hgt));
			Shade.setPosition(0,row*32); Shade.setFillColor(sf::Color(10,10,15));
			Win->draw(Shade);
		}
		Win->display();
	}
	
}

void StartScreen(shared_ptr<sf::RenderWindow> Win, sf::Texture &bg){
	sf::Sprite Back; Back.setTexture(bg);  
	for (int i=116;i>=0;i-=4){
		Win->clear();
		Win->draw(Back);
		for (int row=0;row<15;++row){
			int hgt; //min 0, max 32
			int tr=14-row;
			hgt=i-6*tr;
			if (hgt<0) hgt=0; if (hgt>32) hgt=32;
			sf::RectangleShape Shade(sf::Vector2f(864,hgt));
			Shade.setOrigin(0,hgt);
			Shade.setPosition(0,(row+1)*32); Shade.setFillColor(sf::Color(10,10,15));
			Win->draw(Shade);
		}
		Win->display();
	} 
	
} 
 
void TEndScreen(shared_ptr<sf::RenderWindow> Win, int ite){
	for (int row=0;row<15;++row){
		int hgt; //min 0, max 32
		hgt=ite-6*row;
		if (hgt<0) hgt=0; if (hgt>32) hgt=32;
		sf::RectangleShape Shade(sf::Vector2f(864,hgt));
		Shade.setPosition(0,row*32); Shade.setFillColor(sf::Color(10,10,15));
		Win->draw(Shade);
	}
	
}
 
void TStartScreen(shared_ptr<sf::RenderWindow> Win, int ite){
	for (int row=0;row<15;++row){
		int hgt; //min 0, max 32
		int tr=14-row;
		hgt=ite-6*tr;
		if (hgt<0) hgt=0; if (hgt>32) hgt=32;
		sf::RectangleShape Shade(sf::Vector2f(864,hgt));
		Shade.setOrigin(0,hgt);
		Shade.setPosition(0,(row+1)*32); Shade.setFillColor(sf::Color(10,10,15));
		Win->draw(Shade);
	} 
	
}
 
int EndGameChoice(shared_ptr<sf::RenderWindow> Win, sf::Texture &bg){ 
	sf::Sprite Back; Back.setTexture(bg);  
	sf::Font Fnt; Fnt.loadFromFile("Fonts/bankgthd.ttf");
	sf::Text Play,Exit,Menu;
	sf::Event Evt;
	int CurSelect=2;

	Play.setString("Play Again");	Play.setFont(Fnt);
	Menu.setString("Main Menu");	Menu.setFont(Fnt);
	Exit.setString("Exit");			Exit.setFont(Fnt);

	while (true){
		Win->clear();
		Win->draw(Back);
		while(Win->pollEvent(Evt)){
			if (Evt.type==sf::Event::KeyReleased){
				if (Evt.key.code==sf::Keyboard::Escape){
					Win->close();
					return 0;

				} else if (Evt.key.code==sf::Keyboard::Tilde){
					Win->capture().saveToFile("Images/Screenshots/Screen.png");

				} else if (Evt.key.code==sf::Keyboard::Left||Evt.key.code==sf::Keyboard::A) {
					CurSelect=(CurSelect+1)%3;	//<-

				} else if (Evt.key.code==sf::Keyboard::Right||Evt.key.code==sf::Keyboard::D) {
					CurSelect=(CurSelect+2)%3;	//->

				} else if (Evt.key.code==sf::Keyboard::Space||Evt.key.code==sf::Keyboard::Return) {
					if (CurSelect>=1){			//play again or main menu
						sf::RenderWindow &temp=*Win;
						Win->draw(Play); Win->draw(Menu); Win->draw(Exit); bg.update(temp);
						EndScreen(Win,bg);
					
					} else if (CurSelect==0)	//exit
						Win->close();	

					return CurSelect;
				}
			} else if (Evt.type==sf::Event::MouseMoved){
				sf::Vector2f MPos;
				MPos.x=sf::Mouse::getPosition(*Win).x;
				MPos.y=sf::Mouse::getPosition(*Win).y;
				
				if (Play.getGlobalBounds().contains(MPos)){
					CurSelect=2;
				
				} else if (Menu.getGlobalBounds().contains(MPos)){
					CurSelect=1;
				
				} else if (Exit.getGlobalBounds().contains(MPos)){
					CurSelect=0; 
				} 

			} else if (Evt.type==sf::Event::MouseButtonPressed){
				sf::Vector2f MPos;
				MPos.x=sf::Mouse::getPosition(*Win).x;
				MPos.y=sf::Mouse::getPosition(*Win).y;
				
				if (Play.getGlobalBounds().contains(MPos)||Menu.getGlobalBounds().contains(MPos)){
					sf::RenderWindow &temp=*Win;
					Win->draw(Play); Win->draw(Menu); Win->draw(Exit); bg.update(temp);
					EndScreen(Win,bg);
					if (Play.getGlobalBounds().contains(MPos)) return 2;
					if (Menu.getGlobalBounds().contains(MPos)) return 1; 
	
				} else if (Exit.getGlobalBounds().contains(MPos)){
					Win->close();
					return 0;
				}
			
			} else if (Evt.type==sf::Event::Closed){
				Win->close();
				return 0;
			
			}
		}
		Play.setCharacterSize(25); Play.setColor(sf::Color(255,255,255,120));
		Exit.setCharacterSize(25); Exit.setColor(sf::Color(255,255,255,120));
		Menu.setCharacterSize(25); Menu.setColor(sf::Color(255,255,255,120));
		
		if (CurSelect==2) {
			Play.setCharacterSize(40); Play.setColor(sf::Color::White);

		} else if (CurSelect==1) {
			Menu.setCharacterSize(40); Menu.setColor(sf::Color::White);

		} else {
			Exit.setCharacterSize(40); Exit.setColor(sf::Color::White); 
		}
		Play.setOrigin(Play.getGlobalBounds().width/2,Play.getGlobalBounds().height/2); 
		Menu.setOrigin(Menu.getGlobalBounds().width/2,Menu.getGlobalBounds().height/2); 
		Exit.setOrigin(Exit.getGlobalBounds().width/2,Exit.getGlobalBounds().height/2); 

		Play.setPosition((Win->getSize().x)/6  ,400-Play.getGlobalBounds().height);	Win->draw(Play);
		Menu.setPosition((Win->getSize().x*3)/6,400-Menu.getGlobalBounds().height);	Win->draw(Menu);
		Exit.setPosition((Win->getSize().x*5)/6,400-Exit.getGlobalBounds().height);	Win->draw(Exit);
		 
		Win->display();
	}
	return -1;
}

int PauseGameChoice(shared_ptr<sf::RenderWindow> Win, sf::Texture &bg){
	sf::Texture Misc;
	sf::Sprite Back,Lef,Rig; Back.setTexture(bg);   
	sf::Font Fnt; Fnt.loadFromFile("Fonts/bankgthd.ttf");
	sf::Text Pause,Play,Menu,Exit;
	sf::Event Evt;

	Pause.setString("Game Paused");	Pause.setFont(Fnt);
	Pause.setCharacterSize(50); Pause.setColor(sf::Color::White);	
	Pause.setOrigin(Pause.getGlobalBounds().width/2,0);	Pause.setPosition(Win->getSize().x/2,40); 

	Play.setString("Resume");		Play.setFont(Fnt);	Play.setCharacterSize(32); Play.setColor(sf::Color::White);
	Menu.setString("Main Menu");	Menu.setFont(Fnt);	Menu.setCharacterSize(32); Menu.setColor(sf::Color::White); 
	Exit.setString("Exit");			Exit.setFont(Fnt);	Exit.setCharacterSize(32); Exit.setColor(sf::Color::White); 
	Play.setOrigin(Play.getGlobalBounds().width/2,0);	Play.setPosition(Win->getSize().x/2-2,260-7);	
	Menu.setOrigin(Menu.getGlobalBounds().width/2,0);	Menu.setPosition(Win->getSize().x/2-2,330-7);	
	Exit.setOrigin(Exit.getGlobalBounds().width/2,0);	Exit.setPosition(Win->getSize().x/2-2,400-7);	
	
	sf::RectangleShape Screen(sf::Vector2f(Win->getSize().x,Win->getSize().y));
	Screen.setFillColor(sf::Color(0,0,0,215));
	
	int CurSelect=2;

	Misc.loadFromFile("Images/Misc/Misc.png");
	Rig.setTexture(Misc); Rig.setTextureRect(sf::IntRect(0,0,160,32));	
	Lef.setTexture(Misc); Lef.setTextureRect(sf::IntRect(160,0,-160,32)); Lef.setOrigin(160,0); 
	while(Win->pollEvent(Evt)); 
	while(1) {
		while(Win->pollEvent(Evt)){
			if (Evt.type==sf::Event::Closed){
				Win->close();
				exit(0);

			} else if (Evt.type==sf::Event::KeyReleased){
				if (Evt.key.code==sf::Keyboard::Up||Evt.key.code==sf::Keyboard::W){ 
					CurSelect=(CurSelect+1)%3;

				} else if (Evt.key.code==sf::Keyboard::Down||Evt.key.code==sf::Keyboard::S){
					CurSelect=(CurSelect+2)%3;

				} else if (Evt.key.code==sf::Keyboard::Space||Evt.key.code==sf::Keyboard::Return){
					if (CurSelect==0) { 
						Win->close(); 	exit(0);

					} else if (CurSelect==1) { 
						sf::RenderWindow &temp=*Win;
						sf::Texture Back; 
						Back.create(Win->getSize().x,Win->getSize().y);
						Back.update(temp);
						EndScreen(Win,Back); 

					}
					return CurSelect;

				} else if (Evt.key.code==sf::Keyboard::Escape){
					Win->close();
					exit(0);

				} 
			} else if (Evt.type==sf::Event::MouseMoved){
				sf::Vector2f MPos;
				MPos.x=sf::Mouse::getPosition(*Win).x;
				MPos.y=sf::Mouse::getPosition(*Win).y;
				
					 if (Play.getGlobalBounds().contains(MPos))	CurSelect=2;
				else if (Menu.getGlobalBounds().contains(MPos))	CurSelect=1;
				else if (Exit.getGlobalBounds().contains(MPos))	CurSelect=0; 

			} else if (Evt.type==sf::Event::MouseButtonPressed){
				sf::Vector2f MPos;
				MPos.x=sf::Mouse::getPosition(*Win).x;
				MPos.y=sf::Mouse::getPosition(*Win).y; 

				if (Play.getGlobalBounds().contains(MPos)){
					return 2;

				} else if (Menu.getGlobalBounds().contains(MPos)){ 
					sf::RenderWindow &temp=*Win;
					sf::Texture Back; 
					Back.create(Win->getSize().x,Win->getSize().y);
					Back.update(temp);
					EndScreen(Win,Back); 
					return 1;

				} else if (Exit.getGlobalBounds().contains(MPos)){
					Win->close();
					exit(0);
					return 0;
				} 
			} 
		}
		if (CurSelect==2){
			Lef.setPosition(Win->getSize().x/2-Play.getGlobalBounds().width/2,260);
			Rig.setPosition(Win->getSize().x/2+Play.getGlobalBounds().width/2,260);
		} else if (CurSelect==1){
			Lef.setPosition(Win->getSize().x/2-Menu.getGlobalBounds().width/2,330);
			Rig.setPosition(Win->getSize().x/2+Menu.getGlobalBounds().width/2,330);
		} else {
			Lef.setPosition(Win->getSize().x/2-Exit.getGlobalBounds().width/2,400);
			Rig.setPosition(Win->getSize().x/2+Exit.getGlobalBounds().width/2,400);
		}
		Win->clear();	
		Win->draw(Back); 	Win->draw(Screen);	Win->draw(Pause); 
		Win->draw(Play);	Win->draw(Menu);	Win->draw(Exit);
		Win->draw(Lef);		Win->draw(Rig);		Win->display(); 
	}
	return 0;
}

const int N=4;
//Main menu -> Play game (3), Options/Help(2), Exit(0)
int MainMenu(shared_ptr<sf::RenderWindow> Win){
	sf::Texture bg;
	TextureManager GearTextureManager;
	Tilelist GearTilelist;
	double rot[13];
	int xpos[13],ypos[13],til=0;

	//Load files first 
	std::string filename="Data/menu.xml";
	std::ifstream DataFile(filename);  
	std::string DataContent="",temp;
	while(std::getline(DataFile,temp)) DataContent+=temp;
	std::vector<char> XMLData = std::vector<char>(DataContent.begin(),DataContent.end());
	XMLData.push_back('\0'); 
	xml_document<> Data;	Data.parse<parse_no_data_nodes>(&XMLData[0]);

	xml_node<>* root=Data.first_node()->first_node("imagefile"); 
	xml_node<>* tiles; 
	std::string imagedir; sf::Texture imagetext; 
	imagedir=root->first_attribute("path")->value(); imagetext.loadFromFile(imagedir); 
	GearTextureManager.AddTexture(imagetext);  
	//get tiles dir
	
	tiles = root->first_node("tile");
	while(tiles){
		int x,y,xsiz,ysiz;
		x		 =atoi(tiles->first_attribute("x")->value());
		y		 =atoi(tiles->first_attribute("y")->value());
		xsiz	 =atoi(tiles->first_attribute("xsiz")->value());
		ysiz	 =atoi(tiles->first_attribute("ysiz")->value()); 
		xpos[til]=atoi(tiles->first_attribute("xpos")->value());	
		ypos[til]=atoi(tiles->first_attribute("ypos")->value());	
		rot[til] =atoi(tiles->first_attribute("rot") ->value()); rot[til]/=2;

		GearTilelist.AddTile(GearTextureManager.GetTexture(0),x,y,xsiz,ysiz); 
		GearTilelist.SetOriginCenter(til); 
		tiles=tiles->next_sibling();
		++til;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//end of xml read																						   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	

	sf::Texture tt; sf::Sprite Title;
	tt.loadFromFile("Images/Misc/Title.png"); Title.setTexture(tt); 
	Title.setOrigin(Title.getLocalBounds().width/2,Title.getLocalBounds().height/2);
	Title.setPosition(Win->getSize().x/2,125);
	
	int CurSelect=N-1;
	sf::Texture Misc;
	sf::Sprite Back,Lef,Rig; Back.setTexture(bg);   
	sf::Font Fnt; Fnt.loadFromFile("Fonts/bankgthd.ttf");
	sf::Text Chc[N]; 

	Chc[3].setString("Play");		 
	Chc[2].setString("Help");	  
	Chc[1].setString("Options");
	Chc[0].setString("Exit");

	for(int ite=0;ite<N;++ite){
		Chc[ite].setFont(Fnt);			Chc[ite].setColor(sf::Color::White); 
		Chc[ite].setCharacterSize(32);	Chc[ite].setOrigin(Chc[ite].getGlobalBounds().width/2,0);	
		Chc[ite].setPosition(Win->getSize().x/2-2,420-7-60*ite);
	}	 
	  
	Misc.loadFromFile("Images/Misc/Misc.png");
	Rig.setTexture(Misc); Rig.setTextureRect(sf::IntRect(0,0,160,32));	
	Lef.setTexture(Misc); Lef.setTextureRect(sf::IntRect(160,0,-160,32)); Lef.setOrigin(160,0); 

	sf::Event Evt; 
	int stop=0,top=1,end=0,str=30;
	while(Win->pollEvent(Evt));
	while(1){   
		while(str==0&&end==0&&Win->pollEvent(Evt)){
			if (Evt.type==sf::Event::Resized){
				Win->setSize(sf::Vector2u(864,480));
			} else if (Evt.type==sf::Event::Closed){
				Win->close(); 
				return 0; 

			} else if (Evt.type==sf::Event::KeyReleased&&Evt.key.code==sf::Keyboard::Tilde){
				Win->capture().saveToFile("Images/Screenshots/Screen.png");

			} else if (Evt.type==sf::Event::KeyReleased){
				if (Evt.key.code==sf::Keyboard::Up||Evt.key.code==sf::Keyboard::W){ 
					CurSelect=(CurSelect+1)%N;
					if (!top) for(int id=0;id<til;++id) rot[id]*=-1;
					if (!top) stop=20;
					top=1;

				} else if (Evt.key.code==sf::Keyboard::Down||Evt.key.code==sf::Keyboard::S){
					CurSelect=(CurSelect+N-1)%N;
					if (top) for(int id=0;id<til;++id) rot[id]*=-1;
					if (top) stop=20;
					top=0;

				} else if (Evt.key.code==sf::Keyboard::Space||Evt.key.code==sf::Keyboard::Return){
					if (CurSelect==0) {
						Win->close();
						return 0;
					}
					end=20;

				} else if (Evt.key.code==sf::Keyboard::Escape){
					Win->close();
					return 0;
					printf("If you're reading this something is TERRIBLY wrong\n");
				} 

			} else if (Evt.type==sf::Event::MouseMoved){
				sf::Vector2f MPos;
				MPos.x=sf::Mouse::getPosition(*Win).x;
				MPos.y=sf::Mouse::getPosition(*Win).y;
				for(int ite=0;ite<N;++ite) if (Chc[ite].getGlobalBounds().contains(MPos))	CurSelect=ite; 

			} else if (Evt.type==sf::Event::MouseButtonPressed){
				sf::Vector2f MPos;	int f=0;
				MPos.x=sf::Mouse::getPosition(*Win).x;
				MPos.y=sf::Mouse::getPosition(*Win).y; 
				for(int ite=0;ite<N;++ite) if (Chc[ite].getGlobalBounds().contains(MPos))	{ CurSelect=ite; f=1;}
				if(f){
					if (CurSelect==0) {
						Win->close(); 
						return 0;
					}
					end=10;
				}
			} 
		}
		//gear rotation-controlling black magic
		//read at own risk
		Win->clear(sf::Color::Black); 
		for ( int id=til-1; id>=0; --id ){
			if (!end) if (stop>=13) GearTilelist.Rotate(id,-rot[id]*(stop-12)/9); 
			if (!end) if (stop<=8)  GearTilelist.Rotate(id,rot[id]*(9-stop)/9);
			if (end>=13) GearTilelist.Rotate(id,rot[id]*(end-12)/9); 
			GearTilelist.DrawTile(id,xpos[id],ypos[id],Win);
		}
		stop=max(stop-1,0);

		for ( int id=0;		id<N;  ++id ){
			if (id==CurSelect){
				Chc[id].setColor(sf::Color::White); 
			} else {
				Chc[id].setColor(sf::Color(255,255,255,120));
				if (end>0) Chc[id].setColor(sf::Color(255,255,255,max(0,8*end-40)));
			}
			Win->draw(Chc[id]);
		} 
		Lef.setPosition(Win->getSize().x/2-Chc[CurSelect].getGlobalBounds().width/2,420-60*CurSelect);
		Rig.setPosition(Win->getSize().x/2+Chc[CurSelect].getGlobalBounds().width/2,420-60*CurSelect);
		Win->draw(Lef);
		Win->draw(Rig);
		Win->draw(Title);
		if (str>0){
			TStartScreen(Win,(str-1)*4);
			--str;
		}
		Win->display(); 
		if (end>0){
			--end;
			if (end==0){ 
				sf::RenderWindow &temp=*Win;
				sf::Texture Back; 
				Back.create(Win->getSize().x,Win->getSize().y);
				Back.update(temp);
				EndScreen(Win,Back);  
				return CurSelect;
			}
		}
	}
}
