
CFLAGS := -g -lm -lpthread
#LDLIBS := -lbsd

all: wordcount

wordcount: wordcount.o svec.o
	gcc $(CFLAGS) -o $@ $^ $(LDLIBS)

wordcount.o: wordcount.c svec.h
	gcc $(CFLAGS) -c -o $@ $<

svec.o: svec.c svec.h
	gcc $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o wordcount valgrind.out *.plist

.PHONY: all clean
