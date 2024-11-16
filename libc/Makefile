CC=gcc
CFLAGS=-m32 -ffreestanding -nostdlib
OBJS=objs/__internal.o objs/stdio.o objs/unistd.o objs/stdlib.o objs/math.o

libc.a: $(OBJS)
	ar rcs libc.a $(OBJS)

objs/__internal.o: __internal.c __internal.h
	$(CC) $(CFLAGS) -c __internal.c -o objs/__internal.o

objs/stdio.o: stdio.c stdio.h
	$(CC) $(CFLAGS) -c stdio.c -o objs/stdio.o

objs/unistd.o: unistd.c unistd.h
	$(CC) $(CFLAGS) -c unistd.c -o objs/unistd.o

objs/stdlib.o: stdlib.c stdlib.h
	$(CC) $(CFLAGS) -c stdlib.c -o objs/stdlib.o

objs/math.o: math.c math.h
	$(CC) $(CFLAGS) -c math.c -o objs/math.o

clean:
	rm -f *.o
	rm -f objs/*.o
	rm libc.a