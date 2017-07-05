#COMPILER = $(CXX)
COMPILER = g++

FLAGS = -Wall -Wno-unused-result  -pedantic -Wno-write-strings -fsanitize=address -g -fopenmp

FLINCLUDES  = -I../include
FLLIBPNGINCLUDES = -I../externals/libpng/include
FLLIBJPEGINCLUDES = -I../externals/libjpeg/include
FLLIBTIFFINCLUDES = -I../externals/libtiff/include
FLLIBSVMINCLUDES = -I../externals/libsvm/include
ALL_INCLUDES =  $(FLLIBPNGINCLUDES) $(FLLIBJPEGINCLUDES) $(FLLIBSVMINCLUDES) $(FLINCLUDES)


FL = -L../lib -lFL
LINKER =  $(FL)

USE_IFT=0
#######################ift things
FLLIBIFTINCLUDES = -I../externals/libift/include
LIBCBLAS_LD  = -llapack -lblas -lcblas
LIBIFT_LD    = -L ../externals/libift/lib -lift
#######################

#USE_IFT = 1
ifeq ($(USE_IFT), 1)
	export FLAGS += -DUSE_IFT=1
    ALL_INCLUDES += $(FLLIBIFTINCLUDES)
    LINKER += $(LIBIFT_LD) $(LIBCBLAS_LD)
endif

libFL:
	cd ../; $(MAKE) ; cd demo\

$@.cpp: libFL $@.cpp
	make
	$(COMPILER) $(ALL_INCLUDES) $(FLAGS) $@.cpp -o $@.bin  $(LINKER); \

clean:
	rm *.bin ;


cleanImages:
	rm *.ppm *.pgm *.gif *.png *.jpg *.jpeg

cleanText:
	rm *.txt
