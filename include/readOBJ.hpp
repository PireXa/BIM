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

class readOBJ {
	private:
		const char *filename;
		std::vector<Vertex> vertices;
		std::vector<UV> uvs;
		float *verticesArray;
		int verticesArraySize;
		int faceCount;
		int vertexCount;
        glm::vec3 center;
	public:
		readOBJ(const char *filename) : filename(filename) {
			readVertices();
			readFaces();
            CalculateCenter();
		}
		~readOBJ();
		float *getVerticesArray();
		int getVerticesArraySize();
		int getVertexCount();
		int getFaceCount();
		int getuvCount();
        glm::vec3 getCenter();
		void    readVertices();
		void    readFaces();
        void    PlanarMapping();
        void    CalculateCenter();
};

#endif //BIM_READOBJ_HPP
