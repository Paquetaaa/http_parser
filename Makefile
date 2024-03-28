COMP = gcc
HEADER_GRAM = http_parser.h ##Sinon : redgrammm.h
EXTRA = -g -Wall -Wextra

main: http_parser.o struct_parser.o
	$(COMP) http_parser.o struct_parser.o -o main $(EXTRA)

http_parser.o: http_parser.c struct_parser.o
	$(COMP) http_parser.c -o http_parser.o $(EXTRA)

struct_parser.o: struct_parser.c struct_parser.h $(HEADER_GRAM)
	$(CMP) struct_parser.c -o struct_parser.o $(EXTRA)

test: main
	./main

clean:
	rm main
	rm *.o


