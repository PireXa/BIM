//
// Created by kikop on 31/01/2024.
//

#include "readOBJ.hpp"

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

int readOBJ::getuvCount() {
	return uvs.size();
}

glm::vec3 readOBJ::getCenter() {
    return center;
}

BoundingBox readOBJ::getBoundingBox() {
    return boundingBox;
}

void    readOBJ::CalculateCenter()
{
    float x = 0;
    float y = 0;
    float z = 0;
    for (int i = 0; i < this->vertices.size(); i++)
    {
        x += this->vertices[i].x;
        y += this->vertices[i].y;
        z += this->vertices[i].z;
    }
    this->center = glm::vec3(x / this->vertices.size(), y / this->vertices.size(), z / this->vertices.size());
}

void    readOBJ::CalculateBoundingBox()
{
    float minX = this->vertices[0].x;
    float minY = this->vertices[0].y;
    float minZ = this->vertices[0].z;
    float maxX = this->vertices[0].x;
    float maxY = this->vertices[0].y;
    float maxZ = this->vertices[0].z;
    for (int i = 0; i < this->vertices.size(); i++)
    {
        if (this->vertices[i].x < minX)
            minX = this->vertices[i].x;
        if (this->vertices[i].y < minY)
            minY = this->vertices[i].y;
        if (this->vertices[i].z < minZ)
            minZ = this->vertices[i].z;
        if (this->vertices[i].x > maxX)
            maxX = this->vertices[i].x;
        if (this->vertices[i].y > maxY)
            maxY = this->vertices[i].y;
        if (this->vertices[i].z > maxZ)
            maxZ = this->vertices[i].z;
    }
    this->boundingBox.min = glm::vec3(minX, minY, minZ);
    this->boundingBox.max = glm::vec3(maxX, maxY, maxZ);

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
	std::ifstream file(this->filePath);
    if (!file.is_open())
    {
        std::cout << "Error: Could not open file: " << this->filePath << std::endl;
        return;
    }
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
		else if (!line.compare(0, 2, "v "))
		{
			Vertex v;
			iss >> trash >> v.x >> v.y >> v.z;
			this->vertices.push_back(v);
		}
		else if (!line.compare(0, 3, "vt "))
		{
			UV uv;
			iss >> trash >> trash >> uv.u >> uv.v;
			this->uvs.push_back(uv);
		}
        else if (!line.compare(0, 3, "vn "))
        {
            Normal n;
            iss >> trash >> trash >> n.x >> n.y >> n.z;
            this->normals.push_back(n);
        }
	}
}

