//
// Created by deangeli on 3/14/17.
//

#ifndef _ADJACENCYRELATION_H_
#define _ADJACENCYRELATION_H_

#include "common.h"

typedef struct _adjrel {
    /* displacements to achieve the n adjacent voxels. */
    int *dx, *dy;
    /* number of adjacent pixels/voxels. */
    int n;
} AdjacencyRelation;

AdjacencyRelation * createAdjacencyRelation(int n);
void  destroyAdjacencyRelation(AdjacencyRelation **adjRel);
int countNumberAdjacentsInCircle(float radius);
AdjacencyRelation *createCircularAdjacency(float radius);
void getAdjacentsInCircle(float radius,AdjacencyRelation* adjRel);
void resizeCircularAdjacency(AdjacencyRelation **adjRel, float radius);
AdjacencyRelation *createLosangeAdjacency(int k);
void resizeLosangeAdjacency(AdjacencyRelation **adjRel,int k);

/*
 * cria uma em formato de linha (1D). n é o numérp de adjacentes. origin define o a posicao da origem do kernel
 *
 * */
AdjacencyRelation *createLineAdjacency(int n, int origin);
/*
 * copia a relacao de adjacencia
 * */
AdjacencyRelation *copyAdjacency(AdjacencyRelation *adjacencyRelation);

#endif //LIBFL_ADJACENCYRELATION_H
