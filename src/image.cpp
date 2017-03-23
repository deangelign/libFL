#include "image.h"

GrayImage *createGrayImage(int ncols, int nrows)
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

void destroyGrayImage(GrayImage **img)
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

GrayImage *readGrayImage(char *filename)
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
    img = createGrayImage(ncols,nrows);
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
      img = createGrayImage(ncols,nrows);
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

void writeGrayImage(GrayImage *img,char *filename)
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


int minimumValue(GrayImage *img){
  int i,min;

  min = img->val[0];
  int n = img->nrows*img->ncols;
  for (i = 0; i < n; i++){
    if (img->val[i] < min) {
      min = img->val[i];
    }
  }
  return min;
}
int maximumValue(GrayImage *img){
  int i,max;

  max = img->val[0];
  int n = img->nrows*img->ncols;
  for (i = 0; i < n; i++){
    if (img->val[i] > max) {
      max = img->val[i];
    }
  }
  return max;
}

ColorImage *createColorImage(int nx, int ny)
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
    Error(MSG1,"createColorImage");
  else{
    for (y=0; y < I->ny; y++){
      I->cor[y] = (Cor *)calloc(nx,sizeof(Cor));
      if (I->cor[y] == NULL)
        Error(MSG1,"createColorImage");
    }
  }
  return(I);
}

ColorImage *readColorImage(char *filename)
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
      int c = getc(fp);
      while (c == '#') {
        while (getc(fp) != '\n');
        c = getc(fp);
      }
      ungetc(c, fp);
      if (fscanf(fp, "%d %d", &nx, &ny) != 2) {
        fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
        exit(1);
      }

      I = createColorImage(nx,ny);

      if (fscanf(fp, "%d", &Imax) != 1) {
        fprintf(stderr, "Invalid rgb component (error loading '%s')\n",
                filename);
        exit(1);
      }

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
    }else if(strcmp(type,"P3")==0){
      int c = getc(fp);
      while (c == '#') {
        while (getc(fp) != '\n');
        c = getc(fp);
      }
      ungetc(c, fp);
      if (fscanf(fp, "%d %d", &nx, &ny) != 2) {
        fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
        exit(1);
      }

      I = createColorImage(nx,ny);

      if (fscanf(fp, "%d", &Imax) != 1) {
        fprintf(stderr, "Invalid rgb component (error loading '%s')\n",
                filename);
        exit(1);
      }

      I->Imax = Imax;

      int R=0,G=0,B=0;

      for (y=0; y < ny; y++) {
        for (x=0; x < nx; x++){
          (void)fscanf(fp, "%d %d %d",&R,&G,&B);
          I->cor[y][x].val[0] = R;
          I->cor[y][x].val[1] = G;
          I->cor[y][x].val[2] = B;
        }
      }
    }

    else{
      Error("ColorImage type invalid","ReadColormage");
    }

    fclose(fp);
  }else{
    Error(MSG2,"ReadColorImage");
  }

  return(I);
}

