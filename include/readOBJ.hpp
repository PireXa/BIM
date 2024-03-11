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
		const char *filePath;
		std::vector<Vertex> vertices;
		std::vector<UV> uvs;
        std::vector<Normal> normals;
		float *verticesArray;
		int verticesArraySize = 0;
		int faceCount = 0;
		int vertexCount = 0;
        glm::vec3 center;
        BoundingBox boundingBox;
		std::string fileName;
		float textureScale;
	public:
		readOBJ(const char *filename, float resolution) : filePath(filename), textureScale(resolution) {
			printConstructorMessage();
			readVertices();
			readFaces();
            CalculateCenter();
            CalculateBoundingBox();
            if (uvs.size() == 0) {
				printColoredText(" -No UVs found, generating UVs\n", 255, 30, 155);
                PlanarMapping();
            }
            if (normals.size() == 0) {
                printColoredText(" -No normals found, generating normals\n", 255, 30, 155);
                CalculateNormals();
            }
			printColoredText("Loaded ", 255, 255, 0);
			printColoredText(this->fileName.c_str(), 255, 255, 0);
			printColoredText(":\n", 255, 255, 0);
			printColoredText(" -Vertices: ", 155, 255, 70);
			printColoredText(std::to_string(vertexCount).c_str(), 155, 255, 70);
			printColoredText("\n -Faces: ", 155, 255, 70);
			printColoredText(std::to_string(faceCount).c_str(), 155, 255, 70);
			std::cout << std::endl;
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
        void    PlanarMapping();
        void    CalculateCenter();
        void    CalculateBoundingBox();
        void    CalculateNormals();
		void	changeTextureScale(float scale);
		void	printConstructorMessage()
		{
			std::string str(filePath);
			size_t pos = 0;
			while ((pos = str.find("/")) != std::string::npos)
				str.erase(0, pos + 1);
			this->fileName = str;
			printColoredText("Reading OBJ file: ", 0, 140, 255);
			printColoredText(str.c_str(), 255, 255, 0);
			std::cout << std::endl;
		}
};

#endif //BIM_READOBJ_HPP
