#include "random.h"


static inline
/**
 * Quantidade de ticks desde que a máquina foi ligada.
 *
 * Serve como um marcador de "tempo" sem a <time.h>,
 * mas é dependente da arquitetura.
 */
uint64_t ticks(void) {
    // ReaD TimeStamp Counter
    asm("rdtsc");
    // o resultado da instrução acima é
    // guardado nos registradores EAX e EDX
    register uint32_t lo asm("eax");
    register uint32_t hi asm("edx");

    // que devem então ser combinados
    // em um inteiro de 64 bits
    uint64_t low = (uint64_t) lo;
    uint64_t high = (uint64_t) hi;
    return (high << 32) | low;
}


static inline attribute(const)
/**
 * Inverte a posição dos bits em um inteiro de 32 bits.
 */
uint32_t inverte_bits(uint32_t num) {
    uint32_t resultado = 0;

    // inverte bit a bit
    for (uint8_t bit = 0; bit < 32; bit++) {
        // acha nova posição do bit
        uint8_t ibit = 32 - bit - 1;

        // seleciona aquele bit
        uint32_t b = (num >> bit) & 1;
        // e põe no posição certa do resultado
        resultado |= (b << ibit);
    }
    return resultado;
}

static inline attribute(const)
/**
 * Reduz de 64 bits para 32, tentanto manter uma parte
 * da entropia.
 */
uint32_t combina_entropia(uint64_t num) {
    uint32_t lo = num & 0xFFFFFFFF;
    uint32_t hi = num >> 32;
    // XOR dos bits superiores invertidos
    // com os inferiors
    return lo ^ inverte_bits(hi);
}

static inline
/**
 * Semeia o RNG com base nos ticks do computador,
 * apenas na primeira chamada.
 */
void seed(void) {
    // marcador estático se já semeado
    static bool seeded = false;

    if unlikely(!seeded) {
        // usa ticks reduzido em 32 bits
        // como chave
        uint64_t chave = ticks();
        srand(combina_entropia(chave));
        // marca semeado
        seeded = true;
    }
}

static inline attribute(const)
/**
 * Transformação para a meia vida, usada por `rand_hl`.
 */
uint8_t meia_vida(int num) {
    uint8_t count = 0;
    // marca a metade dos valores possíveis
    int mid = RAND_MAX;
    while (mid > 0) {
        // em cada etapa reduz essa metade
        mid /= 2;
        // testa se 'num' continua menor
        if (num > mid) {
            return count;
        }
        // contando quantas vezes
        // o número "sobreviveu"
        count++;
    }
    // máxima contagem quando
    // 'num' é zero
    return count;
}

static inline
/**
 * Aborta se 'MAX_HL' não é o valor esperado.
 * Quando compilado com otimizações, essa função
 * é completamente removida.
 */
void assert_max_hl(void) {
    if (MAX_RHL != meia_vida(0)) {
        abort();
    }
}

/* Gerador em meia vida. */
uint8_t rand_hl(void) {
    assert_max_hl();
    seed();
    // aplica 'meia_vida' com
    // um número pseudoaleatório
    int num = random();
    return meia_vida(num);
}
