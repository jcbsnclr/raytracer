#pragma once

#include <stddef.h>

struct pool {
    void *buf;
    size_t len;
    size_t ptr;
};

void pool_init(struct pool *pool, size_t size);
void pool_free(struct pool *pool);

void *pool_alloc(struct pool *pool, size_t size);