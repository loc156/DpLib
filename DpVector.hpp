#pragma once

#include "Library.h"


namespace DpLib {
	class Vector3D {
	public:
		double x, y, z;
		Vector3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
		Vector3D operator+(const Vector3D& other) const {
			return Vector3D(x + other.x, y + other.y, z + other.z);
		}
		Vector3D operator-(const Vector3D& other) const {
			return Vector3D(x - other.x, y - other.y, z - other.z);
		}
		Vector3D operator*(double scalar) const {
			return Vector3D(x * scalar, y * scalar, z * scalar);
		}
		Vector3D operator/(double scalar) const {
			return Vector3D(x / scalar, y / scalar, z / scalar);
		}
		double operator*(const Vector3D& other) const {
			return x * other.x + y * other.y + z * other.z;
		}
		Vector3D& operator+=(const Vector3D& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
		Vector3D& operator-=(const Vector3D& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}
		Vector3D& operator*=(double scalar) {
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}
		Vector3D& operator/=(double scalar) {
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}
		double magnitude() const {
			return sqrt(x * x + y * y + z * z);
		}
		Vector3D normalize() const {
			double mag = magnitude();
			if (mag == 0) {
				return Vector3D(0, 0, 0);
			}
			return Vector3D(x / mag, y / mag, z / mag);
		}
	};
	class Vector2D {
	public:
		double x, y;
		Vector2D(double x = 0, double y = 0) : x(x), y(y) {}
		Vector2D operator+(const Vector2D& other) const {
			return Vector2D(x + other.x, y + other.y);
		}
		Vector2D operator-(const Vector2D& other) const {
			return Vector2D(x - other.x, y - other.y);
		}
		Vector2D operator*(double scalar) const {
			return Vector2D(x * scalar, y * scalar);
		}
		Vector2D operator/(double scalar) const {
			return Vector2D(x / scalar, y / scalar);
		}
		double operator*(const Vector2D& other) const {
			return x * other.x + y * other.y;
		}
		Vector2D& operator+=(const Vector2D& other) {
			x += other.x;
			y += other.y;
			return *this;
		}
		Vector2D& operator-=(const Vector2D& other) {
			x -= other.x;
			y -= other.y;
			return *this;
		}
		Vector2D& operator*=(double scalar) {
			x *= scalar;
			y *= scalar;
			return *this;
		}
		Vector2D& operator/=(double scalar) {
			x /= scalar;
			y /= scalar;
			return *this;
		}
		double magnitude() const {
			return sqrt(x * x + y * y);
		}
		Vector2D normalize() const {
			double mag = magnitude();
			if (mag == 0) {
				return Vector2D(0, 0);
			}
			return Vector2D(x / mag, y / mag);
		}
	};

}