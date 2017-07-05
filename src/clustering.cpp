//
// Created by deangeli on 5/20/17.
//

#include <featureVector.h>
#include "clustering.h"




Matrix* kmeansClustering(Matrix* featureMatrix, size_t nClusters,
                         size_t iterMax,
                         double tolerance, int seed,
                         DistanceFunction distanceFunction,
                         ArgumentList* argumentListDistanceFunction){
    srand ( seed );
    Matrix* centroides_old = createMatrix(nClusters,featureMatrix->numberColumns,sizeof(float));
    if(nClusters >= featureMatrix->numberRows){
        float* minimuns = (float*)calloc(featureMatrix->numberColumns,sizeof(float));
        float* maximums = (float*)calloc(featureMatrix->numberColumns,sizeof(float));
        for (size_t col = 0; col < featureMatrix->numberColumns; ++col) {
            minimuns[col] = MATRIX_GET_ELEMENT_PO_AS(float,featureMatrix,0,col);
            maximums[col] = MATRIX_GET_ELEMENT_PO_AS(float,featureMatrix,0,col);
            for (size_t row = 0; row < featureMatrix->numberRows; ++row) {
                if(minimuns[col] > MATRIX_GET_ELEMENT_PO_AS(float,featureMatrix,row,col) ){
                    minimuns[col] =  MATRIX_GET_ELEMENT_PO_AS(float,featureMatrix,row,col);
                }
                if(maximums[col] < MATRIX_GET_ELEMENT_PO_AS(float,featureMatrix,row,col)){
                    maximums[col] = MATRIX_GET_ELEMENT_PO_AS(float,featureMatrix,row,col);
                }
            }
        }

        for (size_t row = 0; row < featureMatrix->numberRows; ++row) {
            for (size_t col = 0; col < featureMatrix->numberColumns; ++col) {
                MATRIX_GET_ELEMENT_PO_AS(float,centroides_old,row,col) =
                        MATRIX_GET_ELEMENT_PO_AS(float,featureMatrix,row,col);
            }
        }

        int diffRow = nClusters - featureMatrix->numberRows;
        for (int row = 0; row < diffRow; ++row) {
            for (size_t col = 0; col < featureMatrix->numberColumns; ++col) {
                MATRIX_GET_ELEMENT_PO_AS(float,centroides_old,row,col) = randomNumber(minimuns[col],maximums[col]);
            }
        }

        free(minimuns);
        free(maximums);
        return centroides_old;
    }


    size_t k = 0;
    bool *isUsed = (bool*)calloc(featureMatrix->numberRows,sizeof(*isUsed));
    while (k < nClusters) {
        int randomIndex = randomInteger(0, featureMatrix->numberRows - 1);
        if(isUsed[randomIndex] == false){
            copyMatrixRow(centroides_old,featureMatrix,k,randomIndex);
            isUsed[randomIndex] = true;
            k++;
        }
    }
    free(isUsed);
    unsigned int printStatusValue = iterMax*0.02;
    Matrix* centroides_new = createMatrix(centroides_old->numberRows, centroides_old->numberColumns,sizeof(float));
    int* count = (int*)calloc(nClusters,sizeof(int));
    for (size_t j = 0; j < nClusters; ++j) {
        count[j] = 0;
    }
    size_t iter = 0;
    double totalDiff = DBL_MAX;
    float currentDiff = 0;
    while( (iter < iterMax) && (totalDiff > tolerance) ){
        currentDiff = 0;
        totalDiff = 0;
        for (size_t featureVectorIndex = 0; featureVectorIndex < featureMatrix->numberRows; ++featureVectorIndex) {
            size_t nearestClusterIndex = findNearestRow(centroides_old,
                                                        featureMatrix,featureVectorIndex,
                                                        distanceFunction,argumentListDistanceFunction);
            MATRIX_ADD_ROWS_AS(float,centroides_new,featureMatrix,nearestClusterIndex,featureVectorIndex);
            count[nearestClusterIndex]++;
        }
        for (size_t i = 0; i < centroides_new->numberRows; ++i) {
            for (size_t j = 0; j < centroides_new->numberColumns; ++j) {
                MATRIX_GET_ELEMENT_PO_AS(float,centroides_new,i,j) =
                        (MATRIX_GET_ELEMENT_PO_AS(float,centroides_new,i,j) +
                         MATRIX_GET_ELEMENT_PO_AS(float,centroides_old,i,j))/(count[i]+1);
                currentDiff = MATRIX_GET_ELEMENT_PO_AS(float,centroides_new,i,j) -
                              MATRIX_GET_ELEMENT_PO_AS(float,centroides_old,i,j);
                currentDiff = fabsf(currentDiff);
                totalDiff += currentDiff;
                MATRIX_GET_ELEMENT_PO_AS(float,centroides_old,i,j) = MATRIX_GET_ELEMENT_PO_AS(float,centroides_new,i,j);
                MATRIX_GET_ELEMENT_PO_AS(float,centroides_new,i,j) = 0.0f;
            }
            count[i] = 0;
            totalDiff = totalDiff/centroides_new->numberRows;
        }
        if(iter % printStatusValue == 0 || iter == 0){
            printf("[kmeansClustering] At iteration %lu, the total displacement is %f\n",iter,totalDiff);
        }

        iter++;
    }
    printf("[kmeansClustering] At iteration %lu, the total displacement is %f\n",(iter-1),totalDiff);
    destroyMatrix(&centroides_new);

    free(count);
    return centroides_old;
}


