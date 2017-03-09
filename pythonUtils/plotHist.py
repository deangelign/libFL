import csv
import numpy as np
import matplotlib.pyplot as plt
import sys

inFile = sys.argv[1]
colBased = True;
if(len(sys.argv) == 3):
  if (sys.argv[2] == 'False'):
    colBased = False;
  else:
    colBased = True;

inputData = np.loadtxt(inFile,delimiter=' ')
if(colBased):
  x = inputData[:, 0]
  y = inputData[:, 1]
else:
  x = inputData[0, :]
  y = inputData[1, :]

plt.bar(x,y)
plt.title("Histogram")
plt.xlabel("bins")
plt.ylabel("counts")
plt.show();