void    readOBJ::readFaces()
{
	std::ifstream file(this->filePath);
	std::string line;
//	this->verticesArray = new float[this->faceCount * 18];
    this->verticesArray = new float[this->faceCount * 27];
	int i = 0;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		char type;
		if (!line.compare(0, 2, "f "))
		{
			long long int numberOfVertices= getNumberOfVertices(line);
			int f1, n1, f2, n2, f3, n3, f4, n4;
			float t1, t2, t3, t4;

			if (numberOfVertices == 3) {
				if (line.find('/') == std::string::npos) {
					// Format 1: f x x x
					iss >> type >> f1 >> f2 >> f3;
				} else {
					// Check the number of '/' occurrences to determine the format
					long long int slashCount = std::count(line.begin(), line.end(), '/');
					int noTexture = line.find("//");
					if (slashCount / 3 == 2 && noTexture == std::string::npos ) {
						// Format 3: f x/1/2 x/3/4 x/5/6
						iss >> type >> f1 >> type >> t1 >> type >> n1 >> f2 >> type >> t2 >> type >> n2 >> f3 >> type
						    >> t3 >> type >> n3;
					} else if (slashCount / 3 == 1) {
						// Format 2: f x/1 x/2 x/3
						iss >> type >> f1 >> type >> t1 >> f2 >> type >> t2 >> f3 >> type >> t3;
					} else {
						// Format 2: f x//1 x//2 x//3
						iss >> type >> f1 >> type >> type >> n1 >> f2 >> type >> type >> n2 >> f3 >> type >> type >> n3;
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
                //vertex 1 texture coordinates
				if (!uvs.empty())
				{
					verticesArray[i] = uvs[t1 - 1].u;
					verticesArray[i + 1] = uvs[t1 - 1].v;
					verticesArray[i + 2] = 0;
				}
				else
				{
					verticesArray[i] = 0.0f;
					verticesArray[i + 1] = 0.0f;
					verticesArray[i + 2] = 0.0f;
				}
				i += 3;
                //vertex 1 normals
                if (!normals.empty())
                {
                    verticesArray[i] = normals[n1 - 1].x;
                    verticesArray[i + 1] = normals[n1 - 1].y;
                    verticesArray[i + 2] = normals[n1 - 1].z;
                }
                else
                {
                    verticesArray[i] = 0.0f;
                    verticesArray[i + 1] = 0.0f;
                    verticesArray[i + 2] = 0.0f;
                }
                i += 3;
				//vertex 2 coordinates
				verticesArray[i] = vertices[f2 - 1].x;
				verticesArray[i + 1] = vertices[f2 - 1].y;
				verticesArray[i + 2] = vertices[f2 - 1].z;
				i += 3;
                //vertex 2 texture coordinates
				if (!uvs.empty())
				{
					verticesArray[i] = uvs[t2 - 1].u;
					verticesArray[i + 1] = uvs[t2 - 1].v;
					verticesArray[i + 2] = 0;
				}
				else
				{
					verticesArray[i] = 0.5f;
					verticesArray[i + 1] = 1.0f;
					verticesArray[i + 2] = 0.0f;
				}
				i += 3;
                //vertex 2 normals
                if (!normals.empty())
                {
                    verticesArray[i] = normals[n2 - 1].x;
                    verticesArray[i + 1] = normals[n2 - 1].y;
                    verticesArray[i + 2] = normals[n2 - 1].z;
                }
                else
                {
                    verticesArray[i] = 0.0f;
                    verticesArray[i + 1] = 0.0f;
                    verticesArray[i + 2] = 0.0f;
                }
                i += 3;
				//vertex 3 coordinates
				verticesArray[i] = vertices[f3 - 1].x;
				verticesArray[i + 1] = vertices[f3 - 1].y;
				verticesArray[i + 2] = vertices[f3 - 1].z;
				i += 3;
                //vertex 3 texture coordinates
				if (!uvs.empty())
				{
					verticesArray[i] = uvs[t3 - 1].u;
					verticesArray[i + 1] = uvs[t3 - 1].v;
					verticesArray[i + 2] = 0;
				}
				else
				{
					verticesArray[i] = 1.0f;
					verticesArray[i + 1] = 0.0f;
					verticesArray[i + 2] = 0.0f;
				}
				i += 3;
                //vertex 3 normals
                if (!normals.empty())
                {
                    verticesArray[i] = normals[n3 - 1].x;
                    verticesArray[i + 1] = normals[n3 - 1].y;
                    verticesArray[i + 2] = normals[n3 - 1].z;
                }
                else
                {
                    verticesArray[i] = 0.0f;
                    verticesArray[i + 1] = 0.0f;
                    verticesArray[i + 2] = 0.0f;
                }
                i += 3;
				this->vertexCount += 3;
//                this->verticesArraySize += 18;
                this->verticesArraySize += 27;
			}
			else
			{
				if (line.find('/') == std::string::npos) {
					// Format 1: f x x x
					iss >> type >> f1 >> f2 >> f3 >> f4;
				} else {
					// Check the number of '/' occurrences to determine the format
					long long int slashCount = std::count(line.begin(), line.end(), '/');
                    int noTexture = line.find("//");
					if (slashCount / 4 == 2 && noTexture == std::string::npos) {
						// Format 3: f x/1/2 x/3/4 x/5/6
						iss >> type >> f1 >> type >> t1 >> type >> n1 >> f2 >> type >> t2 >> type >> n2 >> f3 >> type
						    >> t3 >> type >> n3 >> f4 >> type >> t4 >> type >> n4;
					} else if (slashCount / 4 == 1) {
						// Format 2: f x/1 x/2 x/3
						iss >> type >> f1 >> type >> t1 >> f2 >> type >> t2 >> f3 >> type >> t3 >> f4 >> type >> t4;
					} else {
                        // Format 2: f x//1 x//2 x//3
                        iss >> type >> f1 >> type >> type >> n1 >> f2 >> type >> type >> n2 >> f3 >> type >> type >> n3 >> f4 >> type >> type >> n4;
                    }
				}
//				std::cout << f1 << " " << f2 << " " << f3 << " " << f4 << std::endl;
//                std::cout << t1 << " " << t2 << " " << t3 << " " << t4 << std::endl;
//                std::cout << n1 << " " << n2 << " " << n3 << " " << n4 << std::endl;
				//vertex 1 coordinates
				verticesArray[i] = vertices[f1 - 1].x;
				verticesArray[i + 1] = vertices[f1 - 1].y;
				verticesArray[i + 2] = vertices[f1 - 1].z;
				i += 3;
				//vertex 1 texture coordinates
				if (!uvs.empty())
				{
					verticesArray[i] = uvs[t1 - 1].u;
					verticesArray[i + 1] = uvs[t1 - 1].v;
					verticesArray[i + 2] = 0;
				}
				else
				{
					verticesArray[i] = 0.0f;
					verticesArray[i + 1] = 0.0f;
					verticesArray[i + 2] = 0.0f;
				}
				i += 3;
                //vertex 1 normals
                if (!normals.empty())
                {
                    verticesArray[i] = normals[n1 - 1].x;
                    verticesArray[i + 1] = normals[n1 - 1].y;
                    verticesArray[i + 2] = normals[n1 - 1].z;
                }
                else
                {
                    verticesArray[i] = 0.0f;
                    verticesArray[i + 1] = 0.0f;
                    verticesArray[i + 2] = 0.0f;
                }
                i += 3;
				//vertex 2 coordinates
				verticesArray[i] = vertices[f2 - 1].x;
				verticesArray[i + 1] = vertices[f2 - 1].y;
				verticesArray[i + 2] = vertices[f2 - 1].z;
				i += 3;
				//vertex 2 texture coordinates
				if (!uvs.empty())
				{
					verticesArray[i] = uvs[t2 - 1].u;
					verticesArray[i + 1] = uvs[t2 - 1].v;
					verticesArray[i + 2] = 0;
				}
				else
				{
					verticesArray[i] = 1.0f;
					verticesArray[i + 1] = 0.0f;
					verticesArray[i + 2] = 0.0f;
				}
				i += 3;
                //vertex 2 normals
                if (!normals.empty())
                {
                    verticesArray[i] = normals[n2 - 1].x;
                    verticesArray[i + 1] = normals[n2 - 1].y;
                    verticesArray[i + 2] = normals[n2 - 1].z;
                }
                else
                {
                    verticesArray[i] = 0.0f;
                    verticesArray[i + 1] = 0.0f;
                    verticesArray[i + 2] = 0.0f;
                }
                i += 3;
				//vertex 3 coordinates
				verticesArray[i] = vertices[f3 - 1].x;
				verticesArray[i + 1] = vertices[f3 - 1].y;
				verticesArray[i + 2] = vertices[f3 - 1].z;
				i += 3;
				//vertex 3 texture coordinates
				if (!uvs.empty())
				{
					verticesArray[i] = uvs[t3 - 1].u;
					verticesArray[i + 1] = uvs[t3 - 1].v;
					verticesArray[i + 2] = 0;
				}
				else
				{
					verticesArray[i] = 1.0f;
					verticesArray[i + 1] = 1.0f;
					verticesArray[i + 2] = 0.0f;
				}
				i += 3;
                //vertex 3 normals
                if (!normals.empty())
                {
                    verticesArray[i] = normals[n3 - 1].x;
                    verticesArray[i + 1] = normals[n3 - 1].y;
                    verticesArray[i + 2] = normals[n3 - 1].z;
                }
                else
                {
                    verticesArray[i] = 0.0f;
                    verticesArray[i + 1] = 0.0f;
                    verticesArray[i + 2] = 0.0f;
                }
                i += 3;
				//vertex 4 coordinates
				verticesArray[i] = vertices[f4 - 1].x;
				verticesArray[i + 1] = vertices[f4 - 1].y;
				verticesArray[i + 2] = vertices[f4 - 1].z;
				i += 3;
				//vertex 4 texture coordinates
				if (!uvs.empty())
				{
					verticesArray[i] = uvs[t4 - 1].u;
					verticesArray[i + 1] = uvs[t4 - 1].v;
					verticesArray[i + 2] = 0;
				}
				else
				{
					verticesArray[i] = 0.5f;
					verticesArray[i + 1] = 1.0f;
					verticesArray[i + 2] = 0.5f;
				}
				i += 3;
                //vertex 4 normals
                if (!normals.empty())
                {
                    verticesArray[i] = normals[n4 - 1].x;
                    verticesArray[i + 1] = normals[n4 - 1].y;
                    verticesArray[i + 2] = normals[n4 - 1].z;
                }
                else
                {
                    verticesArray[i] = 0.0f;
                    verticesArray[i + 1] = 0.0f;
                    verticesArray[i + 2] = 0.0f;
                }
                i += 3;
				//vertex 5 coordinates
				verticesArray[i] = vertices[f1 - 1].x;
				verticesArray[i + 1] = vertices[f1 - 1].y;
				verticesArray[i + 2] = vertices[f1 - 1].z;
				i += 3;
				//vertex 5 texture coordinates
				if (!uvs.empty())
				{
					verticesArray[i] = uvs[t1 - 1].u;
					verticesArray[i + 1] = uvs[t1 - 1].v;
					verticesArray[i + 2] = 0;
				}
				else
				{
					verticesArray[i] = 1.0f;
					verticesArray[i + 1] = 0.5f;
					verticesArray[i + 2] = 0.5f;
				}
				i += 3;
                //vertex 5 normals
                if (!normals.empty())
                {
                    verticesArray[i] = normals[n1 - 1].x;
                    verticesArray[i + 1] = normals[n1 - 1].y;
                    verticesArray[i + 2] = normals[n1 - 1].z;
                }
                else
                {
                    verticesArray[i] = 0.0f;
                    verticesArray[i + 1] = 0.0f;
                    verticesArray[i + 2] = 0.0f;
                }
                i += 3;
				//vertex 6 coordinates
				verticesArray[i] = vertices[f3 - 1].x;
				verticesArray[i + 1] = vertices[f3 - 1].y;
				verticesArray[i + 2] = vertices[f3 - 1].z;
				i += 3;
				//vertex 6 texture coordinates
				if (!uvs.empty())
				{
					verticesArray[i] = uvs[t3 - 1].u;
					verticesArray[i + 1] = uvs[t3 - 1].v;
					verticesArray[i + 2] = 0;
				}
				else
				{
					verticesArray[i] = 0.5f;
					verticesArray[i + 1] = 0.5f;
					verticesArray[i + 2] = 1.0f;
				}
				i += 3;
                //vertex 6 normals
                if (!normals.empty())
                {
                    verticesArray[i] = normals[n3 - 1].x;
                    verticesArray[i + 1] = normals[n3 - 1].y;
                    verticesArray[i + 2] = normals[n3 - 1].z;
                }
                else
                {
                    verticesArray[i] = 0.0f;
                    verticesArray[i + 1] = 0.0f;
                    verticesArray[i + 2] = 0.0f;
                }
                i += 3;
				this->vertexCount += 6;
//                this->verticesArraySize += 36;
                this->verticesArraySize += 54;
			}
		}
	}
}

void    readOBJ::PlanarMapping(float resolution)
{
//    float resolution = 0.3;
    for (int i = 0; i < this->verticesArraySize; i += 27)
    {
        Vertex v1 = {verticesArray[i], verticesArray[i + 1], verticesArray[i + 2]};
        Vertex v2 = {verticesArray[i + 9], verticesArray[i + 10], verticesArray[i + 11]};
        Vertex v3 = {verticesArray[i + 18], verticesArray[i + 19], verticesArray[i + 20]};

        glm::vec3 edge1 = glm::vec3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
        glm::vec3 edge2 = glm::vec3(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);
        glm::vec3 normal = glm::cross(edge1, edge2);
        normal = glm::normalize(normal);
        normal.x = fabs(normal.x);
        normal.y = fabs(normal.y);
        normal.z = fabs(normal.z);
        if (normal.x > normal.y && normal.x > normal.z)
        {
            verticesArray[i + 3] = v1.y * resolution;
            verticesArray[i + 4] = v1.z * resolution;
            verticesArray[i + 5] = 0;
            verticesArray[i + 12] = v2.y * resolution;
            verticesArray[i + 13] = v2.z * resolution;
            verticesArray[i + 14] = 0;
            verticesArray[i + 21] = v3.y * resolution;
            verticesArray[i + 22] = v3.z * resolution;
            verticesArray[i + 23] = 0;
        }
        else if (normal.y > normal.x && normal.y > normal.z)
        {
            verticesArray[i + 3] = v1.x * resolution;
            verticesArray[i + 4] = v1.z * resolution;
            verticesArray[i + 5] = 0;
            verticesArray[i + 12] = v2.x * resolution;
            verticesArray[i + 13] = v2.z * resolution;
            verticesArray[i + 14] = 0;
            verticesArray[i + 21] = v3.x * resolution;
            verticesArray[i + 22] = v3.z * resolution;
            verticesArray[i + 23] = 0;
        }
        else {
            verticesArray[i + 3] = v1.x * resolution;
            verticesArray[i + 4] = v1.y * resolution;
            verticesArray[i + 5] = 0;
            verticesArray[i + 12] = v2.x * resolution;
            verticesArray[i + 13] = v2.y * resolution;
            verticesArray[i + 14] = 0;
            verticesArray[i + 21] = v3.x * resolution;
            verticesArray[i + 22] = v3.y * resolution;
            verticesArray[i + 23] = 0;
        }
    }
}

void    readOBJ::CalculateNormals()
{
    for (int i = 0; i < this->verticesArraySize; i += 27)
    {
        Vertex v1 = {verticesArray[i], verticesArray[i + 1], verticesArray[i + 2]};
        Vertex v2 = {verticesArray[i + 9], verticesArray[i + 10], verticesArray[i + 11]};
        Vertex v3 = {verticesArray[i + 18], verticesArray[i + 19], verticesArray[i + 20]};

        glm::vec3 edge1 = glm::vec3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
        glm::vec3 edge2 = glm::vec3(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);
        glm::vec3 normal = glm::cross(edge1, edge2);
        normal = glm::normalize(normal);
        normal.x = (normal.x + 1) / 2;
        normal.y = (normal.y + 1) / 2;
        normal.z = (normal.z + 1) / 2;
        verticesArray[i + 6] = normal.x;
        verticesArray[i + 7] = normal.y;
        verticesArray[i + 8] = normal.z;
        verticesArray[i + 15] = normal.x;
        verticesArray[i + 16] = normal.y;
        verticesArray[i + 17] = normal.z;
        verticesArray[i + 24] = normal.x;
        verticesArray[i + 25] = normal.y;
        verticesArray[i + 26] = normal.z;
    }
}
