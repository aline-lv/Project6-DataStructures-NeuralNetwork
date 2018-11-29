#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

//int HL_NEURONS;

/*
 * FUNCTIONS' SIGNATURE
 */
//int myatoi(const char *str);
void shuffle(int *vet);
// Randomizar ordem de letiura das imagens.

int menorDecimal(int *dec, char *bin);
// Rotaciona vetor em 1 bit e retorna o menor decimal

void mountBinaryVector(int **matriz, int col, int lin, char *vetorBin);
// A função forma uma submatriz 3x3 a partir de um elemento central soma os 9 elementos
// e calcula a média, em seguida cria matriz char 3x3 de elementos 1 ou 0
// 1 : elementos >= média
// 0 : elementos < média.
// Em seguida monta-se o vetor de binários.

float energ(int **matriz, int linCol);
// Energia de cada direção da GLCM.

float contr(int **matriz, int linCol);
// Contraste para as direções da GLCM.

float homog(int **matriz, int linCol);
// Homogeneidade em cada direção da GLCM.

void GLCM(int **matriz, int col, int lin, float *metricas);
// Recebe a matriz do arquivo, calcula a ocorrência de vizinhos nas 8 direções, faz os calculos de contraste,energia
// e homogeneidade e vai salvando na respectiva ordem em um vetor.

float calcDistancia(float *referencia, float **comparador, int posicao);
// Faz a distância euclidiana entre o vetor de referência e o vetor que está sendo comparado

void normalizarVetores(float *ilbpGlcm, float *ilbpGlcmNormalizado);
// Normaliza vetores de dados ILBP e GLCM.
void salvarAsfaltoVetorNormalizado(float * ilbpGlcmNormalizado, int numero);
void salvarGramaVetorNormalizado(float * ilbpGlcmNormalizado, int numero);

/*
 * MAIN FUNCTION
 */
