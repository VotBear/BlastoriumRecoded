#ifndef _WPNROCKET_H
#define _WPNROCKET_H

#include "SFML/Graphics.hpp"

#include <vector>

#include "TextureManager.h"
#include "Tilelist.h"
#include "Level.h"
#include "Player.h"
#include "WpnBomb.h"
#include "WpnMine.h"
 
const int  RCKINI=360;		//12 seconds - Initial cooldown 
const int  RCKDMG=90;		//damage, duh
const int  RCKDUR=30;		//1 second - explosion duration
const int  RCKSTN=30;		//1 second	 - stun base duration
const int  RCKSLW=60;		//2 seconds	 - slow base duration
const int  RCKDIS=60;		//2 seconds	 - disable base duration
const int  RCKCD_=180;		//6 seconds	 - base cooldown
const int  RCKCDD=15;		//0.5 second - reduction in cooldown per level
const int  RCKDL_=57;		//1.9 seconds - rocket hit delay
const int  RCKDLD=3;		//0.1 seconds - reduction in hit delay per level

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
					timer=int(RCKDL_ - RCKDLD*lvl);
				}
			};
		//contains the cooldown each player has remaining (in ticks)
			int Cooldown[5];
		//contains all current explosion visuals.  
			std::vector<Explosion> ExplosionList;		 			 
		//contains all current rockets
			std::vector<Rocket> RocketList;

		std::shared_ptr<GlobalManager>	 Globals;
		std::shared_ptr<sf::RenderWindow> MainWindow;
		
		TextureManager RocketTextureManager;
		Tilelist RocketTileList,TargetTileList,ExplosionTileList;

	public:
		RocketManager();
		
		~RocketManager();

			void Construct(std::shared_ptr<GlobalManager> Glo);
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
