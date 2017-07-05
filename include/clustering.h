//
// Created by deangeli on 5/20/17.
//

#ifndef _CLUSTERING_H
#define _CLUSTERING_H

#include "matrix.h"
#include "matrixUtil.h"
#include "linkedlist.h"

Matrix* kmeansClustering(Matrix* featureMatrix, size_t nClusters,
                         size_t iterMax = 1000,
                         double tolerance = 1, int seed = 0,
                         DistanceFunction distanceFunction = computeNormalizedL1Norm,
                         ArgumentList* argumentListDistanceFunction = NULL);

void kmeansClustering(Matrix* featureMatrix, Matrix* initialClusters,
                      size_t iterMax,
                      double tolerance, int seed,
                      DistanceFunction distanceFunction,
                      ArgumentList* argumentListDistanceFunction);

FeatureMatrix* kMeansClustering(FeatureMatrix* featureMatrix, int numberOfCluster);


Matrix*computeClusterPurity(Matrix* sampleDataMatrix, Matrix* clustersCentroides, GVector* samplesLabel,
                            DistanceFunction distanceFunction,
                            ArgumentList* argumentListDistanceFunction, float *totalPurity);

GVector* generateLabelsSubsets(Matrix* sampleDataMatrix, GVector* samplesLabel, size_t nClasses);

Matrix* findClustersCentroids_supervised(Matrix* allSamples, GVector* subsets, size_t numberOfCentroides,
                                         DistanceFunction distanceFunction,
                                         ArgumentList* argumentListDistanceFunction);

Matrix* clusteringSupervised(Matrix* sampleDataMatrix, GVector* samplesLabel, int nClasses,
                             DistanceFunction distanceFunction,ArgumentList* argumentListDistanceFunction);

GVector* computeSoftAssignmentHistogram(Matrix *clusters,Matrix *samples,
                                        int inverseDistanceFunctionId = 0,
                                        DistanceFunction distanceFunction = computeL1Norm,
                                        ArgumentList* argumentListDistanceFunction = NULL);

Matrix* clusteringGetAllSample(Matrix* sampleDataMatrix);

#endif //LIBFL_CLUSTERING_H