int main(int argc, char *argv[]) {

  //HL_NEURONS = myatoi(argv[1]);
  //printf("HL_NUM_INT: %d\n", HL_NEURONS);
	char mark, pv, vetorBin[9], nomeGrama[50], nomeAsfalto[50];
    int asfalto[51], grama[50]; // Tive que colocar 51 espaços no asfalto pois quando colocava 50 estava dando um bug que
                                // o último número sempre fosse um número completamente aleatório.
    int **matriz, *ilbp, decimal[9], i, j, z, cont, menor, lin, col;
    float *metricas, *ilbpGlcm, *ilbpGlcmNormalizado, distanciaGrama, distanciaAsfalto;
    float **resultadoGrama, **resultadoAsfalto, **resultadoAsfaltoTeste, **resultadoGramaTeste;
    float **resultadoGramaTreinamento, *mediaGrama, **resultadoAsfaltoTreinamento, *mediaAsfalto;
    float taxaAcerto = 0.0, falsaAceitacao = 0.0, falsaRejeicao = 0.0;

    // Alocação dinamica da matriz dos resultadoGramaTreinamento.
    resultadoGrama = (float**)malloc(50*sizeof(float *));
    for (i = 0; i < 50; i++)
    {
        *(resultadoGrama+i) = (float*)malloc(536*sizeof(float));
    }

    resultadoAsfalto = (float**)malloc(50*sizeof(float *));
    for (i = 0; i < 50; i++)
    {
        *(resultadoAsfalto+i) = (float*)malloc(536*sizeof(float));
    }

    resultadoGramaTreinamento = (float**)malloc(25*sizeof(float *));
    resultadoGramaTeste = (float**)malloc(25*sizeof(float *));
    for (i = 0; i < 25; i++)
    {
        *(resultadoGramaTreinamento+i) = (float*)malloc(536*sizeof(float));
        *(resultadoGramaTeste+i) = (float*)malloc(536*sizeof(float));
    }

    resultadoAsfaltoTreinamento = (float**)malloc(25*sizeof(float *));
    resultadoAsfaltoTeste = (float**)malloc(25*sizeof(float *));
    for (i = 0; i < 25; i++)
    {
        *(resultadoAsfaltoTreinamento+i) = (float*)malloc(536*sizeof(float));
        *(resultadoAsfaltoTeste+i) = (float*)malloc(536*sizeof(float));
    }

    mediaGrama = (float *) calloc(536, sizeof (float));

    mediaAsfalto = (float *) calloc(536, sizeof (float));

    shuffle(asfalto);

    // cont = posição do vetor de resultados do asfalto.
    cont = 0;

    for (z = 1; z <= 50; z++)
    {
    	FILE *fileAsphalt;
    	sprintf(nomeAsfalto, "DataSet/VetoresAsfaltoNormalizados/asphalt_%d.txt", asfalto[z]);
    	if(fileAsphalt = fopen(nomeAsfalto, "r")) {
    		printf("Arquivo %s carregado\n\n", nomeAsfalto);
    		fclose(fileAsphalt);
    	}
        else{
	        printf("Trabalhando na imagem de asfalto numero %d\n", z);
	        // Criar ponteiro de arquivo
	        

	        if (asfalto[z] < 10)
	        {
	            sprintf(nomeAsfalto, "DataSet/asphalt/asphalt_0%d.txt", asfalto[z]);
	        }
	        else
	        {
	            sprintf(nomeAsfalto, "DataSet/asphalt/asphalt_%d.txt", asfalto[z]);
	        }
	        printf("Arquivo: %s\n", nomeAsfalto);

	        fileAsphalt = fopen(nomeAsfalto, "r");

	        // Verificar existencia do arquivo
	        if (fileAsphalt == NULL)
	        {
	            printf("O seguinte arquivo nao existe: %s\n", nomeAsfalto);
	            exit(1);
	        }

	        lin = 0;
	        col = 1;

	        // Verificar qnt de linhas e colunas do arquivo
	        while ((mark = fgetc(fileAsphalt)) != EOF)
	        {
	            if (mark == '\n')
	            {
	                lin++;
	            }
	            else if (lin == 1 && mark == ';')
	            {
	                col++;
	            }
	        }

	        // Volta ao início do arquivo
	        rewind(fileAsphalt);

	        // Alocação dinamica da matriz do arquivo
	        matriz = (int**)malloc(lin*sizeof(int *));
	        for (i = 0; i < lin; i++)
	        {
	            *(matriz+i) = (int*)malloc(col*sizeof(int));
	        }

	        // Alocação dinamica do vetor ILBP
	        ilbp = (int *) calloc(512, sizeof (int));

	        // Alocação dinamica do vetor metricas
	        metricas = (float *) calloc(24, sizeof (float));

	        // Alocação dinamica do vetor ilbp+glcm
	        ilbpGlcm = (float *) calloc(536, sizeof (float));

	        // Alocação dinamica do vetor ilbp+glcm normalizado
	        ilbpGlcmNormalizado = (float *) calloc(536, sizeof (float));

	        // Passa elementos do arquivo para a matriz dinamica
	        for (i = 0; i < lin; i++)
	        {
	            for (j = 0; j < col; j++)
	            {
	                if (!feof(fileAsphalt))
	                {
	                    fscanf(fileAsphalt, "%d%c", *(matriz+i)+j, &pv);
	                }
	            }
	        }

	        // Cria vetor ILBP
	        for (i = 1; i < lin - 1; i++)
	        {
	            for (j = 1; j < col - 1; j++)
	            {
	                mountBinaryVector(matriz, j, i, vetorBin);
	                menor = menorDecimal(decimal, vetorBin);
	                ilbp[menor]++;
	            }
	        }

	        GLCM(matriz, col, lin, metricas);

	        // Concatenar ilbp + glcm,
	        for (i = 0; i < 512; i++)
	        {
	            *(ilbpGlcm + i) = *(ilbp + i);
	        }
	        for (i = 512; i < 536; i++)
	        {
	            *(ilbpGlcm + i) = *(metricas + (i - 512));
	        }

	        free(ilbp);
	        free(metricas);

	        // Normaliza vetor após ser concatenado
	        normalizarVetores(ilbpGlcm, ilbpGlcmNormalizado);

	        salvarAsfaltoVetorNormalizado(ilbpGlcmNormalizado, asfalto[z]);

	        free(ilbpGlcm);

	        // Salva vetor normalizado
	        for (i = 0; i < 536; i++)
	        {
	            *(*(resultadoAsfalto+cont)+i) = *(ilbpGlcmNormalizado + i);
	        }
	        cont++;
	        printf("Done\n\n");

	        for (i = 0; i < lin; i++)
	        {
	            free(*(matriz+i));
	        }
	        free(matriz);

	        free(ilbpGlcmNormalizado);

	        fclose(fileAsphalt);
	   	}
    }

    shuffle(grama);

    // cont = posição do vetor de resultados da grama.
    cont = 0;

    for (z = 1; z <= 50; z++)
    {	
    	FILE *fileGrass;
    	sprintf(nomeGrama, "DataSet/VetoresGramaNormalizados/grass_%d.txt", grama[z]);
    	if(fileGrass = fopen(nomeGrama, "r")){
    		printf("Arquivo %s carregado\n\n", nomeGrama);
    		fclose(fileGrass);
    	}
        else{
        	printf("Trabalhando na imagem de grama numero: %d\n", z);
                // Criar ponteiro de arquivo
                
        
                if (grama[z] <= 9)
                {
                    sprintf(nomeGrama, "DataSet/grass/grass_0%d.txt", grama[z]);
                }
                else
                {
                    sprintf(nomeGrama, "DataSet/grass/grass_%d.txt", grama[z]);
                }
                printf("Arquivo: %s\n", nomeGrama);
        
                // Abrir arquivo em modo de leitura apenas
                fileGrass = fopen(nomeGrama, "r");
        
                // Verificar existencia do arquivo
                if (fileGrass == NULL)
                {
                    printf("O seguinte arquivo nao existe: %s\n", nomeGrama);
                    exit(1);
                }
                lin = 0;
                col = 1;
        
                // Verificar qnt de linhas e colunas do arquivo
                while ((mark = fgetc(fileGrass)) != EOF)
                {
                    if (mark == '\n')
                    {
                        lin++;
                    }
                    else if (lin == 1 && mark == ';')
                    {
                        col++;
                    }
                }
        
                // Volta ao início do arquivo
                rewind(fileGrass);
        
                // Alocação dinamica da matriz do arquivo
                matriz = (int**)malloc(lin*sizeof(int *));
                for (i = 0; i < lin; i++)
                {
                    *(matriz+i) = (int*)malloc(col*sizeof(int));
                }
        
                // Alocação dinamica do vetor ILBP
                ilbp = (int *) calloc(512, sizeof (int));
        
                // Alocação dinamica do vetor metricas de 24
                metricas = (float *) calloc(24, sizeof (float));
        
                // AAlocação dinamica do vetor ilbp+glcm
                ilbpGlcm = (float *) calloc(536, sizeof (float));
        
                // Alocação dinamica do vetor ilbp+glcm normalizado
                ilbpGlcmNormalizado = (float *) calloc(536, sizeof (float));
        
                // Passa elementos do arquivo para a matriz dinamica
                for (i = 0; i < lin; i++)
                {
                    for (j = 0; j < col; j++)
                    {
                        if (!feof(fileGrass))
                        {
                            fscanf(fileGrass, "%d%c", *(matriz+i)+j, &pv);
                        }
                    }
                }
        
                // Cria vetor ILBP.
                for (i = 1; i < lin - 1; i++)
                {
                    for (j = 1; j < col - 1; j++)
                    {
                        mountBinaryVector(matriz, j, i, vetorBin);
                        menor = menorDecimal(decimal, vetorBin);
                        ilbp[menor]++;
                    }
                }
        
                GLCM(matriz, col, lin, metricas);
        
                // Concatenando ilbp + glcm,
                for (i = 0; i < 512; i++)
                {
                    *(ilbpGlcm + i) = *(ilbp + i);
                }
                for (i = 512; i < 536; i++)
                {
                    *(ilbpGlcm + i) = *(metricas + (i - 512));
                }
        
                free(ilbp);
                free(metricas);
        
                // Normaliza vetor após ser concatenado
                normalizarVetores(ilbpGlcm, ilbpGlcmNormalizado);
        
                salvarGramaVetorNormalizado(ilbpGlcmNormalizado, grama[z]);
        
                free(ilbpGlcm);
        
                // Salva vetor normalizado
                for (i = 0; i < 536; i++)
                {
                    *(*(resultadoGrama+cont)+i) = *(ilbpGlcmNormalizado + i);
                }
                cont++;
                printf("Done\n\n");
        
                for (i = 0; i < lin; i++)
                {
                    free(*(matriz+i));
                }
                free(matriz);
        
                free(ilbpGlcmNormalizado);
        
                fclose(fileGrass);
        }
    }

	for (i = 0; i < 50; i++)
	{
	    free(*(resultadoGrama+i));
	    free(*(resultadoAsfalto+i));
	}
	free(resultadoGrama);
	free(resultadoAsfalto);
	for (i = 0; i < 25; i++)
	{
	    free(*(resultadoGramaTreinamento+i));
	    free(*(resultadoAsfaltoTreinamento+i));
	    free(*(resultadoGramaTeste+i));
	    free(*(resultadoAsfaltoTeste+i));
	}
	free(resultadoGramaTreinamento);
	free(resultadoGramaTeste);
	free(resultadoAsfaltoTeste);
	free(resultadoAsfaltoTreinamento);
	free(mediaAsfalto);
	free(mediaGrama);

	return 0;
}