void kmeansClustering(Matrix* featureMatrix, Matrix* initialClusters,
                         size_t iterMax,
                         double tolerance, int seed,
                         DistanceFunction distanceFunction,
                         ArgumentList* argumentListDistanceFunction){


    srand ( seed );
    unsigned int printStatusValue = iterMax*0.02;
    Matrix* centroides_new = createMatrix(initialClusters->numberRows, initialClusters->numberColumns,sizeof(float));
    int* count = (int*)calloc(initialClusters->numberRows,sizeof(int));
    for (size_t j = 0; j < initialClusters->numberRows; ++j) {
        count[j] = 0;
    }
    size_t iter = 0;
    double totalDiff = DBL_MAX;
    float currentDiff = 0;
    while( (iter < iterMax) && (totalDiff > tolerance) ){
        currentDiff = 0;
        totalDiff = 0;
        for (size_t featureVectorIndex = 0; featureVectorIndex < featureMatrix->numberRows; ++featureVectorIndex) {
            size_t nearestClusterIndex = findNearestRow(initialClusters,
                                                        featureMatrix,featureVectorIndex,
                                                        distanceFunction,argumentListDistanceFunction);
            MATRIX_ADD_ROWS_AS(float,centroides_new,featureMatrix,nearestClusterIndex,featureVectorIndex);
            count[nearestClusterIndex]++;
        }
        for (size_t i = 0; i < centroides_new->numberRows; ++i) {
            for (size_t j = 0; j < centroides_new->numberColumns; ++j) {
                MATRIX_GET_ELEMENT_PO_AS(float,centroides_new,i,j) =
                        (MATRIX_GET_ELEMENT_PO_AS(float,centroides_new,i,j) +
                         MATRIX_GET_ELEMENT_PO_AS(float,initialClusters,i,j))/(count[i]+1);
                currentDiff = MATRIX_GET_ELEMENT_PO_AS(float,centroides_new,i,j) -
                              MATRIX_GET_ELEMENT_PO_AS(float,initialClusters,i,j);
                currentDiff = fabsf(currentDiff);
                totalDiff += currentDiff;
                MATRIX_GET_ELEMENT_PO_AS(float,initialClusters,i,j) = MATRIX_GET_ELEMENT_PO_AS(float,centroides_new,i,j);
                MATRIX_GET_ELEMENT_PO_AS(float,centroides_new,i,j) = 0.0f;
            }
            count[i] = 0;
            totalDiff = totalDiff/centroides_new->numberRows;
        }
        if(iter % printStatusValue == 0 || iter == 0){
            printf("[kmeansClustering] At iteration %lu, the total displacement is %f\n",iter,totalDiff);
        }

        iter++;
    }
    printf("[kmeansClustering] At iteration %lu, the total displacement is %f\n",(iter-1),totalDiff);
    destroyMatrix(&centroides_new);

    free(count);
}


