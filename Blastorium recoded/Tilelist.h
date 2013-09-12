#ifndef _TILELIST_H
#define _TILELIST_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory> 

class Tilelist {
	private:
		std::vector<sf::Sprite> SpriteList;

	public:
			////Constructor
		Tilelist();

			////Deconstructor
		~Tilelist();
		 
			////Adds entire texture as a tile
		void AddTile(sf::Texture &BaseTexture);
		
			////Adds a portion of texture as a tile
		void AddTile(sf::Texture &BaseTexture,int xst,int yst,int xlen,int ylen);
		
			////Draws tile [id] on window [targetwin] at position [x] and [y]
		void DrawTile(int id,double x,double y,std::shared_ptr<sf::RenderWindow> targetwin);
		
			////Draws tile [id] on texture [targettex] at position [x] and [y]
		void DrawTile(int id,double x,double y,sf::RenderTexture *targettex);
		
			////Draws tile [id] (with opacity [tra]) on window [targetwin] at position [x] and [y]
		void DrawTile(int id,double x,double y,int tra,std::shared_ptr<sf::RenderWindow> targetwin); 

			////Draws tile [id] (with opacity [tra]) on texture [targettex] at position [x] and [y]
		void DrawTile(int id,double x,double y,int tra,sf::RenderTexture *targettex);

			////Draws tile [id] (size scaled with [xsc] and [ysc]) on window [targetwin] at position [x] and [y]
		void DrawTile(int id,double x,double y,double xsc,double ysc,std::shared_ptr<sf::RenderWindow> targetwin); 

			////Draws tile [id] (size scaled with [xsc] and [ysc]) on texture [targettex] at position [x] and [y]
		void DrawTile(int id,double x,double y,double xsc,double ysc,sf::RenderTexture *targettex); 

			////Sets tile [id]'s size scale
		void SetScale(int id,double xsc,double ysc);
		
			////Sets tile [id]'s transparency to [tra] (0..255, transparent to solid)
		void SetColor(int id,double tra);
		
			////Modifies tile [id]'s texture rectangle to [nxlen],[nylen]
		void SetSize (int id,int nxlen,int nylen);
		
			////rotates tile [id] (this increases current rotation by [amt] amount)
		void Rotate (int id,double amt);

			////sets rotation of tile [id] (this sets it to exactly [amt] amount, unlike rotate which is relative)
		void SetRotation (int id, double amt);
		
			////Sets sprite origin to center
		void SetOriginCenter (int id);

			////Resets tilelist.
		void ClearTile();
};

#endif
