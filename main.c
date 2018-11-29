#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double valor_intermediario(const char *nome_arquivo) {
    int i, count=0;
    double num;
    double conjunto_entradas[536];
    int conjunto_pesos[536], deslocamento;
    double somatorio = 0.0;

    FILE *arquivo;  
    arquivo = fopen(nome_arquivo, "r");
    
    // lendo imagem e inserindo os valores no vetor de conjunto de entradas 
    while((fscanf(arquivo, "%lf", &num))!=EOF) {  
        conjunto_entradas[count] = num;
        count++;   
    }
    fclose(arquivo);

    srand(time(NULL));
    
    // inserindo valores aleatorios no vetor conjunto de pesos w[536]
    for(i = 0; i < 536; i++) {
        num = ((double)rand() / ((double)RAND_MAX + 1) * 33000) - 16500.0;
  
         conjunto_pesos[i] = (int)num;
    }
    // valor aleatorio de deslocamento b
    deslocamento = ((double)rand() / ((double)RAND_MAX + 1) * 33000) - 16500.0;
    
    for(i = 0; i < 536; i++) 
        somatorio += conjunto_entradas[i]*conjunto_pesos[i];
    
    // printf("deslocamento = %d\nsomatorio = %.6lf\n", deslocamento, somatorio);
    
    return somatorio + deslocamento;
}

double funcao_logistica(const char *nome_arquivo) {
    
    double n = valor_intermediario(nome_arquivo);
    const double e = 2.7182818284590452354;
    double fn = 1/(1 + pow(e, -n));
    return fn;
}


int main(int argc, char *argv[]) {
    // double n = valor_intermediario("./DataSet/VetoresAsfaltoNormalizados/asphalt_1.txt");
    // printf("n = %.6lf\n", n);
    char load_image[52]; 

    int num_image = 10; //numero da imagem carregada
    sprintf(load_image,"./DataSet/VetoresAsfaltoNormalizados/asphalt_%.2d.txt", num_image);

    // funcao de ativacao logistica
    double fn = funcao_logistica(load_image);
    
    printf("fn = %lf\n", fn);

    return 0;
}