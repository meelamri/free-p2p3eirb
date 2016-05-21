CPPFLAGS=-D_FILE_OFFSET_BITS=64 
CFLAGS=-Wno-unused-label -Wno-unused-function -g


PROGRAMS = tracker

all: tracker


tracker: tracker.c files.c scanner.c lex.yy.c

	gcc $(CPPFLAGS) $(CFLAGS) -Wall $^  -o $@ -lpthread



clean:
	$(RM) $(PROGRAMS) *.log 
