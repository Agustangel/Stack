
#include <stdio.h>
#include <assert.h>

#include "stack.h"


stack_t* create_stack()
{
    stack_t* stack = NULL;

    stack = (stack_t*) malloc(sizeof(stack_t));

    if(stack == NULL)
    {
        printf("ERROR: memory failure.\n");
        exit(ERR_OUT_MEMORY);
    }

    stack->capacity = INIT_SIZE;
    stack->count = 0;

    stack->data = (stack_t*) calloc(stack->capacity, sizeof(int));

    if (stack->data == NULL)
    {
        free(stack);

        printf("ERROR: memory failure.\n");
        exit(ERR_OUT_MEMORY);
    }

    return stack;
}
