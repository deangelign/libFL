//
// Created by deangeli on 5/20/17.
//

#include "featureExtractor.h"



//iftImage *iftCompute_ISF_MIX_MEAN_Superpixels(iftImage *img, int nsuperpixels, float alpha, float beta, int niters, int smooth_niters, int *nseeds, int *finalniters) {
//    iftImage  *mask1, *seeds, *label;
//    iftMImage *mimg;
//    iftAdjRel *A;
//    iftIGraph *igraph;
//
//    /* Set adjacency relation */
//    if (iftIs3DImage(img)){
//        A      = iftSpheric(1.0);
//    } else {
//        A      = iftCircular(1.0);
//    }
//
//
//    if (iftIsColorImage(img)){
//        /* RGB to Lab conversion */
//        mimg   = iftImageToMImage(img,LABNorm_CSPACE);
//    } else {
//        mimg   = iftImageToMImage(img,GRAY_CSPACE);
//    }
//
//    mask1  = iftSelectImageDomain(mimg->xsize,mimg->ysize,mimg->zsize);
//
//    /* Minima of a basins manifold in that domain */
//    igraph = iftImplicitIGraph(mimg,mask1,A);
//
//    /* Seed sampling for ISF */
//    seeds   = iftAltMixedSampling(mimg,mask1,nsuperpixels);
//
//    *nseeds = iftNumberOfElements(seeds);
//
//    iftDestroyImage(&mask1);
//    iftDestroyMImage(&mimg);
//
//    /* Superpixel segmentation */
//    *finalniters = iftIGraphISF_Mean(igraph,seeds,alpha,beta,niters);
//
//    /* Smooth regions in the label map of igraph */
//    if (smooth_niters > 0){
//        iftIGraphSetWeightForRegionSmoothing(igraph, img);
//        iftIGraphSmoothRegions(igraph, smooth_niters);
//    }
//    /* Get superpixel image */
//    label   = iftIGraphLabel(igraph);
//
//    iftDestroyImage(&seeds);
//    iftDestroyIGraph(&igraph);
//    iftDestroyAdjRel(&A);
//
//
//    return label;
//}



Matrix* computeColorHistogram(GVector* vector_images,size_t nbinsPerChannel){
    int totalNumberBins  = pow(nbinsPerChannel,VECTOR_GET_ELEMENT_AS(Image*,vector_images,0)->nchannels);

    Matrix* matrix = createMatrix(vector_images->size,totalNumberBins,sizeof(float));
    int k = 0;
    for (size_t i = 0; i < vector_images->size; ++i) {
        Image* patch = VECTOR_GET_ELEMENT_AS(Image*,vector_images,i);
        GVector* featureVector = computeHistogramForFeatureVectorGivenNBins(patch,nbinsPerChannel,true);
        for (size_t j = 0; j < matrix->numberColumns; ++j) {
            MATRIX_GET_ELEMENT_BI_AS(float,matrix,k) = VECTOR_GET_ELEMENT_AS(float,featureVector,j);
            k++;
        }
        destroyVector(&featureVector);
    }
    return matrix;
}

HogManager* createHogManager(){
    HogManager* hogManager = (HogManager*)calloc(1,sizeof(HogManager));
    hogManager->binSize = 20;
    hogManager->cellSizeX = 8;
    hogManager->cellSizeY = 8;
    hogManager->cellsPerBlockX = 2;
    hogManager->cellsPerBlockY = 2;
    hogManager->gradientImageMagnitude = NULL;
    hogManager->gradientImagePhase = NULL;
    hogManager->image = NULL;
    hogManager->strideX = hogManager->cellSizeX;
    hogManager->strideY = hogManager->cellSizeY;
    hogManager->useUnsignedGradients = false;
    hogManager->fitBlock = false;
    hogManager->computeGradient = true;
    hogManager->hogFeatureVectorSize = 0;
    hogManager->hogFeatureVector = NULL;
    return hogManager;
}



