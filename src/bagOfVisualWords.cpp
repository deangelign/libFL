//
// Created by deangeli on 4/7/17.
//

#include "bagOfVisualWords.h"


BagOfVisualWordsManager* createBagOfVisualWordsManager(){
    BagOfVisualWordsManager* bowManager = (BagOfVisualWordsManager*)calloc(1,sizeof(BagOfVisualWordsManager));
    bowManager->pathsToImages_dictionary = NULL;
    bowManager->pathsToImages_train = NULL;
    bowManager->pathsToImages_test = NULL;
    bowManager->dictionary = NULL;
    bowManager->featureExtractorFunction = NULL;
    bowManager->imageSamplerFunction = NULL;
    bowManager->clusteringFunction = NULL;
    bowManager->argumentListOfFeatureExtractor =NULL;
    bowManager->argumentListOfSampler = NULL;
    bowManager->argumentListOfClustering = NULL;
    bowManager->argumentListOfDistanceFunction = NULL;
    bowManager->freeFunction2SamplerOutput = NULL;
    bowManager->classifier = NULL;
    bowManager->freeFunctionClassifier = NULL;
    bowManager->histogramsTraining = NULL;
    bowManager->labelsTraining = NULL;
    bowManager->dataVisualWords = NULL;
    bowManager->labelsVisualWords = NULL;
    bowManager->storeTrainData = false;
    bowManager->storePredictedData = false;
    bowManager->storeVisualWordsData = false;
    bowManager->currentImage = NULL;
    return bowManager;
}

void destroyBagOfVisualWordsManager(BagOfVisualWordsManager** pBagOfVisualWordsManager){
    BagOfVisualWordsManager* aux = *pBagOfVisualWordsManager;
    if(aux == NULL){
        return;
    }

    destroyVector(&(aux->pathsToImages_dictionary));
    destroyVector(&(aux->pathsToImages_train));
    destroyVector(&(aux->pathsToImages_test));
    destroyVector(&(aux->labelsTraining));
    destroyVector(&(aux->labelsPredicted));
    destroyVector(&(aux->labelsVisualWords));

    destroyArgumentList(&(aux->argumentListOfSampler));
    destroyArgumentList(&(aux->argumentListOfFeatureExtractor));
    destroyArgumentList(&(aux->argumentListOfClustering));
    destroyArgumentList(&(aux->argumentListOfDistanceFunction));
    destroyArgumentList(&(aux->argumentListOfHistogramMounter));


    destroyMatrix(&(aux->dictionary));
    destroyMatrix(&(aux->histogramsTraining));
    destroyMatrix(&(aux->histogramsPredictSamples));
    destroyMatrix(&(aux->dataVisualWords));


    if(aux->freeFunctionClassifier){
        if(aux->classifier){
            aux->freeFunctionClassifier(aux->classifier);
        }
    }


    free(*pBagOfVisualWordsManager);
}

//void setDefaultParameters(BagOfVisualWordsManager* bowManager){
//    if(bowManager == NULL){
//        printf("[setDefaultParameters] bowManager is null");
//        return;
//    }
//
//    ArgumentList* gridSamplingArguments = createArgumentList();
//    ARGLIST_PUSH_BACK_AS(size_t,gridSamplingArguments,64); //patch size X
//    ARGLIST_PUSH_BACK_AS(size_t,gridSamplingArguments,64); //patch size Y
//    bowManager->argumentListOfSampler = gridSamplingArguments;
//    bowManager->imageSamplerFunction = gridSampling;
//
//    //Neste exemplo eu usar o histograma de cores
//    ArgumentList* colorFeatureExtractorArguments = createArgumentList();
//    ARGLIST_PUSH_BACK_AS(size_t,colorFeatureExtractorArguments,8); //nBins per channel
//    ARGLIST_PUSH_BACK_AS(size_t,colorFeatureExtractorArguments,8*8*8); //total number of channels
//    bowManager->featureExtractorFunction = computeColorHistogram;
//
//}

