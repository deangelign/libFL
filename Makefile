#COMPILER = $(CXX)
COMPILER = g++
LIB=./lib
INCLUDE=./include
SRC=./src
OBJ=./obj
BIN=./bin

FLLIBPNGINCLUDES = -I./externals/libpng/include
FLLIBJPEGINCLUDES = -I./externals/libjpeg/include
FLLIBTIFFINCLUDES = -I./externals/libtiff/include
FLLIBSVMINCLUDES = -I./externals/libsvm/include

INCLUDEALL = $(FLLIBPNGINCLUDES) $(FLLIBJPEGINCLUDES) $(FLLIBSVMINCLUDES) -I$(INCLUDE)

FLAGS=  -O3 -Wall -Wno-unused-result -pedantic -Wno-write-strings  -fsanitize=address -fopenmp


#######################ift things
FLLIBIFTINCLUDES = -I./externals/libift/include
LIBCBLAS_LD  = -llapack -lblas -lcblas
LIBIFT_LD    = -L ./externals/libift/lib -lift
#######################

USE_IFT=0
ifeq ($(USE_IFT), 1)
	export FLAGS += -DUSE_IFT=1
    INCLUDEALL += $(FLLIBIFTINCLUDES)
    LINKER += $(LIBIFT_LD) $(LIBCBLAS_LD)
endif


libFL:  lpng ljpeg libsvm $(LIB)/libFL.a
	echo "libFL.a built..."


$(LIB)/libFL.a: \
$(OBJ)/common.o \
$(OBJ)/linkedlist.o \
$(OBJ)/argumentList.o \
$(OBJ)/vector.o \
$(OBJ)/matrix.o \
$(OBJ)/file.o \
$(OBJ)/adjacencyRelation.o \
$(OBJ)/featureVector.o \
$(OBJ)/gif.o \
$(OBJ)/kernel.o \
$(OBJ)/image.o \
$(OBJ)/histogram.o \
$(OBJ)/morphology.o \
$(OBJ)/filtering.o \
$(OBJ)/sampling.o \
$(OBJ)/featureExtractor.o \
$(OBJ)/clustering.o \
$(OBJ)/classifiers.o \
$(OBJ)/bagOfVisualWords.o
	ar csr $(LIB)/libFL.a $(OBJ)/*.o \
./externals/zlib/obj/*.o \
./externals/libpng/obj/*.o \
./externals/libjpeg/obj/*.o \
./externals/libsvm/obj/*.o


# generic src compilation
$(OBJ)/%.o: $(SRC)/%.cpp $(INCLUDE)/%.h
	$(COMPILER) $(INCLUDEALL) $(FLAGS)  -c $<  -o $@


lpng: ./externals/libpng/source/ ./externals/libpng/include
	cd externals/libpng; $(MAKE) ; cd -\

ljpeg: ./externals/libjpeg/source ./externals/libjpeg/include
	cd externals/libjpeg; $(MAKE) ; cd -\

libtiff:
	cd externals/libtiff; (MAKE) ; cd -\

libsvm:
	cd externals/libsvm; $(MAKE) ; cd -\

clean:
	rm $(LIB)/lib*.a; rm $(OBJ)/*.o

cleanAll:
	rm $(LIB)/lib*.a; rm $(OBJ)/*.o;\
	cd externals/libpng; $(MAKE) clean; \
	cd ..; cd libjpeg;  $(MAKE) clean; \
	cd ..; cd zlib; $(MAKE) clean; \
	cd ..; cd libsvm; $(MAKE) clean;  \
	cd ..; cd libtiff; $(MAKE) clean; cd -\
