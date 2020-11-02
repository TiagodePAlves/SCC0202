#include "random.h"
#include "utils.h"


uint64_t ticks(void) {
    asm("rdtsc");
    register uint32_t lo asm("eax");
    register uint32_t hi asm("edx");

    uint64_t low = (uint64_t) lo;
    uint64_t high = (uint64_t) hi;
    return (high << 32) | low;
}
