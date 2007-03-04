#ifndef __MATRIX4X4_H
#define __MATRIX4X4_H

#include <iostream>

class Vector3;
class Vector4;

namespace Math {

class Matrix4x4
{
public:
	inline Matrix4x4() {}
	inline ~Matrix4x4() {}
	Matrix4x4(const float[16]);

	/* constructors */
	Matrix4x4(const Matrix4x4 &v);
	Matrix4x4 &operator=(const Matrix4x4 &v);

	static const Matrix4x4 Identity;

	/* set the matrix to various ops */
	Matrix4x4 &SetIdentity();
	Matrix4x4 &SetScaling(const Vector3 &scale);
	Matrix4x4 &SetTranslation(const Vector3 &xlate);
	Matrix4x4 &SetRotationX(float angle);
	Matrix4x4 &SetRotationY(float angle);
	Matrix4x4 &SetRotationZ(float angle);

	/* operators */
	float *operator*();
	Matrix4x4 operator+(const Matrix4x4 &v) const;
	Matrix4x4 &operator+=(const Matrix4x4 &v);
	Matrix4x4 operator-(const Matrix4x4 &v) const;
	Matrix4x4 &operator-=(const Matrix4x4 &v);
	Matrix4x4 operator*(const Matrix4x4 &v) const;
	Matrix4x4 &operator*=(const Matrix4x4 &v);
	Vector4 operator*(const Vector4 &v) const;

	Vector3 Transform(const Vector3 &point) const;

	/* data */
	union {
		float val[16];
		float vald[4][4];
	};
};

/* debugging */
std::ostream &operator<<(std::ostream &os, Matrix4x4 &v);

}

#endif
