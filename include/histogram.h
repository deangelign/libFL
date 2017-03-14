//
// Created by deangeli on 3/13/17.
//

#ifndef _HISTOGRAM_H_
#define _HISTOGRAM_H_

#include "image.h"
#include "common.h"

typedef struct _histogram {
    float *val;
    int    n;
} Histogram;


Histogram *GrayHistogram(GrayImage *grayImage);
Histogram *GrayHistogram(GrayImage *grayImage, int nbins);
Histogram *GrayHistogram(GrayImage *grayImage, int maxValueAllowed, int minValueAllowed);
Histogram *GrayHistogram(GrayImage *grayImage, int nbins, int maxValueAllowed, int minValueAllowed);
Histogram *GrayHistogramFrom8BitGrayImage(GrayImage *grayImage);
Histogram *ColorHistogram(ColorImage *colorImage, int nbins);
Histogram *ColorHistogramFrom8bitColorIMage(ColorImage *colorImage, int nbinsPerChannel);
GrayImage *DensidadeProbabilidade(ColorImage *img, float K);
GrayImage *DensidadeProbabilidade(GrayImage *img, float K);
GrayImage *DensidadeProbabilidade(GrayImage *img);
GrayImage *DensidadeProbabilidadeWithEuclidianDistance(ColorImage *img, float K);
GrayImage *ProbabilityDensityFunction(ColorImage *img, double stdev);
GrayImage *ProbabilityDensityFunction(GrayImage *img, double stdev);
GrayImage *DensidadeProbabilidade(ColorImage *img, float K);
void WriteHistogram(Histogram *hist, char *filename);
void DestroyHistogram(Histogram **hist);




#endif //_HISTOGRAM_H
