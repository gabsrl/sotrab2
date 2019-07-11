/*Aproximação do matemático francês François Viète para calcular o número Pi*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h>

    int
    main(int argc, char **argv) {

    struct timeval tv1, tv2;
    struct timezone tz;
    omp_set_num_threads(omp_get_num_procs());
    double tempoLevadoNormal = 0, tempoLevadoParalelo = 0;
    int qntMulti;
    int index;

	unsigned long int n;
	double r;
    double pi;
	int i, j;
 
        if (argc != 3) {
		printf("Uso: <nome_seu_programa> <numero de iteracoes agoritmo> <numero de execucoes> \n");
		exit (1);
	}
 
	n = strtol(argv[1], NULL, 10);
	if (!n) {
		printf("ERROR: Numero de iteracoes invalido!\n");
		exit (2);
	}

    qntMulti = atoi(argv[2]);
    if(!qntMulti) {
        printf("ERROR: Numero de execucoes invalido!\n");
        exit(3);
    }

    for(index = 0; index < qntMulti; index++) {
        printf("Execucao %d: \n", index+1); 
        pi = 1;
        gettimeofday(&tv1, &tz); // pega o tempo atual baseado no fuso horário
        /*Cálculo de PI NORMAL.*/
        for (i = 0; i < n; i++) {
            r = 2;
            for (j = i; j; j--) r = 2 + sqrt(r);
            r = sqrt(r);
            pi *= (r / 2);
        }
        pi = 2 / pi;

        gettimeofday(&tv2, &tz); // pega o tempo após acabar o cálculo do Pi.
        tempoLevadoNormal += (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
        printf("- Normal: \n");
        printf("    - Tempo Levado Atual = %lf segundos \n", (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6);
        printf("    - Tempo Levado Cumulativo = %lf segundos \n", tempoLevadoNormal);
        printf("    - Valor aproximado de PI = %1.16f \n", pi);

        /* Pi Paralelo 
            - Privadas: i e j (cada thread tem a sua)
            - Compartilhadas: pi
        */
        pi = 1;
        gettimeofday(&tv1, &tz);
        #pragma omp parallel for private(i, j, r) shared(pi)
        for (i = 0; i < n; i++) {
            r = 2;
            for (j = i; j; j--)
                r = 2 + sqrt(r);
            r = sqrt(r);

            pi *= (r / 2);
        }
        pi = 2 / pi;

        gettimeofday(&tv2, &tz);
        tempoLevadoParalelo += (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
        printf("- Paralelo: \n");
        printf("    - Tempo Levado Atual = %lf segundos \n", (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6);
        printf("    - Tempo Levado Cumulativo = %lf segundos \n", tempoLevadoParalelo);
        printf("    - Valor aproximado de PI = %1.16f \n", pi);
    }
    if(tempoLevadoNormal > tempoLevadoParalelo) {
        printf("Normal foi %lf vezes mais devagar que o Paralelo\n", (tempoLevadoNormal / qntMulti) / (tempoLevadoParalelo / qntMulti));
    }
    else {
        printf("Paralelo foi %lf vezes mais devagar que o Normal\n", (tempoLevadoParalelo / qntMulti) / (tempoLevadoNormal / qntMulti));
    }
    return 0;
}