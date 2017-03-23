#include "common.h"

char *AllocCharArray(int n)
{
  char *v=NULL;
  v = (char *) calloc(n,sizeof(char));
  if (v == NULL)
    Error(MSG1,"AllocCharArray");
  return(v);
}

uchar *AllocUCharArray(int n)
{
  uchar *v=NULL;
  v = (uchar *) calloc(n,sizeof(uchar));
  if (v == NULL)
    Error(MSG1,"AllocUCharArray");
  return(v);
}

ushort *AllocUShortArray(int n)
{
  ushort *v=NULL;
  v = (ushort *) calloc(n,sizeof(ushort));
  if (v == NULL)
    Error(MSG1,"AllocUShortArray");
  return(v);
}

int *AllocIntArray(int n)
{
  int *v=NULL;
  v = (int *) calloc(n,sizeof(int));
  if (v == NULL)
    Error(MSG1,"AllocIntArray");
  return(v);
}

float *AllocFloatArray(int n)
{
  float *v=NULL;
  v = (float *) calloc(n,sizeof(float));
  if (v == NULL)
    Error(MSG1,(char*)"AllocFloatArray");
  return(v);
}

double *AllocDoubleArray(int n)
{
  double *v=NULL;
  v = (double *) calloc(n,sizeof(double));
  if (v == NULL)
    Error(MSG1,(char*)"AllocDoubleArray");
  return(v);
}

void Error(char *msg,char *func){ /* It prints error message and exits
                                    the program. */
  fprintf(stderr,"Error:%s in %s\n",msg,func);
  exit(-1);
}

void Warning(char *msg,char *func){ /* It prints warning message and
                                       leaves the routine. */
  fprintf(stdout,"Warning:%s in %s\n",msg,func);

}

void Change(int *a, int *b){ /* It changes content between a and b */
  int c;
  c  = *a;
  *a = *b;
  *b = c;
}

int NCFgets(char *s, int m, FILE *f) {
  while(fgets(s,m,f)!=NULL)
    if (s[0]!='#') return 1;
  return 0;
}


/**
 * Gera um número inteiro aleatório no intervalo [low,high].
http://www.ime.usp.br/~pf/algoritmos/aulas/random.html
 **/

int RandomInteger (int low, int high){
  int k;
  double d;

  d = (double) rand () / ((double) RAND_MAX + 1);
  k = d * (high - low + 1);
  return low + k;
}

FeatureVector* createFeatureVector(int size){
  FeatureVector* featureVector = (FeatureVector*)calloc(1,sizeof(FeatureVector));
  featureVector->size = size;
  featureVector->features = (float*)calloc(size, sizeof(float));
  return featureVector;
}
void destroyFeatureVector(FeatureVector** featureVector){
  free((*featureVector)->features);
  free((*featureVector));
  (*featureVector) = NULL;
}

void printFeatureVector(FeatureVector* featureVector){
  for (int i = 0; i < featureVector->size; ++i) {
    printf("%f ",featureVector->features[i]);
  }
}

FeatureVector* mergeFeatureVectors(FeatureVector* vector1,FeatureVector* vector2){
  FeatureVector *mergedVector = NULL;
  if(vector1 == NULL || vector2 == NULL){
    printf("vector1 or/and vector2 are NULL\n");
    return mergedVector;
  }
  int mergedSize = vector1->size + vector2->size;
  mergedVector = createFeatureVector(mergedSize);
  for (int i = 0; i < vector1->size; ++i) {
    mergedVector->features[i] = vector1->features[i];
  }

  for (int i = 0; i < vector2->size; ++i) {
    mergedVector->features[i+vector1->size] = vector2->features[i];
  }
  return mergedVector;
}

FeatureVector* createFeatureVector(float* vec,int size){
  FeatureVector* featureVector = (FeatureVector*)calloc(1,sizeof(FeatureVector));
  featureVector->size = size;
  featureVector->features = vec;
  return featureVector;
}


void wirteFeatureVectors(FeatureVector** vectors, int nVectors, char *filename){
  FILE *fp = fopen(filename,"w");
  for (int i = 0; i < nVectors; ++i) {
    FeatureVector* vec = vectors[i];
    for (int j = 0; j < vec->size; ++j) {
      fprintf(fp,"%f",vec->features[j]);
      if(!(j == vec->size-1)){
        fprintf(fp," ");
      }
    }
    fprintf(fp,"\n");
  }
}


float vectorDifference(FeatureVector* vector1,FeatureVector* vector2){
  if(vector1->size != vector2->size){
    printf("vectors mismatch dimensions\n");
    return -1.0;
  }
  float difference = 0;
  FeatureVector* vector = createFeatureVector(vector1->size);
  float diff;
  for (int i = 0; i < vector1->size; ++i) {
    diff = (vector1->features[i]-vector2->features[i]);
    if(diff < 0){
      diff *= -1;
    }
    vector->features[i] = diff;
    //vector->features[i] = (vector1->features[i]-vector2->features[i])*(vector1->features[i]-vector2->features[i]);
    //vector->features[i] = sqrtf(vector->features[i]);
    difference += vector->features[i];
  }
  destroyFeatureVector(&vector);
  return difference;
}
