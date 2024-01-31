//
// Created by kikop on 31/01/2024.
//

#include "readOBJ.hpp"

readOBJ::~readOBJ() {
	delete[] verticesArray;
}

float *readOBJ::getVerticesArray() {
	return verticesArray;
}

int readOBJ::getVerticesArraySize() {
	return verticesArraySize;
}

int readOBJ::getFaceCount() {
	return faceCount;
}

int readOBJ::getVertexCount() {
	return vertexCount;
}

long long int getNumberOfVertices(const std::string &line)
{
	long long int numberOfVertices = 0;
	for (int i = 0; i < line.length(); i++)
	{
		while (line[i] == ' ')
			i++;
		while (line[i] != ' ' && line[i] != '\0')
			i++;
		numberOfVertices++;
	}
	return numberOfVertices - 1;
}

void    readOBJ::readVertices()
{
	std::ifstream file(this->filename);
	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		char trash;
		if (!line.compare(0, 2, "f "))
		{
			long long int numberOfVertices= getNumberOfVertices(line);
			if (numberOfVertices == 3)
				this->faceCount++;
			else
				this->faceCount += 2;
		}
		if (!line.compare(0, 2, "v "))
		{
			Vertex v;
			iss >> trash >> v.x >> v.y >> v.z;
			this->vertices.push_back(v);
		}
	}
}

