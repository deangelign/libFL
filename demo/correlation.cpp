#include "FL.h"

int main(int argc, char **argv) {
    float signal[] = {3,2,4,1,3,8,4,0,3,8,0,7,7,7,1,2};
    float weights[] = {3,7,5};
    AdjacencyRelation *adjRel = createLineAdjacency(3,1);

    FeatureVector *vec1 = createFeatureVector(signal,16);
    Kernel *kernel = createKernel(adjRel);
    kernel->weight[0] = weights[0];
    kernel->weight[1] = weights[1];
    kernel->weight[2] = weights[2];
    FeatureVector *resultado = circularCrossCorrelation1D(vec1,adjRel);
    printf("saida:\n");
    printFeatureVector(FeatureVector);
    printf("\n");

    FeatureVector **correlations = (FeatureVector **)calloc(1,sizeof(FeatureVector *));
    correlations[0] = resultado;
    wirteFeatureVectors(correlations,1, "R_xy.txt");


    destroyFeatureVector(&resultado);
    destroyFeatureVector(&vec1);
    destroyAdjacencyRelation(&adjRel);
    iftDestroyKernel(&kernel);
    return 0;

}