/*
 * FUNCTIONS' SCOPE
 */
/*int myatoi(const char *str){
    int i, res = 0;
    for (i = 0; str[i] != '\0'; ++i) {
        res = res * 10 + str[i] - '0';
    }
    return res;
}*/

void shuffle(int *vect)
{

    int i;

    srand((unsigned)time(0)*100);

    for (i = 1; i <= 50; i++)
    {
        vect[i] = i;
    }

    for (i = 1; i <= 50; i++)
    {
        int tmp = vect[i];
        int aleatorio = rand() % 50;
        if (aleatorio != 0)
        {
            vect[i] = vect[aleatorio];
            vect[aleatorio] = tmp;
        }
    }
}

int menorDecimal(int *dec, char *bin)
{
    int decimal, i, k, c, j, n = 9, m = 0, menor = 512;
    char tmp;

    for (k = 9; k > 0; k--)
    {
        // Transforma em decimal.
        decimal = 0;
        j = 0;
        for (i = 8; i >= 0; i--)
        {
            if (*(bin + i) == '1')
            {
                decimal += pow(2, j);
            }
            j++;
        }

        // Rotacionar vetor a 1 bit.
        tmp = bin[n - 1];
        for (c = n-1; c > 0; c--)
        {
            bin[c] = bin[c - 1];
        }
        bin[0] = tmp;

        dec[m] = decimal;
        m++;
    }

    // Descobre qual o menor valor decimal
    for (int i = 0; i <= 8; i++)
    {
        if (menor > dec[i])
        {
            menor = dec[i];
        }
    }

    return menor;
}

