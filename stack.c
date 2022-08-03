
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "stack.h"

#define DEBUG 0

//===================================================================

stack_t* stack_create()
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

    stack->data = (int*) calloc(stack->capacity, sizeof(int));

    if (stack->data == NULL)
    {
        free(stack);

        printf("ERROR: memory failure.\n");
        exit(ERR_OUT_MEMORY);
    }

    return stack;
}

//===================================================================

void stack_destroy(stack_t* stack)
{
    assert(stack != NULL);

    free(stack->data);
    free(stack);

    stack = NULL;
}

//===================================================================

void stack_resize_increase(stack_t* stack)
{
    assert(stack != NULL);

    #ifdef DEBUG
        printf("stack->capacity = %d\n", stack->capacity);
    #endif

    stack->capacity *= MULTIPLIER;

    #ifdef DEBUG
        printf("stack->capacity = %d\n", stack->capacity);
    #endif

    stack->data = (int*) realloc(stack->data, stack->capacity * sizeof(int));

    if(stack->data == NULL)
    {
        printf("ERROR: memory failure.\n");
        exit(ERR_STACK_OVERFLOW);
    }
}

//===================================================================

void stack_resize_decrease(stack_t* stack)
{
    assert(stack != NULL);

    stack->capacity = stack->count;

    #ifdef DEBUG
        printf("stack->capacity = %d\n", stack->capacity);
    #endif

    stack->data = (int*) realloc(stack->data, stack->capacity * sizeof(int));
}

//===================================================================

int stack_pop(stack_t* stack)
{
    assert(stack != NULL);

    #ifdef DEBUG
        printf("stack->count = %d\n", stack->count);
    #endif

    if(stack->count == 0)
    {
        printf("ERROR: no more items.\n");
        exit(ERR_STACK_UNDERFLOW);
    }

    --(stack->count);

    #ifdef DEBUG
        printf("stack->count = %d\n", stack->count);
    #endif

    return stack->data[stack->count];
}

//===================================================================

int stack_peek(const stack_t* stack)
{
    assert(stack != NULL);

    #ifdef DEBUG
        printf("stack->count = %d\n", stack->count);
    #endif

    if(stack->count == 0)
    {
        printf("ERROR: no more items.\n");
        exit(ERR_STACK_UNDERFLOW);
    }

    return stack->data[stack->count - 1];
}

//===================================================================

void stack_push(stack_t* stack, int value)
{
    assert(stack != NULL);
    assert(value != NAN);

    #ifdef DEBUG
        printf("stack->count = %d\n", stack->count);
        printf("stack->capacity = %d\n", stack->capacity);
    #endif

    if(stack->count >= stack->capacity)
    {
        stack_resize_increase(stack);
    }

    stack->data[stack->count] = value;

    #ifdef DEBUG
        printf("value = %d\n", value);
        printf("stack->data[%d] = %d\n", stack->count, stack->data[stack->count]);
    #endif

    ++stack->count;
}

//===================================================================

void stack_print(stack_t* stack)
{
    assert(stack != NULL);

    #ifdef DEBUG
        printf("stack->count = %d\n", stack->count);
    #endif

    for(int idx = 0; idx < stack->count; ++idx)
    {
        printf("stack->data[%d] = %d\n", idx, stack->data[idx]);
    }
}

//===================================================================
