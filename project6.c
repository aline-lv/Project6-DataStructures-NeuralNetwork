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
//double generate_n(const char *nome_arquivo);
double calculate_neuron(Neuron *neuron, double *input, int size_input);
double feed_forward(double *input, Neuron *neuron, NeurualNetwork *NN);
//void epoch();
//double get_error(); //error == neural network output

/*
 * MAIN FUNCTION
 */
int main(int argc, char *argv[]) {

  HL_NEURONS = arg_to_int(argv[1]);
  NeurualNetwork *NN = NULL;
  double inicial_input[MAX_IN];

  system("clear");
  printf("\n\n\t====================PROJETO 6====================\n\n");
  printf("\t-> Criando dinamicamente a rede neural:\n\n");

  NN = create_neural_network();

  printf("\n\tRede neural criada com sucesso!\n\n");
  //printf("HL_NUM_INT: %d\n", HL_NEURONS);
  //double output;
  //output = feed_forward(input);

  //First input into de NeurualNetwork
  //Randomize w and b
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

  double saida1;
  pass_file_to_vector("./DataSet/VetoresAsfaltoNormalizados/asphalt_20.txt", inicial_input);
  saida1 = feed_forward(inicial_input, NN->In_layer[0], NN);
  printf("\n\tSaída do 1º arquivo com 536 posições: %.2lf\n", saida1);

  //logística com feed_forward de todo mundo



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

  printf("\tCamada de entrada alocada com %d neurônios.\n", new->size_i);

  //allocate Hidden_layer
  Hidden_layer = (Neuron**)malloc(HL_NEURONS * sizeof(Neuron*));
  for (int j = 0; j < HL_NEURONS; j++)
      *(Hidden_layer + j) = create_neuron();

  printf("\tCamada oculta alocada com %d neurônios.\n", new->size_h);

  //allocate Output_layer
  Output_layer = create_neuron();
  printf("\tCamada de saída alocadacom %d neurônios.\n", new->size_o);

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

/*double generate_n(const char *nome_arquivo){
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
        num = ((double)rand() / ((double)RAND_MAX + 1) * 32500) - 16500.0;

         conjunto_pesos[i] = (int)num;
    }
    // valor aleatorio de deslocamento b
    deslocamento = ((double)rand() / ((double)RAND_MAX + 1) * 33000) - 16500.0;

    for(i = 0; i < 536; i++)
        somatorio += conjunto_entradas[i]*conjunto_pesos[i];

    // printf("deslocamento = %d\nsomatorio = %.6lf\n", deslocamento, somatorio);

    return somatorio + deslocamento;
}/*End-generate_n*/

double calculate_neuron(Neuron *neuron, double *input, int size_input){
  double sum_input_w;
  for(int i = 0; i < 536; i++)
      sum_input_w += input[i] * neuron->w[i];

  return sum_input_w + neuron->b;

}/*End-calculate_neuron*/

double feed_forward(double *input, Neuron *neuron, NeurualNetwork *NN){
  double n_i;

    return n_i = calculate_neuron(neuron, input, NN->size_i);



}/*End-feed_forward*/
