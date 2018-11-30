/*
 * O QUE FAZER:
 * - Ter uma NeuralNet, e passar os vetores com as qtds de cada camada, a saida dela é o erro;
 * - Pegar as saidas da C_IN, montar um vetor de entradas para a C_H;
 * - Pegar as saidas da C_H,  montar um vetor de entradas para a C_O e obter o erro em double;
 * - Manda o erro para o back_propa e para um vetor[25] - qdo chegar em 25 manda esse vetor para o MSE();
 * -
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

int HL_NEURONS;
#define MAX_IN 536
#define OUTPUT_NEURONS 1
#define ASPHALT 0 // < 0.5 is asphalt
#define GRASS 1 // >= 0.5 is grass

/*
 * STRUCT
 */
typedef struct neuron{
  double *p;
  double *w;
  double b;
  double logistic_result;
}Neuron;

typedef struct neurualNetwork{
  int size_i, size_h, size_o;
  Neuron **In_layer;
  Neuron **Hidden_layer;
  Neuron *Output_layer;
  //double W_in[MAX_IN][MAX_IN], W_hidden[HL_NEURONS][MAX_IN], W_output[HL_NEURONS];
  double W_in[MAX_IN][MAX_IN], **W_hidden, *W_output;
  double B_in[MAX_IN], *B_hidden, B_output;
  double input[MAX_IN];
}NeurualNetwork;

/*
 * FUNCTIONS' SIGNATURE
 */
int arg_to_int(const char *str);
Neuron* create_neuron();
NeurualNetwork* create_neural_network();
void randomize_w(int quantity, double *w, Neuron *neuron);
double randomize_b();
void pass_file_to_vector(char *file_name, double *inputs_file);

void training_vector(int *numeros, int quantNumeros, int limite);
void test_vector(int *numeros_trein, int *numeros_test);
int exist(int *valores, int tam, int valor);
void randomize_images(int *asphalt_training, int *grass_training, int *asphalt_test, int *grass_test);

double calculate_neuron(Neuron *neuron, double *input, int size_input);
double feed_forward(double *input, NeurualNetwork *NN);
void network_training(double *input, NeurualNetwork *NN);
double mean_square_error(double *error_vector);
void back_propagation(double error, NeurualNetwork *NN);

/*
 * MAIN FUNCTION
 */
int main(int argc, char *argv[]) {

  HL_NEURONS = arg_to_int(argv[1]);
  NeurualNetwork *NN = NULL;
  double inicial_input[MAX_IN];

  system("clear");
  printf("\n\n\t====================PROJETO 6====================\n\n");
  printf("\t-> Criando a rede neural dinamicamente:\n\n");

  NN = create_neural_network();

  printf("\n\tRede neural criada com sucesso!\n\n");
  //printf("HL_NUM_INT: %d\n", HL_NEURONS);
  //double output;
  //output = feed_forward(input);

  /*printf("\nW_output:\n");

  for(int j = 0; j < MAX_IN; j++){
    printf("%.2lf ", NN->W_output[j]);
  }
  printf("\n");

  printf("\nB_hidden:\n");

  for(int j = 0; j < HL_NEURONS; j++){
    printf("%.2lf ", NN->B_hidden[j]);
  }
  printf("\n");*/

  /*double erro1;
  pass_file_to_vector("./DataSet/VetoresAsfaltoNormalizados/asphalt_02.txt", inicial_input);
  erro1 = feed_forward(inicial_input, NN->In_layer[0], NN);
  printf("\n\tSaída do 1º arquivo com 536 posições: %.2lf\n", erro1);*/

  //logística com feed_forward de todo mundo
  network_training(inicial_input, NN);



  return 0;
}/*End-main*/


/*
 * FUNCTIONS' SCOPE
 */
int arg_to_int(const char *str){
    int i, res = 0;
    for (i = 0; str[i] != '\0'; ++i){
        res = res * 10 + str[i] - '0';
    }
    return res;
}/*End-arg_to_int*/

Neuron* create_neuron(){
  Neuron *new = NULL;

  new = (Neuron*) malloc(sizeof(Neuron));

  if (new == NULL)
		exit(-10);

 new->p = NULL;
 new->w = NULL;

 return new;

}/*End-create_neuron*/