void compudeGradientImage(HogManager* hogManager){

    Image *monocromaticImage = NULL;
    if(hogManager->image->colorSpace == COLORSPACE_RGB || hogManager->image->colorSpace == COLORSPACE_RGBA){
        monocromaticImage = convertRGBtoYCbCr(hogManager->image);
    }else{
        monocromaticImage = hogManager->image;
    }

    Kernel *kernelX = createSobelHorizontalGradientKernel(3,3);
    Kernel *kernelY = createSobelVerticalGradientKernel(3,3);

    Image *sobelX = convolveImageWithKernel(monocromaticImage, kernelX, false, false);
    Image *sobelY = convolveImageWithKernel(monocromaticImage, kernelY, false, false);


    Image* gradientMagnitude = createImage(sobelX->nx,sobelX->ny,1,sobelX->nchannels);
    Image* gradientphase = createImage(sobelX->nx,sobelX->ny,1,sobelX->nchannels);

    if(hogManager->useUnsignedGradients){

        for (int k = 0; k < sobelX->numberPixels; ++k) {
            double gradientMagnitudeValue = (sobelX->channel[0][k]*sobelX->channel[0][k]) +
                                            (sobelY->channel[0][k]*sobelY->channel[0][k]);

            gradientMagnitudeValue = (float)sqrt(gradientMagnitudeValue);
            double angle = atan2(sobelY->channel[0][k],sobelX->channel[0][k]);
            if(angle < 0){
                angle = M_PI + angle;
            }

            angle *= 180/M_PI; //rad2degree

            gradientMagnitude->channel[0][k] = gradientMagnitudeValue;
            gradientphase->channel[0][k] = angle;
        }

    }else{

        for (int k = 0; k < sobelX->numberPixels; ++k) {
            double gradientMagnitudeValue = (sobelX->channel[0][k]*sobelX->channel[0][k]) +
                                            (sobelY->channel[0][k]*sobelY->channel[0][k]);

            gradientMagnitudeValue = (float)sqrt(gradientMagnitudeValue);
            double angle = atan2(sobelY->channel[0][k],sobelX->channel[0][k]);
            if(angle < 0){
                angle = 2.0*M_PI + angle;
            }
            angle *= 180/M_PI; //rad2degree

            gradientMagnitude->channel[0][k] = gradientMagnitudeValue;
            gradientphase->channel[0][k] = angle;
        }

    }

    if(hogManager->image->colorSpace == COLORSPACE_RGB || hogManager->image->colorSpace == COLORSPACE_RGBA){
        destroyImage(&monocromaticImage);
    }

    destroyKernel(&kernelX);
    destroyKernel(&kernelY);
    destroyImage(&sobelX);
    destroyImage(&sobelY);

    hogManager->gradientImageMagnitude = gradientMagnitude;
    hogManager->gradientImagePhase = gradientphase;
}

