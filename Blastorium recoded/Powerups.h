#ifndef _POWERUPS_H
#define _POWERUPS_H
 
#include <vector> 
 
#include "Level.h"
#include "Player.h"
#include "WpnBomb.h"
#include "WeaponManager.h"
#include "Tilelist.h"
#include "TextureManager.h"

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

using namespace std;

//forward reference to globals
class GlobalManager;

class PowerupManager {
	private: 
		//SFML Render Window
		shared_ptr<GlobalManager>	 Globals;
		shared_ptr<sf::RenderWindow> MainWindow; 

		int Powerup[20][20],PowerupCount[20],LoopTimer[20][20];
		bool Active[20][20];
		vector<pair<int,int> > Covered; 
		TextureManager PowerupTextureManager;
		Tilelist IconTileList, AnimTileList;
	public:
		PowerupManager();
		~PowerupManager();
		void Construct(shared_ptr<GlobalManager> Glo);
		void Init();									//initialization
		void SetPowerupAmount(vector<int> amt);
		void GeneratePowerup();  
		void GetPowerup(int id,int powerupno);
		void RenderPowerup();
		void CalculateLogic();
		void LoadPowerup(); 
};

#endif
