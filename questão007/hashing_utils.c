#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hashing_utils.h"
// ...existing code...
void gerar_matriculas(char matriculas[][MAT_SIZE + 1], int n) {
    for (int i = 0; i < n; i++) {
        int ano = 2000 + (i % 26);             // 4 dígitos
        int curso = 1 + (i % 9);               // 1..9 (5ª posição deve ser dígito)
        int seq = i % 1000000;                 // 6 dígitos finais
        snprintf(matriculas[i], MAT_SIZE + 1, "%04d%1d%06d", ano, curso, seq);
    }
}
