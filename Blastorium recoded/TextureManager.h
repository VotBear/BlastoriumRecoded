#ifndef _TEXTUREMANAGER_H
#define _TEXTUREMANAGER_H

#include <SFML\Graphics.hpp>
#include <vector>

class TextureManager {
	private: 
		std::vector<sf::Texture> TextureList;
	public:
		
			////Constructor
		TextureManager();
			
			////Deconstructor
		~TextureManager();
		
			////Add texture
			////IMPORTANT: MAKE SURE EVERY TEXTURE NEEDED IS LOADED BEFORE REFERENCING THEM TO TILELIST MANAGER
			////ADDING A NEW TEXTURE WILL CAUSE ALL PRIOR TILELIST POINTERS TO BECOME INVALID
		void AddTexture(sf::Texture &Texture); 
		
			////Resets texturemanager.
		void ClearTexture();
			
			////Gets texture no. [TextureIndex]
		sf::Texture& GetTexture(int TextureIndex);
};

#endif