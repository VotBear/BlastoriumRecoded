#include "TextureManager.h"

#include <SFML/Graphics.hpp>
#include <vector>

//This texturemanager is for gameplay textures only
//Graphics for title screens, etc will be covered separately

TextureManager::TextureManager(){

} 

TextureManager::~TextureManager(){

}

void TextureManager::AddTexture(sf::Texture &Texture){ 
	TextureList.push_back(Texture);
}

void TextureManager::ClearTexture(){
	TextureList.clear();
}

sf::Texture& TextureManager::GetTexture(int TextureIndex){
	return TextureList[TextureIndex];
}
