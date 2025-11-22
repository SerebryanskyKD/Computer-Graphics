#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>
#include <iostream>

template<class T> struct Vec2 {
    T x, y;
    Vec2() : x(0), y(0) {}
    Vec2(T x, T y) : x(x), y(y) {}

    Vec2 operator +(const Vec2& v) const { return Vec2(x + v.x, y + v.y); }
    Vec2 operator -(const Vec2& v) const { return Vec2(x - v.x, y - v.y); }
    Vec2 operator *(float f) const { return Vec2(x * f, y * f); }
    Vec2& operator =(const Vec2& v) { x = v.x; y = v.y; return *this; }
    T& operator [](const int i) { return i <= 0 ? x : y; }
    const T& operator [](const int i) const { return i <= 0 ? x : y; }
};

template<class T> struct Vec3 {
    T x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

    Vec3 operator +(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator -(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator *(float f) const { return Vec3(x * f, y * f, z * f); }
    T operator *(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    Vec3 operator ^(const Vec3& v) const { return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
    Vec3& operator =(const Vec3& v) { x = v.x; y = v.y; z = v.z; return *this; }
    Vec3 operator -() const { return Vec3(-x, -y, -z); }

    float norm() const { return std::sqrt(x * x + y * y + z * z); }

    Vec3& normalize() {
        float n = norm();
        if (n > 0) { float inv_n = 1.0f / n; x *= inv_n; y *= inv_n; z *= inv_n; }
        return *this;
    }

    T& operator [](const int i) {
        if (i <= 0) return x;
        else if (i == 1) return y;
        else return z;
    }
    const T& operator [](const int i) const {
        if (i <= 0) return x;
        else if (i == 1) return y;
        else return z;
    }
};

typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;
typedef Vec3<int> Vec3i;
typedef Vec3<float> Vec3f;

#endif