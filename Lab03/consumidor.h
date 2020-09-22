#ifndef __CONSUMIDOR_H__
/* */
#define __CONSUMIDOR_H__

#include "utils.h"


typedef struct consumidor {
    uint8_t idade;
    bool agravante;
    char nome[];
} consumidor_t;


#endif//__CONSUMIDOR_H__
