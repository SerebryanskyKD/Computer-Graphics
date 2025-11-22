#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "matrix.h"
#include <vector>
#include <algorithm>
#include <limits>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
    Vec3f s[2];
    for (int i = 0; i < 2; i++) {
        s[i].x = B[i] - A[i];
        s[i].y = C[i] - A[i];
        s[i].z = A[i] - P[i];
    }

    Vec3f u = s[0] ^ s[1];

    if (std::abs(u.z) > 1e-2) {
        return Vec3f(1.0f - (u.x + u.y) / u.z, u.x / u.z, u.y / u.z);
    }

    return Vec3f(-1, -1, -1);
}

void triangle_barycentric(Vec3f t0, Vec3f t1, Vec3f t2,
    Vec2f uv0, Vec2f uv1, Vec2f uv2,
    float intensity0, float intensity1, float intensity2,
    TGAImage& image, TGAImage& texture,
    std::vector<float>& z_buffer) {
    int width = image.get_width();
    int height = image.get_height();

    int min_x = std::max(0, (int)std::min(t0.x, std::min(t1.x, t2.x)));
    int max_x = std::min(width - 1, (int)std::max(t0.x, std::max(t1.x, t2.x)));
    int min_y = std::max(0, (int)std::min(t0.y, std::min(t1.y, t2.y)));
    int max_y = std::min(height - 1, (int)std::max(t0.y, std::max(t1.y, t2.y)));

    Vec3f P;
    for (P.y = min_y; P.y <= max_y; P.y++) {
        for (P.x = min_x; P.x <= max_x; P.x++) {
            Vec3f bc = barycentric(t0, t1, t2, P);

            if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;

            P.z = t0.z * bc.x + t1.z * bc.y + t2.z * bc.z;

            int idx = int(P.x) + int(P.y) * width;
            if (P.z > z_buffer[idx] + 0.001f) {
                z_buffer[idx] = P.z;

                Vec2f uv;
                uv.x = uv0.x * bc.x + uv1.x * bc.y + uv2.x * bc.z;
                uv.y = uv0.y * bc.x + uv1.y * bc.y + uv2.y * bc.z;

                float intensity = intensity0 * bc.x + intensity1 * bc.y + intensity2 * bc.z;

                int tex_x = int(uv.x * texture.get_width());
                int tex_y = int(uv.y * texture.get_height());
                tex_x = std::max(0, std::min(texture.get_width() - 1, tex_x));
                tex_y = std::max(0, std::min(texture.get_height() - 1, tex_y));

                TGAColor tex_color = texture.get(tex_x, tex_y);

                TGAColor final_color = TGAColor(
                    tex_color.r * intensity,
                    tex_color.g * intensity,
                    tex_color.b * intensity,
                    255
                );

                image.set(P.x, P.y, final_color);
            }
        }
    }
}

int main(int argc, char** argv) {
    const int width = 800;
    const int height = 800;

    TGAImage image;
    image.read_tga_file("image.tga");

    if (image.get_width() != width || image.get_height() != height) {
        image.scale(width, height);
    }

    std::vector<float> z_buffer(width * height, -std::numeric_limits<float>::max());

    TGAImage texture;
    if (!texture.read_tga_file("african_head_diffuse.tga")) {
        std::cerr << "Ќе удалось загрузить текстуру модели!" << std::endl;
        return -1;
    }
    texture.flip_vertically();

    Model model("obj/african_head.obj");
    Vec3f light_dir(0, 0, -1);
    light_dir.normalize();

    for (int i = 0; i < model.nfaces(); i++) {
        std::vector<int> face = model.face(i);
        std::vector<int> face_tex = model.face_tex(i);
        std::vector<int> face_norm = model.face_norm(i);

        Vec3f screen_coords[3];
        Vec2f tex_coords[3];
        float intensities[3];

        for (int j = 0; j < 3; j++) {
            Vec3f world = model.vert(face[j]);
            screen_coords[j] = Vec3f(
                (world.x + 1.) * width / 2.,
                (world.y + 1.) * height / 2.,
                world.z
            );

            if (model.ntexcoords() > 0) {
                tex_coords[j] = model.texcoord(face_tex[j]);
            }
            else {
                tex_coords[j] = Vec2f(0, 0);
            }

            if (model.nnorms() > 0) {
                Vec3f normal = model.norm(face_norm[j]);
                normal.normalize();
                intensities[j] = std::max(0.0f, -normal * light_dir);
            }
            else {
                intensities[j] = 1.0f;
            }
        }

        triangle_barycentric(
            screen_coords[0], screen_coords[1], screen_coords[2],
            tex_coords[0], tex_coords[1], tex_coords[2],
            intensities[0], intensities[1], intensities[2],
            image, texture, z_buffer
        );
    }

    image.flip_vertically();
    image.write_tga_file("output.tga");
    return 0;
}