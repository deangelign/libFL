#include "FL.h"

int main(int argc, char **argv) {

    Image* image = readImage("../data/Lenna.png");
//    int patchSizeX = 32;
//    int patchSizeY = 32;
    int patchSizeX = image->nx/2;
    int patchSizeY = image->ny/2;

    HogManager* hogManager = createHogManager();

    hogManager->image = image;
    hogManager->useUnsignedGradients = true;
    hogManager->binSize = 20;
    hogManager->cellSizeX = patchSizeX/2;
    hogManager->cellSizeY = patchSizeY/2;
    hogManager->cellsPerBlockX = 2;
    hogManager->cellsPerBlockY = 2;
    hogManager->strideX = hogManager->cellSizeX;
    hogManager->strideY = hogManager->cellSizeY;
    computeHogDescriptor(hogManager);
    printf("hog feature vector:\n");
    for (size_t i = 0; i < hogManager->hogFeatureVectorSize; ++i) {
        printf("%f ", hogManager->hogFeatureVector[i]);
    }
    printf("\nsize: %lu\n",hogManager->hogFeatureVectorSize);


    destroyHogManager(&hogManager);


    return 0;
}


