#ifndef _ENGINE_H
#define _ENGINE_H
 
#include <vector> 
#include "Level.h"
#include "Player.h"
#include "WeaponManager.h"
#include "Powerups.h"
#include "Data.h"

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

using namespace std;

//forward reference to globals
class GlobalManager;


class Engine {
	private:
		//SFML Render Window
		int levelid; 
		/*Level* MainLevel;
		DataManager* MainDataManager;
		PlayerManager* MainPlayerManager; 
		WeaponManager* MainWeaponManager;
		PowerupManager* MainPowerupManager;*/
		shared_ptr<Level>			 MainLevel;
		shared_ptr<DataManager>		 MainDataManager;
		shared_ptr<PlayerManager>	 MainPlayerManager; 
		shared_ptr<WeaponManager>	 MainWeaponManager;
		shared_ptr<PowerupManager>	 MainPowerupManager;
		shared_ptr<sf::RenderWindow> MainWindow;
		
		shared_ptr<GlobalManager>	 Globals;
		 
			////Initializes the engine, creates all required classes 
		bool Init(shared_ptr<sf::RenderWindow> Win);				
		 
			////Loads texture from file (unused)
		void LoadTexture();
		 
			////Main Game Loop, calls ProcessInput, Update and RenderFrame
			////returns int: 2 = still playing, 1 = main menu, 0=exit
		int  MainLoop();
		 
			////Renders one frame, calls render function of every class
		void RenderFrame();
		 
			////Processes user input, and detects early exits
			////returns int: 2 = still playing, 1 = main menu, 0=exit
		int ProcessInput();
		 
			////Updates all Engine internals for the frame, calls logic function of every class
		bool Update();		
		 
			////Finish current game 
		int FinishRun();
		 
			////Reinitializes every class for another round of game 
		void Refresh();

	public:  
			////Constructor
		Engine(shared_ptr<GlobalManager> Global);

			////Deconstructor
		~Engine();

			////Start the game session with map number [Map_ID] and weapon setting [Dat]
		void Go(int Map_ID,vector<int> Dat);					
};

#endif