void writeColorImage(ColorImage *I, char *filename)
{
  FILE *fp=NULL;
  int  x,y;
  ushort rgb16[3];

  fp = fopen(filename,"w");
  if (fp != NULL) {
    fprintf(fp,"P6\n");
    fprintf(fp,"%d %d\n",I->nx,I->ny);

    int max_val = maximumColorValue(I);
    int min_val = minimumColorValue(I);

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

void destroyColorImage(ColorImage **I)
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


int minimumColorValue(ColorImage *img){

  int img_min_val[3];
  for (int i=0; i<3; i++){
    img_min_val[i] = minimumIntensityColor(img,i);
  }

  return MIN(MIN(img_min_val[RED],img_min_val[GREEN]),img_min_val[BLUE]);
}

int maximumColorValue(ColorImage *img){

  int img_max_val[3];
  for (int i=0; i<3; i++){
    img_max_val[i] = maximumIntensityColor(img,i);
  }

  return MAX(MAX(img_max_val[RED],img_max_val[GREEN]),img_max_val[BLUE]);
}



int minimumIntensityColor(ColorImage *img, int c){
  int i,j,min;

  min = INT_MAX;
  for (i = 0; i<img->nx; i++)
    for (j=0; j<img->ny; j++)
      if (img->cor[j][i].val[c] < min)
        min = img->cor[j][i].val[c];

  return min;
}

int maximumIntensityColor(ColorImage *img, int c){
  int i,j,max;

  max = -1;
  for (i = 0; i<img->nx; i++)
    for (j=0; j<img->ny; j++)
      if (img->cor[j][i].val[c] > max)
        max = img->cor[j][i].val[c];

  return max;
}

bool isValidPixelCoordinate(GrayImage *image,int pixelCoordinateX,int pixelCoordinateY){
  if(pixelCoordinateX < 0 || pixelCoordinateY < 0){
    return false;
  }
  if(pixelCoordinateX >= image->ncols || pixelCoordinateY >= image->ncols){
    return false;
  }
  return true;
}

bool isImagesSameDomain(GrayImage *image1,GrayImage *image2){

  if(image1->nrows == image2->nrows && image1->ncols == image2->ncols){
    return true;
  }
  return false;
}

void copyImage(GrayImage *image1,GrayImage **image2){
  if((*image2) == NULL){
    *image2 = createGrayImage(image1->ncols,image1->nrows);
  }

  if(isImagesSameDomain(image1,*image2)){
    for (int n = 0; n < image1->nrows*image1->ncols; ++n) {
      (*image2)->val[n] = image1->val[n];
    }
  }
  else{
    printf("different domains");
  }
}

GrayImage *imageSubtraction(GrayImage *image1, GrayImage *image2, bool saturation){
  GrayImage *outputImage = NULL;
  if(isImagesSameDomain(image1,image2)){
    outputImage = createGrayImage(image1->ncols,image1->nrows);
    int n = image1->ncols*image1->nrows;
    int result;
    for (int i = 0; i < n; ++i) {
      result = image1->val[i] - image2->val[i];
      if(saturation){
        result = (result<0)?0:result;
      }
      outputImage->val[i] = result;
    }
  }
  return outputImage;
}

int sumUpAllPixelsValues(GrayImage *image){
  int sum = 0;
  for (int n = 0; n < image->nrows*image->ncols; ++n) {
    sum += image->val[n];
  }
  return sum;
}


void writeImageAsASCII(GrayImage* image,const char *filename,unsigned int maxValueOnChanel){
  int i, j;
  FILE *fp = fopen(filename, "w");
  (void) fprintf(fp, "P2\n%d %d\n%d\n", image->ncols,image->nrows, (int) maxValueOnChanel);
  int k = 0;
  for (i = 0; i < image->nrows; i++){
    for (j = 0; j < image->ncols; j++){
      (void) fprintf(fp,"%d ",image->val[k]);
      k++;
    }
    (void) fprintf(fp,"\n");
  }
  (void) fclose(fp);
}

void writeImageAsASCII(ColorImage* image,const char *filename, unsigned int maxValueOnChanel){
  int i, j;
  FILE *fp = fopen(filename, "w");
  (void) fprintf(fp, "P3\n%d %d\n%d\n", image->nx,image->ny, (int) maxValueOnChanel);
  for (i = 0; i < image->ny; i++){
    for (j = 0; j < image->nx; j++){
      (void) fprintf(fp,"%d %d %d ",image->cor[i][j].val[0],image->cor[i][j].val[1],image->cor[i][j].val[2]);
    }
    (void) fprintf(fp,"\n");
  }
  (void) fclose(fp);
}

void writeImageAsBinary(GrayImage* image,const char *filename,unsigned int maxValueOnChanel){
  int i, j;
  FILE *fp = fopen(filename, "wb");
  (void) fprintf(fp, "P5\n%d %d\n%d\n", image->ncols,image->nrows, (int) maxValueOnChanel);
  int k = 0;
  for (i = 0; i < image->nrows; i++){
    for (j = 0; j < image->ncols; j++){
      static unsigned char gray[1];
      gray[0] = image->val[k];
      (void) fwrite(gray, 1, 1, fp);
      k++;
    }
    //(void) fprintf(fp,"\n");
  }
  (void) fclose(fp);
}

void writeImageAsBinary(ColorImage* image,const char *filename, unsigned int maxValueOnChanel){
  int i, j;
  FILE *fp = fopen(filename, "wb");
  (void) fprintf(fp, "P6\n%d %d\n%d\n", image->nx,image->ny, (int) maxValueOnChanel);
  for (i = 0; i < image->ny; i++){
    for (j = 0; j < image->nx; j++){
      static unsigned char color[3];
      color[0] = image->cor[i][j].val[0];
      color[1] = image->cor[i][j].val[1];
      color[2] = image->cor[i][j].val[2];
      (void) fwrite(color, 1, 3, fp);
    }
    //(void) fprintf(fp,"\n");
  }
  (void) fclose(fp);
}

void writeImage(ColorImage* image,const char *filename, const char *magicNumber,unsigned int maxValueOnChanel){
  //ASCII
  if (strcmp(magicNumber,"P3")==0){
    writeImageAsASCII(image,filename,maxValueOnChanel);
  }//Binary
  else if(strcmp(magicNumber,"P6")==0){
    writeImageAsBinary(image,filename,maxValueOnChanel);
  }else{
    fprintf(stderr,"magic number desconhecido. Por favor, acesse https://en.wikipedia.org/wiki/Netpbm_format "
            "para saber mais sobre");
  }
}

void writeImage(GrayImage* image,const char *filename, const char *magicNumber,unsigned int maxValueOnChanel){
  //ASCII
  if (strcmp(magicNumber,"P2")==0){
    writeImageAsASCII(image,filename,maxValueOnChanel);
  }//Binary
  else if(strcmp(magicNumber,"P5")==0){
    writeImageAsBinary(image,filename,maxValueOnChanel);
  }else{
    fprintf(stderr,"magic number desconhecido. Por favor, acesse https://en.wikipedia.org/wiki/Netpbm_format "
            "para saber mais sobre");
  }
}

void writeImage(GrayImage* image,const char *filename){
  writeImage(image,filename, "P5",255);
}

void writeImage(ColorImage* image,const char *filename){
  writeImage(image,filename, "P6",255);
}

ColorImage *RGBtoYCbCr(ColorImage *rgb)
{
  ColorImage *ycbcr;
  float a = 16.0;
  float b = 128.0;

  ycbcr = createColorImage(rgb->nx,rgb->ny);

  for (int y=0; y < rgb->ny; y++)
    for (int x=0; x < rgb->nx; x++){
      float lum  = (0.257*(float)rgb->cor[y][x].val[0]+
                    0.504*(float)rgb->cor[y][x].val[1]+
                    0.098*(float)rgb->cor[y][x].val[2]+a);
      float Cb = (-0.148*(float)rgb->cor[y][x].val[0]+
                  -0.291*(float)rgb->cor[y][x].val[1]+
                  0.439*(float)rgb->cor[y][x].val[2]+b);
      float Cr = (0.439*(float)rgb->cor[y][x].val[0]+
                  -0.368*(float)rgb->cor[y][x].val[1]+
                  -0.071*(float)rgb->cor[y][x].val[2]+b);

      if (lum < 0)   lum = 0.0;
      if (lum > 255) lum = 255;
      if (Cb < 0)   Cb = 0.0;
      if (Cb > 255) Cb = 255;
      if (Cr < 0)   Cr = 0.0;
      if (Cr > 255) Cr = 255;

      ycbcr->cor[y][x].val[0] = lum;
      ycbcr->cor[y][x].val[1] = Cb;
      ycbcr->cor[y][x].val[2] = Cr;

    }

  return(ycbcr);
}

GrayImage* extractColorChannelAsGrayImage(ColorImage* colorImage,int channel){
  GrayImage* grayImage= createGrayImage(colorImage->nx,colorImage->ny);
  int k=0;
  for (int i = 0; i < colorImage->ny; ++i) {
    for (int j = 0; j < colorImage->nx; ++j) {
      grayImage->val[k] = colorImage->cor[i][j].val[channel];
      k++;
    }
  }
  return grayImage;
}