GVector* gridSamplingBow(Image* image, BagOfVisualWordsManager* bagOfVisualWordsManager){
    ArgumentList* argumentList = bagOfVisualWordsManager->argumentListOfSampler;

    if(argumentList->length == 0){
        printf("[gridSampling] invalid argument list");
    }else if(argumentList->length == 1){
        size_t patchSize = ARGLIST_GET_ELEMENT_AS(size_t ,argumentList,0);
        return gridSampling(image,patchSize,patchSize);
    }else if(argumentList->length == 2){
        size_t patchSizeX = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,0);
        size_t patchSizeY = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,1);
        return gridSampling(image,patchSizeX,patchSizeY);
    }else{
        return NULL;
    }
    return NULL;
}



Matrix* computeColorHistogramBow(GVector* vector,BagOfVisualWordsManager* bagOfVisualWordsManager){
    ArgumentList* argumentList = bagOfVisualWordsManager->argumentListOfFeatureExtractor;
    if(argumentList->length < 1){
        printf("[computeColorHistogram] invalid argument list");
        return NULL;
    }
    if(vector->size == 0){
        printf("[computeColorHistogram] vector has 0 elements");
        return NULL;
    }
    size_t nbinsPerChannel = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,0);
    return computeColorHistogram(vector,nbinsPerChannel);
}

void generateAllVisualWords(BagOfVisualWordsManager *bagOfVisualWordsManager){
    Matrix* allFeatures = NULL;
    if(!bagOfVisualWordsManager->imageSamplerFunction){
        printf("[generateAllVisualWords] Sampler function not defined\n");
    }


    if(!bagOfVisualWordsManager->featureExtractorFunction){
        printf("[generateAllVisualWords] Feature extractor function not defined\n");
        return;
    }

    printf("[generateAllVisualWords] Generating visual words...\n");
    GVector *visualWordsLabels = createVector(10,sizeof(int));
    int trueLabelImage;
    for (size_t i = 0; i < bagOfVisualWordsManager->pathsToImages_dictionary->size; ++i) {
        char* imagePath = VECTOR_GET_ELEMENT_AS(char*,bagOfVisualWordsManager->pathsToImages_dictionary,i);
        Image* image = readImage(imagePath);
        bagOfVisualWordsManager->currentImage = image;

        if(image == NULL){
            printf("[generateAllVisualWords] invalid image path: %s",imagePath);
            continue;
        }
        GVector* samplingResults = NULL;
        if(bagOfVisualWordsManager->imageSamplerFunction){
            samplingResults = bagOfVisualWordsManager->imageSamplerFunction(image,
                                                                            bagOfVisualWordsManager);
            samplingResults->freeFunction = bagOfVisualWordsManager->freeFunction2SamplerOutput;
        }else{
            samplingResults = createNullVector(1,sizeof(Image*));
            VECTOR_GET_ELEMENT_AS(Image*,samplingResults,0) = image;
        }
        trueLabelImage = findTrueLabelInName(imagePath);

        Matrix* featureMatrix = bagOfVisualWordsManager->featureExtractorFunction(samplingResults,
                                                                                  bagOfVisualWordsManager);
        for (size_t j = 0; j < featureMatrix->numberRows; ++j) {
            VECTOR_PUSH_BACK(int,visualWordsLabels,trueLabelImage);
        }
        Matrix* newData = stackVerticallyMatrices(allFeatures,featureMatrix);
        destroyMatrix(&allFeatures);
        allFeatures = newData;
        destroyImage(&image);
        destroyVector(&samplingResults);
        destroyMatrix(&featureMatrix);
    }

    if(bagOfVisualWordsManager->storeVisualWordsData){
        if(bagOfVisualWordsManager->dataVisualWords != NULL){
            destroyMatrix(&(bagOfVisualWordsManager->dataVisualWords));
        }
        if(bagOfVisualWordsManager->labelsVisualWords != NULL){
            destroyVector(&(bagOfVisualWordsManager->labelsVisualWords));
        }
        bagOfVisualWordsManager->dataVisualWords = allFeatures;
        bagOfVisualWordsManager->labelsVisualWords = visualWordsLabels;

    }else{
        destroyMatrix(&allFeatures);
        destroyVector(&visualWordsLabels);
    }
    printf("[generateAllVisualWords] Visual words generated.\n");
}

