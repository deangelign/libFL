#include "FL.h"

int main(int argc, char **argv) {

    //gray image
    GrayImage *grayImage = readGrayImage("../data/lena.pgm");
    int index=0;
    for (int i = 0; i < grayImage->nrows; ++i) {
        for (int j = 0; j < grayImage->ncols; ++j) {
            index = (i*grayImage->ncols) + j;
            grayImage->val[index] = 255 - grayImage->val[index];
        }
    }
    writeGrayImage(grayImage,"outputGray.pgm");

    //color image
    ColorImage *colorImage = readColorImage("../data/lenaColor.ppm");
    for (int i = 0; i < colorImage->ny; ++i) {
        for (int j = 0; j < colorImage->nx; ++j) {
            colorImage->cor[i][j].val[0] = 255 - colorImage->cor[i][j].val[0];
            colorImage->cor[i][j].val[1] = 255 - colorImage->cor[i][j].val[1];
            colorImage->cor[i][j].val[2] = 255 - colorImage->cor[i][j].val[2];
        }
    }
    writeColorImage(colorImage,"outputColor.ppm");

    return 0;
}

