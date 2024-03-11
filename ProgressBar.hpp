//
// Created by fde-albe on 3/11/24.
//

#ifndef BIM_PROGRESSBAR_HPP
#define BIM_PROGRESSBAR_HPP

#include "BIM.hpp"

class ProgressBar {
    private:
        glm::vec2 position;
        glm::vec2 size;
        float progress = 10.0f;
        float maxProgress;
        float minProgress;
        float vertices[54];
        int vertexCount;
        Texture texture;
        RenderBatch renderBatch;
    public:
        ProgressBar(glm::vec2 position, glm::vec2 size, float maxProgress, float minProgress, const char *texturePath) : position(position), size(size), maxProgress(maxProgress), minProgress(minProgress), texture(texturePath),
                                                                                                                         renderBatch(vertices, 6, *texture.getTextureID()) {
            vertices[0] = position.x;
            vertices[1] = position.y;
            vertices[2] = 0.0f;
            vertices[3] = 0.05f;
            vertices[4] = 0.0f;
            vertices[5] = 0.0f;
            vertices[6] = 0.0f;
            vertices[7] = 0.0f;
            vertices[8] = 0.0f;
            vertices[9] = position.x + size.x;
            vertices[10] = position.y;
            vertices[11] = 0.0f;
            vertices[12] = 1.0f;
            vertices[13] = 0.0f;
            vertices[14] = 0.0f;
            vertices[15] = 0.0f;
            vertices[16] = 0.0f;
            vertices[17] = 0.0f;
            vertices[18] = position.x;
            vertices[19] = position.y + size.y;
            vertices[20] = 0.0f;
            vertices[21] = 0.05f;
            vertices[22] = 1.0f;
            vertices[23] = 0.0f;
            vertices[24] = 0.0f;
            vertices[25] = 0.0f;
            vertices[26] = 0.0f;
            vertices[27] = position.x;
            vertices[28] = position.y + size.y;
            vertices[29] = 0.0f;
            vertices[30] = 0.05f;
            vertices[31] = 1.0f;
            vertices[32] = 0.0f;
            vertices[33] = 0.0f;
            vertices[34] = 0.0f;
            vertices[35] = 0.0f;
            vertices[36] = position.x + size.x;
            vertices[37] = position.y;
            vertices[38] = 0.0f;
            vertices[39] = 1.05f;
            vertices[40] = 0.0f;
            vertices[41] = 0.0f;
            vertices[42] = 0.0f;
            vertices[43] = 0.0f;
            vertices[44] = 0.0f;
            vertices[45] = position.x + size.x;
            vertices[46] = position.y + size.y;
            vertices[47] = 0.0f;
            vertices[48] = 1.05f;
            vertices[49] = 1.0f;
            vertices[50] = 0.0f;
            vertices[51] = 0.0f;
            vertices[52] = 0.0f;
            vertices[53] = 0.0f;

            glGenTextures(1, texture.getTextureID());
            glBindTexture(GL_TEXTURE_2D, *texture.getTextureID());
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.getWidth(), texture.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, texture.getPixels().data());
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        float *getVertices() {
            vertices[9] = position.x + size.x * (progress / maxProgress);
            vertices[36] = position.x + size.x * (progress / maxProgress);
            vertices[45] = position.x + size.x * (progress / maxProgress);
            return vertices;
        }

        int getVertexCount() {
            return 6;
        }

        Texture getTexture() {
            return texture;
        }

        void    setProgress(float progress) {
            this->progress = progress;
        }

        void increaseProgress(float amount) {
            progress += amount;
            if (progress > maxProgress)
                progress = maxProgress;
        }

        void    draw() {
            renderBatch.setVertices(getVertices(), getVertexCount());
            renderBatch.setTexture(*texture.getTextureID());
            renderBatch.draw();
        }
};

#endif //BIM_PROGRESSBAR_HPP
