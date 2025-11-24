
#include <stdio.h>
#include <stdlib.h>

#define MAT_SIZE 11       
#define MAX_STUDENTS 4000 


typedef struct {
    char matricula[MAT_SIZE + 1];
    char nome[64];
    char curso[16];
    int periodo;
    int ano;
    int ocupado;
} Aluno;


void gerar_matriculas(char matriculas[][MAT_SIZE + 1], int n);


int hash_rot(const char m[], int size);
int hash_fold(const char m[], int size);
int inserir_rot(Aluno *vet, int size, const char m[]);
int inserir_fold(Aluno *vet, int size, const char m[]);
