#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

void verifica_vizinhos(int a, int b, int matriz[a][b], int nova_matriz[a][b]){
    //verificar linha por linha
    int i,j;
    #pragma omp parallel for
     for (i=0;i<a;i++){
        #pragma omp parallel for
        for (j=0;j<b;j++){
            int vizinhos_vivos = 0;
            // Verifica vizinhos considerando os limites da matriz
            if (i > 0) vizinhos_vivos += matriz[i-1][j]; // Acima
            if (i < a-1) vizinhos_vivos += matriz[i+1][j]; // Abaixo
            if (j > 0) vizinhos_vivos += matriz[i][j-1]; // Esquerda
            if (j < b-1) vizinhos_vivos += matriz[i][j+1]; // Direita
            if (i > 0 && j > 0) vizinhos_vivos += matriz[i-1][j-1]; // Superior Esquerda
            if (i > 0 && j < b-1) vizinhos_vivos += matriz[i-1][j+1]; // Superior Direita
            if (i < a-1 && j > 0) vizinhos_vivos += matriz[i+1][j-1]; // Inferior Esquerda
            if (i < a-1 && j < b-1) vizinhos_vivos += matriz[i+1][j+1]; // Inferior Direita

            if (vizinhos_vivos > 4 || vizinhos_vivos <2 ) {
                nova_matriz[i][j] = 0;
            } else {
                nova_matriz[i][j] = 1;
            }
        }
    }
}

int main () {
    int matriz[6][6], i, j, nova_matriz[6][6];

    printf("\nMatriz inicial:\n");
    for (i=0;i<6;i++){
        for (j=0;j<6;j++){
            matriz[i][j] = rand() % 2;
            printf("%d | ", matriz[i][j]);
        }
        printf("\n");
    } //matriz inicial populada e impressa na tela

    verifica_vizinhos( 6, 6, matriz, nova_matriz);

    printf("\n\nNova matriz:\n");
    for (i=0;i<6;i++){
        for (j=0;j<6;j++){
            printf("%d | ", nova_matriz[i][j]);
        }
        printf("\n");
    }//matrix após 1 iteração
    return 0;
}