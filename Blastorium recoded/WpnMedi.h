#ifndef _WPNMEDI_H
#define _WPNMEDI_H

#include "SFML/Graphics.hpp"
 
#include "TextureManager.h"
#include "Tilelist.h"
#include "Level.h"
#include "Player.h" 

#include <vector>

const float SLOWN=0.40f;	//60%			- magnitude of slow (normal)
const float SLOW5=0.75f;	//25%			- magnitude of slow (max level)
const int MEDINV=75;		//2.5 seconds	- duration of invulnerability on usage
const int MEDSLO=180;		//6 seconds		- duration of slow on usage
const int MEDHCD=360;		//12 seconds	- periodic regeneration interval
const int MEDWCD=720;		//24 seconds	- weapon cooldown when used
const int MEDCDD=60;		//2 seconds		- cooldown reduction per level
const int PUHEAL=15;		//powerup heal
const int RGHEAL=5;	 	    //base regen heal 
const int DGHEAL=2;			//regen heal increase per level

//////////////////////////////////////////////////////////////////////
//MEDIKITS (ID 4)
//
//PASSIVE SUPPORT EQUIPMENT, USED TO BOOST HEALTH AND TEMPORARY INVULNERABILITY
//PASSIVELY, GRANTS REGEN EVERY SEVERAL SECS AND GIVES EXTRA HP ON POWERUP PICK
//WHEN USED, GRANTS INVULNERABILITY, BUT SLOWS SELF FOR SEVERAL SECS AND DISABLES THE PASSIVE
//HAS LONG COOLDOWN
//
//INCREASING LEVELS INCREASE REGEN AND DECREASE CD
//AT LEVEL 5, MASSIVELY REDUCES INVUL SLOW
//REGEN				= 5 + 2*LEVEL
//REGEN INTERVAL	= 12s
//POWERUP HEAL		= 20
//INVUL DURATION	= 3s
//SLOW				= 60% (20% AT LEVEL 5)
//SLOW DURATION		= 6s
//COOLDOWN			= 24s - 2/LEVEL
//////////////////////////////////////////////////////////////////////
 
//forward reference to globals
class GlobalManager;
 
//this is the medkit manager
//holds data on regen timer, cooldown, etc.
class MediManager{
	private: 
		int HealCD[5],	//CD on regular regen
			MediCD[5],	//CD on weapon due to usage
			MediID[5];	//In which slot is the player's medikit located;

		bool IsMedi[5]; //checks if player is using medikit or not (for Powerups.h to access)

		std::shared_ptr<GlobalManager>	 Globals;
		std::shared_ptr<sf::RenderWindow> MainWindow;
		
	public:
		MediManager();
		
		~MediManager();
		
		//initialization
			void Construct(std::shared_ptr<GlobalManager> Glo);

		//informs that player [id] uses medikit
			void HasMedikit(int id,int slot);

		//duh. Checks cd, gives invul, slows, stuff like that
			void UseMedikit(int id,int medilevel);		

		//used by Powerups.h. Checks if player is using medikit or not, if yes then heal player.
			void UsePowerup(int id);						

		//Ticks all timer and stuff
			void MediLogic();				 
		
		//check availability
			bool IsAvailable(int id,int medilevel);
		
		//returns availability data
			int GetAvailability(int id,int medilevel);							
};


#endif 
