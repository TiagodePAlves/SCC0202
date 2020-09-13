#include "stack.h"
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>


// Initial capacity for a new stack.
#define INITIAL_CAPACITY 64


struct stack {
    // capacity power of two
    size_t cap, len;
    size_t elem_size;
    byte_t data[];
};

/* * * * * * * * *
 * Basic checks  */

static inline attribute(pure, nonnull)
// True when stack is empty.
bool empty(const_stack_t stack) {
    assert(stack != NULL);

    return stack->len == 0;
}

static inline attribute(pure, nonnull)
// True when stack capacity is full.
bool full(const_stack_t stack) {
    assert(stack != NULL);
    // check length validity on debug
    assert(stack->len <= stack->cap);

    return stack->len >= stack->cap;
}

static inline attribute(pure, nonnull)
// Indicates low capacity usage (less than 25%) by the stack.
// Used for shrinking.
bool low_usage(const_stack_t stack) {
    assert(stack != NULL);
    // don't shrink when at initial capacity
    bool small_cap = stack->cap <= INITIAL_CAPACITY;
    // otherwise, shrink when using less than 25% capacity
    bool low_len = stack->len < stack->cap / 4;
    return !small_cap && low_len;
}

attribute(pure, nonnull)
// extern version
bool stack_empty(const_stack_t stack) {
    return empty(stack);
}

/* * * * * * * * * * *
 * Memory management */

// ceil(n / m) for integers, instead of the default integer division
// https://en.wikipedia.org/wiki/Floor_and_ceiling_functions#Quotients
#define ceil_div(n, m) \
    (((n) - 1) / (m) + 1)

static inline attribute(const)
// Calculates necessary allocation size for a stack that holds
// `capacity` elements of size `elem_size`.
size_t size_for(size_t elem_size, size_t capacity) {
    // size of the fixed part of the struct
    const size_t fixed = offsetof(struct stack, data);
    // overflow check on debug
    assert(ceil_div(SIZE_MAX - fixed, elem_size) >= capacity);

    return fixed + elem_size * capacity;
}

static inline attribute(const)
// Next capacity when growing a stack.
size_t next_capacity(size_t current) {
    if (current == 0) {
        return INITIAL_CAPACITY;
    } else {
        // overflow check on debug
        assert(current <= SIZE_MAX / 2);
        return current * 2;
    }
}

static inline
// Creates stack with given capacity
stack_t new(size_t elem_size, size_t capacity) {
    stack_t new = malloc(size_for(elem_size, capacity));
    if (unlikely(new == NULL)) {
        return NULL;
    }
    new->cap = capacity;
    new->len = 0;
    new->elem_size = elem_size;
    return new;
}

// Creates the default stack
stack_t stack_new(size_t elem_size) {
    return new(elem_size, INITIAL_CAPACITY);
}

attribute(nonnull)
// Free stack memory
void stack_free(stack_t stack) {
#ifdef DEBUG
    // making read-after-free more prone to bug
    assert(stack != NULL);
    stack->cap = 0;
    stack->len = 0;
    stack->elem_size = 0;
#endif
    free(stack);
}

// Copies elements from array into a new stack
attribute(nonnull)
stack_t stack_from_array(const void *array, size_t elem_size, size_t length) {
    assert(array != NULL);

    size_t capacity = max(length, INITIAL_CAPACITY);
    stack_t stack = new(elem_size, capacity);
    if (unlikely(stack == NULL)) {
        return NULL;
    }
    memcpy(stack->data, array, elem_size * length);
    stack->len = length;

    return stack;
}

static inline attribute(nonnull)
// Increase stack capacity, moving to another pointer
stack_t grow(stack_t stack) {
    assert(stack != NULL);

    size_t new_cap = next_capacity(stack->cap);
    size_t new_size = size_for(stack->elem_size, new_cap);

    stack_t new = realloc(stack, new_size);
    if (unlikely(new == NULL)) {
        return NULL;
    }
    new->cap = new_cap;
    return new;
}

