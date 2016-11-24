#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <time.h>
#include <omp.h>
double get_time() {
	struct timespec tiempo;
	double sec;

	if (clock_gettime(CLOCK_REALTIME, &tiempo)==0){
		sec=tiempo.tv_sec+tiempo.tv_nsec/(double) 1000000000.0;
    return sec;
	}	else {
		printf("Obtener tiempo: NO HA HABIDO RESPUESTA DEL GETTIME\n");
		return 0;
	}
}


int main(int argc, char* argv[]){

  int thread_count = strtol(argv[1],NULL, 10);
  int af, ac, bf, bc, i, j, k,sum;
  printf("Filas de Matriz A-->");
  scanf("%d", &af);
  printf("Columnas de Matriz  A-->");
  scanf("%d", &ac);
  printf("Filas de matriz B-->");
  scanf("%d", &bf);
  printf("Columnas de matriz  B-->");
  scanf("%d", &bc);
  if(ac!=bf){
             printf("no es posible hacer la multiplicación, las columnas de la matriz A deben ser las mismas que las filas de la matriz B \n");
             system("pause");
             return 0;
  }


  struct tms t1,t2;
  clock_t start, end;
  clock_t astart, aend;
  double start_get, end_get;
  int** matriz_A;
  int** matriz_B;
  int** matriz_C;

  matriz_A = (int**) malloc(af*sizeof(int*));
  matriz_B = (int**) malloc(bf*sizeof(int*));
  matriz_C = (int**) malloc(af*sizeof(int*));

  for(i=0;i<af;i++){
	matriz_A[i] = (int*)malloc(ac*sizeof(int));
  }

  for(i=0;i<bf;i++){
	matriz_B[i] = (int*)malloc(bc*sizeof(int));
  }

  for(i=0;i<af;i++){ 
	matriz_C[i] = (int*) malloc(bc*sizeof(int));
  }

  /*for(i=0;i<af;i++){
      for(j=0;j<ac;j++){
          matriz_A[i][j]=1;
      }
  }
  printf("\n\n");
  for(i=0;i<bf;i++){
      for(j=0;j<bc;j++){
          matriz_B[i][j]=1;
      }
  }*/

  astart=times(&t1);
  start=clock();
  start_get=get_time();
#pragma omp parallel for num_threads(thread_count) private(i,j,k,sum) collapse(2)
  for(i=0;i<af;i++){
      for(j=0;j<bc;j++){
          sum=0;
          for(k=0;k<ac;k++){
              sum=(sum+(matriz_A[i][k]*matriz_B[k][j]));
          }
	matriz_C[i][j] = sum;
      }
  }
  aend=times(&t2);
  end=clock();
  end_get=get_time();
  printf ("salida(times())=%ld\n", (long)(aend-astart));
  printf("La funcion clock() da:%lf\n",((double)(end-start)/CLOCKS_PER_SEC));
  printf ("La función get_time:%lf segundos\n",(end_get-start_get));
}



