//
// Created by fde-albe on 3/12/24.
//

#ifndef BIM_SKYBOX_HPP
#define BIM_SKYBOX_HPP

#include "BIM.hpp"

class Skybox {
	private:
		unsigned int textureID;
		std::vector<std::string> faces = {
			"./Resources/Textures/Skybox/right.jpg",
			"./Resources/Textures/Skybox/left.jpg",
			"./Resources/Textures/Skybox/top.jpg",
			"./Resources/Textures/Skybox/bottom.jpg",
			"./Resources/Textures/Skybox/front.jpg",
			"./Resources/Textures/Skybox/back.jpg"
		};
		GLuint VBO, VAO;
		float vertices[108] = {
			-100.0, 100.0, -100.0,
			-100.0, -100.0, -100.0,
			100.0, -100.0, -100.0,
			100.0, -100.0, -100.0,
			100.0, 100.0, -100.0,
			-100.0, 100.0, -100.0,

			-100.0, -100.0, 100.0,
			-100.0, -100.0, -100.0,
			-100.0, 100.0, -100.0,
			-100.0, 100.0, -100.0,
			-100.0, 100.0, 100.0,
			-100.0, -100.0, 100.0,

			100.0, -100.0, -100.0,
			100.0, -100.0, 100.0,
			100.0, 100.0, 100.0,
			100.0, 100.0, 100.0,
			100.0, 100.0, -100.0,
			100.0, -100.0, -100.0,

			-100.0, -100.0, 100.0,
			-100.0, 100.0, 100.0,
			100.0, 100.0, 100.0,
			100.0, 100.0, 100.0,
			100.0, -100.0, 100.0,
			-100.0, -100.0, 100.0,

			-100.0, 100.0, -100.0,
			100.0, 100.0, -100.0,
			100.0, 100.0, 100.0,
			100.0, 100.0, 100.0,
			-100.0, 100.0, 100.0,
			-100.0, 100.0, -100.0,

			-100.0, -100.0, -100.0,
			-100.0, -100.0, 100.0,
			100.0, -100.0, -100.0,
			100.0, -100.0, -100.0,
			-100.0, -100.0, 100.0,
			100.0, -100.0, 100.0
		};

public:
		Skybox() {
			glGenVertexArrays(1, &this->VAO);
			glGenBuffers(1, &this->VBO);
			glBindVertexArray(this->VAO);
			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108, &this->vertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glBindVertexArray(0);

			loadSkybox();
		}
		void    loadSkybox() {
			glGenTextures(1, &this->textureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);

			int width, height, nrChannels;
			for (unsigned int i = 0; i < this->faces.size(); i++) {
				unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
				if (data) {
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
					stbi_image_free(data);
				} else {
					std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
					stbi_image_free(data);
				}
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}

		unsigned int getTextureID() {
			return this->textureID;
		}

		void draw() {
			glDepthMask(GL_FALSE);
			glBindVertexArray(this->VAO);
			glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
		}
};

#endif //BIM_SKYBOX_HPP
