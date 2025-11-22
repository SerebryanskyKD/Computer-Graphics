#include "matrix.h"
#include <cmath>

Matrix::Matrix(int r, int c) : rows(r), cols(c) {
    m.resize(rows, std::vector<float>(cols, 0.0f));
}

Matrix::Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
    m = other.m;
}

std::vector<float>& Matrix::operator[](int i) {
    return m[i];
}

const std::vector<float>& Matrix::operator[](int i) const {
    return m[i];
}

Matrix Matrix::operator*(const Matrix& other) const {
    Matrix result(rows, other.cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < other.cols; j++) {
            for (int k = 0; k < cols; k++) {
                result[i][j] += m[i][k] * other[k][j];
            }
        }
    }
    return result;
}

Vec3f Matrix::operator*(const Vec3f& v) const {
    float x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
    float y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
    float z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
    float w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3];

    if (std::abs(w) > 1e-5f) {
        return Vec3f(x / w, y / w, z / w);
    }
    return Vec3f(x, y, z);
}

Matrix Matrix::identity() {
    Matrix I(4, 4);
    I[0][0] = I[1][1] = I[2][2] = I[3][3] = 1.0f;
    return I;
}

Matrix Matrix::viewport(int x, int y, int w, int h, int depth) {
    Matrix m = Matrix::identity();
    m[0][3] = x + w / 2.0f;
    m[1][3] = y + h / 2.0f;
    m[2][3] = depth / 2.0f;

    m[0][0] = w / 2.0f;
    m[1][1] = h / 2.0f;
    m[2][2] = depth / 2.0f;
    return m;
}

Matrix Matrix::transpose() const {
    Matrix result(cols, rows);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = m[i][j];
        }
    }
    return result;
}

Matrix Matrix::projection(float coeff) {
    Matrix m = Matrix::identity();
    m[3][2] = coeff;
    return m;
}

Matrix Matrix::lookat(Vec3f eye, Vec3f center, Vec3f up) {
    Vec3f z = (eye - center).normalize();
    Vec3f x = (up ^ z).normalize();
    Vec3f y = (z ^ x).normalize();

    Matrix Minv = Matrix::identity();
    Matrix Tr = Matrix::identity();

    for (int i = 0; i < 3; i++) {
        Minv[0][i] = x[i];
        Minv[1][i] = y[i];
        Minv[2][i] = z[i];
        Tr[i][3] = -center[i];
    }

    return Minv * Tr;
}