void mountBinaryVector(int **matriz, int col, int lin, char *vetorBin)
{
    float soma = 0, media;
    int i, j, x = 0, y = 0;
    char **bin;

    // Alocação dinâmica da submatriz 3x3 de binários.
    bin = (char**)malloc(3*sizeof(char *));
    for (i = 0; i < 3; i++)
    {
        *(bin + i) = (char*)malloc(3*sizeof(char));
    }

    // Soma da submatriz[3][3].
    for (i = lin - 1; i <= lin + 1; i++)
    {
        for (j = col - 1; j <= col + 1; j++)
        {
            soma += *(*(matriz+i)+j);
        }
    }

    media = soma / 9.0;

    // Se: elemento >= média, elemento = 1
    // Se: elemento < média, elemento = 0
    for (i = lin - 1; i <= lin + 1; i++)
    {
        for (j = col - 1; j <= col + 1; j++)
        {
            if (*(*(matriz+i)+j) < media)
            {
                *(*(bin+x)+y) = '0';
                y++;
            }
            else if (*(*(matriz+i)+j) >= media)
            {
                *(*(bin+x)+y) = '1';
                y++;
            }
        }
        y = 0;
        x++;
    }

    // Salva os elementos em vetorBin na ordem definida pelo ILBP.
    vetorBin[0] = *(*(bin+0)+0);
    vetorBin[1] = *(*(bin+0)+1);
    vetorBin[2] = *(*(bin+0)+2);
    vetorBin[3] = *(*(bin+1)+2);
    vetorBin[4] = *(*(bin+2)+2);
    vetorBin[5] = *(*(bin+2)+1);
    vetorBin[6] = *(*(bin+2)+0);
    vetorBin[7] = *(*(bin+1)+0);
    vetorBin[8] = *(*(bin+1)+1);

    for (i = 0; i < 3; i++)
    {
        free(*(bin+i));
    }
    free(bin);
}

