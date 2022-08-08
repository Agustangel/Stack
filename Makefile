
INCLUDES=include
SOURCE=source

stack: main.o stack.o
	gcc -o stack main.o stack.o -llogger

main.o: main.c ${INCLUDES}/stack.h
	gcc -g -O0 -I ${INCLUDES} -c main.c -fno-stack-protector

stack.o: ${SOURCE}/stack.c ${INCLUDES}/stack.h
	gcc -g -O0 -I ${INCLUDES} -c ${SOURCE}/stack.c -fno-stack-protector

clean:
	rm stack main.o stack.o
