//
// Created by pirexa on 16-02-2024.
//

#include "BIM.hpp"

void    Model::vertexBufferSetup(GLuint shaderProgram) {
    glGenTextures(1, this->getTexture().getTextureID());
    glBindTexture(GL_TEXTURE_2D, *this->getTexture().getTextureID());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->getTexture().getWidth(), this->getTexture().getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, this->getTexture().getPixels().data());
    glBindTexture(GL_TEXTURE_2D, 0);

    // Vertex Buffer Object (VBO)
//    GLuint VBO;
    glGenBuffers(1, &VBO);

    // Vertex Array Object (VAO)
//    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind VBO and copy vertex data to it
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * this->getObj().getVertexCount() , this->getObj().getVerticesArray(), GL_STATIC_DRAW);

    // Specify vertex attribute pointers and enable them
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}