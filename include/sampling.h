//
// Created by deangeli on 5/20/17.
//

#ifndef _SAMPLING_H
#define _SAMPLING_H

#include "vector.h"
#include "image.h"
#include "adjacencyRelation.h"
#include "kernel.h"
#include "filtering.h"

#ifdef USE_IFT
#include "iftImage.h"
#endif

GVector* gridSampling(Image* image, size_t patchSizeX,size_t patchSizeY);

GVector* randomSampling_noImage(Image* image, size_t numberOfPatchs, size_t patchSizeX,size_t patchSizeY,int seed = 0);

GVector* samplingSuperPixelsEdges(Image* originalImage, Image* labelImage, Image* gradienImageMagnitude,
                                  size_t patchSizeX,size_t patchSizeY);

#endif //LIBFL_SAMPLING_H
