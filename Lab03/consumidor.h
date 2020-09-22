#ifndef __CONSUMIDOR_H__
/* */
#define __CONSUMIDOR_H__

#include "utils.h"


// Dados de um consumidor
typedef struct consumidor {
    uint8_t idade;
    // Condição agravante
    bool agravante;
    char nome[];
} consumidor_t;


#endif//__CONSUMIDOR_H__
