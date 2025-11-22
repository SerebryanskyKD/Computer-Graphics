#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <string>
#include "geometry.h"

class Model {
private:
    std::vector<Vec3f> verts_;
    std::vector<Vec2f> tex_coords_;
    std::vector<Vec3f> norms_;
    std::vector<std::vector<int>> faces_;
    std::vector<std::vector<int>> faces_tex_;
    std::vector<std::vector<int>> faces_norm_;

public:
    Model(const char* filename);
    ~Model();

    int nverts() const { return verts_.size(); }
    int nfaces() const { return faces_.size(); }
    int ntexcoords() const { return tex_coords_.size(); }
    int nnorms() const { return norms_.size(); }

    Vec3f vert(int i) const { return verts_[i]; }
    Vec2f texcoord(int i) const { return tex_coords_[i]; }
    Vec3f norm(int i) const { return norms_[i]; }
    std::vector<int> face(int idx) const { return faces_[idx]; }
    std::vector<int> face_tex(int idx) const { return faces_tex_[idx]; }
    std::vector<int> face_norm(int idx) const { return faces_norm_[idx]; }
};

#endif