//
// Created by deangeli on 3/22/17.
//

#ifndef _FILTERING_H_
#define _FILTERING_H_

#include "common.h"
#include "kernel.h"
#include "image.h"

/*
 * Material suplementar para o entendimento da correlacao
 * http://www.cs.umd.edu/~djacobs/CMSC426/Convolution.pdf
 *
 * Paginas uteis para o desenvolvimento das funcoes
 * pg 3 e 4 contem um exemplo ilustrativo do como e computado a correlcao
 * pg 4 definicao matematica da correlacao
 * pg 7,8 exemplo de matching utilizando correlacao
 * pg 9 correlacao em 2D
 * pg 11 definicao matematica de convolucao
 * */

/*computa a correlação cruzada entre um sinal e filtro considerando que ambos são vetores/imagem finitos.
 * */
FeatureVector* correlation1D(FeatureVector*X1,FeatureVector*X2,bool normalization);
FeatureVector* correlation1D(FeatureVector*X1,Kernel *kernel);
GrayImage* correlation2D(GrayImage* image,Kernel *kernel);
//n e o tamanho do vetor de saida
float* correlation1D(FeatureVector*X1,Kernel kernel, int *n);



/*computa a correlação cruzada entre um sinal e filtro considerando que ambos são vetores/imagem circulares.
 * */
FeatureVector* circularCrossCorrelation1D(FeatureVector*X1,FeatureVector*X2,bool normalization);
FeatureVector* circularCrossCorrelation1D(FeatureVector*X1, Kernel *kernel);
GrayImage* circularCorrelation2D(GrayImage* image,Kernel *kernel);
//n e o tamanho do vetor de saida
float* circularCrossCorrelation1D(FeatureVector*X1,Kernel *kernel, int *n);





/*computa a convlucao entre um sinal e filtro considerando que ambos são vetores/imaegem finitos.
 * */
FeatureVector* covolution1D(FeatureVector*X1,FeatureVector*X2);
FeatureVector* covolution1D(FeatureVector*X1,Kernel *kernel);
GrayImage* covolution2D(GrayImage* image,Kernel *kernel);
//n e o tamanho do vetor de saida
float* covolutionn1D(FeatureVector*X1,Kernel *kernel, int n);

/*computa a convlucao entre um sinal e filtro considerando que ambos são vetores/imaegem circulares.
 * */
FeatureVector* circularCovolution1D(FeatureVector*X1,FeatureVector*X2);
FeatureVector* circularCovolution1D(FeatureVector*X1,Kernel *kernel);
GrayImage* covolution2D(GrayImage* image,Kernel *kernel);
//n e o tamanho do vetor de saida
float* circularCovolution1D(FeatureVector*X1,Kernel *kernel, int n);


#endif //LIBFL_FILTERING_H
