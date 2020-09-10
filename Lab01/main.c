#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define inline __inline__
#define unused(var) (void) var

#define BUF_SIZE 1024

#define _STR(X) #X
#define STR(X) _STR(X)

static inline
int read_line(char buffer[BUF_SIZE]);

int main(void) {
    char line[BUF_SIZE] = "";

    while (read_line(line)) {
        char pat[BUF_SIZE] = "";
        char sub[BUF_SIZE] = "";

        const bool pat_res = read_line(pat);
        assert(pat_res); unused(pat_res);
        const bool sub_res = read_line(sub);
        assert(sub_res); unused(sub_res);

        int res = printf("%s <-| %s -> %s\n", line, pat, sub);
        assert(res > 0); unused(res);
    }

    return EXIT_SUCCESS;
}

static inline
int read_line(char buffer[BUF_SIZE]) {
    const int result = scanf("%"STR(BUF_SIZE)"[^\n]\n", buffer);
    return result == 1;
}