void    readOBJ::readFaces()
{
	std::ifstream file(this->filename);
	std::string line;
	this->verticesArray = new float[this->faceCount * 18];
	int i = 0;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		char type;
		if (!line.compare(0, 2, "f "))
		{
			long long int numberOfVertices= getNumberOfVertices(line);
//			std::cout << "numberOfVertices " << numberOfVertices << std::endl;
			int f1, t1, n1, f2, t2, n2, f3, t3, n3, f4, t4, n4;

			if (numberOfVertices == 3) {
				if (line.find('/') == std::string::npos) {
					// Format 1: f x x x
					iss >> type >> f1 >> f2 >> f3;
				} else {
					// Check the number of '/' occurrences to determine the format
					long long int slashCount = std::count(line.begin(), line.end(), '/');
					if (slashCount / 3 == 2) {
						// Format 3: f x/1/2 x/3/4 x/5/6
						iss >> type >> f1 >> type >> t1 >> type >> n1 >> f2 >> type >> t2 >> type >> n2 >> f3 >> type
						    >> t3 >> type >> n3;
					} else {
						// Format 2: f x/1 x/2 x/3
						iss >> type >> f1 >> type >> t1 >> f2 >> type >> t2 >> f3 >> type >> t3;
					}
				}
//				std::cout << f1 << " " << f2 << " " << f3 << std::endl;
//				std::cout << t1 << " " << t2 << " " << t3 << std::endl;
//				std::cout << n1 << " " << n2 << " " << n3 << std::endl;
				//vertex 1 coordinates
				verticesArray[i] = vertices[f1 - 1].x;
				verticesArray[i + 1] = vertices[f1 - 1].y;
				verticesArray[i + 2] = vertices[f1 - 1].z;
				i += 3;
				//vertex 1 color
				verticesArray[i] = 1.0f;
				verticesArray[i + 1] = 0.5f;
				verticesArray[i + 2] = 0.5f;
				i += 3;
				//vertex 2 coordinates
				verticesArray[i] = vertices[f2 - 1].x;
				verticesArray[i + 1] = vertices[f2 - 1].y;
				verticesArray[i + 2] = vertices[f2 - 1].z;
				i += 3;
				//vertex 2 color
				verticesArray[i] = 0.5f;
				verticesArray[i + 1] = 1.0f;
				verticesArray[i + 2] = 0.5f;
				i += 3;
				//vertex 3 coordinates
				verticesArray[i] = vertices[f3 - 1].x;
				verticesArray[i + 1] = vertices[f3 - 1].y;
				verticesArray[i + 2] = vertices[f3 - 1].z;
				i += 3;
				//vertex 3 color
				verticesArray[i] = 0.5f;
				verticesArray[i + 1] = 0.5f;
				verticesArray[i + 2] = 1.0f;
				i += 3;
				this->vertexCount += 3;
			}
			else
			{
				if (line.find('/') == std::string::npos) {
					// Format 1: f x x x
					iss >> type >> f1 >> f2 >> f3 >> f4;
				} else {
					// Check the number of '/' occurrences to determine the format
					long long int slashCount = std::count(line.begin(), line.end(), '/');
					if (slashCount / 3 == 2) {
						// Format 3: f x/1/2 x/3/4 x/5/6
						iss >> type >> f1 >> type >> t1 >> type >> n1 >> f2 >> type >> t2 >> type >> n2 >> f3 >> type
						    >> t3 >> type >> n3 >> f4 >> type >> t4 >> type >> n4;
					} else {
						// Format 2: f x/1 x/2 x/3
						iss >> type >> f1 >> type >> t1 >> f2 >> type >> t2 >> f3 >> type >> t3 >> f4 >> type >> t4;
					}
				}
//				std::cout << f1 << " " << f2 << " " << f3 << " " << f4 << std::endl;
				//vertex 1 coordinates
				verticesArray[i] = vertices[f1 - 1].x;
				verticesArray[i + 1] = vertices[f1 - 1].y;
				verticesArray[i + 2] = vertices[f1 - 1].z;
				i += 3;
				//vertex 1 color
				verticesArray[i] = 1.0f;
				verticesArray[i + 1] = 0.5f;
				verticesArray[i + 2] = 0.5f;
				i += 3;
				//vertex 2 coordinates
				verticesArray[i] = vertices[f2 - 1].x;
				verticesArray[i + 1] = vertices[f2 - 1].y;
				verticesArray[i + 2] = vertices[f2 - 1].z;
				i += 3;
				//vertex 2 color
				verticesArray[i] = 0.5f;
				verticesArray[i + 1] = 1.0f;
				verticesArray[i + 2] = 0.5f;
				i += 3;
				//vertex 3 coordinates
				verticesArray[i] = vertices[f3 - 1].x;
				verticesArray[i + 1] = vertices[f3 - 1].y;
				verticesArray[i + 2] = vertices[f3 - 1].z;
				i += 3;
				//vertex 3 color
				verticesArray[i] = 0.5f;
				verticesArray[i + 1] = 0.5f;
				verticesArray[i + 2] = 1.0f;
				i += 3;
				//vertex 4 coordinates
				verticesArray[i] = vertices[f4 - 1].x;
				verticesArray[i + 1] = vertices[f4 - 1].y;
				verticesArray[i + 2] = vertices[f4 - 1].z;
				i += 3;
				//vertex 4 color
				verticesArray[i] = 1.0f;
				verticesArray[i + 1] = 0.5f;
				verticesArray[i + 2] = 0.5f;
				i += 3;
				//vertex 5 coordinates
				verticesArray[i] = vertices[f1 - 1].x;
				verticesArray[i + 1] = vertices[f1 - 1].y;
				verticesArray[i + 2] = vertices[f1 - 1].z;
				i += 3;
				//vertex 5 color
				verticesArray[i] = 0.5f;
				verticesArray[i + 1] = 1.0f;
				verticesArray[i + 2] = 0.5f;
				i += 3;
				//vertex 6 coordinates
				verticesArray[i] = vertices[f3 - 1].x;
				verticesArray[i + 1] = vertices[f3 - 1].y;
				verticesArray[i + 2] = vertices[f3 - 1].z;
				i += 3;
				//vertex 6 color
				verticesArray[i] = 0.5f;
				verticesArray[i + 1] = 0.5f;
				verticesArray[i + 2] = 1.0f;
				i += 3;
				this->vertexCount += 6;
			}
		}
	}
}
