#include "FL.h"

int main(int argc, char **argv) {
    char             ext[10],*pos;

    if (argc != 2) {
        printf("imageCorrelation <image.[pgm]> <adjacency radius> <output filename.[pgm]> \n");
        exit(-1);
    }

    pos = strrchr(argv[1],'.') + 1;
    sscanf(pos,"%s",ext);

    if (strcmp(ext,"pgm")==0){
        GrayImage *grayImage = readGrayImage(argv[1]);
        AdjacencyRelation *adjRel = createCircularAdjacency(atof(argv[2]));
        Kernel *kernel = createKernel(adjRel);
        for (int i = 0; i < kernel->adjacencyRelation->n; ++i) {
            kernel->weight[i] = (i+1.)/kernel->adjacencyRelation->n;
        }
        GrayImage *outputImage = correlation2D(grayImage,kernel);
        writeGrayImage(outputImage,argv[3]);
        destroyGrayImage(&grayImage);
    } else { /* ppm */
        printf("formato da imagem nao suportado\n");
    }

}
