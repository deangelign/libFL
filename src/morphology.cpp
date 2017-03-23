//
// Created by deangeli on 3/15/17.
//
#include "morphology.h"


GrayImage *dilate(GrayImage *image, AdjacencyRelation *AdjRel){
    GrayImage *dilated = createGrayImage(image->ncols,image->nrows);
    int n = image->ncols*image->nrows;

    for (int p=0; p < n; p++) {
        int xp = p % image->ncols;
        int yp = p / image->ncols;
        dilated->val[p] = image->val[p];
        for (int j = 1; j < AdjRel->n; j++) {
            int xq = xp + AdjRel->dx[j];
            int yq = yp + AdjRel->dy[j];
            if (isValidPixelCoordinate(image, xq, yq)) {
                int q = xq + yq * image->ncols;
                if (image->val[q] > dilated->val[p])
                    dilated->val[p] = image->val[q];
            }
        }
    }

    return dilated;
}


GrayImage *erode(GrayImage *image, AdjacencyRelation *AdjRel){
    GrayImage *eroded = createGrayImage(image->ncols,image->nrows);
    int n = image->ncols*image->nrows;

    for (int p=0; p < n; p++) {
        int xp = p % image->ncols;
        int yp = p / image->ncols;
        eroded->val[p] = image->val[p];
        for (int j = 1; j < AdjRel->n; j++) {
            int xq = xp + AdjRel->dx[j];
            int yq = yp + AdjRel->dy[j];
            if (isValidPixelCoordinate(image, xq, yq)) {
                int q = xq + yq * image->ncols;
                if (image->val[q] < eroded->val[p])
                    eroded->val[p] = image->val[q];
            }
        }
    }
    return eroded;
}




GrayImage *open(GrayImage *image, AdjacencyRelation *AdjRel){
    GrayImage *dilated,*eroded;

    eroded  = erode(image,AdjRel);
    dilated = dilate(eroded,AdjRel);
    destroyGrayImage(&eroded);
    return(dilated);
}

GrayImage *close(GrayImage *image, AdjacencyRelation *AdjRel){
    GrayImage *dilated,*eroded;

    dilated  = dilate(image,AdjRel);
    eroded   = erode(dilated,AdjRel);
    destroyGrayImage(&dilated);
    return(eroded);
}



GrayImage *topHat(GrayImage *image,AdjacencyRelation *AdjRel){
    return NULL;
}

GrayImage *bottomHat(GrayImage *image,AdjacencyRelation *AdjRel){
    return NULL;
}

GrayImage *morphologicGradient(GrayImage *image,AdjacencyRelation *AdjRel){
    return NULL;
}




float *applyGranulometryOnImage(GrayImage *image, float startRadius, float endRadius) {
   float *dv;

    if (startRadius < endRadius) {
        int k = (endRadius - startRadius) / 0.5;
        float *v = (float*)calloc(k + 1, sizeof(float));
        dv = (float*)calloc(k + 1, sizeof(float));
        float imageVolume = sumUpAllPixelsValues(image);
        float radius = startRadius;

        v[0] = 0;
        for (int j = 1; j <= k; j++) {
            AdjacencyRelation *A = createCircularAdjacency(radius);
            GrayImage *J = open(image, A);
            float volume = sumUpAllPixelsValues(J);
            v[j] = 1.0 - (volume / imageVolume);
            destroyGrayImage(&J);
            destroyAdjacencyRelation(&A);
        }
        dv[0] = 0;
        for (int j = 1; j <= k; j++) {
            dv[j] = v[j] - v[j - 1];
        }
        free(v);
    } else {
        printf("startRadius %f must be lower than endRadius %f", startRadius, endRadius);
        exit(-1);
    }
    return dv;
}

float *applyGranulometryOnImage(GrayImage *image, int k){
    return NULL;
}