void computeDictionary(BagOfVisualWordsManager *bagOfVisualWordsManager){
    Matrix* allFeatures = NULL;
    if(!bagOfVisualWordsManager->imageSamplerFunction){
        printf("[computeDictionary] Sampler function not defined\n");
    }


    if(!bagOfVisualWordsManager->featureExtractorFunction){
        printf("[computeDictionary] Feature extractor function not defined\n");
        return;
    }
    if(!bagOfVisualWordsManager->clusteringFunction){
        printf("[computeDictionary] Clustering function not defined\n");
        return;
    }

    printf("[computeDictionary] Generating visual words...\n");
    GVector *visualWordsLabels = createVector(10,sizeof(int));
    int trueLabelImage;
    for (size_t i = 0; i < bagOfVisualWordsManager->pathsToImages_dictionary->size; ++i) {
        char* imagePath = VECTOR_GET_ELEMENT_AS(char*,bagOfVisualWordsManager->pathsToImages_dictionary,i);
        Image* image = readImage(imagePath);
        if(image == NULL){
            printf("[computeDictionary] invalid image path: %s",imagePath);
            continue;
        }
        GVector* samplingResults = NULL;
        if(bagOfVisualWordsManager->imageSamplerFunction){
            samplingResults = bagOfVisualWordsManager->imageSamplerFunction(image,
                                                                            bagOfVisualWordsManager);
            samplingResults->freeFunction = bagOfVisualWordsManager->freeFunction2SamplerOutput;
        }else{
            samplingResults = createNullVector(1,sizeof(Image*));
            VECTOR_GET_ELEMENT_AS(Image*,samplingResults,0) = image;
        }
        trueLabelImage = findTrueLabelInName(imagePath);

        Matrix* featureMatrix = bagOfVisualWordsManager->featureExtractorFunction(samplingResults,
                                                                                  bagOfVisualWordsManager);
        for (size_t j = 0; j < featureMatrix->numberRows; ++j) {
            VECTOR_PUSH_BACK(int,visualWordsLabels,trueLabelImage);
        }
        Matrix* newData = stackVerticallyMatrices(allFeatures,featureMatrix);
        destroyMatrix(&allFeatures);
        allFeatures = newData;
        destroyImage(&image);
        destroyVector(&samplingResults);
        destroyMatrix(&featureMatrix);
    }


    if(bagOfVisualWordsManager->storeVisualWordsData){
        if(bagOfVisualWordsManager->dataVisualWords != NULL){
            destroyMatrix(&(bagOfVisualWordsManager->dataVisualWords));
        }
        if(bagOfVisualWordsManager->labelsVisualWords != NULL){
            destroyVector(&(bagOfVisualWordsManager->labelsVisualWords));
        }
        bagOfVisualWordsManager->dataVisualWords = allFeatures;
        bagOfVisualWordsManager->labelsVisualWords = visualWordsLabels;

    }

    printf("[computeDictionary] Finding Visual words...\n");
    bagOfVisualWordsManager->dictionary = bagOfVisualWordsManager->clusteringFunction(allFeatures,
                                                                                      bagOfVisualWordsManager);
    if(!bagOfVisualWordsManager->storeVisualWordsData){
        destroyMatrix(&allFeatures);
        destroyVector(&visualWordsLabels);
    }
    printf("[computeDictionary] Dictionary computed\n");
}





