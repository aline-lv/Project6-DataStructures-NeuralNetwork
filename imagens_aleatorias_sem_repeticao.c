#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void vetor_treinameto(int *numeros, int quantNumeros, int limite);
void vetor_teste(int *numeros_trein, int *numeros_test);
int existe(int *valores, int tam, int valor);

int main() {

    int aspalt_treinamento[25], grass_treinamento[25];
    int aspalt_teste[25], grass_teste[25];
 	
	srand(time(NULL));
	
    vetor_treinameto(aspalt_treinamento, 25, 50);
	vetor_teste(aspalt_treinamento, aspalt_teste);

	vetor_treinameto(grass_treinamento, 25, 50);
	vetor_teste(grass_treinamento, grass_teste);

    printf("\nTREINAMENTO       TESTE\n\n");

    for(int i=1; i<=25; i++)
    	printf("aspalt_%.2d.txt   aspalt_%.2d.txt \n", aspalt_treinamento[i], aspalt_teste[i]);

    printf("\n");
	
	for(int i=1; i<=25; i++) 
    	printf("grass_%.2d.txt   grass_%.2d.txt \n", grass_treinamento[i], grass_teste[i]);
        
    return 0;
}

void vetor_treinameto(int *numeros, int quantNumeros, int limite){
    int v;
    for(int i=1; i<=quantNumeros; i++){
        v = 1+(rand() % limite);
        while(existe(numeros,i,v)){
            v = 1+(rand() % limite);
        }
        numeros[i] = v;
    }	
}

void vetor_teste(int *numeros_trein, int *numeros_test) {
	int i, count=0;
	for(i=1; i<=50; i++) {
        if(!existe(numeros_trein, 25, i)) {
        	count+=1;
        	numeros_test[count] = i;
        }
	}		
}

int existe(int *valores, int tam, int valor){
    for(int i=1; i<=tam; i++){
        if(valores[i]==valor)
            return 1;
    }
    return 0;
}