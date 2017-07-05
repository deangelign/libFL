//
// Created by deangeli on 5/20/17.
//

#include "sampling.h"

GVector* gridSampling(Image* image, size_t patchSizeX,size_t patchSizeY){
    size_t nPatchs_X = ceil((float)image->nx/patchSizeX);
    size_t nPatchs_Y = ceil((float)image->ny/patchSizeY);
    if(nPatchs_X == 0){
        nPatchs_X = 1;
    }

    if(nPatchs_Y == 0){
        nPatchs_Y = 1;
    }
    size_t nPatchs = nPatchs_X*nPatchs_Y;
    //GVector* vector_images = createNullVector(nPatchs,sizeof(Image*));
    GVector* vector_ROIs = createNullVector(nPatchs,sizeof(RegionOfInterest));
    int k = 0;
    for (int y = 0; y < image->ny; y +=patchSizeY) {
        for (int x = 0; x < image->nx; x += patchSizeX) {
            RegionOfInterest regionOfInterest;
            regionOfInterest.coordinateX = x;
            regionOfInterest.coordinateY = y;
            regionOfInterest.coordinateZ = 0;

            regionOfInterest.size_x = patchSizeX;
            regionOfInterest.size_y = patchSizeY;
            regionOfInterest.size_z = 1;

            VECTOR_GET_ELEMENT_AS(RegionOfInterest,vector_ROIs,k) = regionOfInterest;

            //VECTOR_GET_ELEMENT_AS(Image*,vector_images,k) = extractSubImage(image,x,y,patchSizeX,patchSizeY,true);
            k++;
        }
    }
    //return vector_images;
    return vector_ROIs;
}

GVector* randomSampling_noImage(Image* image, size_t numberOfPatchs, size_t patchSizeX,size_t patchSizeY, int seed){
    srand(seed);
    size_t patchsGenerated = 0;
    int randomXcoordinate;
    int randomYcoordinate;
    int lowerBound_X = 0 +  patchSizeX/2;
    int upperBound_X = image->nx -  (patchSizeX/2);
    int lowerBound_Y = 0 +  patchSizeY/2;
    int upperBound_Y = image->ny -  (patchSizeY/2);

    if(lowerBound_X > upperBound_X){
        printf("[randomSampling_noImage] can not generated patchs. Lower bound X is greather than upper bound\n");
        return NULL;
    }

    if(lowerBound_Y > upperBound_Y){
        printf("[randomSampling_noImage] can not generated patchs. Lower bound Y is greather than upper bound\n");
        return NULL;
    }

    GVector* vector = createNullVector(numberOfPatchs,sizeof(RegionOfInterest));
    while (numberOfPatchs > patchsGenerated){
        randomXcoordinate = randomInteger(lowerBound_X,upperBound_X);
        randomYcoordinate = randomInteger(lowerBound_Y,upperBound_Y);
        RegionOfInterest regionOfInterest;

        regionOfInterest.coordinateX = randomXcoordinate - patchSizeX/2;
        regionOfInterest.coordinateY = randomYcoordinate - patchSizeY/2;
        regionOfInterest.coordinateZ = 0;

        regionOfInterest.size_x = patchSizeX;
        regionOfInterest.size_y = patchSizeY;
        regionOfInterest.size_z = 1;

        VECTOR_GET_ELEMENT_AS(RegionOfInterest,vector,patchsGenerated) = regionOfInterest;
        patchsGenerated++;
    }

    return vector;
}




