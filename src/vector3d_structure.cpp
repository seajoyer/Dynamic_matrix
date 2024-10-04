#include "vector3d_structure.h"

#include <iostream>
#include <cmath>

using namespace std;

double x, y, z;

Vector3D::Vector3D(double _x, double _y, double _z): x(_x), y(_y), z(_z) {}

double Vector3D::dot(const Vector3D& other) const {
	return x*other.x+y*other.y+z*other.z;
}

Vector3D Vector3D::cross(const Vector3D& other) const {
	return Vector3D(y*other.z-z*other.y,
			z*other.x-x*other.z,
			x*other.y-y*other.x);
}

Vector3D Vector3D::operator*(double scalar) const {
	return Vector3D(x*scalar, y*scalar, z*scalar);
}

Vector3D Vector3D::operator/(double scalar) const {
	if (scalar == 0.0) {
		cerr << "Деление на ноль" << endl;
		return *this;
	}
	return Vector3D(x/scalar, y/scalar, z/scalar);
}

Vector3D Vector3D::operator+(const Vector3D& other) const {
	return Vector3D(x+other.x, y+other.y, z+other.z);
}

Vector3D Vector3D::operator-(const Vector3D& other) const {
	return Vector3D(x-other.x, y-other.y, z-other.z);
}

Vector3D Vector3D::operator-() const {
	return Vector3D(-x, -y, -z);
}

double Vector3D::lenght() const {
	return sqrt(x*x+y*y+z*z);
}

Vector3D Vector3D::normalize() const {
	double len = lenght();
	if (len == 0.0) {
		return *this;
		}
	return Vector3D(x/len, y/len, z/len);
}

ostream& operator<<(ostream& os, const Vector3D& vec) {
	os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}
