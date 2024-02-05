//
// Created by kikop on 05/02/2024.
//

#ifndef BIM_TEXTURE_HPP
#define BIM_TEXTURE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "glMath.hpp"

class Texture {
	private:
		unsigned int textureID;
		int width, height, nrChannels;
		const char *path;
		std::vector<Pixel> pixels;
	public:
		Texture(const char *path);
		~Texture();
		unsigned int getTextureID();
		int getWidth();
		int getHeight();
		int getNrChannels();
		std::vector<Pixel> getPixels();
		bool loadBMP();
};

#endif //BIM_TEXTURE_HPP