//Matrix* computeHogDescriptorForSuperPixelsInterestPoints(GVector* vector_image,HogManager* hogManager,
//                                                         size_t patchSizeX,size_t patchSizeY){
//
//#ifndef USE_IFT
//    printf("[computeHogDescriptorForSuperPixelsInterestPoints] iftlib not defined\n");
//    return NULL;
//#endif
//
//    Image* image = VECTOR_GET_ELEMENT_AS(Image*,vector_image,0);
//    int niters, nseeds;
//    int nSuperPixels = ((image->ny*image->nx)/((float)(481*321)))*300;
//    float alpha = 0.12;
//    float beta = 12;
//    int nitersInitial = 10;
//    int smooth_niters = 0;
//
//    hogManager->image = image;
//    hogManager->computeGradient = false;
//    hogManager->cellSizeX = patchSizeX/2;
//    hogManager->cellSizeY = patchSizeY/2;
//    hogManager->cellsPerBlockX = 2;
//    hogManager->cellsPerBlockY = 2;
//    hogManager->binSize = 20;
//    hogManager->useUnsignedGradients = true;
//    hogManager->strideX = hogManager->cellSizeX;
//    hogManager->strideY = hogManager->cellSizeY;
//    compudeGradientImage(hogManager);
//
//    Image* selectPointsMap = createImage(hogManager->image->nx,hogManager->image->nx);
//
//
//    iftImage  *img = iftReadImageByExt(image->path);
//    iftImage  *label = iftCompute_ISF_MIX_MEAN_Superpixels(img, nSuperPixels, alpha, beta, nitersInitial, smooth_niters, &(nseeds), &(niters));
//
//    AdjacencyRelation* adjacencyRelation = createRectangularAdjacency(3,3);
//    int neighbourCoordinateX = 0;
//    int neighbourCoordinateY = 0;
//    int neighbourLabel;
//    GVector* uniqueLabel = createVector(9,sizeof(int));
//    GVector* vector_ROIs = createVector(10,sizeof(RegionOfInterest));
//    bool notFound;
//    for (int y = 1; y < label->ysize-1; ++y) {
//        for (int x = 1; x < label->xsize-1; ++x) {
//            //printf("%d %d\n",x,y);
//            for (int k = 0; k < adjacencyRelation->n; ++k) {
//                notFound = true;
//                neighbourCoordinateX = x + adjacencyRelation->dx[k];
//                neighbourCoordinateY = y + adjacencyRelation->dy[k];
//                if(neighbourCoordinateX > 0 && neighbourCoordinateX < image->nx){
//                    if(neighbourCoordinateY > 0 && neighbourCoordinateY < image->ny){
//                        //printf("aki1\n");
//                        neighbourLabel = iftImgVal2D(label,neighbourCoordinateX,neighbourCoordinateY);//simple round
//                        //printf("aki1\n");
//                    }else{
//                        continue;
//                    }
//                }else{
//                    continue;
//                }
//
//                for (size_t i = 0; i < uniqueLabel->size; ++i) {
//                    if(neighbourLabel == VECTOR_GET_ELEMENT_AS(int,uniqueLabel,i)){
//                        notFound = false;
//                        break;
//                    }
//                }
//                if(notFound){
//                    VECTOR_PUSH_BACK(int,uniqueLabel,neighbourLabel);
//                }
//            }
//            if(uniqueLabel->size >= 3){
//
//                if(imageVal(hogManager->gradientImageMagnitude,x,y) >= 20){
//
//                    float sum = imageVal(selectPointsMap,(x-1),(y-1) ) +
//                                imageVal(selectPointsMap,(x),(y-1) ) +
//                                imageVal(selectPointsMap,(x+1),(y-1) ) +
//                                imageVal(selectPointsMap,(x-1),(y) ) ;
//                    if(sum <= 1){
////                        imageValCh(image,x,y,0) = 0;
////                        imageValCh(image,x,y,1) = 255;
////                        imageValCh(image,x,y,2) = 255;
//
//                        imageVal(selectPointsMap,x,y) = 255;
//
//                        RegionOfInterest regionOfInterest;
//                        regionOfInterest.coordinateX = x - patchSizeX/2;
//                        regionOfInterest.coordinateY = y - patchSizeY/2;
//                        regionOfInterest.coordinateZ = 0;
//
//                        regionOfInterest.size_x = patchSizeX;
//                        regionOfInterest.size_y = patchSizeY;
//                        regionOfInterest.size_z = 1;
//
//                        if(regionOfInterest.coordinateX > 0 &&  regionOfInterest.coordinateX + patchSizeX <= image->nx){
//                            if(regionOfInterest.coordinateY > 0 &&  regionOfInterest.coordinateY + patchSizeY <= image->ny){
//                                VECTOR_PUSH_BACK(RegionOfInterest,vector_ROIs,regionOfInterest);
//                            }
//                        }
//
//                    }
//                }
//
//            }
//            resizeVector(uniqueLabel,0);
//        }
//    }
//    //writeImage(image,"miau.png");
//    //printf("nPoints: %lu\n",vector_ROIs->size);
//    destroyImage(&selectPointsMap);
//    Matrix* matrix = computeHogDescriptorForRegionsOfInterest(vector_ROIs,hogManager);
////
////    printf("saiiiii\n");
//    destroyVector(&vector_ROIs);
//    destroyVector(&uniqueLabel);
//    destroyAdjacencyRelation(&adjacencyRelation);
//    hogManager->image = NULL;
//    iftDestroyImage(&img);
//    iftDestroyImage(&label);
//    return matrix;
//
//
//}

Matrix* computeHogDescriptorForRegionsOfInterest(GVector* vector_ROIs,HogManager* hogManager){
    //Matrix* matrix = createMatrix(vector_ROIs->size,totalNumberBins,sizeof(float));
    GVector* featureVector_hog = NULL;
    int shift = 0;
    size_t nrows = vector_ROIs->size;
    size_t ncols = 0;
    //printf("inicio for\n");
    for (size_t i = 0; i < vector_ROIs->size; ++i) {
        Image* workImage = hogManager->image;
        RegionOfInterest regionOfInterest = VECTOR_GET_ELEMENT_AS(RegionOfInterest,vector_ROIs,i);
        workImage->imageROI = regionOfInterest;
        //printImageRegionOfInterest(&regionOfInterest);
        //printf("inicio hog %d %d\n",hogManager->cellSizeX,hogManager->cellSizeY);
        computeHogDescriptor(hogManager);
        //printf("fim hog\n");
        if(featureVector_hog){
            float* data = (float*)featureVector_hog->data;
            appendVectorsSameType_macro(float,data,hogManager->hogFeatureVector,shift, hogManager->hogFeatureVectorSize);
            shift += hogManager->hogFeatureVectorSize;
            free(hogManager->hogFeatureVector);
            hogManager->hogFeatureVector = NULL;
            //VECTOR_PRINT_AS(double, "%f ",featureVector_hog);
        }else{
            ncols = hogManager->hogFeatureVectorSize;
            featureVector_hog = createNullVector(ncols*nrows,sizeof(float));
            float* data = (float*)featureVector_hog->data;
            appendVectorsSameType_macro(float,data,hogManager->hogFeatureVector,shift, ncols);
            shift += ncols;
            free(hogManager->hogFeatureVector);
            hogManager->hogFeatureVector = NULL;
            //VECTOR_PRINT_AS(double, "%f ",featureVector_hog);
        }
    }
    //printf("fim for\n");
    Matrix* matrix = createMatrix(nrows,ncols,sizeof(float));
    destroyVector(&(matrix->matrixData));
    matrix->matrixData = featureVector_hog;
    //MATRIX_PRINT_AS(float,"%f ",matrix);
    //printf("%lu %lu\n",matrix->numberRows,matrix->numberColumns);
    //destroyMatrix(&matrix);
    //destroyVector(&vector_ROIs);
    return matrix;
}

