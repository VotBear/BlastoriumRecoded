#ifndef _WEAPONMANAGER_H
#define _WEAPONMANAGER_H
 
#include "Player.h"
#include "Level.h"
#include "WpnBomb.h"
#include "WpnMine.h"
#include "WpnRocket.h"
#include "TextureManager.h"
#include "Tilelist.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;

//manages all weapons, and the rendering
//has access to datas such as Level and PlayerManager
//contains data regarding players' weapon choices and level

class GlobalManager;

class WeaponManager{
	private:
		int PlayerActiveWeapon[5];
		shared_ptr<GlobalManager>	 Globals;
		shared_ptr<sf::RenderWindow> MainWindow;
		shared_ptr<PlayerManager> PlayerData; 
		shared_ptr<Level> LevelData;
		
		TextureManager WeaponTextureManager;
		Tilelist FrameList,IconList,NumberList,LevelList;
	public: 
		int PlayerEquip[5][5],NowLevel[5][5],MaxLevel[5][5];
		WeaponManager();
		~WeaponManager();
		void Construct(shared_ptr<GlobalManager> Glo);
		void SetWeapon(int id,int w1,int w2,int w3);	//initializes a player's selection of weapons
		void ChangeActiveWeapon(int id);				//Change a player's active weapon
		void UseWeapon(int id);							//Issue an use command of the player's (wpn_no)th weapon
		void UpgradeWeapon(int id,int wpn_no);			//Upgrades a player's weapon
		void CalculateLogic();							//logic loop
		void RenderWeapons();							//render loop
		void Init();									//initialization
		void LoadWeapon();
};

#endif
