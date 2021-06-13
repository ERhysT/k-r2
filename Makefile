CC=cc
CFLAGS=-Wall -g3 -O0 -DDEBUG
LIBS=-lm

.PHONY : clean

knr2.o : knr2.c knr2.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)
	etags $^

% : %.c knr2.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	etags $< knr2.c knr2.h

8-5 : 8-5.c 			# excercise with fsize
	$(CC) $(CFLAGS) -o $@ $^
	etags $^

io.o : io.c io.h		# excercises reimplementing stdio.h
	$(CC) $(CFLAGS) -c $< -o $@
8-% : 8-%.c io.o
	$(CC) $(CFLAGS) -o $@ $^ 

clean :
	rm knr2.o io.o TAGS 
