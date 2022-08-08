#include <scene.h>
#include <colour.h>

#include <math.h>

void ray_pos(struct ray ray, vec3 out) {
    glm_vec3_copy(ray.pos, out);
}

void ray_dir(struct ray ray, vec3 out) {
    glm_vec3_copy(ray.dir, out);
}

void ray_at(struct ray ray, float t, vec3 out) {
    vec3 scaled;
    glm_vec3_scale(ray.dir, t, scaled);
    glm_vec3_add(scaled, ray.pos, out);
}

static colour_t ray_colour(struct ray ray) {
    vec3 unit_dir;
    vec3 d;
    glm_vec3_copy(ray.dir, d);
    float len_squared = d[0]*d[0] + d[1]*d[1] + d[2]*d[2];
    float len = sqrt(len_squared);
    glm_vec3_scale(ray.dir, 1/len, unit_dir);

    unit_dir[1] = -unit_dir[1];

    float t = 0.5 * (unit_dir[1] + 1.0);
    
    uint8_t r, g, b;

    r = ((1.0 - t) * 1.0 + t * 0.5) * 255.0;
    g = ((1.0 - t) * 1.0 + t * 0.7) * 255.0;
    b = ((1.0 - t) * 1.0 + t * 1.0) * 255.0;

    return RGB(r,g,b);
}

void scene_render(struct scene *scene, SDL_Surface *surf) {
    (void)scene;

    // image
    int width = surf->w, height = surf->h;
    float aratio = (float)width / (float)height;

    // camera
    float vp_height = 2.0;
    float vp_width = aratio * vp_height;
    float focal_length = 1.0;

    vec3 pos = {0, 0, 0};
    vec3 hv = {vp_width, 0, 0};
    vec3 vv = {0, vp_height, 0};
    vec3 fv = {0, 0, focal_length};
    
    vec3 h_scaled, v_scaled;
    glm_vec3_scale(hv, 0.5, h_scaled);
    glm_vec3_scale(vv, 0.5, v_scaled);

    vec3 lower_left;

    glm_vec3_sub(pos, h_scaled, lower_left);
    glm_vec3_sub(lower_left, v_scaled, lower_left);
    glm_vec3_sub(lower_left, fv, lower_left);

    // the actual rendering
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            float u = (float)i / (float)(width - 1);
            float v = (float)j / (float)(height - 1);

            struct ray ray;

            vec3 hvxu, vvxv;

            glm_vec3_scale(hv, u, hvxu);
            glm_vec3_scale(vv, v, vvxv);

            glm_vec3_add(lower_left, hvxu, ray.dir);
            glm_vec3_add(ray.dir, vvxv, ray.dir);
            glm_vec3_sub(ray.dir, pos, ray.dir);

            glm_vec3_copy(pos, ray.pos);

            colour_t *fb = surf->pixels;

            fb[j * width + i] = ray_colour(ray);
        }
    }
}