
INCLUDES=include
SOURCE=source

stack: main.o stack.o
	gcc -o stack main.o stack.o -llogger

main.o: main.c ${INCLUDES}/stack.h
	gcc -g -O0 -I ${INCLUDES} $(ARGS) -c  -DPTR -DREMOVE_SAFETY main.c

stack.o: ${SOURCE}/stack.c ${INCLUDES}/stack.h
	gcc -g -O0 -I ${INCLUDES} $(ARGS) -c -DPTR -DREMOVE_SAFETY ${SOURCE}/stack.c

valgrind: stack
	valgrind --leak-check=yes ./stack $(ARGS)

sanitize: main.o stack.o
	gcc -o stack main.o stack.o -llogger -fsanitize=address -g

clean:
	rm stack main.o stack.o
