#include "Engine.h"
 
#include "Globals.h"
#include "Level.h"
#include "Player.h"
#include "WeaponManager.h"
#include "Interface.h"
#include "Data.h"

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <stdlib.h>

using namespace std;

//This engine is exclusively for playing the game only
//Title screen, etc will be covered separately
 
Engine::Engine(shared_ptr<GlobalManager> Global) { 
	Globals		=	Global;
	MainWindow	=	Global->GlobalWindow;
	levelid=1;
	if(!Init(MainWindow)) throw "Could not initialize Engine";
	Globals->GlobalDataManager->PrintData();
	return;
}

Engine::~Engine() { 
	return;
} 

//Main initialization. Create stuff.
bool Engine::Init(shared_ptr<sf::RenderWindow> Win) {  
	Globals->GlobalLevel->Construct(Globals);  
	Globals->GlobalPlayerManager->Construct(Globals);
	Globals->GlobalWeaponManager->Construct(Globals);
	Globals->GlobalPowerupManager->Construct(Globals);
	 

	return true;
}

//Does this really need an explanation
void Engine::RenderFrame() {
	MainWindow->clear(); 
	Globals->GlobalLevel->RenderLevel();
	Globals->GlobalPowerupManager->RenderPowerup();
	Globals->GlobalWeaponManager->RenderWeapons();
	Globals->GlobalPlayerManager->RenderPlayer();
	return;
}

//Processes all input. Will pause game if window went outta focus.
//returns int based on inputs: Continue game (2), Main Menu(1) or exit (0)
int Engine::ProcessInput() { 
	sf::Event evt;
	//Loop through all window events
	while(MainWindow->pollEvent(evt)) {

		if (evt.type==sf::Event::Closed){ 
			MainWindow->close(); 
			exit(0); 
			return 0;

		} else if (evt.type==sf::Event::LostFocus){ 
			sf::RenderWindow &temp=*MainWindow;	sf::Texture Back; 
			Back.create(MainWindow->getSize().x,MainWindow->getSize().y); Back.update(temp);
			return PauseGameChoice(MainWindow,Back); 

		} else if (evt.type==sf::Event::KeyReleased){
			if (evt.key.code==sf::Keyboard::Tilde){
				MainWindow->capture().saveToFile("Images/Screenshots/Screen.png");
			
			} else if (evt.key.code==sf::Keyboard::Q){
				Globals->GlobalWeaponManager->ChangeActiveWeapon(0); 
		
			} else if (evt.key.code==sf::Keyboard::RControl){
				Globals->GlobalWeaponManager->ChangeActiveWeapon(1); 

			} else if (evt.key.code==sf::Keyboard::Escape) {
				sf::RenderWindow &temp=*MainWindow;	sf::Texture Back; 
				Back.create(MainWindow->getSize().x,MainWindow->getSize().y); Back.update(temp);
				return PauseGameChoice(MainWindow,Back);   
			}
		//////////////////////////////////////////////////////
		} 
	}
	//BEHOLD
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))		Globals->GlobalPlayerManager->MovePlayer(0,1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))		Globals->GlobalPlayerManager->MovePlayer(0,2);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))		Globals->GlobalPlayerManager->MovePlayer(0,3);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))		Globals->GlobalPlayerManager->MovePlayer(0,4);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))		Globals->GlobalPlayerManager->MovePlayer(1,1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))	Globals->GlobalPlayerManager->MovePlayer(1,2);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))		Globals->GlobalPlayerManager->MovePlayer(1,3);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))		Globals->GlobalPlayerManager->MovePlayer(1,4);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))		Globals->GlobalWeaponManager->UseWeapon(0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))	Globals->GlobalWeaponManager->UseWeapon(1);
	//THE KEYBOARD LOGIC 
	return 2;
}

//calculates logic and shit
//returns bool, which returns false if a player is dead.
bool Engine::Update() {  
	bool ret=Globals->GlobalPlayerManager->CalculateLogic();
	Globals->GlobalWeaponManager->CalculateLogic();
	Globals->GlobalPowerupManager->CalculateLogic();
	return ret;
}

//Function called post-game. 
//runs the engine but does not take updates, gradually darkening screen for aesthetic purposes
//then calls Data->Finishmatch for statistics, which in turn calls interface->Endgamechoice for input
//returns int : Play again (2), Main Menu(1) or exit (0)
int Engine::FinishRun(){	
	for(int i=1;i<=90;++i){
		sf::Event stuff;	while(MainWindow->pollEvent(stuff)){} //ignore updates
		if (i%9==0) {
			Update();
		}
		RenderFrame();
		
		sf::RectangleShape Shade(sf::Vector2f((float)MainWindow->getSize().x,(float)MainWindow->getSize().y));
		Shade.setFillColor(sf::Color(0,0,0,60+((i-1)/9)*10));
		MainWindow->draw(Shade);

		MainWindow->display();
	}
	sf::RenderWindow &temp=*MainWindow;
	sf::Texture Back; 
	Back.create(MainWindow->getSize().x,MainWindow->getSize().y);
	Back.update(temp);
	EndScreen(MainWindow,Back); 
	int id=0;
	if (Globals->GlobalPlayerManager->IsDead(1)) id+=1;
	if (Globals->GlobalPlayerManager->IsDead(0)) id+=2; 
	sf::Texture Last=Globals->GlobalDataManager->FinishMatch(id,MainWindow); 
	return EndGameChoice(MainWindow,Last); 
}

//Main loop: Process input, update, render.
//returns int : Play again (2), Main Menu(1) or exit (0)
int Engine::MainLoop() { 
	RenderFrame();
	sf::RenderWindow &temp=*MainWindow;	sf::Texture Back; 
	Back.create(MainWindow->getSize().x,MainWindow->getSize().y); Back.update(temp);
	StartScreen(MainWindow,Back); 
	bool Playing=true;
	RenderFrame(); 
	int Ret;
	while(Playing&&MainWindow->isOpen()) { 
		int inp=ProcessInput();
		if (inp!=2){ 
			Ret=inp;
			break;
		}
		Playing=Update();
		RenderFrame();
		MainWindow->display();
		if (!Playing) {
			Ret=FinishRun();
			if (!MainWindow->isOpen()) return 0;
		}
		if (!MainWindow->isOpen()) return 0;
	}
	return (Ret);
}

//Refreshes every class used for a new game session
void Engine::Refresh(){ 
	Globals->GlobalDataManager	  ->Init();
	Globals->GlobalLevel		  ->Init();
	Globals->GlobalPlayerManager ->Init();
	Globals->GlobalPowerupManager->Init();
	Globals->GlobalWeaponManager ->Init(); 
	return;
}

//The function called by main. Launches game mode, and will indefinitely repeat if player chooses play again
void Engine::Go(int Map_ID,vector<int> Dat)
{ 
	Globals->GlobalLevel->SetLevel(Map_ID);
	 
	Refresh();
	Globals->GlobalWeaponManager->SetWeapon(0,Dat[0]+1,Dat[1]+1,Dat[2]+1);
	Globals->GlobalWeaponManager->SetWeapon(1,Dat[3]+1,Dat[4]+1,Dat[5]+1);
	while (MainLoop()==2) {
		Refresh(); 
		Globals->GlobalWeaponManager->SetWeapon(0,Dat[0]+1,Dat[1]+1,Dat[2]+1);
		Globals->GlobalWeaponManager->SetWeapon(1,Dat[3]+1,Dat[4]+1,Dat[5]+1);
	} 
	return;
}