void trainClassifier(BagOfVisualWordsManager* bagOfVisualWordsManager){
    if(!bagOfVisualWordsManager->imageSamplerFunction){
        printf("[trainClassifier] Sampler function not defined\n");
    }


    if(!bagOfVisualWordsManager->featureExtractorFunction){
        printf("[trainClassifier] Feature extractor function not defined\n");
        return;
    }
    if(bagOfVisualWordsManager->dictionary == NULL){
        printf("[trainClassifier] dictionary is empty\n");
        return;
    }
    if(bagOfVisualWordsManager->mountHistogramFunction == NULL){
        printf("[trainClassifier] Mounter histogram function not defined\n");
        return;
    }
    if(bagOfVisualWordsManager->fitFunction == NULL){
        printf("[trainClassifier] Fit function not defined\n");
        return;
    }


    Matrix *bowHistograms = createMatrix(bagOfVisualWordsManager->pathsToImages_train->size,
                                         bagOfVisualWordsManager->dictionary->numberRows,
                                         sizeof(float));
    GVector *imagesLabels = createNullVector(bagOfVisualWordsManager->pathsToImages_train->size,sizeof(int));
    //Matrix *bowHistograms = NULL;
    printf("[trainClassifier] Generating histograms and labels from images\n");
    for (size_t index = 0; index < bagOfVisualWordsManager->pathsToImages_train->size; ++index) {
        char* imagePath = VECTOR_GET_ELEMENT_AS(char*,bagOfVisualWordsManager->pathsToImages_train,index);
        Image* image = readImage(imagePath);
        if(image == NULL){
            printf("[computeDictionary] Invalid image path: %s",imagePath);
            continue;
        }
        GVector* samplingResults = NULL;
        if(bagOfVisualWordsManager->imageSamplerFunction){
            samplingResults = bagOfVisualWordsManager->imageSamplerFunction(image,
                                                                            bagOfVisualWordsManager);
            samplingResults->freeFunction = bagOfVisualWordsManager->freeFunction2SamplerOutput;
        }else{
            samplingResults = createNullVector(1,sizeof(Image*));
            VECTOR_GET_ELEMENT_AS(Image*,samplingResults,0) = image;
        }
        Matrix* featureMatrix = bagOfVisualWordsManager->featureExtractorFunction(samplingResults, bagOfVisualWordsManager);


        GVector* histogram = bagOfVisualWordsManager->mountHistogramFunction(featureMatrix,bagOfVisualWordsManager);
        setRowValueGivenVector(bowHistograms,histogram,index);
        VECTOR_GET_ELEMENT_AS(int,imagesLabels,index) = findTrueLabelInName(imagePath);

        destroyImage(&image);
        destroyMatrix(&featureMatrix);
        destroyVector(&histogram);
        destroyVector(&samplingResults);
    }

    printf("[trainClassifier] Histograms and labels generated\n");
    printf("[trainClassifier] Classifier  trainning...\n");
    bagOfVisualWordsManager->fitFunction(bowHistograms,imagesLabels,bagOfVisualWordsManager->classifier);
    printf("[trainClassifier] Classifier trained...\n");

    if(bagOfVisualWordsManager->storeTrainData){
        if(bagOfVisualWordsManager->histogramsTraining){
            destroyMatrix(&(bagOfVisualWordsManager->histogramsTraining));
        }
        bagOfVisualWordsManager->histogramsTraining = bowHistograms;

        if(bagOfVisualWordsManager->labelsTraining){
            destroyVector(&(bagOfVisualWordsManager->labelsTraining));
        }
        bagOfVisualWordsManager->labelsTraining = imagesLabels;
    }else{
        destroyMatrix(&bowHistograms);
        destroyVector(&imagesLabels);
    }
}

