parapat: parpat.o
	gcc -shared -o libparapat.so parpat.o

example: example.o
	gcc -L./ example.o -lparapat -o example

example.o:
	gcc example.c -c

parpat.o:
	gcc parpat.c -c

clean :
	rm example.o parpat.o example
