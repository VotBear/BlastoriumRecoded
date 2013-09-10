#include "Data.h"

#include <vector> 
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
 
#include "Interface.h" 

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
 
//constructor
DataManager::DataManager(){
	LoadData(); 
	Init();
	SaveData();
}

//deconstructor
DataManager::~DataManager(){

} 

void DataManager::Init(){
	memset(TPowerupsTaken,0,sizeof TPowerupsTaken);
	memset(TDamageDealt,0,sizeof TDamageDealt);
	memset(TWeaponsUsed,0,sizeof TWeaponsUsed);
	memset(TWeaponsStat,0,sizeof TWeaponsStat);
	TBlocksDestroyed=0;
	return;
}
 
//Temporary data increment: damage dealt
void DataManager::DealDamage(int id,int damage){
	TDamageDealt[id]+=damage;
	return;
}

//Temporary data increment: powerups taken
void DataManager::TakePowerup(int id){
	++TPowerupsTaken[id];
	return;
}

//Temporary data increment: weapons used
void DataManager::UseWeapon(int id){
	++TWeaponsUsed[id];
	return;
}
 
//Temporary data increment: destroyed blocks
void DataManager::DestroyBlock(){
	++TBlocksDestroyed;
	return;
}

//Temporary data increment: weapon-based numbers
void DataManager::UpdateWeapon(int wpnid,int amt){
	TWeaponsStat[wpnid]+=amt;
	return;
}

//Updates the data loaded with the numbers from recently finished game. 
void DataManager::UpdateData(){
	for(int id=0;id<2;++id){
		MainData.DamageDealt=min(MainData.DamageDealt+TDamageDealt[id],999999999LL);
		MainData.WeaponsUsed=min(MainData.WeaponsUsed+TWeaponsUsed[id],999999999LL);
		MainData.PowerupsTaken=min(MainData.PowerupsTaken+TPowerupsTaken[id],999999999LL);
	}
	MainData.BlocksDestroyed=min(MainData.BlocksDestroyed+TBlocksDestroyed,999999999LL);
	for(int id=0;id<=20;++id) {
		MainData.WeaponsStat[id]=min(MainData.WeaponsStat[id]+TWeaponsStat[id],999999999LL);
	}
	++MainData.MatchesPlayed; 
	return;
} 