void GenerateHistogramsForDataTrain(BagOfVisualWordsManager* bagOfVisualWordsManager){
    if(!bagOfVisualWordsManager->imageSamplerFunction){
        printf("[GenerateHistogramsForDataTrain] Sampler function not defined\n");
    }


    if(!bagOfVisualWordsManager->featureExtractorFunction){
        printf("[GenerateHistogramsForDataTrain] Feature extractor function not defined\n");
        return;
    }
    if(bagOfVisualWordsManager->dictionary == NULL){
        printf("[GenerateHistogramsForDataTrain] dictionary is empty\n");
        return;
    }
    if(bagOfVisualWordsManager->mountHistogramFunction == NULL){
        printf("[GenerateHistogramsForDataTrain] Mounter histogram function not defined\n");
        return;
    }

    Matrix *bowHistograms = createMatrix(bagOfVisualWordsManager->pathsToImages_train->size,
                                         bagOfVisualWordsManager->dictionary->numberRows,
                                         sizeof(float));
    GVector *imagesLabels = createNullVector(bagOfVisualWordsManager->pathsToImages_train->size,sizeof(int));
    //Matrix *bowHistograms = NULL;
    printf("[GenerateHistogramsForDataTrain] Generating histograms and labels from images\n");
    for (size_t index = 0; index < bagOfVisualWordsManager->pathsToImages_train->size; ++index) {
        char* imagePath = VECTOR_GET_ELEMENT_AS(char*,bagOfVisualWordsManager->pathsToImages_train,index);
        Image* image = readImage(imagePath);
        bagOfVisualWordsManager->currentImage = image;
        if(image == NULL){
            printf("[computeDictionary] Invalid image path: %s",imagePath);
            continue;
        }
        GVector* samplingResults = NULL;
        if(bagOfVisualWordsManager->imageSamplerFunction){
            samplingResults = bagOfVisualWordsManager->imageSamplerFunction(image,
                                                                            bagOfVisualWordsManager);
            samplingResults->freeFunction = bagOfVisualWordsManager->freeFunction2SamplerOutput;
        }else{
            samplingResults = createNullVector(1,sizeof(Image*));
            VECTOR_GET_ELEMENT_AS(Image*,samplingResults,0) = image;
        }
        Matrix* featureMatrix = bagOfVisualWordsManager->featureExtractorFunction(samplingResults, bagOfVisualWordsManager);


        GVector* histogram = bagOfVisualWordsManager->mountHistogramFunction(featureMatrix,bagOfVisualWordsManager);
        setRowValueGivenVector(bowHistograms,histogram,index);
        VECTOR_GET_ELEMENT_AS(int,imagesLabels,index) = findTrueLabelInName(imagePath);

        destroyImage(&image);
        destroyMatrix(&featureMatrix);
        destroyVector(&histogram);
        destroyVector(&samplingResults);
    }

    printf("[GenerateHistogramsForDataTrain] Histograms and labels generated\n");

    if(bagOfVisualWordsManager->storeTrainData){
        if(bagOfVisualWordsManager->histogramsTraining){
            destroyMatrix(&(bagOfVisualWordsManager->histogramsTraining));
        }
        bagOfVisualWordsManager->histogramsTraining = bowHistograms;

        if(bagOfVisualWordsManager->labelsTraining){
            destroyVector(&(bagOfVisualWordsManager->labelsTraining));
        }
        bagOfVisualWordsManager->labelsTraining = imagesLabels;
    }else{
        destroyMatrix(&bowHistograms);
        destroyVector(&imagesLabels);
    }
}


