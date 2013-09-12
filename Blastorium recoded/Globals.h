#ifndef _GLOBALS_H
#define _GLOBALS_H

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

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 

//A global manager that ensures each shared_ptr has at least one counter in it, preventing deletion
class GlobalManager{
	private:
	public:	
		shared_ptr<sf::RenderWindow>	GlobalWindow		;
		shared_ptr<Level>				GlobalLevel			;
		shared_ptr<DataManager>			GlobalDataManager	;
		shared_ptr<PlayerManager>		GlobalPlayerManager	;
		shared_ptr<WeaponManager>		GlobalWeaponManager	;
		shared_ptr<PowerupManager>		GlobalPowerupManager;	
		shared_ptr<BombManager>			GlobalBombManager	;
		shared_ptr<MineManager>			GlobalMineManager	;
		shared_ptr<RocketManager>		GlobalRocketManager	; 
		shared_ptr<MediManager>			GlobalMediManager	;

		GlobalManager(shared_ptr<sf::RenderWindow> Win);

		~GlobalManager();
		
};

#endif