float energ(int **matriz, int linCol)
{

    int i, j;
    float energ = 0;

    for (i=0; i<linCol; i++)
    {
        for (j=0; j<linCol; j++)
        {
            energ += pow(matriz[i][j],2);
        }
    }

    return energ;
}

float contr(int **matriz, int linCol)
{

    int i, j;
    float contr = 0;

    for (i=0; i<linCol; i++)
    {
        for (j=0; j<linCol; j++)
        {
            contr += pow(i-j,2)*matriz[i][j];
        }
    }

    return contr;
}


float homog(int **matriz, int linCol)
{

    int i, j, z;
    float homog = 0;

    for (i=0; i<linCol; i++)
    {
        for (j=0; j<linCol; j++)
        {
            z = i-j;
            if (z < 0)
            {
                z = z*(-1);
            }
            homog += matriz[i][j] / (1 + z);
        }
    }

    return homog;
}


void GLCM(int **matriz, int col, int lin, float *metricas)
{
    int **glcm, i, j, m, glcmLinCol = 512;

    for (m = 0; m < 8; m++)
    {
        // Aloca matriz para salvar o GLCM de cada cordenada.
        glcm = (int**)calloc(glcmLinCol, sizeof(int *));
        for (i = 0; i < glcmLinCol; i++)
        {
            *(glcm+i) = (int*)calloc(glcmLinCol, sizeof(int));
        }

        if (m == 0)
        {
            // Incremento da posição [elemento][elemento leste] da matriz glcm.
            for (i = 0; i < lin; i++)
            {
                for (j = 0; j < col - 1; j++)
                {
                    glcm[matriz[i][j]][matriz[i][j+1]]++;
                }
            }
            *(metricas + 0) = contr(glcm, glcmLinCol);
            *(metricas + 1) = energ(glcm, glcmLinCol);
            *(metricas + 2) = homog(glcm, glcmLinCol);
        }
        else if (m == 1)
        {
            // Incremento da posição [elemento][elemento oeste] da matriz glcm.
            for (i = 0; i < lin; i++)
            {
                for (j = 1; j < col; j++)
                {
                    glcm[matriz[i][j]][matriz[i][j-1]]++;
                }
            }
            *(metricas + 3) = contr(glcm, glcmLinCol);
            *(metricas + 4) = energ(glcm, glcmLinCol);
            *(metricas + 5) = homog(glcm, glcmLinCol);
        }
        else if (m == 2)
        {

            // Incremento da posição [elemento][elemento norte] da matriz glcm.
            for (i = 1; i < lin; i++)
            {
                for (j = 0; j < col; j++)
                {
                    glcm[matriz[i][j]][matriz[i-1][j]]++;
                }
            }
            *(metricas + 6) = contr(glcm, glcmLinCol);
            *(metricas + 7) = energ(glcm, glcmLinCol);
            *(metricas + 8) = homog(glcm, glcmLinCol);
        }
        else if (m == 3)
        {

            // Incremento da posição [elemento][elemento sul] da matriz glcm.
            for (i = 0; i < lin - 1; i++)
            {
                for (j = 0; j < col; j++)
                {
                    glcm[matriz[i][j]][matriz[i+1][j]]++;
                }
            }
            *(metricas + 9) = contr(glcm, glcmLinCol);
            *(metricas + 10) = energ(glcm, glcmLinCol);
            *(metricas + 11) = homog(glcm, glcmLinCol);
        }
        else if (m == 4)
        {

            // Incremento da posição [elemento][elemento noroeste] da matriz glcm.
            for (i = 1; i < lin; i++)
            {
                for (j = 1; j < col; j++)
                {
                    glcm[matriz[i][j]][matriz[i-1][j-1]]++;
                }
            }
            *(metricas + 12) = contr(glcm, glcmLinCol);
            *(metricas + 13) = energ(glcm, glcmLinCol);
            *(metricas + 14) = homog(glcm, glcmLinCol);
        }
        else if (m == 5)
        {

            // Incremento da posição [elemento][elemento sudeste] da matriz glcm.
            for (i = 1; i < lin; i++)
            {
                for (j = 0; j < col - 1; j++)
                {
                    glcm[matriz[i][j]][matriz[i-1][j+1]]++;
                }
            }
            *(metricas + 15) = contr(glcm, glcmLinCol);
            *(metricas + 16) = energ(glcm, glcmLinCol);
            *(metricas + 17) = homog(glcm, glcmLinCol);
        }
        else if (m == 6)
        {

            // Incremento da posição [elemento][elemento sudoeste] da matriz glcm.
            for (i = 0; i < lin - 1; i++)
            {
                for (j = 1; j < col; j++)
                {
                    glcm[matriz[i][j]][matriz[i+1][j-1]]++;
                }
            }
            *(metricas + 18) = contr(glcm, glcmLinCol);
            *(metricas + 19) = energ(glcm, glcmLinCol);
            *(metricas + 20) = homog(glcm, glcmLinCol);
        }
        else if (m == 7)
        {

            // Incremento da posição [elemento][elemento sudeste] da matriz glcm.
            for (i = 0; i < lin - 1; i++)
            {
                for (j = 0; j < col - 1; j++)
                {
                    glcm[matriz[i][j]][matriz[i+1][j+1]]++;
                }
            }
            *(metricas + 21) = contr(glcm, glcmLinCol);
            *(metricas + 22) = energ(glcm, glcmLinCol);
            *(metricas + 23) = homog(glcm, glcmLinCol);
        }


        for (i = 0; i < glcmLinCol; i++)
        {
            free(*(glcm+i));
        }
        free(glcm);

    }
}

