//
// Created by pirexa on 16-02-2024.
//

#include "BIM.hpp"

void    Model::vertexBufferSetup() {
    glGenTextures(1, this->getTexture().getTextureID());
    glBindTexture(GL_TEXTURE_2D, *this->getTexture().getTextureID());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->getTexture().getWidth(), this->getTexture().getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, this->getTexture().getPixels().data());
    glBindTexture(GL_TEXTURE_2D, 0);
}