Matrix*computeClusterPurity(Matrix* sampleDataMatrix, Matrix* clustersCentroides, GVector* samplesLabel,
                            DistanceFunction distanceFunction,
                            ArgumentList* argumentListDistanceFunction, float *totalPurity){

    int nlabels = 0;

    for (size_t i = 0; i < samplesLabel->size; ++i) {
//        printf("%lu : %d \n", i,VECTOR_GET_ELEMENT_AS(int,samplesLabel,i) );
        if(VECTOR_GET_ELEMENT_AS(int,samplesLabel,i) > nlabels){
            nlabels = VECTOR_GET_ELEMENT_AS(int,samplesLabel,i);
        }
    }
    int nclusters = clustersCentroides->numberRows;
    printf("%d %d\n",nclusters,nlabels);
    Matrix* classesPerCluster = createMatrix(nclusters,nlabels,sizeof(int));
    Matrix* purities = createMatrix(nclusters,5,sizeof(float));//purity and nclasses
//
    int trueLabelIndex;
    for (size_t featureVectorIndex = 0; featureVectorIndex < sampleDataMatrix->numberRows; ++featureVectorIndex) {
        size_t nearestClusterIndex = findNearestRow(clustersCentroides,
                                                    sampleDataMatrix,featureVectorIndex,
                                                    distanceFunction,argumentListDistanceFunction);
        trueLabelIndex = VECTOR_GET_ELEMENT_AS(int,samplesLabel,featureVectorIndex) - 1;
        MATRIX_GET_ELEMENT_PO_AS(int,classesPerCluster,nearestClusterIndex,trueLabelIndex) += 1;
    }

    int classMaxValue;
    int classMaxIndex;
    int differentClasses;
    float totalCount;
    float purity;
    *totalPurity = 0.0;
    for (size_t rowIndex = 0; rowIndex < classesPerCluster->numberRows; ++rowIndex) {
        classMaxValue = 0;
        differentClasses = 0;
        classMaxIndex = -1;
        totalCount = 0.0;
        purity = 0.0;
        for (size_t colIndex = 0; colIndex < classesPerCluster->numberColumns; ++colIndex) {
            totalCount += MATRIX_GET_ELEMENT_PO_AS(int,classesPerCluster,rowIndex,colIndex);
            if(MATRIX_GET_ELEMENT_PO_AS(int,classesPerCluster,rowIndex,colIndex) > 0){
                differentClasses++;
                if(MATRIX_GET_ELEMENT_PO_AS(int,classesPerCluster,rowIndex,colIndex)  >=  classMaxValue){
                    classMaxIndex = colIndex;
                    classMaxValue = MATRIX_GET_ELEMENT_PO_AS(int,classesPerCluster,rowIndex,colIndex);
                }
            }
        }

        if(totalCount > 0){
            purity = classMaxValue/totalCount;
        }
        *totalPurity += purity;

        MATRIX_GET_ELEMENT_PO_AS(float,purities,rowIndex,0) = rowIndex; //clusterIndex
        MATRIX_GET_ELEMENT_PO_AS(float,purities,rowIndex,1) = purity; //cluster putiry
        MATRIX_GET_ELEMENT_PO_AS(float,purities,rowIndex,2) = classMaxIndex; //most common class in the cluster
        MATRIX_GET_ELEMENT_PO_AS(float,purities,rowIndex,3) = totalCount; // total number of samples in cluster
        MATRIX_GET_ELEMENT_PO_AS(float,purities,rowIndex,4) = differentClasses;

        for (size_t i = rowIndex; i > 0; --i) {
            if(MATRIX_GET_ELEMENT_PO_AS(float,purities,i,1) > MATRIX_GET_ELEMENT_PO_AS(float,purities, (i-1) ,1)){
                swapMatrixRows(purities,i, (i-1) );
            }
        }
    }
    *totalPurity = (*totalPurity)/classesPerCluster->numberRows;
    //MATRIX_PRINT_AS(float,"%f ",purities);
    printf("[computeClusterPurity] total purity: %f\n",*totalPurity);
    destroyMatrix(&classesPerCluster);
    //printf("%lu %lu\n",classesPerCluster->numberRows,classesPerCluster->numberColumns);
    return purities;
}

GVector* generateLabelsSubsets(Matrix* sampleDataMatrix, GVector* samplesLabel, size_t nClasses){



    GVector *indicesPerClasses = createNullVector(nClasses,sizeof(GVector *));

    for (size_t i = 0; i < samplesLabel->size; ++i) {
        int labelIndex =VECTOR_GET_ELEMENT_AS(int,samplesLabel,i) - 1;

        if(VECTOR_GET_ELEMENT_AS(GVector *,indicesPerClasses,labelIndex) == NULL){
            VECTOR_GET_ELEMENT_AS(GVector *,indicesPerClasses,labelIndex) = createVector(1,sizeof(int));
        }
        GVector* indexVector = VECTOR_GET_ELEMENT_AS(GVector *,indicesPerClasses,labelIndex);
        VECTOR_PUSH_BACK(int, indexVector ,i);
    }

    GVector *subsets = createNullVector(nClasses,sizeof(Matrix *));
    for (size_t i = 0; i < nClasses; ++i) {
        GVector *indexVector = VECTOR_GET_ELEMENT_AS(GVector *,indicesPerClasses,i);
        int* indices = (int*)indexVector->data;
        VECTOR_GET_ELEMENT_AS(Matrix *,subsets,i) = getMatrixRows(sampleDataMatrix, indices, indexVector->size);
        destroyVector(&indexVector);

    }
    destroyVector(&indicesPerClasses);

    return subsets;
}

