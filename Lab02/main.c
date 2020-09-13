#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "stack.h"

static inline
// Reads a `size_t` from stdin.
size_t read_unsigned(void) {
    size_t num;

    int ret = scanf("%zu", &num);
    assert_eq(ret, 1);

    return num;
}

static inline
// Reads the `buy` stack from stdin.
stack_t read_cards(size_t length) {
    size_t *card = malloc(length * sizeof(size_t));

    for (size_t i = length; i > 0; i--) {
        card[i-1] = read_unsigned();
    }

    stack_t buy = stack_from_array(card, sizeof(size_t), length);
    assert(buy != NULL);
    free(card);

    return buy;
}

static inline attribute(pure, nonnull)
// Reads whats the next card to go on the `dead` stack.
size_t expected_card(const_stack_t dead) {
    size_t *top = stack_peek(dead);
    if (top == NULL) {
        return 1;
    } else {
        return *top + 1;
    }
}

static inline attribute(nonnull)
// Plays one round and returns the discarded stack.
stack_t next_round(stack_t *restrict buy, stack_t *restrict dead) {
    stack_t discarded = stack_for(size_t);

    size_t card;
    while (stack_pop(buy, &card)) {
        // choose which stack the card must go
        stack_t *stack = (card == expected_card(*dead))?
            dead : &discarded;

        bool ok = stack_push(stack, &card);
        assert_eq(ok, true);
    }
    return discarded;
}

static inline attribute(nonnull)
// Simulate the entire game starting with given buy stack.
size_t simulate(stack_t buy) {
    stack_t dead = stack_for(size_t);
    size_t rounds = 0;

    while (!stack_empty(buy)) {
        // play the round
        stack_t discarded = next_round(&buy, &dead);
        stack_free(buy);

        // makes the discarded deck into the new buy deck
        bool ok = stack_invert(discarded);
        assert_eq(ok, true);
        buy = discarded;

        // mark another complete round
        rounds += 1;
    }

    stack_free(dead);
    stack_free(buy);
    return rounds;
}

int main(void) {
    size_t N = read_unsigned();
    stack_t buy = read_cards(N);

    size_t rounds = simulate(buy);
    int result = printf("%zu\n", rounds);
    assert_expr(result, result > 0);

    return EXIT_SUCCESS;
}
