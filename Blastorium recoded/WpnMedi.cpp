#include "WpnMedi.h"

#include <vector>
#include <cfloat>
#include <string>
#include <fstream>
#include <ctime>

#include "Globals.h"
#include "Data.h"
#include "Level.h"
#include "Player.h"
#include "SFML\Graphics.hpp"
#include "SFML\Window.hpp" 
 
#define INVDUR 90		//3 seconds
#define SLODUR 180		//6 seconds
#define HEALCD 360		//12 seconds
#define MEDICD 720		//24 seconds
#define	PUHEAL 15		//powerup heal
#define RGHEAL 5		//base regen heal (+2*Level)

using namespace rapidxml;
using namespace std;
  
//////////////////////////////////////////////////////////////////////
//MEDIKITS (ID 4)
//
//PASSIVE SUPPORT EQUIPMENT, USED TO BOOST HEALTH AND TEMPORARY INVULNERABILITY
//PASSIVELY, GRANTS REGEN EVERY SEVERAL SECS AND GIVES EXTRA HP ON POWERUP PICK
//WHEN USED, GRANTS INVULNERABILITY, BUT SLOWS SELF FOR SEVERAL SECS AND DISABLES THE PASSIVE
//HAS LONG COOLDOWN
//
//INCREASING LEVELS INCREASE REGEN AND DECREASE CD
//AT LEVEL 5, MASSIVELY REDUCES INVUL SLOW
//REGEN				= 5 + 2*LEVEL
//REGEN INTERVAL	= 12s
//POWERUP HEAL		= 20
//INVUL DURATION	= 3s
//SLOW				= 60% (15% AT LEVEL 5)
//SLOW DURATION		= 6s
//COOLDOWN			= 24s - 2/LEVEL
//////////////////////////////////////////////////////////////////////
 
MediManager::MediManager(){  

}

MediManager::~MediManager(){

}

void MediManager::Construct(shared_ptr<GlobalManager> Glo) {
	Globals		=	Glo; 
	MainWindow	=	Glo->GlobalWindow;
	for(int i=0 ; i<4 ; ++i){
		HealCD[i]=HEALCD;
		MediCD[i]=0;
		IsMedi[i]=false;
	}
}  
			 
void MediManager::HasMedikit(int id, int slot){
	IsMedi[id]=true;
	MediID[id]=slot;
	return;
}
			 
void MediManager::UseMedikit(int id,int medilevel){
	if (!IsMedi[id]) return;
	if (MediCD[id]>0) return;

	double mult=1;
	if (medilevel==5) mult=0.85f; else mult=0.4f;

	//grants effects
	Globals->GlobalPlayerManager->Debuff(id);
	Globals->GlobalPlayerManager->Invul	(id , INVDUR);
	Globals->GlobalPlayerManager->ChgTmpSpeed (id , mult , SLODUR);
	
	//goes to CD
	MediCD[id]=MEDICD-(60*medilevel);

	return;
}
			 
void MediManager::UsePowerup(int id){
	if (!IsMedi[id]) return;
	if (MediCD[id]>0) return;

	Globals->GlobalPlayerManager->Damage(id,-PUHEAL);
	return;
}
 
void MediManager::MediLogic(){
	for(int id=0 ; id<4 ; ++id){
		if (!IsMedi[id]) continue;
		
		if (MediCD[id]>0){
			--MediCD[id];
		
		} else {
			--HealCD[id];
			if (HealCD[id] <= 0){
				int CurLVL=Globals->GlobalWeaponManager->NowLevel[id][MediID[id]];
				Globals->GlobalPlayerManager->Damage(id,-(RGHEAL+2*CurLVL));
				HealCD[id] = HEALCD;

			}
		}
	}
	return;
}
		

bool MediManager::IsAvailable(int id,int Medilevel){
	return (MediCD[id]==0);
}

int MediManager::GetAvailability(int id,int Medilevel){
	if (MediCD[id]==0) return -1;
	else return ((MediCD[id]+29)/30);
}
  