#include <SFML/Graphics.hpp>
#include "Engine.h"
#include "Interface.h"
#include "Select.h"
#include "Globals.h"

#include <stdlib.h>
 
int main()
{

	//print some stuff to terminal
	printf("Blastorium:Recoded v1.2.1.2 Beta (Debugging version)\n");
	
	printf("WASD for Player 1 to move, Arrow keys for player 2\n");
	printf("For player 1, press E to use weapon/select, and Q to change weapon/cancel select\n");
	printf("For player 2, press Right Shift to use weapon/select, and Right Ctrl to change weapon/cancel select\n"); 
	printf("Press tilde (~) for screenshot\n"); 
	printf("Press escape or the close button to exit\n\n"); 
	
	printf("For possible further updates, check vot1bear.blogspot.com\n");
	printf("(c) 2013 Vot1_Bear\n"); 
	
	shared_ptr<sf::RenderWindow> MainWindow(nullptr);
	shared_ptr<GlobalManager>	 MainGlobalManager(nullptr);
	shared_ptr<SelectManager>    MainSelect(nullptr);
	shared_ptr<Engine>			 MainEngine(nullptr);
	//initialize classes
	MainWindow = make_shared<sf::RenderWindow> (sf::VideoMode(864,480),"Blastorium:Recoded");
	MainWindow -> setFramerateLimit(30); 

	MainGlobalManager = make_shared<GlobalManager>(MainWindow);

	MainSelect = make_shared<SelectManager>();
	MainSelect -> InitSelect();
	 
	MainEngine = make_shared<Engine> (MainGlobalManager);              

	//GUI loop
	puts("Test -2");
	while(MainWindow->isOpen()){
		int now=MainMenu(MainWindow);								//receive input from main menu
		if (now==0||!MainWindow->isOpen()) {						//break if game closed
			puts("Test Exit"); 
			return 0;
			break;
		}
		else if (now==3){											//play game, go to select screen
			puts("Test -1");
			pair < int, vector<int> > sel=MainSelect->SelectScreen(MainWindow); //receive input from select screen
			puts("Test 0");
			if (sel.first==0) exit(0);								//break if game closed
			if (sel.first==-1) continue;							//cancel select, back to main menu
			puts("Test 1");                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
			MainEngine->Go(sel.first-1,sel.second);					//else go to game
			puts("Test 5");
		} else {
			//help screen, WIP
		} 
	} 
	return(0);
}