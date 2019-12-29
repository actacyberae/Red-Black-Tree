all: main

main-valgrind: main.o lib.o RBT.o
	$(CC) main.o lib.o RBT.o -o main
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all ./main
	rm -rf *.o
	
main: main.o lib.o RBT.o
	$(CC) main.o lib.o RBT.o -lm -o main
	rm -rf *.o
	
lib.o: ./libs/lib.c
	$(CC) ./libs/lib.c -c ./libs/lib.c
	
RBT.o: ./libs/RBT.c
	$(CC) ./libs/RBT.c -c ./libs/RBT.c
	
insertion_main.o: main.c
	$(CC) main.c -c main.c
	
clean:
	rm -rf *.o main
