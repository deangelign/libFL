import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import sys
import csv
import os
from sklearn.manifold import TSNE


delimeter = ' ';
dataset = []
samples = []
originalLabels = []
mapOld2NewLabels = dict()
newLabels = []
colorTable = []
nSamples = 0
nFeatures = 0
nClasses = 0;
model = TSNE(n_components=2, random_state=0);


def readDataset():
    messageInputDataSet = "\n" \
                          "Digite o filepath do dataset\n"
    try:
        filepath = raw_input(messageInputDataSet)
        if(len(filepath) == 0 ):
            return
        global dataset,samples,originalLabels,colorTable,newLabels
        global nSamples,nFeatures,nClasses
        global mapOld2NewLabels
        dataset = np.loadtxt(filepath, delimiter=delimeter)
        [nSamples, nFeatures] = dataset.shape
        nFeatures -= 1
        samples = dataset[:,0:nFeatures]
        originalLabels = dataset[:,-1]
        newLabels = originalLabels;
        currentValue = 0

        for i in range(0,len(originalLabels)):
            if (originalLabels[i] not in mapOld2NewLabels):
                mapOld2NewLabels[originalLabels[i]] = currentValue
                currentValue += 1
            newLabels[i] = mapOld2NewLabels[originalLabels[i]]
        colorTable = np.random.random((len(mapOld2NewLabels),3))
        print "Dataset carregado com sucesso\n"
    except ValueError:
        print "Houve um erro ao tentar carregar o dataset: " + str(ValueError)

    return

def projection():
    if (len(dataset) == 0):
        print "nenhum dataset carregado\n"
        return

    x = model.fit_transform(samples);

    colors = ['b', 'c', 'y', 'm', 'r','k','g']
    scatters = list();
    legends = list();
    for i in mapOld2NewLabels:
        newLabel = mapOld2NewLabels[i]
        indices = np.where(newLabels == newLabel)
        px = x[indices,0]
        py = x[indices,1]
        scatters.append(plt.scatter(px[0, :], py[0, :],
                    color=colors[newLabel]));
        legends.append(str(i))
    plt.legend(scatters,legends,scatterpoints=1)
    plt.show()



def clear():
    if (os.name == 'nt'):
        os.system('cls')
    #if (os.name == 'posix'):
    else:
        os.system('clear')

def changeDelimeter():
    global delimeter;
    messageInputDelimeter = "\n" \
                          "Digite o novo delimitador:\n"
    temp = raw_input(messageInputDelimeter)
    if (len(temp) == 0):
        return
    delimeter = temp
    print "novo delimitador definido com sucesso\n"


messageInputKey = "\n" \
               "1 - Carregar dataset\n" \
               "2 - Projetar e visualizar amostras no plano\n" \
               "d - Mudar delimitador\n" \
               "c - Limpar o console\n" \
               "e - Sair do programa\n"

while (True):
    op = raw_input(messageInputKey)

    if op == '1':
    elif op == '2':
        projection()
    elif op == 'd':
        changeDelimeter()
    elif op == 'c':
        clear();
    elif op == 'e':
        break
    else:
        print "\nOpcao invalida\n"

