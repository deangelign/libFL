#include "FL.h"

int main(int argc, char **argv) {
    char             ext[10],*pos;
    Histogram       *hist=NULL;

    if (argc != 4) {
        printf("histogram <image.[pgm,ppm]> <nbins> <histogram.txt>\n");
        exit(-1);
    }

    pos = strrchr(argv[1],'.') + 1;
    sscanf(pos,"%s",ext);

    if (strcmp(ext,"pgm")==0){
        GrayImage *grayImage = readGrayImage(argv[1]);
        hist = GrayHistogram(grayImage,atoi(argv[2]));
        destroyGrayImage(&grayImage);
    } else { /* ppm */
        ColorImage *colorImage =  readColorImage(argv[1]);
        hist = ColorHistogram(colorImage,atoi(argv[2]));
        destroyColorImage(&colorImage);
    }

    WriteHistogram(hist,argv[3]);
    DestroyHistogram(&hist);

    return 0;
}

