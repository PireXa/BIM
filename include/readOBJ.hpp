//
// Created by kikop on 31/01/2024.
//

#ifndef BIM_READOBJ_HPP
#define BIM_READOBJ_HPP

#include <iostream>
#include <vector>
#include "glMath.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

struct  BoundingBox {
    glm::vec3 min;
    glm::vec3 max;
};

class readOBJ {
	private:
		const char *filename;
		std::vector<Vertex> vertices;
		std::vector<UV> uvs;
        std::vector<Normal> normals;
		float *verticesArray;
		int verticesArraySize = 0;
		int faceCount = 0;
		int vertexCount = 0;
        glm::vec3 center;
        BoundingBox boundingBox;
	public:
		readOBJ(const char *filename, float resolution) : filename(filename) {
			readVertices();
			readFaces();
            CalculateCenter();
            CalculateBoundingBox();
            if (uvs.size() == 0) {
                PlanarMapping(resolution);
            }
			std::cout << "Face count: " << faceCount << std::endl;
		}
		float *getVerticesArray();
		int getVerticesArraySize();
		int getVertexCount();
		int getFaceCount();
		int getuvCount();
        glm::vec3 getCenter();
        BoundingBox getBoundingBox();
		void    readVertices();
		void    readFaces();
        void    PlanarMapping(float resolution);
        void    CalculateCenter();
        void    CalculateBoundingBox();
};

#endif //BIM_READOBJ_HPP