NeurualNetwork* create_neural_network(){
  NeurualNetwork *new = NULL;
  Neuron **In_layer = NULL, **Hidden_layer = NULL, *Output_layer = NULL;
  double **W_hidden, *W_output, *B_hidden;

  new = (NeurualNetwork*) malloc(sizeof(NeurualNetwork));

  if (new == NULL)
		exit(-10);

  new->size_i = MAX_IN;
  new->size_h = HL_NEURONS;
  new->size_o = 1;

  //allocate In_layer
  In_layer = (Neuron**)malloc(MAX_IN * sizeof(Neuron*));
  for (int j = 0; j < MAX_IN; j++)
      *(In_layer + j) = create_neuron();

  printf("\t[1] Camada de entrada alocada com %d neurônios.\n", new->size_i);

  //allocate Hidden_layer
  Hidden_layer = (Neuron**)malloc(HL_NEURONS * sizeof(Neuron*));
  for (int j = 0; j < HL_NEURONS; j++)
      *(Hidden_layer + j) = create_neuron();

  printf("\t[2] Camada oculta alocada com %d neurônios.\n", new->size_h);

  //allocate Output_layer
  Output_layer = create_neuron();
  printf("\t[3] Camada de saída alocada com %d neurônios.\n", new->size_o);

  //allocate W_hidden
  W_hidden = (double**)malloc(HL_NEURONS * sizeof(double*));
  for (int j = 0; j < HL_NEURONS; j++)
      *(W_hidden + j) = (double*)malloc(MAX_IN * sizeof(double));

  //allocate W_output
  W_output = (double*)malloc(HL_NEURONS * sizeof(double));

  //allocate B_hidden
  B_hidden = (double*)malloc(HL_NEURONS * sizeof(double));

  new->In_layer = In_layer;
  new->Hidden_layer = Hidden_layer;
  new->Output_layer = Output_layer;
  new->W_hidden = W_hidden;
  new->W_output = W_output;
  new->B_hidden = B_hidden;

  return new;
}/*End-create_neural_network*/

void randomize_w(int quantity, double *w, Neuron *neuron){
  double num;
  srand(time(NULL));

  if(neuron->w == NULL){
    neuron->w = (double*) malloc(quantity * sizeof(double));

    if(neuron->w == NULL)
      exit(-20);
  }

  for(int i = 0; i < quantity; i++){
      num = ((double)rand() / ((double)RAND_MAX + 1) * 32500) - 16500.0;
      *(w + i) = num;
      *(neuron->w + i) = num;
  }

}/*End-randomize_w*/

double randomize_b(){
  srand(time(NULL));

  return ((double)rand() / ((double)RAND_MAX + 1) * 33000) - 16500.0;
}/*End-randomize_b*/

void pass_file_to_vector(char *file_name, double *inputs_file){
  FILE *arquivo;
  int count = 0;
  double num;

  arquivo = fopen(file_name, "r");

  // lendo imagem e inserindo os valores no vetor de conjunto de entradas
  while((fscanf(arquivo, "%lf", &num))!=EOF) {
      inputs_file[count] = num;
      count++;
  }
  fclose(arquivo);
}/*End-pass_file_to_vector*/

void training_vector(int *numeros, int quantNumeros, int limite){
    int v;
    for(int i=1; i<=quantNumeros; i++){
        v = 1+(rand() % limite);
        while(exist(numeros,i,v)){
            v = 1+(rand() % limite);
        }
        numeros[i] = v;
    }
}/*End-training_vector*/

void test_vector(int *numeros_trein, int *numeros_test) {
	int i, count=0;
	for(i=1; i<=50; i++) {
        if(!exist(numeros_trein, 25, i)) {
        	count+=1;
        	numeros_test[count] = i;
        }
	}
}/*End-test_vector*/

int exist(int *valores, int tam, int valor){
    for(int i=1; i<=tam; i++){
        if(valores[i]==valor)
            return 1;
    }
    return 0;
}/*End-exist*/

void randomize_images(int *asphalt_training, int *grass_training, int *asphalt_test, int *grass_test){
  //int asphalt_training[25], grass_training[25];
  //int asphalt_test[25], grass_test[25];

  srand(time(NULL));

  training_vector(asphalt_training, 25, 50);
  test_vector(asphalt_training, asphalt_test);

  training_vector(grass_training, 25, 50);
  test_vector(grass_training, grass_test);

  /*printf("\n\n\tTREINAMENTO       TESTE\n\n");
  for(int i=1; i<=25; i++)
    printf("\taspalt_%.2d.txt   aspalt_%.2d.txt \n", asphalt_training[i], asphalt_test[i]);
  printf("\n");
  for(int i=1; i<=25; i++)
    printf("\tgrass_%.2d.txt   grass_%.2d.txt \n", grass_training[i], grass_test[i]);*/

  printf("\t\tImagens escolhidas!\n");
}/*End-randomize_images*/

double calculate_neuron(Neuron *neuron, double *input, int size_input){
  double sum_input_w, logistics_function, n;

  for(int i = 0; i < size_input; i++)
      sum_input_w += input[i] * neuron->w[i];

  n = sum_input_w + neuron->b;
  logistics_function = 1 / (1 + exp(-n));

  return logistics_function;

}/*End-calculate_neuron*/

