#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "utils.h"

/* Stack as a pointer type. */
typedef struct stack *stack_t;
/* Stack as a immutable pointer. */
typedef const struct stack *const_stack_t;

/* Create stack for a specific type of element. */
#define stack_for(type) \
    stack_new(sizeof(type))

/* Creates stack for elements of `elem_size`.
 *
 * # Example
 *
 * For a stack of bytes, on a x86 archtecture, this would
 * be created with `stack_new(1)`.
 */
stack_t stack_new(size_t elem_size);

/* Creates a new stack from an array of `length` with elements of
 * `elem_size`.
 *
 * The array pointer should never be `NULL`, not even for an empty
 * array.
 *
 * The array is considered in increasing order, that is, the last
 * element of the array will be the top of the stack.
 */
stack_t stack_from_array(const void *array, size_t elem_size, size_t length)
attribute(nonnull);

/* Checks that a stack is empty. */
bool stack_empty(const_stack_t stack)
attribute(pure, nonnull);

/* Push element onto the stack.
 * Return wether operation was successful.
 *
 * `elem` must point to an object of same size as the declared
 * `elem_size` of the given `stack`.
 *
 * If `stack` is full, this function will try to grow the stack
 * capacity before pushing the element. Operation will only fail
 * if stack is full and can't be grown.
 */
bool stack_push(stack_t * restrict stack, const void * restrict elem)
attribute(nonnull);

/* Get reference to the top element on the stack.
 * Returns `NULL` when stack is empty.
 */
void *stack_peek(const stack_t * restrict stack)
attribute(pure, nonnull);

/* Pop top element from the stack.
 * Return wether operation was successful.
 *
 * If stack is empty, nothing is written on `elem` and operation
 * is considered unsuccessful.
 *
 * `elem` must point be able to hold an object of same size as the
 * declared `elem_size` of the given `stack`.
 *
 * If `stack` is using less than 25% of the capacity, the stack
 * allocated memory may be shrunk.
 */
bool stack_pop(stack_t * restrict stack, void * restrict elem)
attribute(nonnull);
