#include "FL.h"

int main(int argc, char **argv) {



//    if (argc!=2){
//        printf("Operation types:\n");
//        printf("0: Mean Filter\n");
//        printf("1: Median Filter\n");
//        printf("2: Gaussian Filter\n");
//        printf("3: Sobel Filter\n");
//        printf("4: Laplacian Filter \n");
//        printf("5: DOG Filter \n");
//        printf("6: LOG Filter\n");
//        printf("Usage: <operation type> \n");
//        //adj. radius tipically  1  to sqrt(8)
//        exit(-1);
//    }

    //gray image
    Image *image = readImage("../data/lena.pgm");
    //addUniformNoise(image,40,0.03);
    //addSaltAndPepperNoise(image, 0.03);
    //addAdditiveGaussianNoise(image,0,0.01);
    Image *outputImage = NULL;
    Image *diffImage = NULL;
    int type = 0;
    switch (type){
        case 0:
            outputImage = convolveImagewithMeanFilter(image);
            break;
        case 1:
            outputImage = convolveImagewithGaussianFilter(image);
            break;
        case 2:
            outputImage = convolveImagewithMedianFilter(image);
            break;
        case 3:
            outputImage = convolveImagewithSobelFilter(image);
            break;
        case 4:
            outputImage = convolveImagewithLaplacianFilter(image);
            break;
        case 5:
            outputImage = convolveImagewithDOG(image);
            break;
        case 6:
            outputImage = convolveImagewithLOG(image);
            break;
        default:
            printf("Invalid Operation type:\n");
            exit(-1);
    }
//    diffImage = imageSubtraction(image,outputImage,true);
//    writeImageP2(outputImage,"output.pgm");
    writeImageP2(image,"input.pgm");
//    writeImageP2(diffImage,"diff.pgm");
//    destroyImage(&image);
//    destroyImage(&outputImage);
//    destroyImage(&diffImage);

    return 0;
}
