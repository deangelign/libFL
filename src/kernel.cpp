#include "kernel.h"

Kernel *createKernel(AdjacencyRelation *adjRel){
    Kernel* kernel = NULL;
    size_t size = adjRel->n;
    kernel  = (Kernel*) calloc(1,sizeof(Kernel));
    kernel->adjacencyRelation = copyAdjcencyRelation(adjRel);
    kernel->weight = (float*)calloc( size , sizeof(float) );
    return kernel;
}

Kernel *createKernel(FeatureVector* weights){
    Kernel* kernel = NULL;
    size_t size = (size_t)weights->size;
    kernel  = (Kernel*) calloc(1,sizeof(Kernel));
    kernel->weight = (float*)calloc(size,sizeof(float));
    AdjacencyRelation* adjacencyRelation = createAdjacencyRelation(weights->size);
    int centerX = weights->size/2;
    adjacencyRelation->maxDx = centerX;
    adjacencyRelation->maxDy = 0;
    for (int i = 0; i < weights->size; ++i) {
        adjacencyRelation->dx[i] = i - centerX;
        adjacencyRelation->dy[i] = 0;
        kernel->weight[i] = weights->features[i];
    }
    kernel->adjacencyRelation = adjacencyRelation;
    return kernel;
}


void destroyKernel(Kernel **kernel){
    if((*kernel) == NULL){
        return;
    }
    destroyAdjacencyRelation(&((*kernel)->adjacencyRelation));
    free((*kernel)->weight);
    free(*kernel);
    *kernel = NULL;
}

void setKernelWeights(Kernel* kernel, float* weights, int n){
    if(n != kernel->adjacencyRelation->n){
        printf("dimensions mismatch\n");
    }
    for (int i = 0; i < kernel->adjacencyRelation->n; ++i) {
        kernel->weight[i] = weights[i];
    }
}

void setKernelWeights(Kernel *kernel,FeatureVector* weights){
    if(weights->size != kernel->adjacencyRelation->n){
        printf("dimensions mismatch\n");
    }
    for (int i = 0; i < kernel->adjacencyRelation->n; ++i) {
        kernel->weight[i] = weights->features[i];
    }
}

void rotateKernel2D(Kernel *kernel, float angle_rad){
    for (int i = 0; i < kernel->adjacencyRelation->n; ++i) {
        kernel->adjacencyRelation->dx[i] = round((cos(angle_rad)*kernel->adjacencyRelation->dx[i]) +
                                                 (-sin(angle_rad)* kernel->adjacencyRelation->dy[i]));

        kernel->adjacencyRelation->dy[i] = round((sin(angle_rad)*kernel->adjacencyRelation->dx[i]) +
                                                 (cos(angle_rad)* kernel->adjacencyRelation->dy[i]));

        if(abs(kernel->adjacencyRelation->dx[i]) >  kernel->adjacencyRelation->maxDx){
            kernel->adjacencyRelation->maxDx = abs(kernel->adjacencyRelation->dx[i]);
        }
        if(abs(kernel->adjacencyRelation->dy[i]) >  kernel->adjacencyRelation->maxDy){
            kernel->adjacencyRelation->maxDy = abs(kernel->adjacencyRelation->dy[i]);
        }
    }
}

void rotateKernel2D_degree(Kernel *kernel, float angle_degree){
    float angle_rad = (angle_degree * M_PI)/180.0;
    rotateKernel2D(kernel,angle_rad);
}

Kernel *copyKernel(Kernel* kernel){
    Kernel* kernelCopy = createKernel(kernel->adjacencyRelation);
    setKernelWeights(kernelCopy,kernel->weight,kernel->adjacencyRelation->n);
    return kernelCopy;
}
