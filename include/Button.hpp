//
// Created by fde-albe on 2/28/24.
//

#ifndef BIM_BUTTON_HPP
#define BIM_BUTTON_HPP

#include "GLM/glm.hpp"
#include "Texture.hpp"
#include "Font.hpp"

class Button {
    private:
        glm::vec2 position;
        int width, height;
        float vertices[36];
        GLuint VAO, VBO;
        Texture texture;
        TextFont font;
        const char *label;
    public:
        Button(int width, int height, glm::vec2 position, const char *Label) : width(width), height(height), position(position), texture("./Resources/Textures/Button.bmp"), font("./Fonts/Font3.png"), label(Label) {
            vertices[0] = position.x;
            vertices[1] = position.y;
            vertices[2] = 0.0f;
            vertices[3] = 0.0f;
            vertices[4] = 0.0f;
            vertices[5] = 0.0f;
            vertices[6] = position.x + width;
            vertices[7] = position.y;
            vertices[8] = 0.0f;
            vertices[9] = 1.0f;
            vertices[10] = 0.0f;
            vertices[11] = 0.0f;
            vertices[12] = position.x;
            vertices[13] = position.y + height;
            vertices[14] = 0.0f;
            vertices[15] = 0.0f;
            vertices[16] = 1.0f;
            vertices[17] = 0.0f;
            vertices[18] = position.x;
            vertices[19] = position.y + height;
            vertices[20] = 0.0f;
            vertices[21] = 0.0f;
            vertices[22] = 1.0f;
            vertices[23] = 0.0f;
            vertices[24] = position.x + width;
            vertices[25] = position.y;
            vertices[26] = 0.0f;
            vertices[27] = 1.0f;
            vertices[28] = 0.0f;
            vertices[29] = 0.0f;
            vertices[30] = position.x + width;
            vertices[31] = position.y + height;
            vertices[32] = 0.0f;
            vertices[33] = 1.0f;
            vertices[34] = 1.0f;
            vertices[35] = 0.0f;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // Generate texture ID for the button
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

        ~Button() {
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

        const char *getLabel() {
            return label;
        }

		void    translate(glm::vec2 translation) {
			position += translation;
			vertices[0] += translation.x;
			vertices[1] += translation.y;
			vertices[6] += translation.x;
			vertices[7] += translation.y;
			vertices[12] += translation.x;
			vertices[13] += translation.y;
			vertices[18] += translation.x;
			vertices[19] += translation.y;
			vertices[24] += translation.x;
			vertices[25] += translation.y;
			vertices[30] += translation.x;
			vertices[31] += translation.y;
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void addWidth(int width) {
			this->width += width;
			vertices[6] += width;
			vertices[24] += width;
			vertices[30] += width;
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void addHeight(int height) {
			this->height += height;
			vertices[13] += height;
			vertices[19] += height;
			vertices[31] += height;
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void addPositionWidth(int width) {
			this->position.x += width;
			vertices[0] += width;
			vertices[6] += width;
			vertices[12] += width;
			vertices[18] += width;
			vertices[24] += width;
			vertices[30] += width;
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void addPositionHeight(int height) {
			this->position.y += height;
			vertices[1] += height;
			vertices[7] += height;
			vertices[13] += height;
			vertices[19] += height;
			vertices[25] += height;
			vertices[31] += height;
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void	setWidth(int width) {
			this->width = width;
			vertices[6] = position.x + width;
			vertices[24] = position.x + width;
			vertices[30] = position.x + width;
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void	setHeight(int height) {
			this->height = height;
			vertices[13] = position.y + height;
			vertices[19] = position.y + height;
			vertices[31] = position.y + height;
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void	setPosition(glm::vec2 position) {
			this->position = position;
			vertices[0] = position.x;
			vertices[1] = position.y;
			vertices[6] = position.x + width;
			vertices[7] = position.y;
			vertices[12] = position.x;
			vertices[13] = position.y + height;
			vertices[18] = position.x;
			vertices[19] = position.y + height;
			vertices[24] = position.x + width;
			vertices[25] = position.y;
			vertices[30] = position.x + width;
			vertices[31] = position.y + height;
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

        void    setLabel(const char *label) {
            this->label = label;
        }

		void draw() {
			//Draw Button Background
			glBindVertexArray(VAO);
			glBindTexture(GL_TEXTURE_2D, *texture.getTextureID());
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(0);

			int length = strlen(label) * 0.3f * 20;
			// Draw Button Label
			font.renderText(label, position.x + width / 2 - length / 2, position.y + height / 2, 0.3f);
		}

		int isClicked(glm::vec2 click) {
			if (click.x >= position.x && click.x <= position.x + width && click.y >= position.y && click.y <= position.y + height)
				return 1;
			return 0;
		}
};

#endif //BIM_BUTTON_HPP