float calcDistancia(float *referencia, float **comparador, int posicao)
{
    int i, j;
    float resultado = 0.0, elementoVetorReferencia, elementoVetorComparador, diferenca;

    // Calcula a distância entre os vetores
    for (j = 0; j < 536; j++)
    {
        elementoVetorReferencia = *(referencia + j);
        elementoVetorComparador = *(*(comparador+posicao)+j);
        diferenca = elementoVetorReferencia - elementoVetorComparador;
        resultado += pow(diferenca, 2);
    }
    resultado = sqrt(resultado);

    return resultado;
}

void normalizarVetores(float *ilbpGlcm, float *ilbpGlcmNormalizado)
{
    int i;
    float maior = 0, menor = 99999999;

    // Descobre o menor elemento e o maior elemento do vetor ilbpGlcm
    for (i = 0; i < 536; i++)
    {
        if (*(ilbpGlcm + i) > maior)
        {
            maior = *(ilbpGlcm + i);
        }
        if (menor > *(ilbpGlcm + i))
        {
            menor = *(ilbpGlcm + i);
        }
    }

    // Calcula e guarda os valores normalizados
    for (i = 0; i < 536; i++)
    {
        *(ilbpGlcmNormalizado + i) = (*(ilbpGlcm + i) - menor) / (maior - menor);
        if (*(ilbpGlcmNormalizado + i) < 0)
        {
            *(ilbpGlcmNormalizado + i) = *(ilbpGlcmNormalizado + i) * (-1);
        }
    }
}

void salvarAsfaltoVetorNormalizado(float * ilbpGlcmNormalizado, int numero){
	FILE *arquivo;
	char nomeAsfalto[50];

    sprintf(nomeAsfalto, "DataSet/VetoresAsfaltoNormalizados/asphalt_%d.txt", numero);

    printf("Salvando arquivo: %s\n", nomeAsfalto);

    // Abrir arquivo em modo de leitura apenas
    arquivo = fopen(nomeAsfalto, "w");

    int i;
    for (i = 0; i < 536; i++)
    {
        fprintf(arquivo, "%f\n", *(ilbpGlcmNormalizado + i));
        //printf("%f\n", *(ilbpGlcmNormalizado+i));
    }
    fclose(arquivo);

}

void salvarGramaVetorNormalizado(float * ilbpGlcmNormalizado, int numero){
	FILE *arquivo;
	char nomeGrama[50];

    
    sprintf(nomeGrama, "DataSet/VetoresGramaNormalizados/grass_%d.txt", numero);
    
    printf("Salvando arquivo: %s\n", nomeGrama);

    // Abrir arquivo em modo de leitura apenas
    arquivo = fopen(nomeGrama, "w");

    int i;
    for (i = 0; i < 536; i++)
    {
        fprintf(arquivo, "%f\n", *(ilbpGlcmNormalizado + i));
        //printf("%f\n", *(ilbpGlcmNormalizado+i));
    }
    fclose(arquivo);
}
