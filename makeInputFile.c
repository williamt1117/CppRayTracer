#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    FILE* fptr = fopen("testLots.txt", "w");
    fprintf(fptr, "NEAR 1\nLEFT -1\nRIGHT 1\nBOTTOM -1\nTOP 1\nRES 600 600\nOUTPUT testLots.ppm\n");

    srand(time(NULL));

    for (int i = 0; i < atoi(argv[1]); i++) {
        float posX, posY, posZ, sclX, sclY, sclZ, colR, colG, colB, amb, dif, spec, ref;
        int n;
        posX = (float)rand() / RAND_MAX * 20 - 10; //-10 to 10
        posY = (float)rand() / RAND_MAX * 20 - 10; //-10 to 10
        posZ = (float)rand() / RAND_MAX * 20 - 30; //-10 to -30
        sclX = (float)rand() / RAND_MAX * 4.8 + 0.2; //0.2 to 5
        sclY = (float)rand() / RAND_MAX * 4.8 + 0.2; //0.2 to 5
        sclZ = (float)rand() / RAND_MAX * 4.8 + 0.2; //0.2 to 5
        colR = (float)rand() / RAND_MAX * 0.8 + 0.2; //0.2 to 1
        colG = (float)rand() / RAND_MAX * 0.8 + 0.2; //0.2 to 1
        colB = (float)rand() / RAND_MAX * 0.8 + 0.2; //0.2 to 1
        amb = (float)rand() / RAND_MAX * 0.8 + 0.2; //0.2 to 1
        dif = (float)rand() / RAND_MAX * 0.8 + 0.2; //0.2 to 1
        spec = (float)rand() / RAND_MAX * 0.8 + 0.2; //0.2 to 1
        n = (int)((float)rand() / RAND_MAX * 999 + 1); //1 to 1000

        fprintf(fptr, "SPHERE emptyname %f %f %f %f %f %f %f %f %f %f %f %f %f %d\n",
            posX, posY, posZ, sclX, sclY, sclZ, colR, colG, colB, amb, dif, spec, ref, n);
    }

    for (int i = 0; i < atoi(argv[2]); i++) {
        float posX, posY, posZ, iR, iG, iB;
        posX = (float)rand() / RAND_MAX * 20 - 10; //-10 to 10
        posY = (float)rand() / RAND_MAX * 20 - 10; //-10 to 10
        posZ = (float)rand() / RAND_MAX * 80 - 50; //-0 to -50
        iR = (float)rand() / RAND_MAX * 0.8 + 0.2; //0.2 to 1
        iG = (float)rand() / RAND_MAX * 0.8 + 0.2; //0.2 to 1
        iB = (float)rand() / RAND_MAX * 0.8 + 0.2; //0.2 to 1

        fprintf(fptr, "LIGHT emptyname %f %f %f %f %f %f\n",
            posX, posY, posZ, iR, iG, iB);
    }

    float backR, backG, backB, ambR, ambG, ambB;
    backR = (float)rand() / RAND_MAX * 0.8 + 0.2; //0.2 to 1
    backG = (float)rand() / RAND_MAX * 0.8 + 0.2; //0.2 to 1
    backB = (float)rand() / RAND_MAX * 0.8 + 0.2; //0.2 to 1
    ambR = (float)rand() / RAND_MAX * 0.2 + 0.1; //0.1 to 0.3
    ambG = (float)rand() / RAND_MAX * 0.2 + 0.1; //0.1 to 0.3
    ambB = (float)rand() / RAND_MAX * 0.2 + 0.1; //0.1 to 0.3

    fprintf(fptr, "BACK %f %f %f\n", backR, backG, backB);
    fprintf(fptr, "AMBIENT %f %f %f\n", ambR, ambG, ambB);

    fclose(fptr);
}