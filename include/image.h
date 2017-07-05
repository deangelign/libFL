/*
 *Created by Deangeli Gomes Neves
 *
 * This software may be freely redistributed under the terms
 * of the MIT license.
 *
 */


#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "common.h"
#include "file.h"
#include "png.h"
#include "jpeglib.h"
//#include  "tiffio.h"




enum ColorSpace {
    COLORSPACE_UNKNOWN = 0,
    COLORSPACE_GRAYSCALE = 1,
    COLORSPACE_GRAYSCALE_ALPHA = 2,
    COLORSPACE_RGB = 3,
    COLORSPACE_RGBA = 4,
    COLORSPACE_YCbCr = 5,
    COLORSPACE_YCbCrK = 6,
    COLORSPACE_HSV = 7,
    COLORSPACE_HSVA = 8,
    COLORSPACE_CMYK = 9
};

enum DataType {
    FLOAT,
    DOUBLE
};


typedef struct _RegionOfInterest {
    float coordinateX;
    float coordinateY;
    float coordinateZ;
    float size_x;
    float size_y;
    float size_z;
}RegionOfInterest;


typedef struct _image {
    int   nx,ny,nz;    /* dimensoes da imagem */
    float dx,dy, dz;    /* tamanho do pixel em unidades de comprimento */
    unsigned  char channelDepth;
    int numberPixels;
    float **channel; /*might be RGB, or Ycbcr*/
    int scalingFactor;
    char  unid[10]; /* unidade de comprimento */
    int nchannels;
    ColorSpace colorSpace;
    DataType dataTypeId;
    char path[100];
    RegionOfInterest imageROI;
} Image;




//macro para facil acesso aos pixels da imagem. a macro computa o indice para acessar a posicao (x,y) da imagem
#define imageVal(image, x, y) image->channel[0][(y*image->nx) + x]
#define imageValCh(image, x, y, c) image->channel[c][(y*image->nx) + x]
#define imageVolume(image, x, y,z) image->channel[0][(z*image->ny*image->nx) + (y*image->nx) + x]
#define imageVolumeCh(image, x, y, z,c) image->channel[c][(z*image->ny*image->nx) + (y*image->nx) + x]

#define areImagesSameDimension(image1, image2) ( (image1->nx == image2->nx) && (image1->ny == image2->ny) && (image1->nz == image2->nz) )

Image* createImage(int nx, int ny,int nchannels);
Image* createImage(int nx, int ny,int nz, int nchannels);
Image* createImage(int nx, int ny);
void findAppropriateColorSpace(Image* image);
void destroyImage(Image**image );
void destroyImageVoidPointer(void* data);
Image *readImagePGM(char *filename);
Image *readImagePPM(char *filename);
Image *readImageTIFF(char *filename);
Image *readImagePNG(char *filename);
Image *readImageJPEG(char *filename);
Image* readImage(char *filename);
void writeImageP2(Image *image,char *filename);
void writeImageP3(Image *image, char *filename);
void writeImageP5(Image *image, char *filename);
void writeImageP6(Image *image, char *filename);
void writeImagePGM(Image* image, char *filename,  char* magicNumber);
void writeImagePPM(Image* image, char *filename,  char* magicNumber);
void writeImagePNG(Image *image,char *filename);
void writeImageJPEG(Image *image,char *filename);
void writeImageTIFF(Image *image,char *filename);
void writeImage(Image* image, char *filename);
bool isValidPixelCoordinate(Image *image,int pixelCoordinateX,int pixelCoordinateY);


Image *imageSubtraction(Image *image1, Image *image2, bool saturation);
bool isImagesSameDomain(Image *image1,Image *image2);
Image *convertRGBtoYCbCr(Image *rgbImage);
float sumUpAllPixelsValues(Image *image, bool normalize);
Image* extractImageChannel(Image* image, int channel);
void printImage(Image* image);
void printImageInfo(Image* image);
void printImageRegionOfInterest(RegionOfInterest* regionOfInterest);
Image* createAlphaChannel(Image* image,float alpha);
Image* convertGrayImage2RGBImage(Image* image_ppm);
Image* mergeImages(Image* image1, Image* image2);
Image* getSlice(Image* image, int sliceIndex);
void putSlice(Image* volume, Image* image, int sliceIndex);
Image* packImagesFromDirectory(DirectoryManager* directoryManager);
Image* extractSubImage(Image*image, int xCoord,int yCoord, int xsize, int ysize, bool zeroPadding);



uint8_t* convertImage2IntergerArray8bits(Image* imageRGBA);
Image* copyImage(Image* image, bool copyChannels);
void addUniformNoise(Image* image, float uniformValue, double probability);
void addSaltAndPepperNoise(Image* image, double probability);
void addAdditiveGaussianNoise(Image* image, double mean, double variance);

#endif
