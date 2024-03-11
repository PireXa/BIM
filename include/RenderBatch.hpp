//
// Created by fde-albe on 3/4/24.
//

#ifndef BIM_RENDERBATCH_HPP
#define BIM_RENDERBATCH_HPP

#include "BIM.hpp"

class RenderBatch {
    private:
        GLuint VAO, VBO;
        float *vertices;
        int vertexCount;
        unsigned int textureID;
    public:
        RenderBatch(float *vertices, int vertexCount, unsigned int textureID) : vertices(vertices), vertexCount(vertexCount), textureID(textureID) {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9 * this->vertexCount, this->vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        ~RenderBatch() {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
        }

        void    setVertices(float *vertices, int vertexCount) {
            this->vertices = vertices;
            this->vertexCount = vertexCount;
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9 * this->vertexCount, this->vertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void    setTexture(unsigned int textureID) {
            this->textureID = textureID;
        }

        void    setVerticeArrayObject() {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9 * this->vertexCount, this->vertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void draw() {
            glBindTexture(GL_TEXTURE_2D, textureID);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, vertexCount);
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
};

#endif //BIM_RENDERBATCH_HPP