Matrix* findClustersCentroids_supervised(Matrix* allsamples, GVector* subsets, size_t numberOfCentroides,
                                         DistanceFunction distanceFunction,
                                         ArgumentList* argumentListDistanceFunction){

    Matrix* centroides = NULL;
    size_t numberOfFeatures = VECTOR_GET_ELEMENT_AS(Matrix*,subsets,0)->numberColumns;
    size_t numberOfClasses = subsets->size;
    GVector* subsets_copy = createNullVector(subsets->size,subsets->elementSize);
    GVector* subsets_mean = createNullVector(subsets->size,subsets->elementSize);
    for (size_t i = 0; i < subsets->size; ++i) {
        Matrix* subset = VECTOR_GET_ELEMENT_AS(Matrix*,subsets,i);
        VECTOR_GET_ELEMENT_AS(Matrix*,subsets_copy,i) = copyMatrix(subset);
        VECTOR_GET_ELEMENT_AS(Matrix*,subsets_mean,i) = createMatrix(1,subset->numberColumns,subset->matrixData->elementSize);
    }


    for (size_t i = 0; i < subsets_copy->size; ++i) {
        Matrix* subset = VECTOR_GET_ELEMENT_AS(Matrix*,subsets_copy,i);
        Matrix* subset_mean = VECTOR_GET_ELEMENT_AS(Matrix*,subsets_mean,i);
        for (size_t row = 0; row < subset->numberRows; ++row) {
            for (size_t col = 0; col < subset->numberColumns; ++col) {
                MATRIX_GET_ELEMENT_PO_AS(float,subset_mean,0,col) += MATRIX_GET_ELEMENT_PO_AS(float,subset,row,col);
            }
        }

        for (size_t col = 0; col < subset->numberColumns; ++col) {
            MATRIX_GET_ELEMENT_PO_AS(float,subset_mean,0,col) /= subset->numberRows;
        }
    }
    size_t currentNumbeOfCentroides = 0;
    size_t currentSetIndex = 0;
    size_t zeroCount = 0;
    centroides = createMatrix(numberOfCentroides,numberOfFeatures,sizeof(float));
    while(currentNumbeOfCentroides < numberOfCentroides){
        Matrix* subset = VECTOR_GET_ELEMENT_AS(Matrix*,subsets_copy,currentSetIndex);
        Matrix* subset_mean = VECTOR_GET_ELEMENT_AS(Matrix*,subsets_mean,currentSetIndex);
        if(subset->numberRows == 0){
            if(zeroCount == numberOfClasses){
                printf("[findClustersCentroids_supervised] cant find more clusters\n");
                break;
            }
            continue;
        }
        size_t nearestElementIndex = findNearestRow(subset,
                                                    subset_mean,0,
                                                    distanceFunction,argumentListDistanceFunction);
        copyMatrixRow(centroides,subset_mean,currentNumbeOfCentroides,0);
        //copyMatrixRow(centroides,subset,currentNumbeOfCentroides,nearestElementIndex);

        if(subset->numberRows == 1){
            for (size_t col = 0; col < subset->numberColumns; ++col) {
                MATRIX_GET_ELEMENT_PO_AS(float, subset_mean, 0, col) = 0;
            }
            zeroCount++;
        }else{
            for (size_t col = 0; col < subset->numberColumns; ++col) {
                MATRIX_GET_ELEMENT_PO_AS(float,subset_mean,0,col) =
                        (MATRIX_GET_ELEMENT_PO_AS(float,subset_mean,0,col)*subset->numberRows) -
                        MATRIX_GET_ELEMENT_PO_AS(float,subset,nearestElementIndex,col);
                MATRIX_GET_ELEMENT_PO_AS(float,subset_mean,0,col) /= (subset->numberRows-1);
            }
        }

        removeMatrixRow(subset,nearestElementIndex);
        currentSetIndex++;
        if(currentSetIndex >= numberOfClasses){
            currentSetIndex = 0;
        }
        currentNumbeOfCentroides++;
    }
    kmeansClustering(allsamples, centroides, 1000,
                          0.0001, 0,
                          distanceFunction,
                          argumentListDistanceFunction);

    for (size_t i = 0; i < subsets_copy->size; ++i) {
        Matrix* subset =VECTOR_GET_ELEMENT_AS(Matrix*,subsets_copy,i);
        Matrix* subset_mean =VECTOR_GET_ELEMENT_AS(Matrix*,subsets_mean,i);
        destroyMatrix(&subset);
        destroyMatrix(&subset_mean);
    }
    destroyVector(&subsets_copy);
    destroyVector(&subsets_mean);


    return centroides;
}