static inline attribute(nonnull)
// Reduces stack capacity, moving to another pointer
//
// NOTE: Doesn't update stack length, that must be checked before shrinking
stack_t shrink(stack_t stack) {
    assert(stack != NULL);

    size_t new_cap = stack->cap / 2;
    size_t new_size = size_for(stack->elem_size, new_cap);

    stack_t new = realloc(stack, new_size);
    if (unlikely(new == NULL)) {
        return NULL;
    }
    new->cap = new_cap;
    // check length is valid on debug
    assert(new->len <= new_cap);
    return new;
}

static inline attribute(pure, nonnull)
// Get pointer to element at position `pos` on the stack.
//
// NOTE: does not check bounds
void *get_elem(const_stack_t stack, size_t pos) {
    assert(stack != NULL);

    size_t byte_pos = stack->elem_size * pos;
    return (void *) &stack->data[byte_pos];
}

static inline attribute(nonnull)
// Read element at position `pos` on the stack, writing to `elem`.
void read(const_stack_t restrict stack, size_t pos, void * restrict elem) {
    memcpy(elem, get_elem(stack, pos), stack->elem_size);
}

static inline attribute(nonnull)
// Write `elem` at position `pos` on the stack.
void write(stack_t restrict stack, size_t pos, const void * restrict elem) {
    memcpy(get_elem(stack, pos), elem, stack->elem_size);
}

static inline attribute(nonnull)
// Move `elem` at position `from` on the stack to position `to`, overwriting values.
// Positions must be distinct.
void move(stack_t stack, size_t from, size_t to) {
    assert(from != to);
    memcpy(get_elem(stack, to), get_elem(stack, from), stack->elem_size);
}

/* * * * * * * * * * *
 * Stack operations  */

attribute(nonnull)
bool stack_push(stack_t * restrict stack, const void * restrict elem) {
    assert(stack != NULL);

    // tries growing space when full
    if (unlikely(full(*stack))) {
        stack_t new = grow(*stack);
        if (unlikely(new == NULL)) {
            return false;
        }
        *stack = new;
    }

    write(*stack, (*stack)->len++, elem);
    return true;
}

attribute(pure, nonnull)
void *stack_peek(const_stack_t stack) {
    assert(stack != NULL);

    if (unlikely(empty(stack))) {
        return NULL;
    }
    return get_elem(stack, stack->len-1);
}

attribute(nonnull)
bool stack_pop(stack_t * restrict stack, void * restrict elem) {
    assert(stack != NULL);

    if (unlikely(empty(*stack))) {
        return false;
    }
    read(*stack, --(*stack)->len, elem);

    // tries to shrink unused memory
    if (unlikely(low_usage(*stack))) {
        stack_t new = shrink(*stack);
        if (likely(new != NULL)) {
            *stack = new;
        }
    }
    return true;
}

static inline attribute(nonnull)
// Invert stack with given buffer.
void invert_with(stack_t restrict stack, void * restrict tmp) {
    for (size_t i = 0; i < stack->len / 2; i++) {
        size_t inv = stack->len - i - 1;

        read(stack, i, tmp);
        move(stack, inv, i);
        write(stack, inv, tmp);
    }
}

static inline attribute(nonnull)
// Invert stack with small (up to 32 bytes) `elem_size`.
void invert_small(stack_t stack) {
    byte_t tmp[32];
    invert_with(stack, tmp);
}

attribute(nonnull)
// Optimized stack order inversion.
bool stack_invert(stack_t stack) {
    assert(stack != NULL);

    if (stack->elem_size <= 32) {
        invert_small(stack);
        return true;
    }

    void *tmp = malloc(stack->elem_size);
    if (unlikely(tmp == NULL)) {
        return false;
    }
    invert_with(stack, tmp);

    free(tmp);
    return true;
}
