#ifndef _KERNEL_H_
#define _KERNEL_H_


#include "common.h"
#include "adjacencyRelation.h"
#include "featureVector.h"



typedef struct _kernel {
  AdjacencyRelation *adjacencyRelation;
  float *weight;
} Kernel;

//aloca memoria para a estrutura do kernel e cria uma relacao de adjacencia e inicializa os pesos
Kernel *createKernel(AdjacencyRelation *adjRel);
Kernel *createKernel(FeatureVector* weights);

//destroi uma estrutura de kernel
void destroyKernel(Kernel **kernel);

//define os pesos do kernel
void setKernelWeights(Kernel *kernel,float* weights, int n);
void setKernelWeights(Kernel *kernel,FeatureVector* weights);

//rotationa o kernel
void rotateKernel2D(Kernel *kernel, float angle_rad);
Kernel *copyKernel(Kernel* kernel);


//filtros
Kernel *createMean3by3();
Kernel *createMeanFilter();
Kernel *createGaussian3x3();
Kernel *createMeanFilter();






#endif
