CC=gcc
LD=gcc

CFLAGS=$(NIX_CFLAGS_COMPILE)
LD_FLAGS=$(NIX_LDFLAGS)
LIBS=-lusb-1.0 -lc


OBJ=main.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f *.o *~ core main