GVector* predictLabels(BagOfVisualWordsManager* bagOfVisualWordsManager){
    if(!bagOfVisualWordsManager->imageSamplerFunction){
        printf("[predictLabels] Sampler function not defined\n");
    }


    if(!bagOfVisualWordsManager->featureExtractorFunction){
        printf("[predictLabels] Feature extractor function not defined\n");
        return NULL;
    }
    if(bagOfVisualWordsManager->dictionary == NULL){
        printf("[predictLabels] dictionary is empty\n");
        return NULL;
    }
    if(bagOfVisualWordsManager->mountHistogramFunction == NULL){
        printf("[predictLabels] Mounter histogram function not defined\n");
        return NULL;
    }
    if(bagOfVisualWordsManager->predictFunction == NULL){
        printf("[predictLabels] Predict function not defined\n");
        return NULL;
    }
    Matrix *bowHistograms = createMatrix(bagOfVisualWordsManager->pathsToImages_test->size,
                                         bagOfVisualWordsManager->dictionary->numberRows,
                                         sizeof(float));
    printf("[predictLabels] Generating histograms and labels from images\n");
    for (size_t index = 0; index < bagOfVisualWordsManager->pathsToImages_test->size; ++index) {
        char* imagePath = VECTOR_GET_ELEMENT_AS(char*,bagOfVisualWordsManager->pathsToImages_test,index);
        Image* image = readImage(imagePath);
        bagOfVisualWordsManager->currentImage = image;
        if(image == NULL){
            printf("[predictLabels] Invalid image path: %s",imagePath);
            continue;
        }
        GVector* samplingResults = NULL;
        if(bagOfVisualWordsManager->imageSamplerFunction){
            samplingResults = bagOfVisualWordsManager->imageSamplerFunction(image,
                                                                            bagOfVisualWordsManager);
            samplingResults->freeFunction = bagOfVisualWordsManager->freeFunction2SamplerOutput;
        }else{
            samplingResults = createNullVector(1,sizeof(Image*));
            VECTOR_GET_ELEMENT_AS(Image*,samplingResults,0) = image;
        }
        Matrix* featureMatrix = bagOfVisualWordsManager->featureExtractorFunction(samplingResults, bagOfVisualWordsManager);

        GVector* histogram = bagOfVisualWordsManager->mountHistogramFunction(featureMatrix,bagOfVisualWordsManager);
        setRowValueGivenVector(bowHistograms,histogram,index);

        destroyImage(&image);
        destroyMatrix(&featureMatrix);
        destroyVector(&histogram);
        destroyVector(&samplingResults);
    }
    printf("[predictLabels] Histograms and labels generated\n");
    printf("[predictLabels] Predicting labels...\n");


    GVector* labelsPredicted = bagOfVisualWordsManager->predictFunction(bowHistograms,bagOfVisualWordsManager->classifier);
    printf("[predictLabels] Labels predicted...\n");
    if(bagOfVisualWordsManager->storePredictedData){
        if(bagOfVisualWordsManager->histogramsPredictSamples){
            destroyMatrix(&(bagOfVisualWordsManager->histogramsPredictSamples));
        }
        bagOfVisualWordsManager->histogramsPredictSamples = bowHistograms;
        if(bagOfVisualWordsManager->labelsPredicted){
            destroyVector(&(bagOfVisualWordsManager->labelsPredicted));
        }
        bagOfVisualWordsManager->labelsPredicted = labelsPredicted;
    }else{
        destroyMatrix(&bowHistograms);
    }
    return labelsPredicted;
}

GVector* computeHardAssignmentHistogram_bow(Matrix *featureMatrix, BagOfVisualWordsManager *bagOfVisualWordsManager){
    GVector* bowHistogram = createNullVector(bagOfVisualWordsManager->dictionary->numberRows,sizeof(float));
    for (size_t patchIndex = 0; patchIndex < featureMatrix->numberRows; ++patchIndex) {
        size_t nearestClusterIndex = findNearestRow(bagOfVisualWordsManager->dictionary,
                                                    featureMatrix,patchIndex,
                                                    bagOfVisualWordsManager->distanceFunction,
                                                    bagOfVisualWordsManager->argumentListOfDistanceFunction);
        VECTOR_GET_ELEMENT_AS(float,bowHistogram,nearestClusterIndex) += 1.0;
    }
    double total = 0;
    for (size_t i = 0; i < bowHistogram->size; ++i) {
        VECTOR_GET_ELEMENT_AS(float,bowHistogram,i) /= featureMatrix->numberRows;
        total +=VECTOR_GET_ELEMENT_AS(float,bowHistogram,i);
    }
    return bowHistogram;
}



GVector* computeSoftAssignmentHistogram_bow(Matrix *featureMatrix, BagOfVisualWordsManager *bagOfVisualWordsManager){
    Matrix *clusters = bagOfVisualWordsManager->dictionary;
    Matrix* samples = featureMatrix;
    ArgumentList* argumentList = bagOfVisualWordsManager->argumentListOfHistogramMounter;
    int inverseDistanceFunctionId = ARGLIST_GET_ELEMENT_AS(int,argumentList,0);
    DistanceFunction distanceFunction = ARGLIST_GET_ELEMENT_AS(DistanceFunction,argumentList,1);
    ArgumentList* argumentListDistanceFunction = ARGLIST_GET_ELEMENT_AS(ArgumentList*,argumentList,2);
    return computeSoftAssignmentHistogram(clusters,samples,inverseDistanceFunctionId,distanceFunction,argumentListDistanceFunction);
}



