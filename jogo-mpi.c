#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TAM 10
#define iter 10

int contar_vizinhos(int matriz_aux[TAM][TAM], int x, int y, int pedaco_matriz)
{
     int vizinhos = 0;
     for (int dx = -1; dx <= 1; dx++)
     {
          for (int dy = -1; dy <= 1; dy++)
          {
               int nx = x + dx;
               int ny = y + dy;
               // tem que fazer essa lógica enorme pra não pegar valores out of bounds
               if (!((dx == 0 && dy == 0) || nx < 0 || nx >= pedaco_matriz || ny < 0 || ny >= TAM))
               {
                    vizinhos += matriz_aux[nx][ny];
               }
          }
     }
     return vizinhos;
}

int main(int argc, char *argv[])
{
     int rank, worldSize;
     MPI_Init(&argc, &argv);
     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
     MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

     int matriz[TAM][TAM];
     int pedaco_matriz = TAM / worldSize;
     int matriz_aux[TAM][TAM];

     if (rank == 0)
     {
          printf("Matriz inicial: \n");
          if (rank == 0)
          {
               for (int i = 0; i < TAM; i++)
               {
                    for (int j = 0; j < TAM; j++)
                    {
                         matriz[i][j] = rand() % 2;
                         printf("%d | ", matriz[i][j]);
                    }
                    printf("\n");
               }
          }
     }

     // vou usar scatter pq deixa mais simples que send e recv
     MPI_Scatter(matriz, pedaco_matriz * TAM, MPI_INT, matriz_aux, pedaco_matriz * TAM, MPI_INT, 0, MPI_COMM_WORLD);
     // NÃO PODE COLOCAR O SCATTER DENTRO DO LOOP DAS ITERAÇÕES!!!!!

     for (int a = 0; a < iter; a++)
     {
          int matriz_aux_dos_pedacos[pedaco_matriz][TAM];
          for (int i = 0; i < pedaco_matriz; i++)
          {
               for (int j = 0; j < TAM; j++)
               {
                    int vizinhos = contar_vizinhos(matriz_aux, i, j, pedaco_matriz);
                    if (matriz_aux[i][j] == 1 && (vizinhos == 2 || vizinhos == 3))
                    {
                         matriz_aux_dos_pedacos[i][j] = 1; // Continua vivo
                    }
                    else if (matriz_aux[i][j] == 0 && vizinhos == 3)
                    {
                         matriz_aux_dos_pedacos[i][j] = 1; // Renasce
                    }
                    else
                    {
                         matriz_aux_dos_pedacos[i][j] = 0; // Morreu
                    }
               }
          } // quando terminar de calcular os pedaços tem que juntar de novo pra aux
          for (int i = 0; i < pedaco_matriz; i++)
          {
               for (int j = 0; j < TAM; j++)
               {
                    matriz_aux[i][j] = matriz_aux_dos_pedacos[i][j];
               }
          }

          // juntar a matriz na matriz original
          MPI_Gather(matriz_aux, pedaco_matriz * TAM, MPI_INT,
                     matriz, pedaco_matriz * TAM, MPI_INT,
                     0, MPI_COMM_WORLD);

          if (rank == 0)
          {
               printf("Matriz da iter num %d: \n", a);
               for (int i = 0; i < TAM; i++)
               {
                    for (int j = 0; j < TAM; j++)
                    {
                         printf("%d | ", matriz[i][j]);
                    }
                    printf("\n");
               }
          }
     }
     MPI_Finalize();
     return 0;
}
