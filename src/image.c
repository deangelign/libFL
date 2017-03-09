#include "image.h"

GrayImage *CreateGrayImage(int ncols, int nrows)
{
  GrayImage *img=NULL;
  int i;

  img = (GrayImage *) calloc(1,sizeof(GrayImage));
  if (img == NULL){
    Error(MSG1,"CreateImage");
  }

  img->val   = AllocIntArray(nrows*ncols);
  img->tbrow = AllocIntArray(nrows);

  img->tbrow[0]=0;
  for (i=1; i < nrows; i++)
    img->tbrow[i]=img->tbrow[i-1]+ncols;
  img->ncols = ncols;
  img->nrows = nrows;
 
 return(img);
}

void DestroyGrayImage(GrayImage **img)
{
  GrayImage *aux;

  aux = *img;
  if(aux != NULL){
    if (aux->val != NULL)   free(aux->val); 
    if (aux->tbrow != NULL) free(aux->tbrow);
    free(aux);    
    *img = NULL;
  }
}

GrayImage *ReadGrayImage(char *filename)
{
  FILE *fp=NULL;
  unsigned char *value=NULL;
  char type[10];
  int  i,ncols,nrows,n;
  GrayImage *img = NULL;
  char z[256];

  fp = fopen(filename,"rb");
  if (fp == NULL){
    fprintf(stderr,"Cannot open %s\n",filename);
    exit(-1);
  }
  fscanf(fp,"%s\n",type);
  if((strcmp(type,"P5")==0)){
    NCFgets(z,255,fp);
    sscanf(z,"%d %d\n",&ncols,&nrows);
    n = ncols*nrows;
    NCFgets(z,255,fp);
    sscanf(z,"%d\n",&i);
    fgetc(fp);
    value = (unsigned char *)calloc(n,sizeof(unsigned char));
    if (value != NULL){
      fread(value,sizeof(unsigned char),n,fp);
    }else{
      fprintf(stderr,"Insufficient memory in ReadImage\n");
      exit(-1);
    }
    fclose(fp);
    img = CreateGrayImage(ncols,nrows);
    for (i=0; i < n; i++)
      img->val[i]=(int)value[i];
    free(value);
  }else{
    if((strcmp(type,"P2")==0)){
      NCFgets(z,255,fp);
      sscanf(z,"%d %d\n",&ncols,&nrows);
      n = ncols*nrows;
      NCFgets(z,255,fp);
      sscanf(z,"%d\n",&i);
      img = CreateGrayImage(ncols,nrows);
      for (i=0; i < n; i++)
	fscanf(fp,"%d",&img->val[i]);
      fclose(fp);
    }else{
      fprintf(stderr,"Input image must be P2 or P5\n");
      exit(-1);
    }
  }

  return(img);
}

void WriteGrayImage(GrayImage *img,char *filename)
{
  FILE *fp;
  int i, n, Imax,Imin;

  fp = fopen(filename,"wb");
  if (fp == NULL){
    fprintf(stderr,"Cannot open %s\n",filename);
    exit(-1);
  }
  n    = img->ncols*img->nrows;
  Imax = INT_MIN; Imin = INT_MAX;
  for (i=0; i < n; i++) {

    if (img->val[i] > Imax)
      Imax = img->val[i];
    if (img->val[i] < Imin)
      Imin = img->val[i];
  }


  fprintf(fp,"P2\n");
  fprintf(fp,"%d %d\n",img->ncols,img->nrows);
  fprintf(fp,"%d\n",Imax-Imin);
 
  for (i=0; i < n; i++) {
    fprintf(fp,"%d ",img->val[i]-Imin);
    if (((i+1)%17) == 0)
      fprintf(fp,"\n");
  }

  fclose(fp);
}


ColorImage *CreateColorImage(int nx, int ny)
{
  ColorImage *I=(ColorImage *)calloc(1,sizeof(ColorImage));
  int y;

  I->nx = nx;
  I->ny = ny;
  I->dx = 1.0;
  I->dy = 1.0;
  sprintf(I->unid,"mm");

  I->cor = (Cor **)calloc(ny,sizeof(Cor *));  
  if (I->cor == NULL)
    Error(MSG1,"CreateColorImage");
  else{
    for (y=0; y < I->ny; y++){ 
      I->cor[y] = (Cor *)calloc(nx,sizeof(Cor));  
      if (I->cor[y] == NULL)
	Error(MSG1,"CreateColorImage");
    }
  }
  return(I);
}

