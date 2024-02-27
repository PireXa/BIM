//
// Created by fde-albe on 2/22/24.
//

#ifndef BIM_GUI_HPP
#define BIM_GUI_HPP

#include "BIM.hpp"

class GUI {
    private:
        glm::vec2 position;
        int width, height;
        float vertices[36];
        GLuint VAO, VBO;
        Texture texture;
    public:
        GUI() : texture("./Resources/Textures/pattern4.bmp") {
            width = 100;
            height = 100;
            position = glm::vec2(100.0f, 100.0f);
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
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            glBindVertexArray(0);

            glGenTextures(1, texture.getTextureID());
            glBindTexture(GL_TEXTURE_2D, *texture.getTextureID());
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.getWidth(), texture.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, texture.getPixels().data());
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        ~GUI() {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
        }

        void    setPosition(glm::vec2 position) {
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

        void draw() {
            glBindVertexArray(VAO);
            glBindTexture(GL_TEXTURE_2D, *texture.getTextureID());
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0);
        }

        int isClicked(glm::vec2 click) {
            click = glm::vec2(click.x, WIN_HEIGHT - click.y);
            //Top Right Corner
            glm::vec2 topRight = glm::vec2(position.x + width, position.y + height);
            int boundry = 10;
            if (click.x >= topRight.x - boundry && click.x <= topRight.x + boundry && click.y >= topRight.y - boundry && click.y <= topRight.y + boundry)
                return 1;
            //Top Left Corner
            glm::vec2 topLeft = glm::vec2(position.x, position.y + height);
            if (click.x >= topLeft.x - boundry && click.x <= topLeft.x + boundry && click.y >= topLeft.y - boundry && click.y <= topLeft.y + boundry)
                return 2;
            //Bottom Right Corner
            glm::vec2 bottomRight = glm::vec2(position.x + width, position.y);
            if (click.x >= bottomRight.x - boundry && click.x <= bottomRight.x + boundry && click.y >= bottomRight.y - boundry && click.y <= bottomRight.y + boundry)
                return 3;
            //Bottom Left Corner
            glm::vec2 bottomLeft = glm::vec2(position.x, position.y);
            if (click.x >= bottomLeft.x - boundry && click.x <= bottomLeft.x + boundry && click.y >= bottomLeft.y - boundry && click.y <= bottomLeft.y + boundry)
                return 4;
            //Inside GUI
            if (click.x >= position.x && click.x <= position.x + width && click.y >= position.y && click.y <= position.y + height)
                return 5;
            return false;
        }

        void    dragResize(glm::vec2 beginDrag, glm::vec2 currentDrag, int corner)
        {
            beginDrag = glm::vec2(beginDrag.x, WIN_HEIGHT - beginDrag.y);
            currentDrag = glm::vec2(currentDrag.x, WIN_HEIGHT - currentDrag.y);
            glm::vec2 translation = currentDrag - beginDrag;
            if (corner == 1) {
                addWidth(translation.x);
                addHeight(translation.y);
            }
            else if (corner == 2) {
                addPositionWidth(translation.x);
                addWidth(-translation.x);
                addHeight(translation.y);
            }
            else if (corner == 3) {
                addPositionHeight(translation.y);
                addWidth(translation.x);
                addHeight(-translation.y);
            }
            else if (corner == 4) {
                addPositionWidth(translation.x);
                addPositionHeight(translation.y);
                addWidth(-translation.x);
                addHeight(-translation.y);
            }
            else if (corner == 5) {
                translate(translation);
            }
        }
};

#endif //BIM_GUI_HPP
