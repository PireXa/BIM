//
// Created by pirexa on 19-02-2024.
//

#ifndef BIM_FONT_HPP
#define BIM_FONT_HPP

#include <iostream>
#include "BIM.hpp"
#include "stb_image.h"

class Font {
	private:
		unsigned int textureID;
		int width;
		int height;
		GLuint VAO, VBO;
		struct Character {
			unsigned int TextureID;  // ID handle of the glyph texture
			glm::ivec2   Size;       // Size of glyph
			glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
			unsigned int Advance;    // Horizontal offset to advance to next glyph
			glm::ivec2  TexturePos;  // Position of the glyph in the texture
		};
		std::map<GLchar, Character> Characters;
		glm::mat4 matrix;
	public:
		Font(const char *texturePath) {

			// Generate VBO and VAO for the text
			glGenBuffers(1, &VBO);
			glGenVertexArrays(1, &VAO);

			// Bind VAO for the text
			glBindVertexArray(VAO);

			// Bind VBO for the text and copy vertex data to it
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 3, NULL, GL_DYNAMIC_DRAW);

			// Specify vertex attribute pointers and enable them for the text
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			// Unbind VAO and VBO for the text
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			// Set the texture wrapping/filtering options (on the currently bound texture object)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Load and generate the texture
			unsigned char *data = stbi_load(texturePath, &width, &height, 0, STBI_rgb_alpha);
			if (data) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			} else {
				std::cout << "Failed to load texture" << std::endl;
			}
			stbi_image_free(data);
			glBindTexture(GL_TEXTURE_2D, 0);

			matrix = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
		}

		void	readFNT(const char *fntPath) {
			std::ifstream file(fntPath);
			if (!file.is_open()) {
				std::cout << "Failed to open file: " << fntPath << std::endl;
				return;
			}
			std::string line;
			while (std::getline(file, line)) {
				if (line.find("char") != std::string::npos) {
					Character character;
					std::string id;
					std::string x;
					std::string y;
					std::string width;
					std::string height;
					std::string xoffset;
					std::string yoffset;
					std::string xadvance;
					std::string page;
					std::string chnl;
					std::string temp;
					std::stringstream ss(line);
					while (std::getline(ss, temp, ' ')) {
						if (temp.find("id=") != std::string::npos) {
							id = temp.substr(3);
						} else if (temp.find("x=") != std::string::npos) {
							x = temp.substr(2);
						} else if (temp.find("y=") != std::string::npos) {
							y = temp.substr(2);
						} else if (temp.find("width=") != std::string::npos) {
							width = temp.substr(6);
						} else if (temp.find("height=") != std::string::npos) {
							height = temp.substr(7);
						} else if (temp.find("xoffset=") != std::string::npos) {
							xoffset = temp.substr(8);
						} else if (temp.find("yoffset=") != std::string::npos) {
							yoffset = temp.substr(8);
						} else if (temp.find("xadvance=") != std::string::npos) {
							xadvance = temp.substr(9);
						} else if (temp.find("page=") != std::string::npos) {
							page = temp.substr(5);
						} else if (temp.find("chnl=") != std::string::npos) {
							chnl = temp.substr(5);
						}
					}
					if (id.empty() || x.empty() || y.empty() || width.empty() || height.empty() || xoffset.empty() || yoffset.empty() || xadvance.empty() || page.empty() || chnl.empty()) {
//						std::cout << "Failed to parse character" << std::endl;
//						std::cout << "line: " << line << std::endl;
						continue;
					}
					character.TextureID = textureID;
					character.Size = glm::ivec2(std::stoi(width), std::stoi(height));
					character.Bearing = glm::ivec2(std::stoi(xoffset), std::stoi(yoffset));
					character.Advance = std::stoi(xadvance);
					character.TexturePos = glm::ivec2(std::stoi(x), std::stoi(y));
					Characters[std::stoi(id)] = character;
				}
			}
		}

		unsigned int getTextureID() {
			return textureID;
		}

		int getWidth() {
			return width;
		}

		int getHeight() {
			return height;
		}

		glm::mat4 getMatrix() {
			return matrix;
		}

		void	renderText(std::string text, float x, float y, float scale, GLuint shaderProgram) {
			// Activate corresponding render state
			glUseProgram(shaderProgram);
			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(VAO);

			// Iterate through all characters
			std::string::const_iterator c;
			for (c = text.begin(); c != text.end(); c++) {
				Character ch = Characters[*c];
                if (*c >= 97 && *c <= 122)
                {
                    scale = 0.15f;
                }

				// Calculate texture coordinates based on the position and size of the character glyph in the texture atlas
				float texLeft = ch.TexturePos.x / static_cast<float>(width);   // Left edge of the character glyph
				float texTop = ch.TexturePos.y / static_cast<float>(height);   // Top edge of the character glyph
				float texRight = (ch.TexturePos.x + ch.Size.x) / static_cast<float>(width);   // Right edge of the character glyph
				float texBottom = (ch.TexturePos.y + ch.Size.y) / static_cast<float>(height);   // Bottom edge of the character glyph

				// Calculate the scaled dimensions of the character glyph
				float scaledWidth = ch.Size.x * scale;
				float scaledHeight = ch.Size.y * scale;

				// Calculate the scaled positions of the character glyph
				float scaledXPos = x + ch.Bearing.x * scale;
				float scaledYPos = y - (ch.Size.y - ch.Bearing.y) * scale;

				if (*c == ':' || *c == 'o' || *c == 'r' || *c == 's' || *c == 'u' || *c == 'v' || *c == 'w' || *c == 'x' || *c == 'z')
				{
					scaledYPos -= scale * 4.0f;
				}
//                if (*c == ':')
//                {
//                    scaledYPos -= scale * 10;
//                }
				if (*c == 46)
				{
					scaledYPos -= scale * 32;
				}

				// Update VBO for each character with correct texture coordinates
				float vertices[36] = {
						// Position x, y, z, Texture coordinates x, y, z
						scaledXPos,        scaledYPos,  0.0f, texLeft,  texBottom, 0.0f, // Bottom-left
						scaledXPos,        scaledYPos + scaledHeight,                0.0f, texLeft,  texTop,    0.0f, // Top-left
						scaledXPos + scaledWidth, scaledYPos + scaledHeight,         0.0f, texRight, texTop,    0.0f, // Top-right
						scaledXPos,        scaledYPos,  0.0f, texLeft,  texBottom, 0.0f, // Bottom-left
						scaledXPos + scaledWidth, scaledYPos + scaledHeight,         0.0f, texRight, texTop,    0.0f, // Top-right
						scaledXPos + scaledWidth, scaledYPos, 0.0f, texRight, texBottom, 0.0f  // Bottom-right
				};

				// Render glyph texture over quad
				glBindTexture(GL_TEXTURE_2D, ch.TextureID);
				// Update content of VBO memory
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, vertices, GL_STATIC_DRAW);
				// Render quad
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
				x += ch.Advance * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
			}
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void renderTriangle(GLuint shaderProgram) {
			glBindTexture(GL_TEXTURE_2D, textureID);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}


};

#endif //BIM_FONT_HPP
