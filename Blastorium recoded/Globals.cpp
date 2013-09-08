#include "Globals.h"

#include <vector>
#include "Data.h"
#include "Level.h"
#include "Player.h"
#include "WeaponManager.h"
#include "Powerups.h"
#include "WpnBomb.h"
#include "WpnMine.h"
#include "WpnRocket.h"
#include "WpnMedi.h"

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

using namespace std;

GlobalManager::GlobalManager(shared_ptr<sf::RenderWindow> Win){
	GlobalWindow		 = Win;
	GlobalDataManager	 = make_shared<DataManager>		();
	GlobalLevel			 = make_shared<Level>			();
	GlobalPlayerManager	 = make_shared<PlayerManager>	();
	GlobalWeaponManager	 = make_shared<WeaponManager>	();
	GlobalPowerupManager = make_shared<PowerupManager>	();
	GlobalBombManager	 = make_shared<BombManager>		();
	GlobalMineManager	 = make_shared<MineManager>		();	
	GlobalRocketManager	 = make_shared<RocketManager>	(); 
	GlobalMediManager	 = make_shared<MediManager>		();
	return;
}

GlobalManager::~GlobalManager(){
	return;
}