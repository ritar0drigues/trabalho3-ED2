#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashing_utils.h"

int hash_fold(const char m[], int size) {
    int result = 0;
    if (m) {
        size_t len = strlen(m);
        if (len >= 11) {
            if (size > 0) {
                int g1 = (m[5] - '0') * 100 + (m[6] - '0') * 10 + (m[10] - '0');
                int g2 = (m[7] - '0') * 100 + (m[8] - '0') * 10 + (m[9] - '0');
                int num = g1 + g2;
                result = num % size;
            }
        }
    }
    return result;
}

int inserir_fold(Aluno *vet, int size, const char m[]) {
    int colisoes = 0;
    if (m) {
        size_t len = strlen(m);
        if (len >= 11) {
            if (vet && size > 0) {
                int pos = hash_fold(m, size);

                int step = (m[5] - '0') * 10 + (m[10] - '0');
                if (step == 0) step = 1;

                int original_pos = pos;
                int tentativas = 0;
                int encontrou = 0;

                while (vet[pos].ocupado && tentativas < size) {
                    if (strcmp(vet[pos].matricula, m) == 0) {
                        /* já existe: atualiza campos */
                        snprintf(vet[pos].matricula, sizeof(vet[pos].matricula), "%s", m);
                        snprintf(vet[pos].nome, sizeof(vet[pos].nome), "Aluno_%s", m);
                        snprintf(vet[pos].curso, sizeof(vet[pos].curso), "Curso%c", m[4]);
                        vet[pos].periodo = 1 + (pos % 8);

                        char ano_str[5];
                        strncpy(ano_str, m, 4);
                        ano_str[4] = '\0';
                        vet[pos].ano = atoi(ano_str);

                        encontrou = 1;
                        break;
                    }

                    colisoes++;
                    tentativas++;
                    pos = (original_pos + step * tentativas) % size;
                }

                if (!encontrou) {
                    if (!vet[pos].ocupado) {
                        /* insere em posição vazia */
                        snprintf(vet[pos].matricula, sizeof(vet[pos].matricula), "%s", m);
                        snprintf(vet[pos].nome, sizeof(vet[pos].nome), "Aluno_%s", m);
                        snprintf(vet[pos].curso, sizeof(vet[pos].curso), "Curso%c", m[4]);
                        vet[pos].periodo = 1 + (pos % 8);

                        char ano_str[5];
                        strncpy(ano_str, m, 4);
                        ano_str[4] = '\0';
                        vet[pos].ano = atoi(ano_str);

                        vet[pos].ocupado = 1;
                    } else {
                        /* não encontrou posição vazia: substitui na primeira posição de colisão */
                        int alt = (original_pos + step) % size;
                        snprintf(vet[alt].matricula, sizeof(vet[alt].matricula), "%s", m);
                        snprintf(vet[alt].nome, sizeof(vet[alt].nome), "Aluno_%s", m);
                        snprintf(vet[alt].curso, sizeof(vet[alt].curso), "Curso%c", m[4]);
                        vet[alt].periodo = 1 + (alt % 8);

                        char ano_str[5];
                        strncpy(ano_str, m, 4);
                        ano_str[4] = '\0';
                        vet[alt].ano = atoi(ano_str);

                        vet[alt].ocupado = 1;
                    }
                }
            }
        }
    }
    return colisoes;
}