ColorImage *ReadColorImage(char *filename)
{
  FILE *fp=NULL;
  char type[10]; 
  int  x,y,nx,ny,Imax; 
  ColorImage *I=NULL;
  char remarks[256];
  ushort rgb16[3];

  fp = fopen(filename,"r");
  if (fp != NULL) {
    fscanf(fp,"%s",type);
    NCFgets(remarks,255,fp);

    if (strcmp(type,"P6")==0){
      fscanf(fp,"%d",&nx);
      fscanf(fp,"%d\n",&ny);
      I = CreateColorImage(nx,ny);
      fscanf(fp,"%d\n",&Imax);
      I->Imax = Imax;

      if (Imax <= 255){
        for (y=0; y < ny; y++)
	     for (x=0; x < nx; x++){               
	         I->cor[y][x].val[RED]=(int)fgetc(fp);//red
	         I->cor[y][x].val[GREEN]=(int)fgetc(fp);//green
	         I->cor[y][x].val[BLUE]=(int)fgetc(fp);//blue
	     }
      }else if (Imax > 255 && Imax <= 65535){
        int rgbBitDepth = 9;

        while ((1 << rgbBitDepth) <= Imax) {
             rgbBitDepth++;
        }
        for (y=0; y < ny; y++) {
          for (x=0; x < nx; x++){                            
            if (fread(rgb16, 2, 3, fp) == 3) {
              I->cor[y][x].val[RED]=((rgb16[0] & 0xff) << 8) | ((ushort) rgb16[0] >> 8);
              I->cor[y][x].val[GREEN]=((rgb16[1] & 0xff) << 8) | ((ushort) rgb16[1] >> 8);
              I->cor[y][x].val[BLUE]=((rgb16[2] & 0xff) << 8) | ((ushort) rgb16[2] >> 8);
            } else{
              Error("Reading 16-bit error","ReadColorImage");
            }
          }
        }
      }
    }else{
      Error("GrayImage type invalid","ReadColormage");
    }

    fclose(fp);
  }else{
    Error(MSG2,"ReadColorImage");
  }
  
  return(I);
}

void WriteColorImage(ColorImage *I, char *filename)
{
  FILE *fp=NULL;
  int  x,y; 
  ushort rgb16[3];

  fp = fopen(filename,"w");
  if (fp != NULL) {
    fprintf(fp,"P6\n");
    fprintf(fp,"%d %d\n",I->nx,I->ny);    

    int max_val = MaximumColorValue(I);
    int min_val = MinimumColorValue(I);

    if (min_val < 0){
      Error(MSG2,"WriteColorImage");
      return;
    }

    if (max_val < 256){
      fprintf(fp,"255\n");
      for (y=0; y < I->ny; y++)
        for (x=0; x < I->nx; x++){
          fputc((uchar)I->cor[y][x].val[RED],fp);//red
          fputc((uchar)I->cor[y][x].val[GREEN],fp);//green
          fputc((uchar)I->cor[y][x].val[BLUE],fp);//blue
        }
    } else if (max_val < 65536){
      int rgbBitDepth = 9;
      // find the bit depth for the maximum value img_max_val            
      while ((1 << rgbBitDepth) <= max_val) {
          rgbBitDepth++;
      }      

      fprintf(fp, "%d\n", (1 << rgbBitDepth) - 1);
      for (y=0; y < I->ny; y++)
        for (x=0; x < I->nx; x++){
          rgb16[RED] = ((I->cor[y][x].val[RED] & 0xff) << 8) | ((ushort) I->cor[y][x].val[RED] >> 8);//I->cor[y][x].val[RED];
          rgb16[GREEN] = ((I->cor[y][x].val[GREEN] & 0xff) << 8) | ((ushort) I->cor[y][x].val[GREEN] >> 8);//I->cor[y][x].val[GREEN];
          rgb16[BLUE] = ((I->cor[y][x].val[BLUE] & 0xff) << 8) | ((ushort) I->cor[y][x].val[BLUE] >> 8);//I->cor[y][x].val[BLUE];
          // write 6 bytes for each image pixel
          if (fwrite(rgb16, 2, 3, fp) != 3) {
              Error("Cannot write 16-bit image as P6", "iftWriteImageP6");
          }
        }
    }
    fclose(fp);
  }else{
    Error(MSG2,"WriteColorImage");
  }
  
}

void DestroyColorImage(ColorImage **I)
{
  int y;

  if ((*I) != NULL) {    
    for (y=0; y < (*I)->ny; y++) 
      free((*I)->cor[y]);
    free((*I)->cor);
    free(*I);
    *I = NULL;
  }
}


int MinimumColorValue(ColorImage *img){

  int img_min_val[3];
  for (int i=0; i<3; i++){
    img_min_val[i] = MinimumIntensityColor(img,i);
  }

  return MIN(MIN(img_min_val[RED],img_min_val[GREEN]),img_min_val[BLUE]);
}

int MaximumColorValue(ColorImage *img){

  int img_max_val[3];
  for (int i=0; i<3; i++){
    img_max_val[i] = MaximumIntensityColor(img,i);
  }

  return MAX(MAX(img_max_val[RED],img_max_val[GREEN]),img_max_val[BLUE]);
}



int MinimumIntensityColor(ColorImage *img, int c){
  int i,j,min;

  min = INT_MAX;
  for (i = 0; i<img->nx; i++)
    for (j=0; j<img->ny; j++)
      if (img->cor[j][i].val[c] < min)
        min = img->cor[j][i].val[c];

  return min;
}

int MaximumIntensityColor(ColorImage *img, int c){
  int i,j,max;

  max = -1;
  for (i = 0; i<img->nx; i++)
    for (j=0; j<img->ny; j++)
      if (img->cor[j][i].val[c] > max)
        max = img->cor[j][i].val[c];

  return max;
}