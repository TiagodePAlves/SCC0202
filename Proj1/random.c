#include "random.h"
#include <stdlib.h>


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
 * em chamadas aleatórias.
 */
void reseed(void) {
    // contador de quantas vezes até semear
    static unsigned count = 0;

    if unlikely(count == 0) {
        // usa ticks reduzido em 32 bits
        // como chave
        uint64_t chave = ticks();
        srand(combina_entropia(chave));
        // gera novo contador
        size_t cnt = rand();
        cnt = (cnt * 1024) / RAND_MAX;
        count = 1 + (unsigned) cnt;
    }
    // reduz contagem
    count--;
}

/* Gerador de inteiro com 31 bits de entropia. */
uint32_t rand_int(void) {
    reseed();
    return (uint32_t) rand();
}

// Maior valor de retorno de `meia_vida`.
#define MAX_MV 31

static inline attribute(const)
/**
 * Encontra o First Set Bit de um número positivo.
 */
uint8_t fsb(uint32_t num) {
    uint8_t pos = (uint8_t) __builtin_ffs((int) num);
    return pos - 1;
}

/**
 * Gerador em meia vida.
 *
 * Funcionamento: gera um número de UINT8_MAX bits,
 * MAX_MV bits por vez, até encontrar o primeiro
 * bit ativo. Assim, a posição 0 tem 50% de chance
 * de estar ativa, a posição 1 tem 25% de chance de
 * ser a primeira ativa (50% de bit 0 baixo e 50% de
 * bit 1 ativo), e assim por diante.
 */
uint8_t rand_hl(void) {
    // contador de bit inativos
    uint16_t count = 0;
    do {
        // gera MAX_MV bits de entropia
        uint32_t num = rand_int();

        // algum bit ativo
        if likely(num > 0) {
            // encontra ele
            return count + fsb(num);
        // nenhum bit ativo
        } else {
            count += MAX_MV;
        }
    } while (count < UINT8_MAX - 1);

    return UINT8_MAX - 1;
}
