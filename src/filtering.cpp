//
// Created by deangeli on 3/22/17.
//
#include "filtering.h"

float* circularCrossCorrelation1D(float *X1,float *X2,int N1,int N2,bool normalization){
    float* signal = NULL;
    float* filter = NULL;
    float* output = NULL;
    int signalSize;
    int filterSize;

    if(N1 > N2){
        signal = X1;
        filter = X2;
        signalSize = N1;
        filterSize = N2;
    }else{
        filter = X1;
        signal = X2;
        signalSize = N2;
        filterSize = N1;
    }
    output = (float*)calloc(signalSize,sizeof(float));
    int filterCenter = signalSize/2;
    double sum;
    double signalSquared;
    double filterSquared;
    for (int i = 0; i < signalSize; ++i) {
        sum = 0;
        signalSquared = 0;
        filterSquared = 0;
        for (int j = 0; j < filterSize; ++j) {
            int index = i-filterCenter+j;
            if(index < 0){
                index = signalSize+index;
            }
            if(index >= signalSize){
                index %= signalSize;
            }
            sum += signal[index]*filter[j];
            if (normalization){
                signalSquared += signal[index]*signal[index];
                filterSquared += filter[j]*filter[j];
            }
        }
        if (normalization){
            double denominator = sqrt(signalSquared)*sqrt(filterSquared) + 0.000001;
            sum /= denominator;
        }
        output[i] = (float)sum;

    }
    return output;
}

FeatureVector* circularCrossCorrelation1D(FeatureVector*X1,FeatureVector*X2,bool normalization){
    FeatureVector* signal = NULL;
    FeatureVector* filter = NULL;
    FeatureVector* output = NULL;

    if(X1->size > X2->size){
        signal = X1;
        filter = X2;
    }else{
        filter = X1;
        signal = X2;
    }
    output = createFeatureVector(signal->size);

    int filterCenter = filter->size/2;
    double sum;
    double signalSquared;
    double filterSquared;
    for (int i = 0; i < signal->size; ++i) {
        sum = 0;
        signalSquared = 0;
        filterSquared = 0;
        for (int j = 0; j < filter->size; ++j) {
            int index = i-filterCenter+j;
            if(index < 0){
                index = signal->size+index;
            }
            if(index >= signal->size){
                index %= signal->size;
            }
            sum += signal->features[index]*filter->features[j];
            if (normalization){
                signalSquared += signal->features[index]*signal->features[index];
                filterSquared += filter->features[j]*filter->features[j];
            }
        }
        if (normalization){
            double denominator = sqrt(signalSquared)*sqrt(filterSquared) + 0.000001;
            sum /= denominator;
        }
        output->features[i] = sum;
    }
    return output;
}

FeatureVector* circularCrossCorrelation1D(FeatureVector* featureVector, Kernel* kernel ,bool normalization){
    FeatureVector* output = NULL;
    output = createFeatureVector(featureVector->size);

    int filterCenter = kernel->adjacencyRelation->n/2;
    float sum;
    float signalSquared;
    float filterSquared;
    for (int i = 0; i < featureVector->size; ++i) {
        sum = 0;
        signalSquared = 0;
        filterSquared = 0;
        for (int j = 0; j < kernel->adjacencyRelation->n; ++j) {
            int index = i-filterCenter+j;
            if(index < 0){
                index *= -1;
                index = index%featureVector->size;
                index = featureVector->size-index;
            }
            if(index >= featureVector->size){
                index %= featureVector->size;
            }
            sum += featureVector->features[index]*kernel->weight[j];
            if (normalization){
                signalSquared += featureVector->features[index]*featureVector->features[index];
                filterSquared += kernel->weight[j]*kernel->weight[j];
            }
        }
        if (normalization){
            float denominator = (sqrtf(signalSquared)*sqrtf(filterSquared)) + 0.000001;
            sum /= denominator;
        }
        output->features[i] = (float)sum;
    }
    return output;
}



FeatureVector* Convolution1D(FeatureVector*X1,FeatureVector*X2){
    int sizeOutputArray = 0;
    int windowSize = 0;
    float *signal = NULL;
    float *filter = NULL;
    FeatureVector *outputSignal = NULL;

    if(X1 == NULL || X2 == NULL){
        printf("X1 or/and X2 are NULL\n");
    }
    if (X1->size > X2->size){
        windowSize = X2->size;
        signal = X1->features;
        filter = X2->features;
    }else{
        windowSize = X1->size;
        signal = X2->features;
        filter = X1->features;
    }
    sizeOutputArray = X1->size + X2->size - 1;
    outputSignal = createFeatureVector(sizeOutputArray);
    float sum;
    int index;
    int windowPosition;
    int windowLastPositionIndex = windowSize-1;
    for (int i = 0; i < sizeOutputArray; ++i) {
        windowPosition = i-windowLastPositionIndex;
        sum = 0;
        for (int j = windowLastPositionIndex; j >= 0 ; j--) {
            index = j+windowPosition;
            if(index < 0 || index > X1->size){
                continue;
            }
            sum += signal[index]*filter[windowLastPositionIndex-j];
        }
        outputSignal->features[i] = sum;
    }
    return outputSignal;
}


FeatureVector* matchingWithCorrelation(FeatureVector*X1,FeatureVector*X2, int type){
    FeatureVector* signal = NULL;
    FeatureVector* filter = NULL;
    FeatureVector* output = NULL;

    if(X1->size > X2->size){
        signal = X1;
        filter = X2;
    }else{
        filter = X1;
        signal = X2;
    }
    output = createFeatureVector(signal->size);

    int filterCenter = filter->size/2;
    double sum;
    for (int i = 0; i < signal->size; ++i) {
        sum = 0;
        for (int j = 0; j < filter->size; ++j) {
            int index = i-filterCenter+j;
            if(index < 0){
                index = signal->size+index;
            }
            if(index >= signal->size){
                index %= signal->size;
            }
            if(type == 1){
                sum += fabs(signal->features[index]-filter->features[j]);
            }else if(type == 2){
                sum += (signal->features[index]-filter->features[j])*(signal->features[index]-filter->features[j]);
            }
        }
        output->features[i] = sum;
    }
    return output;
}

Image* convolveImagewithMeanFilter(Image* image){
    return NULL;
}

Image* convolveImagewithGaussianFilter(Image* image){
    return NULL;
}
Image* convolveImagewithMedianFilter(Image* image){
    return NULL;
}
Image* convolveImagewithSobelFilter(Image* image){
    return NULL;
}
Image* convolveImagewithLaplacianFilter(Image* image){
    return NULL;
}
Image* convolveImagewithDOG(Image* image){
    return NULL;
}
Image* convolveImagewithLOG(Image* image){
    return NULL;
}



