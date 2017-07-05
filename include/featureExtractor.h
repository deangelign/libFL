//
// Created by deangeli on 5/20/17.
//

#ifndef LIBFL_FEATUREEXTRACTOR_H
#define LIBFL_FEATUREEXTRACTOR_H

#include "vector.h"
#include "vectorUtil.h"
#include "matrix.h"
#include "image.h"
#include "histogram.h"
#include "kernel.h"
#include "filtering.h"

#ifdef USE_IFT
#include "ift.h"
#endif


Matrix* computeColorHistogram(GVector* vector_images,size_t nbinsPerChannel);

typedef struct _hogManager {
    int cellSizeX;
    int cellSizeY;
    int strideX;
    int strideY;
    int cellsPerBlockX;
    int cellsPerBlockY;
    int binSize;
    bool useUnsignedGradients;
    bool fitBlock;
    bool computeGradient;
    Image* gradientImageMagnitude;
    Image* gradientImagePhase;
    Image* image;
    float* hogFeatureVector;
    size_t hogFeatureVectorSize;
}HogManager;

HogManager* createHogManager();
void destroyHogManager(HogManager** pHogManager);
void computeHogDescriptor(HogManager* hogManager);
Matrix* computeHogDescriptorForRegionsOfInterest(GVector* vector_ROIs,HogManager* hogManager);
Matrix* computeHogDescriptorForSuperPixelsInterestPoints(GVector* vector_image,HogManager* hogManager,
                                                         size_t patchSizeX,size_t patchSizeY);
#endif //LIBFL_FEATUREEXTRACTOR_H
