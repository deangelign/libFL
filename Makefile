LIB=./lib
INCLUDE=./include
SRC=./src
OBJ=./obj
BIN=./bin


#FLAGS= -g  -O0 -Wall -D _DEBUG -Wno-unused-result -fPIC -std=gnu99 -pedantic
FLAGS=  -O3 -Wall -Wno-unused-result -fPIC  -pedantic -Wno-write-strings -fopenmp
#-std=gnu99

libFL: $(LIB)/libFL.a
	echo "libFL.a built..."

$(LIB)/libFL.a: \
$(OBJ)/image.o \
$(OBJ)/common.o \
$(OBJ)/histogram.o \

	ar csr $(LIB)/libFL.a \
$(OBJ)/common.o \
$(OBJ)/image.o \
$(OBJ)/histogram.o \

$(OBJ)/common.o: $(SRC)/common.cpp
	$(CC) $(FLAGS) -c $(SRC)/common.cpp -I$(INCLUDE) \
	-o $(OBJ)/common.o 

$(OBJ)/image.o: $(SRC)/image.cpp
	$(CC) $(FLAGS) -c $(SRC)/image.cpp -I$(INCLUDE) \
	-o $(OBJ)/image.o 

$(OBJ)/histogram.o: $(SRC)/histogram.cpp
	$(CC) $(FLAGS) -c $(SRC)/histogram.cpp -I$(INCLUDE) \
	-o $(OBJ)/histogram.o

clean: 
	rm $(LIB)/lib*.a; rm $(OBJ)/*.o;





