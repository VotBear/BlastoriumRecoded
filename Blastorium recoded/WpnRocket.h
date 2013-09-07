#ifndef _WPNROCKET_H
#define _WPNROCKET_H

#include "SFML\Graphics.hpp"
 
#include "TextureManager.h"
#include "Tilelist.h"
#include "Level.h"
#include "Player.h"
#include "WpnBomb.h"
#include "WpnMine.h"

#include <vector>

using namespace std;

//////////////////////////////////////////////////////////////////////
//ROCKETS
//
//SECONDARY OFFENSIVE WEAPON TO HINDER AND/OR DISABLE ENEMIES
//WHEN USED, LAUNCHES A ROCKET AT CURRENT ENEMY POSITION.
//DEALS IMMEDIATE DAMAGE AFTER SEVERAL SECS DELAY. EXPLOSION JUST FOR VISUALS
//SIGNIFICANTLY DISABLES TARGET ON HIT
//HAS A COOLDOWN.
//
//WITH BOMB: ROCKETS WILL PREMATURELY TRIGGER BOMBS
//WITH RocketS: ROCKETS WILL PREMATURELY TRIGGER RocketS
//
//INCREASING LEVELS REDUCES COOLDOWN AND HIT DELAY
//AT LEVEL 5, ROCKETS HAVE 1-TILE EXPLOSION SPREAD RADIUS
//COOLDOWN(s)  = 5.50 - 0.50*(LEVEL)
//HIT DELAY(s) = 2.75 - 0.25*(LEVEL)
//DAMAGE	   = 75	(LEVEL 5 SPREAD DEALS 50)
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
					timer=int((30*(11-level))/4);
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