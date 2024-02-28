//
// Created by pirexa on 16-02-2024.
//

#ifndef BIM_DEFAULTPLANE_HPP
#define BIM_DEFAULTPLANE_HPP

#include <iostream>
#include "GLEW/glew.h"
#include "Texture.hpp"
#include "Button.hpp"

class DefaultPlane {
    private:
        int vertexCount = 6;
		float vertices[36] = {
			// Position            // Texture coordinates
			-1000.0f, -5.0f, -1000.0f,    0.0f, 0.0f, 0.0f,  // Bottom-left
            -1000.0f, -5.0f, 1000.0f,     0.0f, 10.0f, 0.0f,  // Top-left
			 1000.0f, -5.0f, -1000.0f,     5.0f, 0.0f, 0.0f, // Bottom-right
			1000.0f, -5.0f, -1000.0f,     5.0f, 0.0f, 0.0f,  // Bottom-right
            -1000.0f, -5.0f, 1000.0f,     0.0f, 10.0f, 0.0f,  // Top-left
			1000.0f, -5.0f, 1000.0f,      5.0f, 10.0f, 0.0f,  // Top-right
		};
		GLuint VAO;
		GLuint VBO;
		Texture texture;
	public:
		DefaultPlane(const char *texturePath) : texture(texturePath) {
			// Generate VBO and VAO for the XZ plane
			glGenBuffers(1, &VBO);
			glGenVertexArrays(1, &VAO);

			// Bind VAO for the XZ plane
			glBindVertexArray(VAO);

			// Bind VBO for the XZ plane and copy vertex data to it
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * vertexCount, vertices, GL_STATIC_DRAW);

			// Specify vertex attribute pointers and enable them for the XZ plane
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			// Unbind VAO and VBO for the XZ plane
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			// Generate texture ID for the XZ plane
			glGenTextures(1, texture.getTextureID());

			// Bind the texture and set its parameters
			glBindTexture(GL_TEXTURE_2D, *texture.getTextureID());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Load texture data into the texture object
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.getWidth(), texture.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, texture.getPixels().data());

			// Unbind the texture
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		~DefaultPlane() {
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
		}

		GLuint getVAO() {
			return VAO;
		}

		GLuint getVBO() {
			return VBO;
		}

		Texture getTexture() {
			return texture;
		}

		int getVertexCount() {
			return vertexCount;
		}

        void    draw() {
            // Bind the texture for the XZ plane
            glBindTexture(GL_TEXTURE_2D, *texture.getTextureID());

            // Bind the VAO for the XZ plane
            glBindVertexArray(VAO);

            // Draw the XZ plane
            glDrawArrays(GL_TRIANGLES, 0, vertexCount);

            // Unbind the VAO for the XZ plane
            glBindVertexArray(0);

            // Unbind the texture for the XZ plane
            glBindTexture(GL_TEXTURE_2D, 0);
        }
};

#endif //BIM_DEFAULTPLANE_HPP
