#ifndef _WPNMINE_H
#define _WPNMINE_H

#include "SFML\Graphics.hpp"
 
#include "TextureManager.h"
#include "Tilelist.h"
#include "Level.h"
#include "Player.h"
#include "WpnBomb.h"

#include <vector>

using namespace std;

//////////////////////////////////////////////////////////////////////
//MINES
//
//PASSIVE WEAPONS, USED TO BLOCK PASSAGES AND HINDER ENEMY MOVEMENT
//WHEN PUT, SPENDS SEVERAL SECONDS FADE TIME BEFORE IT IS ACTIVE
//DEALS FULL DAMAGE IMMEDIATELY WHEN STEPPED. EXPLOSION JUST FOR VISUALS
//
//WITH BOMB: MINES CAN BE PUT BEFORE BOMBS. NO TRIGGER BETWEEN THEM
//WITH ROCKET: ROCKETS WILL PREMATURELY TRIGGER MINES
//
//INCREASING LEVELS INCREASE MINE STOCK, DAMAGE, AND FASTER FADE TIME
//AT LEVEL 5, MINES DEAL LESS DAMAGE TO ITS OWNER (LESS FRIENDLY FIRE)
//MINE STOCK = 2*(LEVEL)
//DAMAGE	 = 50 + 10*(LEVEL). LEVEL 5 FRIENDLY FIRE DAMAGE IS HALVED
//FADETIME(s)= 3.5 - 0.5*(LEVEL)
//////////////////////////////////////////////////////////////////////
 
//forward reference to globals
class GlobalManager;
 
//this is the mine manager
//holds data on all mines, and performs collision, damage logic plus all rendering.
class MineManager{
	private: 
		//this is the explosion entity, stores data about the explosions and timer
			struct Explosion{
				pair<int,int> coor;	//row/col
				int timer;			//fade timer
				Explosion(){
					timer=0;
				}
				Explosion(int r,int c){
					coor=make_pair(r,c);
					timer=0;
				}
			}; 
		//this is the mine entity, stores data about the mine's fade timer and owner
			struct Mine{
				int timer,level,owner;
				Mine(){
				}
				Mine(int id,int lvl){
					owner=id;
					level=lvl;
					timer=0;
				}
			};

		//contains how many mines a player has active
			int MineAmt[5];							
		//contains all current explosion visuals.  
			vector<Explosion> ExplosionList;		 			 
		//true if tile contians mine
			bool IsMined[25][25];					
		//holds the mines, duh
			Mine MineMap[25][25];					
		
		shared_ptr<GlobalManager>	 Globals;
		shared_ptr<sf::RenderWindow> MainWindow;
		
		TextureManager MineTextureManager;
		Tilelist MineTileList;

	public:
		MineManager();
		
		~MineManager();
		
			void Construct(shared_ptr<GlobalManager> Glo);
		//duh. Checks usage limit first based on the level.
			void PutMine(int id,int minelevel,int row,int col);		
		//explodes the mine at that coordinate, if any.
			void TriggerMine(int row,int col);						
		//checks all mines for collision
			void MineLogic();										
		//render
			void RenderMine();	
		//initialization
			void Init();									
		//load the necessary datas
			void LoadMine();			
		//check availability
			bool IsAvailable(int id,int minelevel);
		//returns availability data
			int GetAvailability(int id,int minelevel);							
};


#endif 