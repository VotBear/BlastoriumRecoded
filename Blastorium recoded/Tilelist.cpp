#include "Tilelist.h"

#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;
Tilelist::Tilelist(){

}

Tilelist::~Tilelist(){
	SpriteList.clear();
}

void Tilelist::AddTile(sf::Texture &BaseTexture){
	sf::Sprite BaseSprite;
	BaseSprite.setTexture(BaseTexture);
	SpriteList.push_back(BaseSprite);
	return;
}

void Tilelist::AddTile(sf::Texture &BaseTexture,int xst,int yst,int xlen,int ylen){
	sf::Sprite BaseSprite;
	BaseSprite.setTexture(BaseTexture);
	BaseSprite.setTextureRect(sf::IntRect(xst,yst,xlen,ylen));
	SpriteList.push_back(BaseSprite);
	return;
} 

void Tilelist::DrawTile(int id,double x,double y,shared_ptr<sf::RenderWindow> targetwin){
	if (id<SpriteList.size()){
		SpriteList[id].setPosition(x,y); 
		targetwin->draw(SpriteList[id]);
	}
	return;
}

void Tilelist::DrawTile(int id,double x,double y,sf::RenderTexture *targettex){
	if (id<SpriteList.size()){
		SpriteList[id].setPosition(x,y); 
		targettex->draw(SpriteList[id]);
	}
	return;
}

void Tilelist::DrawTile(int id,double x,double y,int tra,shared_ptr<sf::RenderWindow> targetwin){
	if (id<SpriteList.size()){
		SetColor(id,tra);
		SpriteList[id].setPosition(x,y); 
		targetwin->draw(SpriteList[id]);
		SetColor(id,255);
	}
	return;
}

void Tilelist::DrawTile(int id,double x,double y,int tra,sf::RenderTexture *targettex){
	if (id<SpriteList.size()){
		SetColor(id,tra);
		SpriteList[id].setPosition(x,y); 
		targettex->draw(SpriteList[id]);
		SetColor(id,255);
	}
	return;
}

void Tilelist::DrawTile(int id,double x,double y,double xsc,double ysc,shared_ptr<sf::RenderWindow> targetwin){
	if (id<SpriteList.size()){
		SpriteList[id].setScale(xsc,ysc);
		SpriteList[id].setPosition(x,y); 
		targetwin->draw(SpriteList[id]);
	}
	return;
}

void Tilelist::DrawTile(int id,double x,double y,double xsc,double ysc,sf::RenderTexture *targettex){
	if (id<SpriteList.size()){
		SpriteList[id].setScale(xsc,ysc);
		SpriteList[id].setPosition(x,y); 
		targettex->draw(SpriteList[id]);
	}
	return;
}

void Tilelist::SetScale(int id,double xsc,double ysc){
	if (id<SpriteList.size()) SpriteList[id].setScale(xsc,ysc);
	return;
}

void Tilelist::SetColor(int id,double tra){
	if (id<SpriteList.size()) SpriteList[id].setColor(sf::Color(255,255,255,tra));
	return;
}

void Tilelist::SetSize(int id,int nxlen,int nylen){
	if (id<SpriteList.size()) {
		sf::IntRect prev=SpriteList[id].getTextureRect();
		prev.width=nxlen;
		prev.height=nylen;
		SpriteList[id].setTextureRect(prev);
	}
	return;
}

void Tilelist::Rotate (int id,double amt){
	SpriteList[id].rotate(amt);
	return;
}

void Tilelist::SetRotation (int id,double amt){
	SpriteList[id].setRotation(amt);
	return;
}
			 
void Tilelist::SetOriginCenter (int id){
	SpriteList[id].setOrigin(SpriteList[id].getLocalBounds().width/2,SpriteList[id].getLocalBounds().height/2);
	return;
}

void Tilelist::ClearTile(){
	SpriteList.clear();
	return;
}
