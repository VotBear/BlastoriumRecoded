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
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"  

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
		HealCD[i]=MEDHCD;	//heal cooldown is default
		MediCD[i]=0;		//weapon cooldown is 0 (ready for use)
		IsMedi[i]=false;
	}
}  
			 
void MediManager::HasMedikit(int id, int slot){
	IsMedi[id]=true;
	MediID[id]=slot; 
}
			 
void MediManager::UseMedikit(int id,int medilevel){
	if (!IsMedi[id]) return;
	if (MediCD[id]>0) return;

	float mult;
	if (medilevel==5) mult=SLOW5; else mult=SLOWN;	//slow multiplier

	printf("%f\n",mult);
	//grants effects
	Globals->GlobalPlayerManager->Debuff(id);
	Globals->GlobalPlayerManager->ChgTmpSpeed (id , mult , MEDSLO);
	Globals->GlobalPlayerManager->Invul	(id , MEDINV);
	
	//goes to CD
	MediCD[id]=MEDWCD-(MEDCDD*medilevel); 
}
			 
void MediManager::UsePowerup(int id){
	if (!IsMedi[id]) return;
	if (MediCD[id]>0) return;

	//if medikit is active, heal
	Globals->GlobalPlayerManager->Damage(id,-PUHEAL);	 
}
 
void MediManager::MediLogic(){
	for(int id=0 ; id<2 ; ++id){
		if (!IsMedi[id]) continue;
		
		if (MediCD[id]>0){
			--MediCD[id];	//tick cooldown timer
		
		} else {
			--HealCD[id];	//tick cooldown timer
			if (HealCD[id] <= 0){
				int CURLVL=Globals->GlobalWeaponManager->NowLevel[id][MediID[id]];	//get current level

				Globals->GlobalPlayerManager->Damage(id,-(RGHEAL+DGHEAL*CURLVL));	//heal based on base & lvl
				HealCD[id] = MEDHCD;												//go to cooldown

			}
		}
	} 
}
		

bool MediManager::IsAvailable(int id,int Medilevel){
	return (MediCD[id]==0);				//return true if ready for use
}

int MediManager::GetAvailability(int id,int Medilevel){
	if (MediCD[id]==0) return -1;		//no number when ready for use
	else return ((MediCD[id]+29)/30);	//else return cooldown in seconds
}
  
