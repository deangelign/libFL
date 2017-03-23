//
// Created by deangeli on 3/15/17.
//
#include "image.h"
#include "adjacencyRelation.h"


#ifndef _MORPHOLOGY_H_
#define _MORPHOLOGY_H_

#include "adjacencyRelation.h"

GrayImage *dilate(GrayImage *image, AdjacencyRelation *AdjRel);
GrayImage *erode(GrayImage *image, AdjacencyRelation *AdjRel);
GrayImage *close(GrayImage *image, AdjacencyRelation *AdjRel);
GrayImage *open(GrayImage *image, AdjacencyRelation *AdjRel);
GrayImage *topHat(GrayImage *image,AdjacencyRelation *AdjRel);
GrayImage *bottomHat(GrayImage *image,AdjacencyRelation *AdjRel);
GrayImage *morphologicGradient(GrayImage *image,AdjacencyRelation *AdjRel);
/*computa os pattern spectrums (https://en.wikipedia.org/wiki/Granulometry_(morphology))
 * e gera uma feature para cada um deles. O elemento estruturante usado tem formato de circulo
 * */
float *applyGranulometryOnImage(GrayImage *image, float startRadius, float endRadius);
/*computa os pattern spectrums (https://en.wikipedia.org/wiki/Granulometry_(morphology))
 * e gera uma feature para cada um deles. O elemento estruturante usado tem formato de losango
 * */
float *applyGranulometryOnImage(GrayImage *image, int k);
FeatureVector *getMorphologicalPdf(GrayImage *image, int k);
FeatureVector *getMorphologicalPdf(ColorImage *image, int k);

#endif //LIBFL_MORPHOLOGY_H
