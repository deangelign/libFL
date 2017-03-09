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



void         *ReadImageByExt(char *filename);
GrayImage    *CreateGrayImage(int ncols,int nrows);
void          DestroyGrayImage(GrayImage **img);
GrayImage     *ReadGrayImage(char *filename);
void          WriteGrayImage(GrayImage *img, char *filename);

ColorImage   *CreateColorImage(int nx, int ny); 
void          DestroyColorImage(ColorImage **I);
ColorImage   *ReadColorImage(char *filename);
void          WriteColorImage(ColorImage *I, char *filename);
int MinimumColorValue(ColorImage *img);
int MaximumColorValue(ColorImage *img);
int MinimumIntensityColor(ColorImage *img, int c);
int MaximumIntensityColor(ColorImage *img, int c);


#endif
