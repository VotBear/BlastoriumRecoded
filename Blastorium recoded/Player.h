#ifndef _PLAYER_H
#define _PLAYER_H

#include <vector>

#include "Level.h"
#include "TextureManager.h"
#include "Tilelist.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std; 

//forward reference to globals
class GlobalManager;

class Player{ 
	public:
		int col,									//player color
			cur_col, cur_row,						//player's current tile coordinate
			nex_col, nex_row,						//player's target tile coordinate (this player will try to move to this position)
			disabled, stunned,						//player's disable/stun counter (some weapons can disable the target's weapons/stun target)
			invul, octimer;							//player's invulnerability timer and overclock animation timer
		
		double	now_hp, tmp_hp, max_hp,				//player's current HP, lagging HP (for cosmetic purposes) and max HP
				row_pos, col_pos,					//the player's actual position in the game
				max_move_speed, move_speed;			//self-explanatory
		
		bool overclock;								//checks if player is overclocked

		vector<pair<double,int> > speed_modifier;	//modifiers for the player's speed (first=magnitude,second=duration)
		vector<pair<int,int>	> effect_list;		//stores list of visual animations on player (id/duration)
													//for now, there's only one ID: 0->heal anim
		Player(int rowpos,int colpos,int color);	
		~Player();
			////Moves player one tick
		void Move(); 
			////if the player is dead, returns true
		bool Logic();								
};

class PlayerManager{
	private:
		std::vector<Player> PlayerList;
		TextureManager	PlayerTextureManager;
		Tilelist		PlayerSprList,  
							//0-3: Colors
							//4-7: Expressions
							//8-13: Heal effect
							//14-17: Invul effect
							//18-21: Overclock effect
						HPBarList, 
						NumberList;
		
		int colamt;

		shared_ptr<GlobalManager>	 Globals;
		shared_ptr<sf::RenderWindow> MainWindow;
			////internal function
		bool check(int row,int col);
	public:
		PlayerManager();
		~PlayerManager();	

		void Construct(shared_ptr<GlobalManager> Glo);
			////initialization
		void Init();				

			////Loads data and stuff
		void LoadPlayer();				

			////Adds a player
		void AddPlayer(int rowpos,int colpos);	

			////clockwise, 1-4 = north, east, south, west respectively
		void MovePlayer(int id,int dir);		

			////renders all player-related stuff at once
		void RenderPlayer();				

			////subfunction of renderplayer	
		void DrawNum(int id);						

			////damages the player (Plays heal animation if negative)
		void Damage(int id,int amt);	

			////disables the player/refreshes timer up to [amt]
		void Disable(int id,int amt);

			////gives player invulnerability, timer up to [amt]
		void Invul(int id,int amt);

			////stuns the player/refreshes timer up to [amt]
		void Stun(int id,int amt);	

			////debuffs player; resets disable and speed modifiers
		void Debuff(int id);					

			////edits the player's max speed by adding set amount
		void ChgMaxSpeed(int id,double amt);	

			////edits the player's current speed by a % 
		void ChgTmpSpeed(int id,double mul,int dur);	

			////returns the player positon (Row/Y , Col/X)
		std::pair<int,int> GetPosition(int id);	

			////returns the player cur_pos (Row/Y , Col/X)
		std::pair<int,int> GetCurPos(int id);	

			////returns the player nex_pos (Row/Y , Col/X)
		std::pair<int,int> GetNexPos(int id);	

			////if the player is dead, returns true
		bool CalculateLogic();					

			////if the player is disabled, returns true
		bool IsDisabled(int id);				

			////checks if player is dead.
		bool IsDead(int id);					
};

#endif
