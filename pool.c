#include <pool.h>
#include <util.h>

void pool_init(struct pool *pool, size_t size) {
    pool->buf = malloc(size);
    pool->len = size;
    pool->ptr = 0;
    
    if (!pool->buf)
        die_fancy("failed to allocate pool of size %zu", size);
}

void pool_free(struct pool *pool) {
    free(pool->buf);
}

void *pool_alloc(struct pool *pool, size_t size) {
    if (pool->ptr + size >= pool->len)
        return NULL;

    size_t offset = pool->ptr;
    pool->ptr += size;

    return pool->buf + offset;
}