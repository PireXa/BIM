//
// Created by kikop on 16/01/2024.
//

#ifndef BIM_CAMERA_HPP
#define BIM_CAMERA_HPP

#include <cmath>
#include "Quaternion.hpp"

class Camera {
public:
	Quaternion orientation;
	float position[3];

	Camera() : orientation(), position {0.0f, 0.0f, 0.0f} {}

	void move(float dx, float dy, float dz) {
		// Rotate the movement vector
//		rotateVector(dx, dy, dz);

		// Update camera position
		position[0] += dx;
		position[1] += dy;
		position[2] += dz;
	}

	// Rotate the camera around an arbitrary axis
	void rotate(float angle, float axisX, float axisY, float axisZ) {
		// Convert rotation to quaternion
		float halfAngleRad = angle * 0.5f;
		float sinHalfAngle = std::sin(halfAngleRad);
		Quaternion rotationQuat(
				std::cos(halfAngleRad),
				axisX * sinHalfAngle,
				axisY * sinHalfAngle,
				axisZ * sinHalfAngle
		);
		// Update camera orientation
//		orientation = rotationQuat * orientation * rotationQuat.inverse();
		orientation = rotationQuat * orientation;
		orientation.normalize();
	}

	// Rotate a vector using the camera's orientation (ex: rotate the camera's forward vector to get the direction the camera is facing)
	void rotateVector(float& vx, float& vy, float& vz) const {
		// Convert vector to quaternion
		Quaternion vectorQuat(0.0f, vx, vy, vz);

		// Rotate the vector using quaternion rotation formula
		Quaternion rotatedVectorQuat = orientation * vectorQuat * orientation.inverse();
		rotatedVectorQuat.normalize();

		// Extract the rotated vector
		vx = rotatedVectorQuat.x;
		vy = rotatedVectorQuat.y;
		vz = rotatedVectorQuat.z;
	}

	// Get the camera's rotation matrix
	void getRotationMatrix(float matrix[16]) const {
		matrix[0] = 1.0f - 2.0f * (orientation.y * orientation.y + orientation.z * orientation.z);
		matrix[1] = 2.0f * (orientation.x * orientation.y - orientation.w * orientation.z);
		matrix[2] = 2.0f * (orientation.x * orientation.z + orientation.w * orientation.y);
		matrix[3] = 0.0f;

		matrix[4] = 2.0f * (orientation.x * orientation.y + orientation.w * orientation.z);
		matrix[5] = 1.0f - 2.0f * (orientation.x * orientation.x + orientation.z * orientation.z);
		matrix[6] = 2.0f * (orientation.y * orientation.z - orientation.w * orientation.x);
		matrix[7] = 0.0f;

		matrix[8] = 2.0f * (orientation.x * orientation.z - orientation.w * orientation.y);
		matrix[9] = 2.0f * (orientation.y * orientation.z + orientation.w * orientation.x);
		matrix[10] = 1.0f - 2.0f * (orientation.x * orientation.x + orientation.y * orientation.y);
		matrix[11] = 0.0f;

		matrix[12] = 0.0f;
		matrix[13] = 0.0f;
		matrix[14] = 0.0f;
		matrix[15] = 1.0f;
	}

};

#endif //BIM_CAMERA_HPP
