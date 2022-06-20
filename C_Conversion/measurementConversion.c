#include <stdio.h>

int main(){
    char colorIden[1000];
    int comX[1000];
    int comY[1000];
    int comMass[1000];
    int BBleft_x[1000];
    int BBtop_y[1000];
    int BBright_x[1000];
    int BBbottom_y[1000];

    FILE *fp = fopen("measurementData.txt", "r");
    if (fp == NULL) {
        perror("Unable to open file");
    }
    FILE *to = fopen("outputData.txt", "w");

    char buffer[1000];
    
    size_t lines = 0;

    while (fgets(buffer, sizeof buffer, fp) != NULL) {
        if (sscanf(buffer, "%s %x %x %x %x %x %x %x",
                   colorIden[lines], &comX[lines], &comY[lines], &comMass[lines], &BBleft_x[lines],&BBtop_y[lines], &BBright_x[lines], &BBbottom_y[lines]) == 8) {
            ++lines;

        }
    }

    for (size_t i = 0; i < lines; i++) {
        printf("%s,%d,%d,%d,%d,%d,%d,%d", colorIden[i], comX[i], comY[i], comMass[i], BBleft_x[i],BBtop_y[i], BBright_x[i], BBbottom_y[i]);
    }

    fclose(fp);
}