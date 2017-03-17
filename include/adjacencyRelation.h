//
// Created by deangeli on 3/14/17.
//

#ifndef _ADJACENCYTRELATION_H_
#define _ADJACENCYTRELATION_H_

#include "common.h"

typedef struct _adjrel {
    /* displacements to achieve the n adjacent voxels. */
    int *dx, *dy;
    /* number of adjacent pixels/voxels. */
    int n;
} AdjacencyRelation;

AdjacencyRelation * createAdjacencyRelation(int n);
void  destroyAdjacencyRelation(AdjacencyRelation **adjRel);
int countNumberAdjacentsOnCircle(float radius);
AdjacencyRelation *createCircularAdjacency(float radius);
void getAdjacentsOnCircle(float radius,AdjacencyRelation* adjRel);
void resizeCircularAdjacency(AdjacencyRelation **adjRel, float radius);
AdjacencyRelation *createLosangeAdjacency(int k);
void resizeLosangeAdjacency(AdjacencyRelation **adjRel,int k);

#endif //LIBFL_ADJACENCYTRELATION_H
