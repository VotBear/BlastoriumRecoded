#ifndef _SELECT_H
#define _SELECT_H
 
#include <vector>  
#include <array>

#include "Tilelist.h"
#include "TextureManager.h"
#include "Interface.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 

const int Width=6;
const int Heigt=2;
const int Dist=100;
const int Stageamt=7;
 
class SelectManager{
	private:	 
		TextureManager SelectTextureManager;
		Tilelist SelectTilelist[5];
		sf::Texture BGTex;
		sf::Sprite BG;
		std::vector<int> Ret;
			//Returned vector

		bool IsValid[40],Fin;
			//checks if weapon id [..] is available based on the prequisites

		int WpnS1, WpnS2,	//amt of wpn selected
			SelS1, SelS2,	//current selection
			StgSel,			//current selected stage 
			CurPos,			//current stage-wheel position (Value range is 100*StgSel)
			Warn[5];		//weaponselect warning (no softblock destroyer)
			 
		std::string GetTitle(int WpnNo);
		std::string GetDescr(int WpnNo,int Done);

		//checks if a weapon has been unlocked via data.h
		void CheckValid();

		//Renders bottom part of screen (separated from renderselect due to size)
		void RenderStage (std::shared_ptr<sf::RenderWindow> Win);

		//Render
		void RenderSelect (std::shared_ptr<sf::RenderWindow> Win);
		
		//Moves stage in direction dir
		void MoveStage(int Dir,std::shared_ptr<sf::RenderWindow> Win);

		//Cancels the screen and return to main menu
		void CancelSelection (std::shared_ptr<sf::RenderWindow> Win);

	public:
		SelectManager();

		~SelectManager();

		//initiates stuff
		void InitSelect();

		//restarts variables
		void Init();

		//Level and wpn Select -> Choose level (1-7) + Vector of weapons, Exit(0), Cancelled (-1)
		//Vector of weapons consists of 3 P1's weapons, followed by 3 P2's weapons
		//IMPORTANT: Starts from 0, so add 1 before calling WeaponManager->SetWeapon
		std::pair < int, std::vector<int> > SelectScreen(std::shared_ptr<sf::RenderWindow> Win); 
};

#endif
