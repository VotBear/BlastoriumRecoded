#ifndef _WPNROCKET_H
#define _WPNROCKET_H

#include "SFML\Graphics.hpp"

#include <vector>

#include "TextureManager.h"
#include "Tilelist.h"
#include "Level.h"
#include "Player.h"
#include "WpnBomb.h"
#include "WpnMine.h"

#define MAP_LENGTH 17
#define MAP_HEIGHT 15
#define INI 360		//12 seconds
#define DUR 30		//1 second
#define DMG 90
#define STN 30		//1 second
#define SLW 60		//2 seconds
#define DIS 60		//2 seconds
#define CD_ 180		//6 seconds
#define CDD 15		//0.5 second a level
#define DL_ 57		//1.9 seconds 
#define DLD 3		//0.1 seconds

using namespace rapidxml;
using namespace std;

//////////////////////////////////////////////////////////////////////
//ROCKETS (ID 3)
//
//SECONDARY OFFENSIVE WEAPON TO HINDER AND/OR DISABLE ENEMIES
//WHEN USED, LAUNCHES A ROCKET AT CURRENT ENEMY POSITION.
//DEALS IMMEDIATE EFFECT AFTER SEVERAL SECS DELAY. EXPLOSION JUST FOR VISUALS
//SIGNIFICANTLY DISABLES AND SLOWS TARGET ON HIT
//HAS A COOLDOWN.
//
//WITH BOMB: ROCKETS WILL PREMATURELY TRIGGER BOMBS
//WITH MINES: ROCKETS WILL PREMATURELY TRIGGER MINES
//
//INCREASING LEVELS REDUCES COOLDOWN AND HIT DELAY
//AT LEVEL 5, ROCKETS STUNS
//INITIAL CD(s)= 12.00
//COOLDOWN(s)  = 6.00 - 0.50*(LEVEL)
//HIT DELAY(s) = 1.90 - 0.10*(LEVEL)
//DISABLE(s)   = 2.00 + 0.5*(LEVEL)
//SLOW(s)	   = 2.00 + 0.5*(LEVEL)
//SLOW(magn)   = 60%
//STUN(lv5)	   = 1.00 s
//DAMAGE	   = 90
//////////////////////////////////////////////////////////////////////

//forward reference to globals
class GlobalManager;

//this is the rocket manager
//holds data on all Rockets, and performs collision, damage logic plus all rendering.
class RocketManager{
	private: 
		//this is the explosion entity, stores data about the explosions and timer
			struct Explosion {
				pair<int,int> coor;	//row/col
				int timer;			//fade timer
				Explosion(){
					timer=0;
				}
				Explosion(pair<int,int> c){
					coor=c;
					timer=0;
				}
			}; 
		//this is the Rocket entity, stores data about the Rocket's position, level and timer
			struct Rocket {
				pair<int,int> coor;	//row/col
				int timer,level;
				Rocket(){
					timer=0;
				}
				Rocket(int r,int c,int lvl){
					coor=make_pair(r,c);
					level=lvl;
					timer=int(DL_ - DLD*lvl);
				}
			};
		//contains the cooldown each player has remaining (in ticks)
			int Cooldown[5];
		//contains all current explosion visuals.  
			vector<Explosion> ExplosionList;		 			 
		//contains all current rockets
			vector<Rocket> RocketList;

		shared_ptr<GlobalManager>	 Globals;
		shared_ptr<sf::RenderWindow> MainWindow;
		
		TextureManager RocketTextureManager;
		Tilelist RocketTileList,TargetTileList,ExplosionTileList;

	public:
		RocketManager();
		
		~RocketManager();

			void Construct(shared_ptr<GlobalManager> Glo);
		//checks if the cooldown is off first
			void UseRocket(int id,int rocketlevel);						
		//checks all Rockets for impact
			void RocketLogic();										
		//render
			void RenderRocket();										
		//initialization
			void Init();
		//load the necessary datas
			void LoadRocket();		
		//check availability
			bool IsAvailable(int id,int rocketlevel);
		//returns availability data
			int GetAvailability(int id,int rocketlevel);
};
#endif