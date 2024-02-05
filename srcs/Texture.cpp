//
// Created by kikop on 05/02/2024.
//

#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION

Texture::Texture(const char *path) : path(path) {
	loadBMP();
}

Texture::~Texture() = default;

unsigned int Texture::getTextureID() {
	return textureID;
}

int Texture::getWidth() {
	return width;
}

int Texture::getHeight() {
	return height;
}

int Texture::getNrChannels() {
	return nrChannels;
}

std::vector<Pixel> Texture::getPixels() {
	return pixels;
}

bool    Texture::loadBMP() {
	std::ifstream file(this->path, std::ios::binary);

	if (!file.is_open()) {
		std::cerr << "Error opening file: " << this->path << std::endl;
		return false;
	}

	// Read BMP header
	char header[54];
	file.read(header, 54);

	// Extract width and height from the header
	this->width = *(int*)&header[18];
	this->height = *(int*)&header[22];

	// BMP uses 24 bits per pixel (RGB)
	int dataSize = width * height * 3;
	this->pixels.resize(dataSize / 3);

	// Read pixel data
	file.read(reinterpret_cast<char*>(this->pixels.data()), dataSize);

	// BMP stores pixel data in BGR format, so swap R and B components
	for (auto& pixel : this->pixels) {
		std::swap(pixel.r, pixel.b);
	}

	file.close();

	return true;
}