Matrix* kmeansClusteringBow(Matrix* featureMatrix, BagOfVisualWordsManager* bagOfVisualWordsManager){
    ArgumentList* argumentList = bagOfVisualWordsManager->argumentListOfClustering;
    if(argumentList->length <= 0){
        printf("[kmeansClustering] invalid argument list\n");
        return NULL;
    }
    if(featureMatrix->numberRows == 0 || featureMatrix->numberColumns == 0){
        printf("[kmeansClustering] invalid matrix dimension: R%lu C%lu\n",featureMatrix->numberRows,featureMatrix->numberColumns);
        return NULL;
    }

    if(argumentList->length == 1){
        size_t numberOfCluster = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,0);
        return kmeansClustering(featureMatrix,numberOfCluster);
    }
    else if(argumentList->length == 2){
        size_t numberOfCluster = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,0);
        size_t maximumNumberIterations = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,1);
        return kmeansClustering(featureMatrix,numberOfCluster,
                                maximumNumberIterations);
    }else if(argumentList->length == 3){
        size_t numberOfCluster = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,0);
        size_t maximumNumberIterations = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,1);
        double tolerance = ARGLIST_GET_ELEMENT_AS(double,argumentList,2);

        return kmeansClustering(featureMatrix,numberOfCluster,
                                maximumNumberIterations,tolerance);
    }else if(argumentList->length == 4){
        size_t numberOfCluster = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,0);
        size_t maximumNumberIterations = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,1);
        double tolerance = ARGLIST_GET_ELEMENT_AS(double,argumentList,2);
        int seed = ARGLIST_GET_ELEMENT_AS(int,argumentList,3);
        return kmeansClustering(featureMatrix,numberOfCluster,
                                maximumNumberIterations,tolerance,seed);
    }else if (argumentList->length == 6){
        size_t numberOfCluster = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,0);
        size_t maximumNumberIterations = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,1);
        double tolerance = ARGLIST_GET_ELEMENT_AS(double,argumentList,2);
        int seed = ARGLIST_GET_ELEMENT_AS(int,argumentList,3);
        DistanceFunction distanceFunction = ARGLIST_GET_ELEMENT_AS(DistanceFunction ,argumentList,4);
        ArgumentList* distanceFunctionArgs = ARGLIST_GET_ELEMENT_AS(ArgumentList* ,argumentList,5);
        return kmeansClustering(featureMatrix,numberOfCluster,
                                maximumNumberIterations,tolerance,seed,
                                distanceFunction, distanceFunctionArgs);
    }else{
        printf("[kmeansClusteringBow] invalid arguments for kmeans\n");
    }
    return NULL;

}



Matrix* clusteringSupervisedBow(Matrix* featureMatrix, BagOfVisualWordsManager* bagOfVisualWordsManager){

    ArgumentList* argumentList = bagOfVisualWordsManager->argumentListOfClustering;
    int nclasses = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,0);
    DistanceFunction distanceFunction = ARGLIST_GET_ELEMENT_AS(DistanceFunction,argumentList,1);
    ArgumentList* argumentListDistanceFunction = ARGLIST_GET_ELEMENT_AS(ArgumentList*,argumentList,2);

    return clusteringSupervised(bagOfVisualWordsManager->dataVisualWords,
                                bagOfVisualWordsManager->labelsVisualWords,
                                nclasses,distanceFunction,argumentListDistanceFunction);
}

Matrix* clusteringGetAllSampleBow(Matrix* featureMatrix, BagOfVisualWordsManager* bagOfVisualWordsManager){
    return clusteringGetAllSample(featureMatrix);
}

//Matrix* computeHogDescriptorForSuperPixelsInterestPointsBow(GVector* vector_image,BagOfVisualWordsManager* bagOfVisualWordsManager){
//    ArgumentList* argumentList = bagOfVisualWordsManager->argumentListOfFeatureExtractor;
//    HogManager* hogManager = ARGLIST_GET_ELEMENT_AS(HogManager*,argumentList,0);
//    size_t patchSizeX = ARGLIST_GET_ELEMENT_AS(size_t ,argumentList,1);
//    size_t patchSizeY = ARGLIST_GET_ELEMENT_AS(size_t ,argumentList,2);
//    return computeHogDescriptorForSuperPixelsInterestPoints(vector_image,hogManager,patchSizeX,patchSizeY);
//}