//Finishes match, showing statistics and stuff number with animation (which is why it's long, really. fuck animation)
//Will later call interface->Endgamechoice to retrieve input.
//returns int : Play again (2), Main Menu(1) or exit (0)
sf::Texture DataManager::FinishMatch(int id,shared_ptr<sf::RenderWindow> MainWindow){ 
	sf::Font Fnt;
	sf::RenderTexture TempBg;
	sf::Texture Bg;
	sf::Text Title,Txt,Num1,Num2;
	sf::Sprite Spr;
	std::string Number1, Number2;

	Fnt.loadFromFile("Fonts/bankgthd.ttf");
	TempBg.create(MainWindow->getSize().x,MainWindow->getSize().y);
	Bg.create(MainWindow->getSize().x,MainWindow->getSize().y);

	if (id==1) Title.setString("Player 1 Wins!"); 
	else if (id==2) Title.setString("Player 2 Wins!");
	else			Title.setString("Draw");
	Title.setFont(Fnt);	Title.setCharacterSize(45); Title.setOrigin(Title.getLocalBounds().width/2,0);
	Title.setPosition(MainWindow->getSize().x/2,20); 
	Txt.setFont(Fnt); Txt.setCharacterSize(30);
	Num1.setFont(Fnt); Num1.setCharacterSize(30);
	Num2.setFont(Fnt); Num2.setCharacterSize(30);

	TempBg.clear();
	TempBg.draw(Title);
	TempBg.display();
	Bg=TempBg.getTexture();

	StartScreen(MainWindow,Bg);
	sf::Event evt,t;
	int timecntr=0,ite=0;
	Spr.setTexture(Bg);
	while (MainWindow->pollEvent(t)) if (t.type==sf::Event::Closed){
		MainWindow->close(); 
		UpdateData();
		SaveData();
		return Bg;
	}
	while (true){  
		MainWindow->clear();
		MainWindow->draw(Spr);
		while(MainWindow->pollEvent(evt)){
			if (evt.type==sf::Event::Closed){
				MainWindow->close(); 
				UpdateData();
				SaveData();
				return Bg;

			} else if (evt.type==sf::Event::KeyReleased||evt.type==sf::Event::MouseButtonReleased) {	 
				if (ite<=3){  
					for (int i=ite;i<=3;++i){
						if (i==0) {
							Txt.setString("Damage Received");
							Number1 = to_string(TDamageDealt[0]);
							Number2 = to_string(TDamageDealt[1]);
						} else if (i==1)	{
							Txt.setString("Powerups Taken");
							Number1 = to_string(TPowerupsTaken[0]);
							Number2 = to_string(TPowerupsTaken[1]);
						} else if (i==2)	{
							Txt.setString("Weapons Used");
							Number1 = to_string(TWeaponsUsed[0]);
							Number2 = to_string(TWeaponsUsed[1]);
						} else if (i==3){
							string tmp="Blocks Destroyed  :  ";
							Number1 = to_string(TBlocksDestroyed);
							tmp+=Number1;
							Txt.setString(tmp);
							Number1[0]='\0';
							Number2[0]='\0';
						}
						Num1.setString(Number1);	Num2.setString(Number2); 
						Txt.setOrigin(Txt.getLocalBounds().width/2,0);	 Txt.setPosition(MainWindow->getSize().x/2,120+i*50);	
						Num1.setOrigin(Num1.getLocalBounds().width/2,0); Num1.setPosition(MainWindow->getSize().x/2-300,120+i*50);	
						Num2.setOrigin(Num2.getLocalBounds().width/2,0); Num2.setPosition(MainWindow->getSize().x/2+300,120+i*50);	
		 
						TempBg.clear();	TempBg.draw(Spr); TempBg.draw(Txt);	TempBg.draw(Num1);	TempBg.draw(Num2);	TempBg.display();

						Bg=TempBg.getTexture(); 
					}
					ite=4;
				} 
			}
		}
		timecntr=(timecntr+1)%60; 
		if (ite<=3) {
			if (ite==0) {
				Txt.setString("Damage Received");
				Number1 = to_string((TDamageDealt[0]*timecntr)/30);
				Number2 = to_string((TDamageDealt[1]*timecntr)/30);
			} else if (ite==1)	{
				Txt.setString("Powerups Taken");
				Number1 = to_string((TPowerupsTaken[0]*timecntr)/30);
				Number2 = to_string((TPowerupsTaken[1]*timecntr)/30);
			} else if (ite==2)	{
				Txt.setString("Weapons Used");	
				Number1 = to_string((TWeaponsUsed[0]*timecntr)/30);
				Number2 = to_string((TWeaponsUsed[1]*timecntr)/30);
			} else if (ite==3){
				string tmp="Blocks Destroyed  :  ";
				Number1 = to_string((TBlocksDestroyed*timecntr)/30);
				tmp+=Number1;
				Txt.setString(tmp);
				Number1[0]='\0';
				Number2[0]='\0';
			}
			Num1.setString(Number1);	Num2.setString(Number2);
			
			Txt.setOrigin(Txt.getLocalBounds().width/2,0);	 Txt.setPosition(MainWindow->getSize().x/2,120+ite*50);	
			Num1.setOrigin(Num1.getLocalBounds().width/2,0); Num1.setPosition(MainWindow->getSize().x/2-300,120+ite*50);	
			Num2.setOrigin(Num2.getLocalBounds().width/2,0); Num2.setPosition(MainWindow->getSize().x/2+300,120+ite*50);	
		
			MainWindow->draw(Txt);	
			MainWindow->draw(Num1);
			MainWindow->draw(Num2);
			if (timecntr>=30){
				TempBg.clear();	TempBg.draw(Spr); TempBg.draw(Txt);	TempBg.draw(Num1);	TempBg.draw(Num2);	TempBg.display();

				Bg=TempBg.getTexture();
				timecntr=0;
				++ite; 
			}
			MainWindow->display();
		} else if (ite==4) { 
			UpdateData();
			SaveData();
			return Bg;
		} 
	}
	return(Bg);
}

//Loads data from .bin file
void DataManager::LoadData(){
	ifstream Input_File("Data/data.bin",ios::binary); 
    Input_File.read((char*)&MainData, sizeof(MainData));
	Input_File.close();
	return;
	//debug
}

//Saves data to .bin file
void DataManager::SaveData(){ 
    ofstream output_file("Data/data.bin", ios::binary);
    output_file.write((char*)&MainData, sizeof(MainData));
    output_file.close(); 
	return;
}

//Prints data via terminal to inform user
void DataManager::PrintData(){
	printf("Lifetime Stats:\n");
	printf("Damage Dealt %lld\n",MainData.DamageDealt);
	printf("Powerups Taken %lld\n",MainData.PowerupsTaken);
	printf("Matches Played %lld\n",MainData.MatchesPlayed);
	printf("Weapons Used %lld\n",MainData.WeaponsUsed);
	printf("Blocks Destroyed %lld\n",MainData.BlocksDestroyed);
	for(int i=0;i<12;++i) {
		//MainData.WeaponsStat[i]=0;
		//printf("weapon %d: %lld\n",i,MainData.WeaponsStat[i]);
	}
	return;
}
