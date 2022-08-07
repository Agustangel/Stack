
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <logger.h>

#include "stack.h"

static int* canary_1_ = (int*) 0xDEADBEAF;
static int* canary_2_ = (int*) 0xBACBEDEF;
static int* canary_3_ = (int*) 0xAD666FED;
static int* canary_begin_array_ = (int*) 0xBAD0BEDA;

static int flag_multiplier_upper = TWO;
static int flag_multiplier_down  = ONEHALF;

//===================================================================
int stack_init(stack_t* stack, int init_size)
{
    if(stack == NULL)
    {
        return ERR_NULL_POINTER;
    }

    stack->canary_1 = (int*) 0xDEADBEAF;
    stack->canary_2 = (int*) 0xBACBEDEF;
    stack->canary_3 = (int*) 0xAD666FED;
    int* canary_begin_array = (int*) 0xBAD0BEDA;

    stack->capacity = init_size + sizeof(canary_begin_array) / sizeof(int);
    stack->count = 0;

    stack->data = (int*) calloc(stack->capacity, sizeof(int));

    if(stack->data == NULL)
    {
        return ERR_OUT_MEMORY;
    }

    memcpy(stack->data, &canary_begin_array, sizeof(canary_begin_array));

    for(int idx = 0; idx < init_size; ++idx)
    {
        *(stack->data + sizeof(canary_begin_array) + idx * sizeof(int)) = POISON;
    }

    return 0;
}

//===================================================================

int stack_destroy(stack_t* stack)
{
    if(stack == NULL)
    {
        return ERR_NULL_POINTER;
    }

    free(stack->data);

    stack = NULL;
}

//===================================================================

int stack_resize_increase(stack_t* stack)
{
    if(stack == NULL)
    {
        return ERR_NULL_POINTER;
    }

    LOG("stack->capacity = %d\n", stack->capacity);
    
    if(flag_multiplier_down == TWO)
    {
        flag_multiplier_upper = ONEHALF;

        stack->capacity = (stack->count * MULTIPLIER_2) + (sizeof(int*) / sizeof(int));
        LOG("after stack->capacity = %d\n", stack->capacity);

        int* check_ptr = (int*) realloc(stack->data, (stack->capacity) * sizeof(int));

        if(check_ptr != NULL)
        {
            stack->data = check_ptr;
        }
        else
        {
            return ERR_OUT_MEMORY;\
            // must not be exited
        }
    }

    if(flag_multiplier_down == ONEHALF)
    {
        flag_multiplier_upper = TWO;

        stack->capacity = (stack->count / MULTIPLIER_1) + (sizeof(int*) / sizeof(int));
        LOG("after stack->capacity = %d\n", stack->capacity);

        int* check_ptr = (int*) realloc(stack->data, (stack->capacity) * sizeof(int));

        if(check_ptr != NULL)
        {
            stack->data = check_ptr;
        }
        else
        {
            return ERR_OUT_MEMORY;\
            // must not be exited
        }
    }
}

//===================================================================

int stack_resize_decrease(stack_t* stack)
{
    if(stack == NULL)
    {
        return ERR_NULL_POINTER;
    }

    LOG("stack->capacity = %d\n", stack->capacity);

    if(flag_multiplier_upper == TWO)
    {
        flag_multiplier_down = ONEHALF;

        stack->capacity = (stack->count / MULTIPLIER_2) + (sizeof(int*) / sizeof(int));
        LOG("after stack->capacity = %d\n", stack->capacity);

        stack->data = (int*) realloc(stack->data, stack->capacity * sizeof(int));
    }

    if(flag_multiplier_upper == ONEHALF)
    {
        flag_multiplier_down = TWO;

        stack->capacity = (stack->count / MULTIPLIER_1) + (sizeof(int*) / sizeof(int));
        LOG("after stack->capacity = %d\n", stack->capacity);

        stack->data = (int*) realloc(stack->data, stack->capacity * sizeof(int));
    }
}

//===================================================================
//to call stack_resize_decrease in stack_pop
int stack_pop(stack_t* stack)
{
    if(stack == NULL)
    {
        return ERR_NULL_POINTER;
    }

    //LOG("stack->count = %d\n", stack->count);

    if(stack->count == 0)
    {
        printf("ERROR: no more items.\n");
        exit(ERR_STACK_UNDERFLOW);
    }
    --(stack->count);
    LOG("stack->count = %d\n", stack->count);

    
    int capacity = stack->capacity - sizeof(int*) / sizeof(int);
    stack_dump(stack);


    if(stack->count <= capacity / 2)
    {
        stack_resize_decrease(stack);
    }

    stack_dump(stack);

    return *(stack->data + sizeof(canary_begin_array_) + stack->count * sizeof(int));
}

//===================================================================

int stack_peek(const stack_t* stack)
{
    if(stack == NULL)
    {
        return ERR_NULL_POINTER;
    }

    //LOG("stack->count = %d\n", stack->count);

    if(stack->count == 0)
    {
        printf("ERROR: no more items.\n");
        return ERR_STACK_UNDERFLOW;
    }

    return *(stack->data + sizeof(canary_begin_array_) + (stack->count - 1) * sizeof(int));
}

//===================================================================

int stack_push(stack_t* stack, int value)
{
    if(stack == NULL)
    {
        return ERR_NULL_POINTER;
    }

    //LOG("stack->count = %d\n", stack->count);
    //LOG("stack->capacity = %d\n", stack->capacity);

    // capacity should not be changed during push
    // commit more often
    // write verifier earlier
    int capacity = stack->capacity - sizeof(int*) / sizeof(int);

    if(stack->count >= capacity)
    {
        stack_resize_increase(stack);
    }

    *(stack->data + sizeof(canary_begin_array_) + stack->count * sizeof(int)) = value;


    //LOG("value = %d\n", value);
    //printf("stack->data[%d] = %d\n", stack->count, *(stack->data + sizeof(canary_begin_array_) + stack->count * sizeof(int)));
    //stack_dump(stack);
    ++(stack->count);
}

//===================================================================

int stack_dump(stack_t* stack)
{
    if(stack == NULL)
    {
        return ERR_NULL_POINTER;
    }

    //LOG("stack->count = %d\n", stack->count);

    printf("stack->count = %d\n", stack->count);
    printf("stack->capacity = %d\n", stack->count);
    printf("\n");

    printf("Elements of stack \tAddresses\n");
    for(int idx = 0; idx < stack->count; ++idx)
    {
        printf("stack->data = %d\n", *(stack->data + sizeof(canary_begin_array_) + idx * sizeof(int)));
        printf("stack->data[%d] = %d\t", idx,  *(stack->data + sizeof(canary_begin_array_) + idx * sizeof(int)));
        printf("%p\n", stack->data + sizeof(canary_begin_array_) + idx * sizeof(int));
    }
}

//===================================================================
