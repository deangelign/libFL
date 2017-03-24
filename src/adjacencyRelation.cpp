//
// Created by deangeli on 3/14/17.
//
#include "adjacencyRelation.h"


AdjacencyRelation * createAdjacencyRelation(int n)
{
    AdjacencyRelation *AdjRel =(AdjacencyRelation *)calloc(1,sizeof(AdjacencyRelation));

    AdjRel->dx = (int *)calloc(n, sizeof(int));
    AdjRel->dy = (int *)calloc(n, sizeof(int));
    AdjRel->n  = n;

    return(AdjRel);
}



void  destroyAdjacencyRelation(AdjacencyRelation **AdjRel) {
    AdjacencyRelation *aux = *AdjRel;

    if (aux != NULL){
        if (aux->dx != NULL) free(aux->dx);
        if (aux->dy != NULL) free(aux->dy);
        free(aux);
        *AdjRel = NULL;
    }
}

AdjacencyRelation *createCircularAdjacency(float radius)
{

    AdjacencyRelation *AdjRel=NULL;
    int numberAdjacents = countNumberAdjacentsInCircle(radius);
    AdjRel = createAdjacencyRelation(numberAdjacents);
    //int indexCerter = AdjRel->n/2;
    getAdjacentsInCircle(radius, AdjRel);

    return AdjRel;
}

void resizeCircularAdjacency(AdjacencyRelation **adjRel,float radius){
    AdjacencyRelation *AdjRelOut = NULL;
    destroyAdjacencyRelation(adjRel);
    AdjRelOut = createCircularAdjacency(radius);
    *adjRel = AdjRelOut;
}

AdjacencyRelation *createLosangeAdjacency(int k)
{
    AdjacencyRelation *adjRel = NULL;
    int numberAdjacents = 0;
    if(k == 0){
        adjRel = createCircularAdjacency(0.0f);
        return adjRel;
    }

    for (int dy = -k; dy <= k; ++dy) {
        for (int dx = -k; dx <= k; ++dx) {
            if(abs(dx) + abs(dy) <= k){
                numberAdjacents++;
            }
        }
    }

    adjRel = createAdjacencyRelation(numberAdjacents);
    int index = 0;
    for (int dy = -k; dy <= k; ++dy) {
        for (int dx = -k; dx <= k; ++dx) {
            if(abs(dx) + abs(dy) <= k){
                adjRel->dx[index] = dx;
                adjRel->dy[index] = dy;
                index++;
            }
        }
    }

    return adjRel;
}

void resizeLosangeAdjacency(AdjacencyRelation **adjRel,int k){
    AdjacencyRelation *AdjRelOut = NULL;
    destroyAdjacencyRelation(adjRel);
    AdjRelOut = createLosangeAdjacency(k);
    *adjRel = AdjRelOut;
}

int countNumberAdjacentsInCircle(float radius){
    int radius_int = radius;
    int counter = 0;
    float radius_squared = (radius*radius)+0.1;
    for(int dy= -radius_int;dy <= radius_int; dy++){
        for(int dx= -radius_int;dx <= radius_int; dx++){
            int distance = (dx*dx) + (dy*dy);
            if( distance <= radius_squared){
                counter++;
            }
        }
    }
    return counter;
}

void getAdjacentsInCircle(float radius,AdjacencyRelation* AdjRel){
    if(AdjRel == NULL){
        return;
    }
    int radius_int = radius;

    float radius_squared = (radius*radius)+0.1;
    int i=0;
    for(int dy= -radius_int;dy <= radius_int; dy++){
        for(int dx= -radius_int;dx <= radius_int; dx++){
            int distance = (dx*dx) + (dy*dy);
            if( distance <= radius_squared){
                AdjRel->dx[i] = dx;
                AdjRel->dy[i] = dy;
                i++;
            }
        }
    }
}

AdjacencyRelation *createLineAdjacency(int n, int origin){
    return NULL;
}


AdjacencyRelation *copyAdjacency(AdjacencyRelation *adjacencyRelation){
    return NULL;
}