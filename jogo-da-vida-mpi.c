#include <stdio.h>
#include <mpi.h>

#define WIDTH 10
#define HEIGHT 10

int matriz[HEIGHT][WIDTH];
int nova_matriz[HEIGHT][WIDTH];
int above_row[WIDTH];
int below_row[WIDTH];

int main(int argc, char const *argv[])
{
     int worldSize, myRank, i, j;

     MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
     MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

     int local_height = HEIGHT / worldSize;

     MPI_Init(NULL, NULL); // Inicialização

     printf("\nMatriz inicial:\n");
     for (i = 0; i < HEIGHT; i++)
     {
          for (j = 0; j < WIDTH; j++)
          {
               matriz[i][j] = rand() % 2;
               printf("%d | ", matriz[i][j]);
          }
          printf("\n");
     } // matriz inicial populada e impressa na tela

     // Comunicar as linhas de borda de maneira bloqueante
     if (myRank > 0)
     {
          MPI_Send(matriz[0], WIDTH, MPI_INT, myRank - 1, 0, MPI_COMM_WORLD);
          MPI_Recv(above_row, WIDTH, MPI_INT, myRank - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
     }
     if (myRank < worldSize - 1)
     {
          MPI_Send(matriz[local_height - 1], WIDTH, MPI_INT, myRank + 1, 1, MPI_COMM_WORLD);
          MPI_Recv(below_row, WIDTH, MPI_INT, myRank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
     }

     MPI_Finalize();

     return 0;
}
