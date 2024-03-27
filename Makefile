main: http_parser.o redgrammm.o
	gcc -g -o $@ $^ -Wall -Wextra 

http_parser.o: http_parser.c redgrammm.h
	gcc -g -c $<

redgrammm.o: redgrammm.c redgrammm.h
	gcc -g -c $<

clean: 
	rm *.o
	rm main