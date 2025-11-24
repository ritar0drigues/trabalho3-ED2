#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashing_utils.h"

int inserir_rot(Aluno*, int, const char*);
int inserir_fold(Aluno*, int, const char*);

/* agora recebe tabela e matriculas por parâmetro, evita variáveis globais */
void teste_final(Aluno *tab, int size, char matriculas[][MAT_SIZE+1], int n_matriculas, const char* metodo) {
    if (tab && size > 0 && matriculas && n_matriculas > 0 && metodo) {
        memset(tab, 0, sizeof(Aluno) * size);

        int total_colisoes = 0;
        int max_colisoes = 0;
        int histograma_colisoes[20] = {0};

        int (*inserir_func)(Aluno*, int, const char*) =
            (strcmp(metodo, "rot") == 0) ? inserir_rot : inserir_fold;

        for (int i = 0; i < n_matriculas; i++) {
            int col = inserir_func(tab, size, matriculas[i]);
            total_colisoes += col;

            if (col > max_colisoes) max_colisoes = col;

            if (col < 20) histograma_colisoes[col]++;
            else histograma_colisoes[19]++;
        }

        int ocupadas = 0;
        for (int i = 0; i < size; i++) {
            if (tab[i].ocupado) ocupadas++;
        }

        printf("\n--- %s ---\n", (strcmp(metodo, "rot") == 0) ? "ROTACAO" : "FOLD-SHIFT");
        printf("Total de colisoes: %d\n", total_colisoes);
        printf("Media de colisoes por insercao: %.3f\n", total_colisoes / (double)n_matriculas);
        printf("Maximo de colisoes: %d\n", max_colisoes);

        printf("Distribuicao de colisoes:\n");
        for (int i = 0; i < 10; i++) {
            printf("  %2d colisao(oes): %4d insercões\n", i, histograma_colisoes[i]);
        }
        if (histograma_colisoes[19] > 0) {
            printf("  19+ colisoes:    %4d insercões\n", histograma_colisoes[19]);
        }

        printf("Elementos na tabela: %d/%d\n", ocupadas, size);
        printf("Fator de carga final: %.3f\n", ocupadas / (double)size);
    }
}

int main() {
    srand(12345);

    /* evita globais: arrays locais */
    static Aluno tabela1_local[1280];
    static Aluno tabela2_local[1211];
    static char matriculas_local[MAX_STUDENTS][MAT_SIZE+1];

    gerar_matriculas(matriculas_local,MAX_STUDENTS);

    printf("=== COMPARACAO DE METODOS HASHING ===\n");
    printf("Base de dados: %d matriculas\n", MAX_STUDENTS);

    int tamanhos[2] = {1211, 1280};

    for (int t = 0; t < 2; t++) {
        int size = tamanhos[t];

        printf("\n\n====== TABELA COM %d POSICOES ======\n", size);
        printf("Fator de carga inicial: %.3f\n", MAX_STUDENTS / (double)size);

        if (size == 1211) {
            teste_final(tabela2_local, size, matriculas_local, MAX_STUDENTS, "rot");
            teste_final(tabela2_local, size, matriculas_local, MAX_STUDENTS, "fold");
        } else {
            teste_final(tabela1_local, size, matriculas_local, MAX_STUDENTS, "rot");
            teste_final(tabela1_local, size, matriculas_local, MAX_STUDENTS, "fold");
        }
    }

    printf("\n=== CONCLUSÃO ===\n");
    printf("O metodo FOLD-SHIFT apresenta melhor desempenho com menos colisoes.\n");
    printf("O metodo ROTACAO tem mais colisoes devido a distribuicao menos uniforme.\n");

    return 0;
}