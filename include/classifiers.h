//
// Created by deangeli on 5/19/17.
//

#ifndef _CLASSIFIERS_H
#define _CLASSIFIERS_H

#include "matrix.h"
#include "matrixUtil.h"
#include "vector.h"
#include "vectorUtil.h"
#include "distanceFunctions.h"
#include "argumentList.h"
#include "svm.h"

typedef void (*FitFunction) (Matrix* matrix,GVector* vector,void* classifierObj);
typedef GVector* (*PredictFunction) (Matrix* matrix,void* classifierObj);


typedef struct _knn_Classifier{
    Matrix* dataMatrixTrain;
    GVector* labelsTrain;
    FitFunction fitFunction;
    PredictFunction predictFunction;
    DistanceFunction distanceFunction;
    ArgumentList* distanceFunctionParameters;
    size_t k;
    size_t nlabels;
}Knn_Classifier;


typedef struct _kmeans_Classifier{
    Matrix* centroides;
    GVector* labelsTrain;
    FitFunction fitFunction;
    PredictFunction predictFunction;
    DistanceFunction distanceFunction;
    ArgumentList* distanceFunctionParameters;
    size_t nlabels;
}Kmeans_Classifier;


//go to ./external/libsvm/include/svm.h for for more details
typedef struct _svm_Classifier{
    struct svm_parameter param;		// set by parse_command_line
    struct svm_model *model;
    int cross_validation;
    int nr_fold;
    bool useSparseDataRepresentation;
    bool isClassification;
    bool predict_probability;
}SVM_Classifier;



//knn-classifier
Knn_Classifier* createKnnClassifier();
Knn_Classifier* createKnnClassifier(Matrix* dataMatrixTrain,GVector* labelsTrain,
                                    DistanceFunction distanceFunction = computeNormalizedL1Norm,
                                    ArgumentList* argumentListDistanceFunction = NULL);
void destroyKnnClassifier(Knn_Classifier** pknnClassifier);
void destroyKnnClassifierForVoidPointer(void* pknnClassifier);
void knnClassifierFit(Knn_Classifier* knnClassifier, Matrix* dataMatrixTrain,GVector* labelsTrain);
void knn_Classifier_fit(Matrix* matrix, GVector* vector,void* knnClassifier);
GVector* knnClassifierPredict(Matrix* matrix,Knn_Classifier* knnClassifier);
GVector* knn_Classifier_predict(Matrix* matrix,void* knnClassifier);

//kmeans-classifier
Kmeans_Classifier* createKmeansClassifier();
void kmeansClassifierFit(Kmeans_Classifier* kmeansClassifier, Matrix* dataMatrixTrain,GVector* labelsTrain);
void kmeans_Classifier_fit(Matrix* matrix, GVector* vector,void* kmeansClassifier);
GVector* kmeansClassifierPredict(Matrix* matrix,Kmeans_Classifier* kmeansClassifier);
GVector* kmeans_Classifier_predict(Matrix* matrix,void* kmeansClassifier);
void destroyKmeansClassifier(Kmeans_Classifier** pkmeansClassifier);
void destroyKmeansClassifierForVoidPointer(void* pkmeansClassifier);


//SVM-Classifier
SVM_Classifier* createSVMClassifier();
void setSVMDefaultParameters(SVM_Classifier* svmClassifier);
void svmClassifierFit(SVM_Classifier* svmClassifier,double* X, size_t nrows,size_t ncols, double* Y);

int convertOriginalDataFormat2SvmFormatTrain_usingSparseRepresentation(double* X, size_t nrows,size_t ncols, double* Y,
                                                                   struct svm_problem prob,
                                                                   struct svm_node *x_space);

int convertOriginalDataFormat2SvmFormatTrain(double* X, size_t nrows,size_t ncols, double* Y,
                                         struct svm_problem prob,
                                         struct svm_node *x_space);

void convertOriginalDataFormat2SvmFormatPredict(double* X, size_t nrows,size_t ncols,
                                                struct svm_node *x_space);

void doCrossValidationOnSVM2FindParameters(SVM_Classifier* svmClassifier);
#endif //LIBFL_CLASSIFIERS_H
