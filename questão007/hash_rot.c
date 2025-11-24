#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashing_utils.h"

int hash_rot(const char m[], int size) {
    int result = 0;
    if (m){
        size_t len = strlen(m);
        if (len >= 11 && size > 0) {
            char rotated[6];
            rotated[0] = m[4];
            rotated[1] = m[0];
            rotated[2] = m[1];
            rotated[3] = m[2];
            rotated[4] = m[3];
            rotated[5] = '\0';

            int dig2 = rotated[1] - '0'; // 2º da rotacionada
            int dig4 = rotated[3] - '0'; // 4º
            int dig5 = rotated[4] - '0'; // 5º

            int key = dig2 * 100 + dig4 * 10 + dig5; // apenas os três dígitos
            result = key % size;
        }
    }
    return result;
}

int inserir_rot(Aluno *vet, int size, const char m[]) {
    int colisoes = 0;
    if (m) {
        size_t len = strlen(m);
        if (len >= 11) {
            if (vet && size > 0) {
                int pos = hash_rot(m, size);

                char rotated_first = m[4];
                int step = rotated_first - '0';
                if (step == 0) step = 1;

                int original_pos = pos;
                int tentativas = 0;
                int encontrou = 0;
                int first_ocupado = -1;

                while (vet[pos].ocupado && tentativas < size) {
                    if (strcmp(vet[pos].matricula, m) == 0) {
                        /* já existe: atualiza */
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
                    } else {
                        if (first_ocupado == -1) first_ocupado = pos;
                        colisoes++;
                        tentativas++;
                        pos = (original_pos + step * tentativas) % size;
                    }
                }

                if (!encontrou) {
                    if (!vet[pos].ocupado) {
                        /* insere em pos livre */
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
                        /* tabela aparentemente cheia na sondagem:
                           evict a primeira posição ocupada encontrada durante a sondagem (se houver) */
                        int evict_pos = (first_ocupado != -1) ? first_ocupado : ((original_pos + step) % size);
                        snprintf(vet[evict_pos].matricula, sizeof(vet[evict_pos].matricula), "%s", m);
                        snprintf(vet[evict_pos].nome, sizeof(vet[evict_pos].nome), "Aluno_%s", m);
                        snprintf(vet[evict_pos].curso, sizeof(vet[evict_pos].curso), "Curso%c", m[4]);
                        vet[evict_pos].periodo = 1 + (evict_pos % 8);
                        char ano_str[5];
                        strncpy(ano_str, m, 4);
                        ano_str[4] = '\0';
                         vet[evict_pos].ano = atoi(ano_str);
 
                       vet[evict_pos].ocupado = 1;
                    }
                }
            }
        }
    }

    return colisoes;
}
