#ifndef _SELECT_H
#define _SELECT_H
 
#include <vector>  
#include "Tilelist.h"
#include "TextureManager.h"
#include "Interface.h"

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

using namespace std; 

#define Width 6
#define Heigt 2
#define Dist 100
#define Stageamt 7
 
class SelectManager{
	private:	 
		TextureManager SelectTextureManager;
		Tilelist SelectTilelist[5];
		sf::Texture BGTex;
		sf::Sprite BG;
		vector<int> Ret;
			//Returned vector
		bool IsValid[40],Fin;
			//checks if weapon id [..] is available based on the prequisites
		int WpnS1, WpnS2, SelS1, SelS2, StgSel, CurPos, Warn[5];
			//WpnS = amt of wpn selected
			//SelS = current selection
			//StgSel = current selected stage
			//CurPos = current stage-wheel position (Value range is 100*StgSel)
			//Warn = weaponselect warning (no softblock destroyer)
		string GetTitle(int WpnNo);
		string GetDescr(int WpnNo,int Done);
		void CheckValid();
		void RenderStage (shared_ptr<sf::RenderWindow> Win);
		void RenderSelect (shared_ptr<sf::RenderWindow> Win);
		void MoveStage(int Dir,shared_ptr<sf::RenderWindow> Win);
		void CancelSelection (shared_ptr<sf::RenderWindow> Win);

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
		pair < int, vector<int> > SelectScreen(shared_ptr<sf::RenderWindow> Win); 
};

#endif
