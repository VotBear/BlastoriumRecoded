#ifndef _INTERFACE_H
#define _INTERFACE_H
 
#include <vector>  
#include "Tilelist.h"
#include "TextureManager.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 

//Transition screen -> fade
//Gradually covers screen, using [bg] as background
void EndScreen(std::shared_ptr<sf::RenderWindow> Win, sf::Texture &bg);

//Transition screen -> appear
//Gradually uncovers screen, using [bg] as background
void StartScreen(std::shared_ptr<sf::RenderWindow> Win, sf::Texture &bg);
 
//Same as EndScreen, but renders only a single layer of iteration ite
//Used for animations where the background is still moving/dynamic
void TEndScreen(std::shared_ptr<sf::RenderWindow> Win, int ite);

//Same as StartScreen, but renders only a single layer of iteration ite
//Used for animations where the background is still moving/dynamic
void TStartScreen(std::shared_ptr<sf::RenderWindow> Win, int ite);

//Endgame Selection screen -> renders until it receives input
//returns int: Play again (2), Main Menu(1) or exit (0)
int EndGameChoice(std::shared_ptr<sf::RenderWindow> Win, sf::Texture &bg);

//Pause Selection screen -> renders until it receives input
//returns int: Resume (2), Main Menu(1) or exit (0)
int PauseGameChoice(std::shared_ptr<sf::RenderWindow> Win, sf::Texture &bg);

//Main menu screen -> renders all gears, and receives input. Loops until input received.
//returns int: Play game (3), Options/Help(2), Exit(0), Cancelled (-1)
int MainMenu(std::shared_ptr<sf::RenderWindow> Win);

//Options screen (WIP)
void Options(std::shared_ptr<sf::RenderWindow> Win);

//Help screen (WIP)
void Help(std::shared_ptr<sf::RenderWindow> Win);

#endif
