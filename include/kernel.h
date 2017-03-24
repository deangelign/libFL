#ifndef _KERNEL_H_
#define _KERNEL_H_


#include "common.h"
#include "adjacencyRelation.h"


/* Multiband kernel: Kernel *K; K->A->n; K->A->dx[i],
   K->A->dy[i],K->A->dz[i]; K->weight[i]  */
typedef struct _kernel {
  AdjacencyRelation *adjacencyRelation;
  float     *weight;
} Kernel;

//aloca memoria para a estrutura do kernel
Kernel *createKernel(AdjacencyRelation *adjRel);
//destroi uma estrutura de kernel
void destroyKernel(Kernel **K);

//define os pesos do kernel
void setKernelWeights(float* weights);


#endif
