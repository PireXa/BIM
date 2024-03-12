//
// Created by fde-albe on 2/8/24.
//

#ifndef BIM_MODEL_HPP
#define BIM_MODEL_HPP

#include "glMath.hpp"
#include "readOBJ.hpp"
#include "Texture.hpp"
#include "RenderBatch.hpp"

class Model {
    private:
        glm::quat   orientation;
        glm::vec3   position;
        glm::vec3   center;
        BoundingBox boundingBox;
        float       scale;
        Texture     texture;
        readOBJ     obj;
        int         buildAnimationVertexCount;
		RenderBatch renderBatch;

    public:
        Model(const char *texturePath, const char *objPath, float textureScale) : orientation(), position(), texture(texturePath), obj(objPath, textureScale),
		renderBatch(obj.getVerticesArray(), obj.getVertexCount(), *texture.getTextureID())
		{
            orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            position = glm::vec3(0.0f, 0.0f, 0.0f);
            center = obj.getCenter();
            boundingBox = obj.getBoundingBox();
            scale = glm::length(boundingBox.max - boundingBox.min);
        }

        void    vertexBufferSetup();

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

        int getVertexCount() {
            return obj.getVertexCount();
        }

        int getBuildAnimationVertexCount() {
            return buildAnimationVertexCount;
        }

		glm::vec3 getPosition() {
			return position;
		}

		RenderBatch *getRenderBatch() {
			return &renderBatch;
		}

        void    setCenter(glm::vec3 center) {
            this->center = center;
        }

        void    setBoundingBox(BoundingBox boundingBox) {
            this->boundingBox = boundingBox;
            scale = glm::length(boundingBox.max - boundingBox.min);
        }

        void    setOBJ(std::string path) {
            this->obj = readOBJ(path.c_str(), 0.3f);
        }

        void    setScale(float scale) {
            this->scale = scale;
        }

        void    setTexture(const char *path) {
            this->texture = Texture(path);
        }

        void    setBuildAnimationVertexCount(int vertexCount) {
            this->buildAnimationVertexCount = vertexCount;
            this->renderBatch.setVertexCount(vertexCount);
        }

		void	changeTextureScale(float scale) {
			this->obj.changeTextureScale(scale);
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
			renderBatch.draw();
        }

        //check if ray intersects with model using Moeller-Trumbore algorithm
        bool intersectRay(glm::vec3 origin, glm::vec3 direction)
        {
            for (int i = 0; i < obj.getVerticesArraySize(); i += 27)
            {
                glm::vec3 v0 = glm::vec3(obj.getVerticesArray()[i], obj.getVerticesArray()[i + 1], obj.getVerticesArray()[i + 2]);
                glm::vec3 v1 = glm::vec3(obj.getVerticesArray()[i + 9], obj.getVerticesArray()[i + 10], obj.getVerticesArray()[i + 11]);
                glm::vec3 v2 = glm::vec3(obj.getVerticesArray()[i + 18], obj.getVerticesArray()[i + 19], obj.getVerticesArray()[i + 20]);
                glm::vec3 e1 = v1 - v0;
                glm::vec3 e2 = v2 - v0;
                glm::vec3 h = glm::cross(direction, e2);
                float a = glm::dot(e1, h);
                if (fabs(a) > -0.00001)
                {
                    glm::vec3 s = origin - v0;
                    glm::vec3 q = glm::cross(s, e1);
                    float u = glm::dot(h, s) / a;
                    float v = glm::dot(q, direction) / a;
                    float t = glm::dot(q, e2) / a;
                    if (t >= 0.0f && u >= 0.0f && v >= 0.0f && u + v <= 1.0f)
                        return true;
                }
            }
            return false;
        }
};

#endif //BIM_MODEL_HPP
