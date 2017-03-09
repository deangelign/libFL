BIN=.

#FLAGS= -g -O0 -Wall -Wno-unused-result -std=gnu99 -pedantic
FLAGS= -O3  -Wall -Wno-unused-result -std=gnu99 -pedantic
FLLIB    = -L../lib -lFL -lm
FLFLAGS  = -I../include

libift:
	cd ../; $(MAKE) ; cd -\


$@.c: libft $@.c
	make
	$(CC) $(FLAGS) $@.c -o $(BIN)/$@ $(FLFLAGS) $(FLLIB) ; \

clean:	
	rm rwImage








