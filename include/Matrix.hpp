//
// Created by kikop on 16/01/2024.
//

#ifndef BIM_MATRIX_HPP
#define BIM_MATRIX_HPP

#include <cmath>
#include <iostream>

class MatrixUtils {
public:
	// Initialize a 4x4 matrix with the identity matrix
	static void identityMatrix(float* matrix) {
		for (int i = 0; i < 16; ++i) {
			matrix[i] = (i % 5 == 0) ? 1.0f : 0.0f;
		}
	}

	// Multiply the given matrix by another 4x4 matrix
	static void	multiplyMatrix(const float* a, const float* b, float* result)
	{
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result[i * 4 + j] =
						a[i * 4] * b[j] +
						a[i * 4 + 1] * b[j + 4] +
						a[i * 4 + 2] * b[j + 8] +
						a[i * 4 + 3] * b[j + 12];
			}
		}
	}
};

//class Model {
//public:
//	float translateX, translateY, translateZ;
//	float rotateAngle, rotateAxisX, rotateAxisY, rotateAxisZ;
//	float scaleX, scaleY, scaleZ;
//
//	Model()
//			: translateX(0.0f), translateY(0.0f), translateZ(0.0f),
//			  rotateAngle(0.0f), rotateAxisX(0.0f), rotateAxisY(0.0f), rotateAxisZ(0.0f),
//			  scaleX(1.0f), scaleY(1.0f), scaleZ(1.0f) {}
//
//	// Calculate model matrix
//	void getModelMatrix(float* modelMatrix) const {
//		MatrixUtils::identityMatrix(modelMatrix);
//
//		// Translate
//		translateMatrix(modelMatrix, translateX, translateY, translateZ);
//
//		// Rotate
//		rotateMatrix(modelMatrix, rotateAngle, rotateAxisX, rotateAxisY, rotateAxisZ);
//
//		// Scale
//		scaleMatrix(modelMatrix, scaleX, scaleY, scaleZ);
//	}
//
//private:
//	// Multiply the given matrix by a translation matrix
//	void translateMatrix(float* matrix, float x, float y, float z) const {
//		float translationMatrix[16] = {
//				1.0f, 0.0f, 0.0f, x,
//				0.0f, 1.0f, 0.0f, y,
//				0.0f, 0.0f, 1.0f, z,
//				0.0f, 0.0f, 0.0f, 1.0f
//		};
//		float	result[16];
//		MatrixUtils::multiplyMatrix(matrix, translationMatrix, result);
//		matrix = result;
//	}
//
//	// Multiply the given matrix by a rotation matrix
//	void rotateMatrix(float* matrix, float angle, float x, float y, float z) const {
//		float length = std::sqrt(x * x + y * y + z * z);
//		if (length > 0.0f) {
//			x /= length;
//			y /= length;
//			z /= length;
//		}
//
//		float c = std::cos(angle);
//		float s = std::sin(angle);
//		float rotationMatrix[16] = {
//				x * x * (1 - c) + c, x * y * (1 - c) - z * s, x * z * (1 - c) + y * s, 0.0f,
//				x * y * (1 - c) + z * s, y * y * (1 - c) + c, y * z * (1 - c) - x * s, 0.0f,
//				x * z * (1 - c) - y * s, y * z * (1 - c) + x * s, z * z * (1 - c) + c, 0.0f,
//				0.0f, 0.0f, 0.0f, 1.0f
//		};
//		float	result[16];
//		MatrixUtils::multiplyMatrix(matrix, rotationMatrix, result);
//		matrix = result;
//	}
//
//	// Multiply the given matrix by a scaling matrix
//	void scaleMatrix(float* matrix, float scaleX, float scaleY, float scaleZ) const {
//		float scaleMatrix[16] = {
//				scaleX, 0.0f, 0.0f, 0.0f,
//				0.0f, scaleY, 0.0f, 0.0f,
//				0.0f, 0.0f, scaleZ, 0.0f,
//				0.0f, 0.0f, 0.0f, 1.0f
//		};
//		float result[16];
//		MatrixUtils::multiplyMatrix(matrix, scaleMatrix, result);
//		matrix = result;
//	}
//};
//
//class Projection {
//public:
//	float left, right, bottom, top, nearClip, farClip;
//
//	Projection() : left(-1.0f), right(1.0f), bottom(-1.0f), top(1.0f), nearClip(0.1f), farClip(100.0f) {}
//
//	// Calculate orthographic projection matrix
//	void getOrthoProjectionMatrix(float screenWidth, float screenHeight, float* projectionMatrix) const {
//		float aspectRatio = screenWidth / screenHeight;
//
//		float orthoProjectionMatrix[16] = {
//				2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left),
//				0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom),
//				0.0f, 0.0f, -2.0f / (farClip - nearClip), -(farClip + nearClip) / (farClip - nearClip),
//				0.0f, 0.0f, 0.0f, 1.0f
//		};
//		MatrixUtils::multiplyMatrix(projectionMatrix, orthoProjectionMatrix, projectionMatrix);
//	}
//	void getPerspectiveProjectionMatrix(float fov, float aspectRatio, float nearClip, float farClip, float* projectionMatrix) {
//        	// Convert the field of view from degrees to radians
//        	float fovRad = (fov / 2.0f) * (M_PI / 180.0f);
//
//        	// Calculate the parameters for the perspective matrix
//        	float tanHalfFov = std::tan(fovRad);
//        	float range = nearClip - farClip;
//
//        	// Set up the perspective matrix
//        	projectionMatrix[0] = 1.0f / ( tanHalfFov);
//        	projectionMatrix[1] = 0.0f;
//        	projectionMatrix[2] = 0.0f;
//        	projectionMatrix[3] = 0.0f;
//
//        	projectionMatrix[4] = 0.0f;
//        	projectionMatrix[5] = 1.0f / tanHalfFov;
//        	projectionMatrix[6] = 0.0f;
//        	projectionMatrix[7] = 0.0f;
//
//        	projectionMatrix[8] = 0.0f;
//        	projectionMatrix[9] = 0.0f;
//       		projectionMatrix[10] = -(farClip) / (farClip - nearClip);
//        	projectionMatrix[11] = -1.0f;
//
//        	projectionMatrix[12] = 0.0f;
//        	projectionMatrix[13] = 0.0f;
//        	projectionMatrix[14] = -(farClip *  nearClip) / (farClip - nearClip);
//     		projectionMatrix[15] = 0.0f;
//
//		for (int i = 0; i < 4; ++i) {
//            		for (int j = 0; j < 4; ++j) {
//               			std::cout << projectionMatrix[i * 4 + j] << "\t";
//            		}
//            		std::cout << std::endl;
//        	}
//	}
//};


#endif //BIM_MATRIX_HPP
