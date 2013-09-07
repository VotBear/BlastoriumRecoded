#ifndef _LEVEL_H
#define _LEVEL_H
 
#include <vector>
#include <string>
#include <fstream>
 
#include "TextureManager.h"
#include "Tilelist.h"
#include "Data.h"

#include "rapidxml.hpp"

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

using namespace std;

//forward reference to globals
class GlobalManager;

class Level{
	private:
		std::vector<std::pair<int,int> > StartPosition;
		int usedpos,softamt;
		TextureManager LevelTextureManager; 
		Tilelist LevelTilelist;
		shared_ptr<GlobalManager>	 Globals;
		shared_ptr<sf::RenderWindow> MainWindow;
		int LevelID,LevelMap[25][25];
	public:  
		int SoftMap[25][25],SoftHP[25][25]; 
			 
			////constructor 
		Level();

			////deconstructor
		~Level();
		
			////initialize
		void Construct(shared_ptr<GlobalManager> Glo);
		
			////initialization
		void Init();		

			////Sets the level to map no. [ID], loading and generating as needed
		void SetLevel(int ID);

			////loads map structure and tileset based on ID
			////also calls GenerateSoftTiles
		void LoadLevel(); 
		
			////generates (amt) soft tiles across the map using random distribution
		void GenerateSoftTiles(int amt); 
		
			////renders the level
		void RenderLevel(); 
		
			////gets the level's starting position for players (x/col, y/row)
			////called by the PlayerManager class to decide players' starting points
		std::pair<int,int> GetStartPos();
};

#endif