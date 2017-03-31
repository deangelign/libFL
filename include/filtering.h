//
// Created by deangeli on 3/22/17.
//

#ifndef _FILTERING_H_
#define _FILTERING_H_

#include "common.h"
#include "kernel.h"
#include "image.h"


/*
 * Material suplementar para o entendimento de correlacao
 * http://www.cs.umd.edu/~djacobs/CMSC426/Convolution.pdf
 *
 * Paginas uteis para o desenvolvimento das funcoes
 * pg 3 e 4 contem um exemplo ilustrativo do como e computado a correlcao
 * pg 4 definicao matematica da correlacao
 * pg 7,8 exemplo de matching utilizando correlacao
 * pg 9 correlacao em 2D
 * pg 11 definicao matematica de convolucao
 * */

/*computa a correlação cruzada entre X e Y considerando que ambos são vetores finitos.
 * Se X = Y, entao temos a autocorrelacao do sinal. Se normalized é True então devemos computar
 * a correlação normalizada.
 * */



/*computa a correlação cruzada entre X e Y considerando que ambos são arrays circulares.
 * Se X = Y, entao temos a autocorrelacao do sinal. Se normalized é True então devemos computar
 * a correlação normalizada.
 * */
float* circularCrossCorrelation1D(float *X1,float *X2,int N1,int N2,bool normalization);
FeatureVector* circularCrossCorrelation1D(FeatureVector*X1,FeatureVector*X2,bool normalization);
FeatureVector* circularCrossCorrelation1D(FeatureVector* featureVector, Kernel* kernel ,bool normalization);

/*
 * type 1 - norma l1
 * type 2 - norma l2
 * */
FeatureVector* matchingWithCorrelation(FeatureVector*X1,FeatureVector*X2, int type);



//filtragem
Image* convolveImagewithMeanFilter(Image* image);
Image* convolveImagewithGaussianFilter(Image* image);
Image* convolveImagewithMedianFilter(Image* image);
Image* convolveImagewithSobelFilter(Image* image);
Image* convolveImagewithLaplacianFilter(Image* image);
Image* convolveImagewithDOG(Image* image);
Image* convolveImagewithLOG(Image* image);

#endif //LIBFL_FILTERING_H
