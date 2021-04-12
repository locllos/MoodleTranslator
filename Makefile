FLAGS = -mavx2 -O0 -march=native -g3 #-fsanitize=address

all: main
	-rm -rf bin/*.o 

main: bin/hashTable.o bin/Array.o bin/Translator.o bin/Parser.o bin/HolyPoly.o
	gcc $(FLAGS) src/main.cpp bin/hashTable.o bin/HolyPoly.o bin/Array.o bin/Translator.o bin/Parser.o -o main

bin/hashTable.o: src/hashTable/hashTable.cpp
	gcc $(FLAGS) -c src/hashTable/hashTable.cpp -o bin/hashTable.o

bin/Array.o: src/Array/Array.cpp
	gcc $(FLAGS) -c src/Array/Array.cpp -o bin/Array.o

bin/Translator.o: src/Translator/Translator.cpp
	gcc $(FLAGS) -c src/Translator/Translator.cpp -o bin/Translator.o

bin/HolyPoly.o:
	nasm -felf64 -g -F dwarf src/Translator/HolyPoly.nasm -o bin/HolyPoly.o

bin/Parser.o: src/Parser/Parser.cpp
	gcc $(FLAGS) -c src/Parser/Parser.cpp -o bin/Parser.o

