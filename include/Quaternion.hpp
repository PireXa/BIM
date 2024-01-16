//
// Created by kikop on 16/01/2024.
//

#ifndef BIM_QUATERNION_HPP
#define BIM_QUATERNION_HPP

#include <cmath>

struct Vector3
{
	Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	Vector3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	void set(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	void Normalize()
	{
		float length = sqrt(x*x + y*y + z*z);
		if (length > 0.0f)
		{
			float invLength = 1.0f / length;
			x *= invLength;
			y *= invLength;
			z *= invLength;
		}
	}

	float Length()
	{
		return sqrt(x*x + y*y + z*z);
	}


	Vector3 Cross(const Vector3 &vec)
	{
		return Vector3(y*vec.z - z*vec.y,
		               z*vec.x - x*vec.z,
		               x*vec.y - y*vec.x);
	}

	static Vector3 Normalize(const Vector3 &vec)
	{
		float length = sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
		if (length > 0.0f)
		{
			float invLength = 1.0f / length;
			return Vector3(vec.x * invLength, vec.y * invLength, vec.z * invLength);
		}
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	static float Dot(const Vector3 &vec1, const Vector3 &vec2)
	{
		return (vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z);
	}

	static Vector3 Cross(const Vector3 &vec1, const Vector3 &vec2)
	{
		return Vector3(vec1.y*vec2.z - vec1.z*vec2.y,
		               vec1.z*vec2.x - vec1.x*vec2.z,
		               vec1.x*vec2.y - vec1.y*vec2.x);
	}

	static Vector3 Min(const Vector3 &a, const Vector3 &b)
	{
		return Vector3(a.x < b.x ? a.x : b.x,
		               a.y < b.y ? a.y : b.y,
		               a.z < b.z ? a.z : b.z);
	}

	static Vector3 Max(const Vector3 &a, const Vector3 &b)
	{
		return Vector3(a.x > b.x ? a.x : b.x,
		               a.y > b.y ? a.y : b.y,
		               a.z > b.z ? a.z : b.z);
	}

	Vector3 operator+(const Vector3 &vec) const
	{
		return Vector3(x + vec.x, y + vec.y, z + vec.z);
	}
	Vector3 operator-(const Vector3 &vec) const
	{
		return Vector3(x - vec.x, y - vec.y, z - vec.z);
	}
	Vector3 operator*(float scalar) const
	{
		return Vector3(x * scalar, y * scalar, z * scalar);
	}
	Vector3 operator/(float scalar) const
	{
		return Vector3(x / scalar, y / scalar, z / scalar);
	}
	Vector3 operator-() const
	{
		return Vector3(-x, -y, -z);
	}
	Vector3& operator+=(const Vector3 &vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}
	Vector3& operator-=(const Vector3 &vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}
	Vector3& operator*=(const Vector3 &vec)
	{
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
		return *this;
	}

	Vector3& operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
	Vector3& operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}
	bool operator==(const Vector3 &vec) const
	{
		return (x == vec.x && y == vec.y && z == vec.z);
	}
	bool operator!=(const Vector3 &vec) const
	{
		return (x != vec.x || y != vec.y || z != vec.z);
	}


	float x, y, z;
};

class Quaternion {
public:
	float w, x, y, z;

	Quaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}
	Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

	Quaternion operator*(const Quaternion& other) const {
		return {
				w * other.w - x * other.x - y * other.y - z * other.z,
				w * other.x + x * other.w + y * other.z - z * other.y,
				w * other.y - x * other.z + y * other.w + z * other.x,
				w * other.z + x * other.y - y * other.x + z * other.w
		};
	}

	void normalize() {
		float magnitude = std::sqrt(w * w + x * x + y * y + z * z);
		if (magnitude > 0.0f) {
			w /= magnitude;
			x /= magnitude;
			y /= magnitude;
			z /= magnitude;
		}
	}

	Quaternion inverse() const {
		return {w, -x, -y, -z};
	}
};

#endif //BIM_QUATERNION_HPP
