#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int HL_NEURONS;

/*
 * FUNCTIONS' SIGNATURE
 */
int myatoi(const char *str);


/*
 * MAIN FUNCTION
 */
int main(int argc, char *argv[]) {

  HL_NEURONS = myatoi(argv[1]);
  //printf("HL_NUM_INT: %d\n", HL_NEURONS);

  return 0;
}

/*
 * FUNCTIONS' SCOPE
 */
int myatoi(const char *str){
    int i, res = 0;
    for (i = 0; str[i] != '\0'; ++i) {
        res = res * 10 + str[i] - '0';
    }
    return res;
}
