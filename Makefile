
INCLUDES=include
SOURCE=source

stack: main.o stack.o
	gcc -o stack main.o stack.o -llogger

main.o: main.c ${INCLUDES}/stack.h
	gcc -g -O0 -I ${INCLUDES} $(ARGS) -c  -DINT main.c

stack.o: ${SOURCE}/stack.c ${INCLUDES}/stack.h
	gcc -g -O0 -I ${INCLUDES} $(ARGS) -c -DINT ${SOURCE}/stack.c

valgrind: stack
	valgrind --leak-check=yes ./stack $(ARGS)

sanitize: main.o stack.o
	gcc -o stack main.o stack.o -llogger -fsanitize=address -g

clean:
	rm stack main.o stack.o
