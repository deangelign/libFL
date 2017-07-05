#include "FL.h"

int main(int argc, char **argv) {

    size_t nclasses = 100;


    //Caminhos onde esta o arquivo txt gerado pelo o script python "selec_samples2.py"
    //os caminhos vao mudar para cada pessoa
    const char *  fileName_createDict = "/home/deangeli/databases/train_paths.txt";
    const char *  fileName_createTrain = "/home/deangeli/databases/train_paths.txt";
    const char *  fileName_createTest = "/home/deangeli/databases/test_paths.txt";

    //cada posicao do vetor tem uma string para o caminho de uma imagem
    GVector* vectorSamplesUsed2CreateDict =  splitsLinesInTextFile(fileName_createDict);
    GVector* vectorSamplesUsed2TrainClassifier =  splitsLinesInTextFile(fileName_createTrain);
    GVector* vectorSamplesUsed2TestClassifier =  splitsLinesInTextFile(fileName_createTest);

    //apenas checkando se o vetor vazio. Caso o vetor esteja vazio, talvez seu caminho ate o arquivo
    //txt nao esteja correto
    if(vectorSamplesUsed2CreateDict == NULL){
        printf("error\n");
        return -1;
    }
    if(vectorSamplesUsed2CreateDict->size == 0){
        printf("error\n");
        return -1;
    }

    if(vectorSamplesUsed2TrainClassifier == NULL){
        printf("error\n");
        return -1;
    }

    if(vectorSamplesUsed2TrainClassifier->size == 0){
        printf("error\n");
        return -1;
    }

    if(vectorSamplesUsed2TestClassifier == NULL){
        printf("error\n");
        return -1;
    }

    if(vectorSamplesUsed2TestClassifier->size == 0){
        printf("error\n");
        return -1;
    }
    int patchSizeX = 32;
    int patchSizeY = 32;

    //pipeline para a construncao do dicionario. Para mais detalhes olhe a imagem que esta
    //em data/bowArquiteturaImplementada.png

    //a estrutura bow manager e encarregada de fazer o processo do bow
    BagOfVisualWordsManager* bowManager = createBagOfVisualWordsManager();

    ////////////////////////////////////////////////////////////////////////
    //Passando os vetores que contem os caminhos das imagens para...
    bowManager->pathsToImages_dictionary = vectorSamplesUsed2CreateDict;//criar o dicionario
    bowManager->pathsToImages_train = vectorSamplesUsed2TrainClassifier;//treinar o classificador
    bowManager->pathsToImages_test = vectorSamplesUsed2TestClassifier;//testar o classificador
    //////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////
    //metodo de sampling que vai ser usado para criar os patchs. Se vc passar NULL aqui o estrutura
    // do bow vai criar um vetor de tamanho 1 onde o unico elemento desse vetor vai ser a imagem.
    bowManager->imageSamplerFunction = gridSamplingBow;//ponteiro da funcao para o sampling

    //Nesta demo o metodo de sampling  usado é o grid. Entao eu vou criar um argument list
    //para colocar os parametros do metodo de grinding que eu fiz.
    //Note que o cabecalho geral para a funcao de sammpling e
    //GVector* minhaFuncaoDeSampling(Image* image, BagOfVisualWordsManager* bagOfVisualWordsManager);
    ArgumentList* gridSamplingArguments = createArgumentList();
    ARGLIST_PUSH_BACK_AS(size_t,gridSamplingArguments,patchSizeX); //patch size X
    ARGLIST_PUSH_BACK_AS(size_t,gridSamplingArguments,patchSizeY); //patch size Y
    bowManager->argumentListOfSampler = gridSamplingArguments;//passando a lista de argumentos para o bow manager
    //////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////
    //Essa função serve como um garbage collector para o metodo do sampling. Ao final de
    //cada iteracao, ela limpa da memoria os patchs gerados.
    //Se por acaso seu metodo de sampling nao gerar um vetor de imagens (GVector* de Image*),
    //voce pode passar NULL, porém fique consciente que vai ter um pouco de memory leak.
    //Ao final do programa seu sistema operional vai limpar toda a sujeira.
    bowManager->freeFunction2SamplerOutput = destroyImageVoidPointer;
    /////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////
    //Neste exemplo eu irei usar o descritor de cores aprendindo em aula.
    bowManager->featureExtractorFunction = computeColorHistogramBow;//ponteiro da funcao para a extracao de features
    ArgumentList* colorFeatureExtractorArguments = createArgumentList();
    size_t nbins = 7;
    ARGLIST_PUSH_BACK_AS(size_t,colorFeatureExtractorArguments,nbins); //nBins per channel
    ARGLIST_PUSH_BACK_AS(size_t,colorFeatureExtractorArguments,nbins*nbins*nbins); //total number of channels
    bowManager->argumentListOfFeatureExtractor = colorFeatureExtractorArguments; //passando a lista de argumentos do feature extractor para o bow manager
    ///////////////////////////////////////

    ///////////////////////////////////////////////////////
    //Existem muitas maneiras de computar distancias entre pontos e vetores. A mais comum delas talvez
    //seja a distancia Euclidianda (norma l2). Neste exemplo eu vou usar a norma l1.
    //Quando vc implementar seu metodo de sampling, feature extraxtion, ou clustering, voce
    //pode usar essa funcao distancia.
    bowManager->distanceFunction = computeNormalizedL1Norm;
    bowManager->argumentListOfDistanceFunction = NULL;
    ////////////////////////////////////////////////////


    /////////////////////////////////////////////////////
    //Aqui precisamos definir qual funcao de clsutering vamos usar para encontrar as palavras
    //do dicionario. Eu optei de usar o kmeans clustering. Meu metodo do kmeans recebe 6 parametros,
    //desta forma eu preciso criar uma ArgumentList com 6 parametros.
    //Note que o cabecalho geral para a funcao de clustering e
    //typedef Matrix* minhaFuncaoDeClustering(Matrix* outputFeatureExtractor_allSamples, BagOfVisualWordsManager* bagOfVisualWordsManager);
    //bowManager->clusteringFunction = kmeansClusteringBow;
    //ArgumentList* clusteringMethodArguments = createArgumentList();
//    ARGLIST_PUSH_BACK_AS(size_t,clusteringMethodArguments,numberOfVisualWords); //number of words
//    ARGLIST_PUSH_BACK_AS(size_t,clusteringMethodArguments,100); //maximum number of iterations
//    ARGLIST_PUSH_BACK_AS(double,clusteringMethodArguments,0.0001); //tolerance
//    ARGLIST_PUSH_BACK_AS(int,clusteringMethodArguments,0); //seed
//    ARGLIST_PUSH_BACK_AS(DistanceFunction,clusteringMethodArguments,computeNormalizedL1Norm); //seed
//    ARGLIST_PUSH_BACK_AS(ArgumentList*,clusteringMethodArguments,NULL); //seed
    //bowManager->argumentListOfClustering = clusteringMethodArguments;


    bowManager->clusteringFunction = kmeansClusteringBow;
    //bowManager->clusteringFunction = clusteringGetAllSampleBow;
    //bowManager->clusteringFunction = clusteringSupervisedBow;
    bowManager->storeVisualWordsData = true;
    size_t numberOfVisualWords = 2000;
    ArgumentList* clusteringMethodArguments = createArgumentList();
    ARGLIST_PUSH_BACK_AS(size_t,clusteringMethodArguments,numberOfVisualWords);
    ARGLIST_PUSH_BACK_AS(DistanceFunction ,clusteringMethodArguments,computeNormalizedL1Norm);
    ARGLIST_PUSH_BACK_AS(ArgumentList* ,clusteringMethodArguments,NULL);
    bowManager->argumentListOfClustering = clusteringMethodArguments;
    ///////////////////////////////////////////////////////////////

    ////////////
    //computa o dicionario
    computeDictionary(bowManager);
    /////////////

    //////////////////////
    //define a funcao para montar o histograma
    bowManager->mountHistogramFunction = computeHardAssignmentHistogram_bow;
    ArgumentList* histogramMouterArguments = createArgumentList();
    ARGLIST_PUSH_BACK_AS(int,histogramMouterArguments,0);
    ARGLIST_PUSH_BACK_AS(DistanceFunction,histogramMouterArguments,computeL1Norm);
    ARGLIST_PUSH_BACK_AS(ArgumentList*,histogramMouterArguments,NULL);
    bowManager->argumentListOfHistogramMounter = histogramMouterArguments;
    //bowManager->argumentListOfHistogramMounter = NULL;
    ///////////////////////


    /////////////////////////////////////////////////
    //criar um classificador e define os parametros
    //do classficiador. Em seguida, o bow manager recebe
    //o ponteiro do classificador. Desta forma o classificador
    //podera ser usado internamente dentro do bow manager.

    //knn
//    Knn_Classifier* classifierknn = createKnnClassifier();
//    classifierknn->k = 1;
//    classifierknn->nlabels = nclasses;
//    bowManager->classifier = (void*)classifierknn;
//    bowManager->fitFunction = knn_Classifier_fit;
//    bowManager->storeTrainData = false;
//    bowManager->predictFunction = knn_Classifier_predict;
//    bowManager->storePredictedData = false;
//    bowManager->freeFunctionClassifier = destroyKnnClassifierForVoidPointer;


    //"kmeans classifier"
//    Kmeans_Classifier* classifierkmeans = createKmeansClassifier();
//    classifierkmeans->nlabels = nclasses;
//    bowManager->classifier = (void*)classifierkmeans;
//    bowManager->fitFunction = kmeans_Classifier_fit;
//    bowManager->storeTrainData = false;
//    bowManager->predictFunction = kmeans_Classifier_predict;
//    bowManager->storePredictedData = false;
//    bowManager->freeFunctionClassifier = destroyKmeansClassifierForVoidPointer;


    //SVM Classifier
    SVM_Classifier* classifiersvm = createSVMClassifier();
    classifiersvm->param.kernel_type = LINEAR;
    classifiersvm->param.gamma = 2;
    classifiersvm->crossValidation = false;
    bowManager->classifier = (void*)classifiersvm;
    bowManager->storeTrainData = false;
    bowManager->fitFunction = svm_Classifier_fit;
    bowManager->predictFunction = svm_Classifier_predict;
    bowManager->storePredictedData = false;
    bowManager->freeFunctionClassifier = destroySVMClassifierForVoidPointer;
    //////////////////////////////////////

    ///////
    //monta os histogramas, le os label e em seguida treina o classificador
    trainClassifier(bowManager);
    //////////

    /////////////////////////////////////////////////////
    //monta os histogramas e usa o classificador treinado para
    //classificar as amostras do conjunto de teste
    GVector* labelsPredicted = predictLabels(bowManager);
    //////////////////////////

    //////////////////////////
    //Le os true labels das imagens e checa com os labels predizidos pelo o classificador.
    //computa uma simples acuracia (numero de amostras rotuladas corretamente / numero de amostras do conjunto)
    GVector* trueLabels = createNullVector(bowManager->pathsToImages_test->size,sizeof(int));
    Matrix* confusionMAtrix = createMatrix(nclasses,nclasses,sizeof(float));
    GVector* hitPerClass = createNullVector(nclasses,sizeof(float));
    GVector* count = createNullVector(nclasses,sizeof(int));
    int hit = 0;
    printf("file | predicted true\n");
    //MATRIX_GET_ELEMENT_PO_AS(float,confusionMAtrix,0,predictedLabel-1) += 0;
    for (size_t index = 0; index < bowManager->pathsToImages_test->size; ++index) {
        char * path = VECTOR_GET_ELEMENT_AS(char*,bowManager->pathsToImages_test,index);
        VECTOR_GET_ELEMENT_AS(int,trueLabels,index) = findTrueLabelInName(path);

        int truelabel = VECTOR_GET_ELEMENT_AS(int,trueLabels,index);
        int predictedLabel = VECTOR_GET_ELEMENT_AS(int,labelsPredicted,index);
        //printf("%d %d %lu\n",(truelabel-1),predictedLabel-1,nclasses);
        MATRIX_GET_ELEMENT_PO_AS(float,confusionMAtrix,(truelabel-1),(predictedLabel-1)) += 1;
        VECTOR_GET_ELEMENT_AS(float,count, (truelabel-1)) += 1;
        if(truelabel == predictedLabel){
            VECTOR_GET_ELEMENT_AS(float,hitPerClass,(truelabel-1)) += 1;
            hit++;
        }
//        printf("%s | %d %d\n",
//               path,
//               VECTOR_GET_ELEMENT_AS(int,labelsPredicted,index),
//               VECTOR_GET_ELEMENT_AS(int,trueLabels,index)
//        );
    }

    for (size_t i = 0; i < nclasses; ++i) {
        for (size_t j = 0; j < nclasses; ++j) {
            MATRIX_GET_ELEMENT_PO_AS(float,confusionMAtrix,i,j) /= VECTOR_GET_ELEMENT_AS(float,count,i);
        }
        VECTOR_GET_ELEMENT_AS(float,hitPerClass,i) /= VECTOR_GET_ELEMENT_AS(float,count,i);
    }

    printf("\n");
//    printf("confusion matrix:\n");
//    MATRIX_PRINT_AS(float,"%f ",confusionMAtrix);
    printf("\n");
    printf("accuracia por classe:\n");
    for (size_t k = 1; k <= nclasses; ++k) {
        printf("%lu: %f\n",k,VECTOR_GET_ELEMENT_AS(float,hitPerClass,(k-1)));
    }

    double acuracia = ((double)hit)/bowManager->pathsToImages_test->size;
    printf("acuracia: %f\n",acuracia);
    /////////////////////////////////////
//
    destroyBagOfVisualWordsManager(&bowManager);
    destroyVector(&trueLabels);
    destroyVector(&labelsPredicted);
    destroyVector(&hitPerClass);
    destroyVector(&count);
    destroyMatrix(&confusionMAtrix);
    return 0;

}