void computeHogDescriptor(HogManager* hogManager){
    float range =  (hogManager->useUnsignedGradients)? 180 : 360;
    int nBins = range/hogManager->binSize;
    float *binsCenter = (float*)calloc(nBins,sizeof(float));

    float value = 0;
    for (int i = 0; i < nBins; ++i) {
        binsCenter[i] = value;
        value += hogManager->binSize;
    }

    if(hogManager->image == NULL){
        printf("[computeHogDescriptor] could not perform hog. Image is empty\n");
        return;
    }


    if(hogManager->gradientImageMagnitude == NULL){
        compudeGradientImage(hogManager);
    } else if(areImagesSameDimension(hogManager->image,hogManager->gradientImageMagnitude)){
        destroyImage(&hogManager->gradientImageMagnitude);
        destroyImage(&hogManager->gradientImagePhase);
        compudeGradientImage(hogManager);
    } else if(hogManager->computeGradient){
        destroyImage(&hogManager->gradientImageMagnitude);
        destroyImage(&hogManager->gradientImagePhase);
        compudeGradientImage(hogManager);
    }


//    printImageInfo(hogManager->image);
//    printImageInfo(hogManager->gradientImageMagnitude);
//    printImageInfo(hogManager->gradientImagePhase);

    Image* workImage = hogManager->image;
    int startY = workImage->imageROI.coordinateY;
    int startX = workImage->imageROI.coordinateX;
    int height = workImage->imageROI.coordinateY+workImage->imageROI.size_y;
    if(height > hogManager->image->ny){
        height = hogManager->image->ny;
    }
    int width = workImage->imageROI.coordinateX+workImage->imageROI.size_x;
    if(width > hogManager->image->nx){
        width = hogManager->image->nx;
    }
    int blockSizeX = hogManager->cellsPerBlockX*hogManager->cellSizeX;
    int blockSizeY = hogManager->cellsPerBlockY*hogManager->cellSizeY;
    int strideX = hogManager->strideX;
    int strideY = hogManager->strideY;

    int y_cell_start = 0;
    int x_cell_start = 0;
    int y_cell_end = 0;
    int x_cell_end = 0;

    int y_block_start = 0;
    int x_block_start = 0;
    int y_block_end = 0;
    int x_block_end = 0;

    float phaseValueDegree;
    float binIndex_float;
    int firstCloseIndex;
    int secondCloseIndex;
    float factor1;
    float factor2;
    float distance1;
    float distance2;
    float *histogramCell = NULL;



    y_block_start = startY;
    x_block_start = startX;
    float *hogFeatureVector = NULL;
    int hogFeatureVectorSize = 0;

    //for each block
    while(y_block_start < height && x_block_start < width){
        //printf("[computeHogDescriptor] block [%d %d]\n",x_block_start,y_block_start);

        if(x_block_start + blockSizeX > width || y_block_start + blockSizeY > height){
            //printf("[computeHogDescriptor] skip block [%d %d]\n",x_block_start,y_block_start);
            x_block_start += strideX;
            if(x_block_start >= width){
                x_block_start = startX;
                y_block_start += strideY;
            }
            continue;
        }

        y_block_end = y_block_start + blockSizeY;
        x_block_end = x_block_start + blockSizeY;

        x_cell_start = x_block_start;
        y_cell_start = y_block_start;
        float *histogramBlock = NULL;
        int sizeHistogramBlock = 0;
        while(y_cell_start < y_block_end && x_cell_start < x_block_end){
            x_cell_end = x_cell_start + hogManager->cellSizeX;
            y_cell_end = y_cell_start +  hogManager->cellSizeY;
            //printf("[computeHogDescriptor] cell [%d %d]\n",x_cell_start,y_cell_start);
            histogramCell = (float*)calloc(nBins,sizeof(float));

            //compute histogram for cell
            for (int coordY_cell =  y_cell_start;  coordY_cell < y_cell_end; coordY_cell++) {
                for (int coordX_cell =  x_cell_start;  coordX_cell < x_cell_end; coordX_cell++) {

                    phaseValueDegree = imageVal(hogManager->gradientImagePhase,coordX_cell,coordY_cell);
                    binIndex_float = phaseValueDegree/hogManager->binSize;
                    firstCloseIndex = (int)(binIndex_float - 0.000001);
                    if(binIndex_float - firstCloseIndex <= 0.5){
                        secondCloseIndex = firstCloseIndex-1;
                        if(secondCloseIndex < 0){
                            secondCloseIndex = nBins-1;
                        }
                    }else{
                        secondCloseIndex = firstCloseIndex+1;
                        if(secondCloseIndex >= nBins){
                            secondCloseIndex = 0;
                        }
                    }
                    distance1 = phaseValueDegree-binsCenter[firstCloseIndex];
                    if(distance1 < 0){
                        distance1 = -distance1;
                    }

                    distance2 = phaseValueDegree-binsCenter[secondCloseIndex];
                    if(distance2 < 0){
                        distance2 = -distance2;
                    }
                    factor1 = distance2/(hogManager->binSize);
                    factor2 = distance1/(hogManager->binSize);


                    histogramCell[firstCloseIndex] += factor1*imageVal(hogManager->gradientImageMagnitude,coordX_cell,coordY_cell);
                    histogramCell[secondCloseIndex] += factor2*imageVal(hogManager->gradientImageMagnitude,coordX_cell,coordY_cell);
                }
            }

            //normalization on cell histogram
            double histogramMagnitudeCell = 0;
            for (int i = 0; i < nBins; ++i) {
                histogramMagnitudeCell += histogramCell[i]*histogramCell[i];
            }


            histogramMagnitudeCell = sqrt(histogramMagnitudeCell);
            for (int i = 0; i < nBins; ++i) {
                histogramCell[i] /= (histogramMagnitudeCell+0.000001);
            }

            //merging cells hitograms
            float *histogramAux = NULL;
            mergeVectorsSameType_macro(float, histogramBlock,sizeHistogramBlock,histogramCell,nBins,histogramAux);
            free(histogramCell);
            free(histogramBlock);
            histogramBlock = histogramAux;
            sizeHistogramBlock += nBins;

            x_cell_start += hogManager->cellSizeX;
            if(x_cell_start >= x_block_end){
                x_cell_start = x_block_start;
                y_cell_start += strideY;
            }
        }
        //normalization on block histogram
        double histogramMagnitudeBlock = 0;
        for (int i = 0; i < sizeHistogramBlock; ++i) {
            histogramMagnitudeBlock += histogramBlock[i]*histogramBlock[i];
        }

        histogramMagnitudeBlock = sqrt(histogramMagnitudeBlock);
        for (int i = 0; i < sizeHistogramBlock; ++i) {
            histogramBlock[i] /= (histogramMagnitudeBlock+0.000001);
        }


        //merging block hitograms
        float *featureVectorAux = NULL;
        mergeVectorsSameType_macro(float, hogFeatureVector, hogFeatureVectorSize,histogramBlock,sizeHistogramBlock,featureVectorAux);
        free(hogFeatureVector);
        free(histogramBlock);
        hogFeatureVector = featureVectorAux;
        hogFeatureVectorSize += sizeHistogramBlock;

        x_block_start += strideX;
        if(x_block_start >= width){
            x_block_start = startX;
            y_block_start += strideY;
        }
    }

    hogManager->hogFeatureVector = hogFeatureVector;
    hogManager->hogFeatureVectorSize = hogFeatureVectorSize;
    free(binsCenter);
}

void destroyHogManager(HogManager** pHogManager){
    if(pHogManager == NULL){
        return;
    }
    HogManager* aux = *pHogManager;
    if(aux == NULL){
        return;
    }
    if(aux->image){
        destroyImage(& ( (*pHogManager)->image) );
    }
    if(aux->gradientImagePhase){
        destroyImage(&(aux->gradientImagePhase));
    }
    if(aux->gradientImageMagnitude){
        destroyImage(&(aux->gradientImageMagnitude));
    }
    if(aux->hogFeatureVector){
        free(aux->hogFeatureVector);
    }
    free(*pHogManager);
    *pHogManager = NULL;
}



