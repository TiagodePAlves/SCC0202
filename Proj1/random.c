#include "random.h"
#include <string.h>


static
uint64_t ticks(void) {
    asm("rdtsc");
    register uint32_t lo asm("eax");
    register uint32_t hi asm("edx");

    uint64_t low = (uint64_t) lo;
    uint64_t high = (uint64_t) hi;
    return (high << 32) | low;
}


static inline attribute(const)
uint32_t inverte_bits(uint32_t num) {
    uint32_t ans = 0;

    for (uint8_t bit = 0; bit < 32; bit++) {
        uint8_t ibit = 32 - bit - 1;

        uint32_t b = (num >> bit) & 1;
        ans |= (b << ibit);
    }
    return ans;
}

static inline attribute(const)
uint32_t combina_entropia(uint64_t num) {
    uint32_t lo = num & 0xFFFFFFFF;
    uint32_t hi = num >> 32;

    return lo ^ inverte_bits(hi);
}

#define unlikely(x) \
    __builtin_expect(!!(x), 0)

static inline
void seed(void) {
    static bool seeded = false;
    if (unlikely(!seeded)) {
        uint64_t chave = ticks();
        srand(combina_entropia(chave));

        seeded = true;
    }
}


uint8_t rand_mv(void) {
    seed();
    int num = random();

    uint8_t count = 0;
    int mid = RAND_MAX / 2;
    while (mid > 0) {
        if (num >= mid) {
            return count;
        }
        mid /= 2;
        count++;
    }
    return count;
}
