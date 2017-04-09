//
// Created by deangeli on 4/7/17.
//

#ifndef _BAGOFVISUALWORDS_H_
#define _BAGOFVISUALWORDS_H_

#include "common.h"
#include "featureVector.h"
#include "file.h"
#include "image.h"
#include "histogram.h"


typedef struct _bagOfVisualWords {
    int patchSize;
    DirectoryManager* directoryManager;
    FeatureMatrix* vocabulary;
} BagOfVisualWords;

FeatureMatrix* computeFeatureVectors(Image* imagePack, int patchSize);
FeatureMatrix* computeFeatureVectors(DirectoryManager* directoryManager, int patchSize);
FeatureMatrix* kMeansClustering(FeatureMatrix* featureMatrix, int numberOfCluster);




#endif //LIBFL_BAGOFVISUALWORDS_H
