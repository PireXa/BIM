//
// Created by fde-albe on 2/8/24.
//

#ifndef BIM_MODEL_HPP
#define BIM_MODEL_HPP

#include "glMath.hpp"
#include "readOBJ.hpp"
#include "Texture.hpp"

class Model {
    private:
        glm::quat   orientation;
        glm::vec3   position;
        glm::vec3   center;
        BoundingBox boundingBox;
        float       scale;
        Texture     texture;
        readOBJ     obj;
        GLuint     VAO;
        GLuint     VBO;

    public:
        Model(const char *texturePath, const char *objPath, float textureScale) : orientation(), position(), texture(texturePath), obj(objPath, textureScale) {
            orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            position = glm::vec3(0.0f, 0.0f, 0.0f);
            center = obj.getCenter();
            boundingBox = obj.getBoundingBox();
            scale = glm::length(boundingBox.max - boundingBox.min);
        }

		~Model() {
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
		}

        void    vertexBufferSetup(GLuint shaderProgram);

        glm::vec3 getCenter() {
            return center;
        }

        BoundingBox getBoundingBox() {
            return boundingBox;
        }

        float  getScale() {
            return scale;
        }

        Texture getTexture() {
            return texture;
        }

        readOBJ getObj() {
            return obj;
        }

        GLuint getVAO() {
            return VAO;
        }

        GLuint getVBO() {
            return VBO;
        }

		glm::vec3 getPosition() {
			return position;
		}

        void    setCenter(glm::vec3 center) {
            this->center = center;
        }

        void    setBoundingBox(BoundingBox boundingBox) {
            this->boundingBox = boundingBox;
            scale = glm::length(boundingBox.max - boundingBox.min);
        }

        void    rotate(float angle, float x, float y, float z) {
            glm::quat rotation = glm::angleAxis(angle, glm::vec3(x, y, z));
            orientation = rotation * orientation;
            orientation = glm::normalize(orientation);
        }

        void    rotate_pitch(float pitch, glm::vec3 right) {
            glm::quat rotation = glm::angleAxis(-pitch, right);
            orientation = glm::normalize(rotation * orientation);
        }

        void    rotate_yaw(float yaw) {
            glm::quat rotation = glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f));
            orientation = glm::normalize(rotation * orientation);
        }

        void    translate(float x, float y, float z) {
            position.x += x;
            position.y += y;
            position.z += z;
            center.x += x;
            center.y += y;
            center.z += z;
        }

        glm::mat4 getModelMatrix() {
            glm::mat4 model = glm::mat4(1.0f);

            // Translate model to align centroid with origin
            model = glm::translate(model, center);

            // Apply rotation (using quaternion)
            glm::mat4 rotationMatrix = glm::mat4_cast(orientation);
            model = model * rotationMatrix;

            // Translate back to original position
            model = glm::translate(model, -center);

            // Apply model translation
            model = glm::translate(model, position);

            return model;
        }
        void    reset() {
            orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            position = glm::vec3(0.0f, 0.0f, 0.0f);
			center = obj.getCenter();
        }

        void    draw() {
            glBindVertexArray(VAO);
            glBindTexture(GL_TEXTURE_2D, *texture.getTextureID());
            glDrawArrays(GL_TRIANGLES, 0, obj.getVertexCount());
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0);
        }
};

#endif //BIM_MODEL_HPP
