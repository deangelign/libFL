LIB=./lib
INCLUDE=./include
SRC=./src
OBJ=./obj
BIN=./bin


#FLAGS= -g  -O0 -Wall -D _DEBUG -Wno-unused-result -fPIC -std=gnu99 -pedantic
FLAGS=  -O3 -Wall -Wno-unused-result -fPIC -std=gnu99 -pedantic

libFL: $(LIB)/libFL.a
	echo "libFL.a built..."

$(LIB)/libFL.a: \
$(OBJ)/image.o \
$(OBJ)/common.o \

	ar csr $(LIB)/libFL.a \
$(OBJ)/common.o \
$(OBJ)/image.o \

$(OBJ)/common.o: $(SRC)/common.c
	$(CC) $(FLAGS) -c $(SRC)/common.c -I$(INCLUDE) \
	-o $(OBJ)/common.o 

$(OBJ)/image.o: $(SRC)/image.c
	$(CC) $(FLAGS) -c $(SRC)/image.c -I$(INCLUDE) \
	-o $(OBJ)/image.o 

clean: 
	rm $(LIB)/lib*.a; rm $(OBJ)/*.o;





