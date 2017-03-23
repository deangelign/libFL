#include "FL.h"

int main(int argc, char **argv) {

    char *pos = NULL;
    char fileExtension[10];
    int nbins = 216;
    int k = 60;

    if (argc!=4){
        printf("Usage: <image1.[pgm/ppm]> <image2.[pgm/ppm]> <image3.[pgm/ppm]>\n");
        exit(-1);
    }

    FeatureVector **featureMatrix = (FeatureVector **)calloc(1,sizeof(FeatureVector *));
    for (int i = 0; i < 3; ++i) {
        pos = strrchr(argv[i+1],'.') + 1;
        sscanf(pos,"%s",fileExtension);

        if (strcmp(fileExtension,"pgm")==0){
            GrayImage *grayImage = readGrayImage(argv[i+1]);
            Histogram *hist = GrayHistogram(grayImage,nbins);
            FeatureVector *vec1 = createFeatureVector(hist);
            FeatureVector *vec2 = getMorphologicalPdf(grayImage,k);
            featureMatrix[i] =  mergeFeatureVectors(vec1,vec2);
            destroyGrayImage(&grayImage);
            destroyFeatureVector(&vec1);
            destroyFeatureVector(&vec2);
            DestroyHistogram(&hist);
        } else if(strcmp(fileExtension,"ppm")==0){ /* ppm */
            ColorImage *colorImage = readColorImage(argv[i+1]);
            Histogram *hist = ColorHistogram(colorImage,nbins);
            FeatureVector *vec1 = createFeatureVector(hist);
            FeatureVector *vec2 = getMorphologicalPdf(colorImage,k);
            featureMatrix[i] = mergeFeatureVectors(vec1,vec2);
            destroyColorImage(&colorImage);
            destroyFeatureVector(&vec1);
            destroyFeatureVector(&vec2);
            DestroyHistogram(&hist);
        }else{
            printf("Invalide image format: %s\n",fileExtension);
        }
        printf("feature vector da imagem %d computado\n",i);
        printf("\n");
    }


    wirteFeatureVectors(featureMatrix,3, "fv2.txt");
    FeatureVector* vec1 = circularCrossCorrelation1D(featureMatrix[0],featureMatrix[1],false);
    FeatureVector* vec2 = circularCrossCorrelation1D(featureMatrix[0],featureMatrix[2],false);
    FeatureVector* vec3 = circularCrossCorrelation1D(featureMatrix[0],featureMatrix[0],false);


    for (int i = 0; i < 3; ++i) {
        destroyFeatureVector(&featureMatrix[i]);
    }
    FeatureVector **correlations = (FeatureVector **)calloc(3,sizeof(FeatureVector *));
    correlations[0] = vec1;
    correlations[1] = vec2;
    correlations[2] = vec3;
    wirteFeatureVectors(correlations,3, "R_xy.txt");

    //free(pos);
    return 0;
    
}
