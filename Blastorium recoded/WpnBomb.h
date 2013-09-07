#ifndef _WPNBOMB_H
#define _WPNBOMB_H

#include "SFML\Graphics.hpp"
 
#include "TextureManager.h"
#include "Tilelist.h"
#include "Level.h"
#include "Player.h"

#include <vector>

using namespace std;

//////////////////////////////////////////////////////////////////////
//BOMBS
//
//PRIMARY OFFENSIVE WEAPON AND BLOCK CLEARER
//WHEN PUT, EXPLODES AFTER SEVERAL SECONDS DELAY
//DEALS DAMAGE OVER TIME. CLEARS BLOCKS
//
//WITH MINES: MINES CAN BE PUT BEFORE BOMBS, NOT AFTER. NO TRIGGER BETWEEN THEM
//WITH ROCKET: ROCKETS WILL PREMATURELY TRIGGER BOMBS
//
//INCREASING LEVELS INCREASE BOMB STOCK AND EXPLOSION SIZE
//AT LEVEL 5, BOMBS CAN PIERCE SOFT BLOCKS. LEVEL 5 DOESNT GRANT EXTRA BOMB OR EXPLOSION SIZE
//STOCK		= 1 + (MIN(LEVEL,4))
//EXPLOSION	= 2*(MIN(LEVEL,4))
//////////////////////////////////////////////////////////////////////

//forward reference to globals
class GlobalManager;

//this is the bomb entity
//stores data about the bomb, including timer, strength, and bomb owner
class Bomb{ 
	public:
		int owner,level,timer,row_pos,col_pos;
		//reminder
		//1 second = 30 ticks
		Bomb(int id,int lvl,int row,int col);
		~Bomb();
		bool Logic(); 
};

//this is the bomb manager
//holds data on all bombs, and performs most (explosion, collision and damaging) logic. 
class BombManager{
	private:
		struct Explosion{
			int timer,dmg,row,col,sprite;
			Explosion(int time,int damage,int r,int c,int spr){
				timer=time;
				dmg=damage;
				row=r;
				col=c;
				sprite=spr;
			}
		}; 
		//contains all player's currently active bombs
			std::vector<Bomb> BombList[5];		
		//contains all fires, coordinates, duration and damage
			std::vector<Explosion> ExplosionList;	
		
		sf::Clock DEBUGGER_COUNTER;
		
		TextureManager BombTextureManager;
		Tilelist BombTileList,FireTileList,DebrisTileList;

		shared_ptr<GlobalManager>	 Globals;
		shared_ptr<sf::RenderWindow> MainWindow;


	public:
		bool BombMap[25][25],JustExploded[25][25];	//
		int FireMap[25][25];

		BombManager();
	
		~BombManager();
		
			void Construct(shared_ptr<GlobalManager> Glo);
		//duh. Checks usage limit first based on the level.
			void PutBomb(int id,int bomblevel,int row,int col);		
		//if a bomb's time is up, it goes boom via this function
			void Explode(int row,int col,int level,int srcdir);		
		//checks if there is a bomb in the coordinate, explodes it if true. (explosion due to external reasons)
			void TriggerBomb(int row,int col);						
		//checks all bombs, deals damage 	 
			void BombLogic();										
		//render
			void RenderBomb();	
		//initialization
			void Init();				
		//load the necessary datas
			void LoadBomb();				
		//check availability
			bool IsAvailable(int id,int bomblevel);
		//returns availability data
			int GetAvailability(int id,int bomblevel);								
};


#endif 