Matrix* clusteringSupervised(Matrix* sampleDataMatrix, GVector* samplesLabel, int nClasses,
                             DistanceFunction distanceFunction,ArgumentList* argumentListDistanceFunction){
    GVector* subsets = generateLabelsSubsets(sampleDataMatrix, samplesLabel, nClasses);
    Matrix* purity = NULL;
    Matrix* centroides = NULL;
    float purity_old = 0;
    float purity_new = 0;
    int iterMax = 20;

    for (int j = 0; j < iterMax; ++j) {
        centroides = findClustersCentroids_supervised(sampleDataMatrix,subsets, (j+1)*nClasses,distanceFunction,argumentListDistanceFunction);
        purity = computeClusterPurity(sampleDataMatrix, centroides, samplesLabel,
                                      distanceFunction,argumentListDistanceFunction,&purity_new);
        if(purity_new < purity_old){
            break;
        }
        purity_old = purity_new;
        destroyMatrix(&purity);
        destroyMatrix(&centroides);
    }

    for (int j = purity->numberRows-1; j >=  0; --j) {
        if(MATRIX_GET_ELEMENT_PO_AS(float,purity,j,1) <= 0.0001){
            int clusterIndex = MATRIX_GET_ELEMENT_PO_AS(float,purity,j,0);
            removeMatrixRow(purity,j);
            removeMatrixRow(centroides,clusterIndex);
        }else{
            break;
        }
    }
    destroyMatrix(&purity);
    purity = computeClusterPurity(sampleDataMatrix, centroides, samplesLabel,
                                  distanceFunction,argumentListDistanceFunction,&purity_new);

    for (size_t i = 0; i < subsets->size; ++i) {
        Matrix* matrix = VECTOR_GET_ELEMENT_AS(Matrix*,subsets,i);
        destroyMatrix(&matrix);
    }
    destroyVector(&subsets);
    //destroyMatrix(&centroides);
    destroyMatrix(&purity);
    return centroides;
}

GVector* computeSoftAssignmentHistogram(Matrix *clusters,Matrix *samples,
                                        int inverseDistanceFunctionId,
                                        DistanceFunction distanceFunction,
                                        ArgumentList* argumentListDistanceFunction){

    InverseDistanceFunction inverseDistanceFunction = NULL;
    if(inverseDistanceFunctionId == 0){
        inverseDistanceFunction = computeInverseDistanceUsingInverseX;
    }else{
        inverseDistanceFunction = computeInverseDistanceUsingExponential;
    }


    GVector* bowHistogram = createNullVector(clusters->numberRows,sizeof(float));
    double *distaces2Cluster = (double*)calloc(clusters->numberRows,sizeof(double));
    double totalDistance = 0;
    double parcialDistance = 0;
    double distance;
    double inverseDistance;
    for (size_t sampleIndex = 0; sampleIndex < samples->numberRows; ++sampleIndex) {
        parcialDistance = 0;
        for (size_t clusterIndex = 0; clusterIndex <clusters->numberRows; ++clusterIndex) {
            distance = computeDistanceBetweenRows(samples,
                                                         clusters,
                                                         sampleIndex,clusterIndex,
                                                         distanceFunction,
                                                         argumentListDistanceFunction);
            inverseDistance = inverseDistanceFunction(distance);
            distaces2Cluster[clusterIndex] = inverseDistance;
            parcialDistance += inverseDistance;
        }
        for (size_t wordIndex = 0; wordIndex < bowHistogram->size; ++wordIndex) {
            VECTOR_GET_ELEMENT_AS(float,bowHistogram,wordIndex) += distaces2Cluster[wordIndex]/parcialDistance;
            totalDistance += distaces2Cluster[wordIndex]/parcialDistance;
        }
    }

    if(fabs(totalDistance) < 0.000){
        return bowHistogram;
    }
    parcialDistance = 0;
    for (size_t wordIndex = 0; wordIndex < bowHistogram->size; ++wordIndex) {
        VECTOR_GET_ELEMENT_AS(float,bowHistogram,wordIndex) /= totalDistance;
        parcialDistance += VECTOR_GET_ELEMENT_AS(float,bowHistogram,wordIndex);
    }
    //VECTOR_PRINT_AS(float,"%f ",bowHistogram);
    //printf("\n");
    free(distaces2Cluster);
    return bowHistogram;


}

Matrix* clusteringGetAllSample(Matrix* sampleDataMatrix){
    return copyMatrix(sampleDataMatrix);
}

