//
// Created by deangeli on 4/7/17.
//

#include "bagOfVisualWords.h"

/*
 * Nesta abordagem nao e necessario computar quantas linhas (patchs) a matrix vai ter, pois eu estou usando a funcao
 * "addNewLines" para adicionar uma nova linha na matrix toda vez que um novo FeatureVector e computado.
 * */
FeatureMatrix* computeFeatureVectors(DirectoryManager* directoryManager, int patchSize){

    Image* currentImage;
    Image* patch;
    Histogram* histogram;
    FeatureVector* patchVector;
    int binSize = 64;

    FeatureMatrix* featureMatrix = createFeatureMatrix();
    int k=0;
    for (size_t fileIndex = 0; fileIndex < directoryManager->nfiles; ++fileIndex) {
        currentImage = readImage(directoryManager->files[fileIndex]->path);
        for (int y = 0; y <= currentImage->ny-patchSize; y +=patchSize) {
            for (int x = 0; x <= currentImage->nx-patchSize; x += patchSize) {
                patch = extractSubImage(currentImage,x,y,patchSize,patchSize,true);
                histogram = computeHistogram(patch,binSize,true);
                patchVector = createFeatureVector(histogram);
                addNewLines(&featureMatrix,1);
                featureMatrix->featureVector[k] = patchVector;
                k++;
                destroyHistogram(&histogram);
                destroyImage(&patch);
            }
        }
        destroyImage(&currentImage);
    }
    return featureMatrix;
}

FeatureMatrix* computeFeatureVectors(Image* imagePack, int patchSize){
    Image* currentSlice;
    Image* patch;
    Histogram* histogram;
    FeatureVector* patchVector;
    int patchX_axis = imagePack->nx/patchSize;
    int patchY_axis = imagePack->ny/patchSize;
    int numberPatchsPerImage = patchX_axis*patchY_axis;
    int numberPatchs = numberPatchsPerImage*imagePack->nz;
    int binSize = 64;

    FeatureMatrix* featureMatrix = createFeatureMatrix(numberPatchs);
    int k=0;
    for (int z = 0; z < imagePack->nz; ++z) {
        currentSlice = getSlice(imagePack,z);
        for (int y = 0; y <= imagePack->ny-patchSize; y +=patchSize) {
            for (int x = 0; x <= imagePack->nx-patchSize; x += patchSize) {
                patch = extractSubImage(currentSlice,x,y,patchSize,patchSize,true);
                histogram = computeHistogram(patch,binSize,true);
                patchVector = createFeatureVector(histogram);
                featureMatrix->featureVector[k] = patchVector;
                k++;
                destroyHistogram(&histogram);
                destroyImage(&patch);
            }
        }
        destroyImage(&currentSlice);
    }
    return featureMatrix;
}

FeatureMatrix* kMeansClustering(FeatureMatrix* featureMatrix, int numberOfCluster){
    FeatureMatrix* dict = createFeatureMatrix(numberOfCluster);
    int k = 0;
    bool *isUsed = (bool*)calloc(featureMatrix->nFeaturesVectors,sizeof(*isUsed));
    int* labels = (int*)calloc(featureMatrix->nFeaturesVectors,sizeof(*labels));
    while (k < numberOfCluster) {
        int randomIndex = RandomInteger(0,featureMatrix->nFeaturesVectors);
        if(isUsed[randomIndex] == false){
            dict->featureVector[k] = copyFeatureVector(featureMatrix->featureVector[randomIndex]);
            isUsed[randomIndex] = true;
            k++;
        }
    }
    free(isUsed);
    //not finished yet





    return NULL;
}
