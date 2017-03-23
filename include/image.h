#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "common.h"

/* Implementacoes vetoriais de uma imagem cinza 2D: inteira e double */

typedef struct _grayImage {
    int *val; /* brilho do pixel */
    int  ncols,nrows; /* dimensoes da imagem */
    int *tbrow; /* tabela de linhas */
} GrayImage;


typedef struct _cor {
    int val[3]; /* R,G,B */
} Cor;

typedef struct _colorimage {
    int   nx,ny;    /* dimensoes da imagem */
    Cor **cor;      /* matriz com a cor dos pixels */
    float dx,dy;    /* tamanho do pixel em unidades de comprimento */
    int   Imax;     /* depth (2^b-1)*/
    char  unid[10]; /* unidade de comprimento */
} ColorImage;


void         *readImageByExt(char *filename);
GrayImage    *createGrayImage(int ncols,int nrows);
void          destroyGrayImage(GrayImage **img);
GrayImage     *readGrayImage(char *filename);
void          writeGrayImage(GrayImage *img, char *filename);
int           minimumValue(GrayImage *img);
int           maximumValue(GrayImage *img);

ColorImage   *createColorImage(int nx, int ny);
void          destroyColorImage(ColorImage **I);
ColorImage   *readColorImage(char *filename);
void          writeColorImage(ColorImage *I, char *filename);
int minimumColorValue(ColorImage *img);
int maximumColorValue(ColorImage *img);
int minimumIntensityColor(ColorImage *img, int c);
int maximumIntensityColor(ColorImage *img, int c);

bool isValidPixelCoordinate(GrayImage *image,int pixelCoordinateX,int pixelCoordinateY);
bool isImagesSameDomain(GrayImage *image1,GrayImage *image2);
void copyImage(GrayImage *image1,GrayImage **image2);
/* Subtrai a imagem2 da imagem1 e guarda o resultado na imagem1.
 * O parametro saturation define se valores abaixo de zero serao armazenados na imagem1 ou nao*/
GrayImage *imageSubtraction(GrayImage *image1, GrayImage *image2, bool saturation);
/* soma a intensidade de todos os pixels na imagem*/
int sumUpAllPixelsValues(GrayImage *image);

/*salva uma imagem em ASCII. O parametro maxValueOnChanel define o valor maximo no canal...comumente
 * esse valor e 255*/
void writeImageAsASCII(ColorImage* image,const char *filename, unsigned int maxValueOnChanel);
void writeImageAsASCII(GrayImage* image,const char *filename,unsigned int maxValueOnChanel);

/*salva uma imagem em binario. O parametro maxValueOnChanel define o valor maximo no canal...comumente
 * esse valor e 255*/
void writeImageAsBinary(ColorImage* image,const char *filename, unsigned int maxValueOnChanel);
void writeImageAsBinary(GrayImage* image,const char *filename,unsigned int maxValueOnChanel);

void writeImage(ColorImage* image,const char *filename, const char *Magicnumber,unsigned int maxValueOnChanel);
void writeImage(GrayImage* image,const char *filename, const char *Magicnumber,unsigned int maxValueOnChanel);
void writeImage(GrayImage* image,const char *filename);
void writeImage(ColorImage* image,const char *filename);


ColorImage *RGBtoYCbCr(ColorImage *rgb);
GrayImage* extractColorChannelAsGrayImage(ColorImage* colorImage,int channel);



#endif
