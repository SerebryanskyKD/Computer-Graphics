#include "model.h"
#include <fstream>
#include <sstream>
#include <iostream>

Model::Model(const char* filename) {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) {
        std::cerr << "Failed to open " << filename << std::endl;
        return;
    }

    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;

        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            iss >> v.x >> v.y >> v.z;
            verts_.push_back(v);
        }
        else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vec2f tc;
            iss >> tc.x >> tc.y;
            tex_coords_.push_back(tc);
        }
        else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            Vec3f n;
            iss >> n.x >> n.y >> n.z;
            norms_.push_back(n);
        }
        else if (!line.compare(0, 2, "f ")) {
            iss >> trash;
            std::vector<int> f;
            std::vector<int> f_tex;
            std::vector<int> f_norm;
            int v_idx, t_idx, n_idx;
            char slash;

            while (iss >> v_idx >> slash >> t_idx >> slash >> n_idx) {
                v_idx--; t_idx--; n_idx--;
                f.push_back(v_idx);
                f_tex.push_back(t_idx);
                f_norm.push_back(n_idx);
            }
            faces_.push_back(f);
            faces_tex_.push_back(f_tex);
            faces_norm_.push_back(f_norm);
        }
    }
}

Model::~Model() {}