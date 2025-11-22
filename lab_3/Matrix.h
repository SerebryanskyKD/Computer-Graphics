#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <vector>
#include <iostream>
#include "geometry.h"

class Matrix {
private:
    std::vector<std::vector<float>> m;
    int rows, cols;

public:
    Matrix(int r = 4, int c = 4);
    Matrix(const Matrix& other);

    Matrix operator*(const Matrix& other) const;
    Vec3f operator*(const Vec3f& v) const;
    std::vector<float>& operator[](int i);
    const std::vector<float>& operator[](int i) const;

    static Matrix identity();
    static Matrix viewport(int x, int y, int w, int h, int depth = 255);
    static Matrix projection(float coeff = 0.0f);
    static Matrix lookat(Vec3f eye, Vec3f center, Vec3f up);

    Matrix transpose() const;
};

#endif