double feed_forward(double *input, NeurualNetwork *NN){
  double input_hidden[NN->size_i], input_o[NN->size_h];

  //In layer
  for(int i = 0; i < NN->size_i; i++){
    NN->In_layer[i]->logistic_result = calculate_neuron(NN->In_layer[i], input, NN->size_i);
    input_hidden[i] = NN->In_layer[i]->logistic_result;
  }

  //Hidden layer
  for(int i = 0; i < NN->size_h; i++){
    NN->Hidden_layer[i]->logistic_result = calculate_neuron(NN->Hidden_layer[i], input_hidden, NN->size_h);
    input_o[i] = NN->Hidden_layer[i]->logistic_result;
  }

  //Hidden layer
  NN->Output_layer->logistic_result = calculate_neuron(NN->Output_layer, input_o, NN->size_h);

  return NN->Output_layer->logistic_result;

}/*End-feed_forward*/

void network_training(double *input, NeurualNetwork *NN){
  int asphalt_training[25], grass_training[25];
  int asphalt_test[25], grass_test[25];
  int epoch = 0, cycle_asphalt = 0, cycle_grass = 0;
  char path_file_asphalt[1000], path_file_grass[1000], path1[500], path2[500];
  double tmp_error, asphalt_error, grass_error, mse, error_vector_a[25], error_vector_g[25];

  printf("\t-> Treinando a rede neural:\n\n");

  printf("\t[1] Escolhendo as imagens de grama e asfalto aleatoriamente...\n");
  randomize_images(asphalt_training, grass_training, asphalt_test, grass_test);

  /*Randomizes w and b to the 1º input into de NeurualNetwork*/
    for(int i = 0; i < NN->size_i; i++){ //W_in
      randomize_w(NN->size_i, NN->W_in[i], NN->In_layer[i]);
    }

    for(int i = 0; i < NN->size_i; i++){ //B_in
      srand(time(NULL));
      NN->B_in[i] = randomize_b();
      NN->In_layer[i]->b = NN->B_in[i];
    }

    for(int i = 0; i < NN->size_h; i++){ //W_hidden
      randomize_w(NN->size_i, NN->W_hidden[i], NN->Hidden_layer[i]);
    }

    for(int i = 0; i < NN->size_h; i++){ //B_hidden
      srand(time(NULL));
      NN->B_hidden[i] = randomize_b();
      NN->Hidden_layer[i]->b = NN->B_hidden[i];
    }

    randomize_w(NN->size_h, NN->W_output, NN->Output_layer); //W_output
    srand(time(NULL));
    NN->B_output = randomize_b(); //B_output
    NN->Output_layer->b = NN->B_output;
  /*End-Randomizes w and b to the 1º input*/

  printf("\n\t[2] Treinando a rede...\n");
  for(int i = 1; i <=50; i++){
    if(i <= 25){//Grass training loop

      strcpy(path_file_grass, "./DataSet/VetoresGramaNormalizados/grass_");
      sprintf(path2, "%d.txt", grass_training[i - 0]);
      strcat(path_file_grass, path2);

      pass_file_to_vector(path_file_grass, input);

      tmp_error = feed_forward(input, NN);
      grass_error = GRASS - tmp_error;
      //grass_error = 1.00;
      error_vector_g[i - 1] = grass_error;
      back_propagation(grass_error, NN);

      //printf("\t\t%.2dº arquivo (error): %.5lf\n", i, grass_error);
    }else{ //Asphalt training loop
      strcpy(path_file_asphalt, "./DataSet/VetoresAsfaltoNormalizados/asphalt_");
      sprintf(path1, "%.2d.txt", asphalt_training[i - 25]);
      strcat(path_file_asphalt, path1);
      pass_file_to_vector(path_file_asphalt, input);

      tmp_error = feed_forward(input, NN);
      //asphalt_error = tmp_error;
      asphalt_error = 0.00;
      error_vector_a[i - 25] = asphalt_error;

      back_propagation(asphalt_error, NN);
      //printf("\t\t%.2dº arquivo (error): %.5lf\n", i, asphalt_error);
    }/*End-if*/

    if(i == 25 || i == 50){
      epoch++;

      if(i==25)
        mse = mean_square_error(error_vector_g);
      else
        mse = mean_square_error(error_vector_a);

      if(mse >= 0.2){
          if(i == 50)
           i = 0;
      }else{
        printf("\t\tMSE foi menor do que 0.2! Saindo do treinamento!\n\t\tTotal de épocas ocorridas: %.2d\n", epoch);
        break;
      }
    }/*End-if*/

    if(epoch == 1000){
      printf("\t\tTreinamento da rede neural chegou a 1000 épocas! Saindo do treinamento!\n");
      break;
    }

  }/*End-for*/

  printf("\n\tFim do treinamento!\n");
}/*End-network_training*/

double mean_square_error(double *error_vector){
  double sum_error = 0;
  for(int i = 0; i < 25; i++){
    sum_error+= (error_vector[i] * error_vector[i]);
  }
  return sum_error / 25;

}/*End-mean_square_error*/

void back_propagation(double error, NeurualNetwork *NN){

}/*End-back_propagation*/
