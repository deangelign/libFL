#include "FL.h"

int main(int argc, char **argv) {

    //gray image

    Image *grayImage = readImage("../data/lena.pgm");
    GrayImage* grayImage2 = readGrayImage("../data/lena.pgm");//estrutura obsoleta
    int index=0;
    for (int i = 0; i < grayImage->ny; ++i) {
        for (int j = 0; j < grayImage->nx; ++j) {
            index = (i*grayImage->nx) + j;
            grayImage->channel[0][index] = 255 - grayImage->channel[0][index];
            grayImage2->val[index] = 255 - grayImage2->val[index];
        }
    }
    writeImage(grayImage,"outputGray.pgm");

    //color image
    Image *colorImage = readImage("../data/lenaColor.ppm");
    ColorImage *colorImage2 = readColorImage("../data/lenaColor.ppm");//estrutura obsoleta
    for (int i = 0; i < colorImage->ny; ++i) {
        for (int j = 0; j < colorImage->nx; ++j) {
            int index = (i*colorImage->nx) + j;
            colorImage->channel[0][index] = 255 - colorImage->channel[0][index];
            colorImage->channel[1][index] = 255 - colorImage->channel[1][index];
            colorImage->channel[2][index] = 255 - colorImage->channel[2][index];

            colorImage2->cor[i][j].val[0] = 255 - colorImage2->cor[i][j].val[0];
            colorImage2->cor[i][j].val[1] = 255 - colorImage2->cor[i][j].val[1];
            colorImage2->cor[i][j].val[2] = 255 - colorImage2->cor[i][j].val[2];
        }
    }
    writeImage(colorImage,"outputColor.ppm");

    destroyImage(&grayImage);
    destroyGrayImage(&grayImage2);
    destroyImage(&colorImage);
    destroyColorImage(&colorImage2);
    return 0;
}
