#pragma once

#include <cglm/cglm.h>

#include <SDL2/SDL.h>

#include <stdbool.h>

// largely taken from "Raytracing In One Weekend"

struct scene {
    vec3 pos;
    float radius;
};

struct ray { 
    vec3 pos;
    vec3 dir;
};

void ray_pos(struct ray ray, vec3 out);
void ray_dir(struct ray ray, vec3 out);

void ray_at(struct ray ray, float t, vec3 out);

void scene_render(struct scene *scene, SDL_Surface *surf);