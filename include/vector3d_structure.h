#pragma once

#include <iostream>

struct Vector3D {
	 double x, y, z;

	 Vector3D(double _x = 0, double _y = 0, double _z = 0);

	 double dot(const Vector3D& other) const;
	 Vector3D cross(const Vector3D& other) const;
	 Vector3D operator*(double scalar) const;
	 Vector3D operator/(double scalar) const;
	 Vector3D operator+(const Vector3D& other) const;
	 Vector3D operator-(const Vector3D& other) const;
	 Vector3D operator-() const;
	 double lenght() const;
	 Vector3D normalize() const;
	 friend std::ostream& operator<<(std::ostream& os, const Vector3D& vec);
};
