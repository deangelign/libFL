#include "FL.h"

int main(int argc, char **argv) {

    SVM_Classifier* svm = createSVMClassifier();
    int nrows = 6;
    int ncols = 4;

    double *data = (double*)calloc(nrows*ncols,sizeof(double));
    double *labels = (double*)calloc(nrows,sizeof(double));

    int k = 0;
    for (int i = 0; i < nrows; ++i) {
        labels[i] = (i%3) + 1;
        //printf("%d",(int)labels[i]);
        for (int j = 0; j < ncols; ++j) {
            data[k] = (i*ncols)+j;
            //printf(" %d:%f",j+1,data[k]);
        }
        //printf("\n");
    }
    svmClassifierFit(svm,data,nrows,ncols,labels);

//    free(data);
//    free(labels);
    return 0;
}


