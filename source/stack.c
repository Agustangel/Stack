
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <logger.h>
#include <stdint.h>

#include "stack.h"

static int* canary_1_ = (int*) 0xDEADBEAF;
static int* canary_2_ = (int*) 0xBACBEDEF;
static int* canary_3_ = (int*) 0xAD666FED;
static int* canary_begin_array_ = (int*) 0xBAD0BEDA;

static int flag_multiplier_upper = TWO;
static int flag_multiplier_down  = ONEHALF;

static uint32_t hash = 0;
static uint32_t previous_hash = 0;

//===================================================================
int stack_init(stack_t* stack, int init_size)
{
    if(stack == NULL)
    {
        return ERR_NULL_POINTER;
    }

    if(init_size <= 0)
    {
        fprintf(stderr, RED "ERROR: " RESET "INCORRECT SIZE OF STACK.\n");
        exit(ERR_INC_INPUT);
    }

    #ifdef SAFETY
        stack->canary_1 = (int*) 0xDEADBEAF;
        stack->canary_2 = (int*) 0xBACBEDEF;
        stack->canary_3 = (int*) 0xAD666FED;
    
        int* canary_begin_array = (int*) 0xBAD0BEDA;
    #endif

    #ifdef SAFETY
        stack->capacity = init_size + sizeof(canary_begin_array) / sizeof(int);
    #else
        stack->capacity = init_size;
    #endif

    stack->count = 0;

    stack->data = (int*) calloc(stack->capacity, sizeof(int));

    if(stack->data == NULL)
    {
        return ERR_OUT_MEMORY;
    }
    
    #ifdef SAFETY
        memcpy(stack->data, &canary_begin_array, sizeof(canary_begin_array));

        for(int idx = 0; idx < init_size; ++idx)
        {
            *((char*) stack->data + sizeof(canary_begin_array_) + idx * sizeof(int)) = POISON;
        }
    #else
        for(int idx = 0; idx < init_size; ++idx)
        {
            *((char*) stack->data + idx * sizeof(int)) = POISON;
        }
    #endif

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

    #ifdef SAFETY
        int real_capacity = stack->capacity - sizeof(int*) / sizeof(int);
        LOG("LINE %d: real_capacity = %d\n", __LINE__, real_capacity);
    #else
        int real_capacity = stack->capacity;
        LOG("LINE %d: real_capacity = %d\n", __LINE__, real_capacity);
    #endif
    
    if(flag_multiplier_down == TWO)
    {
        flag_multiplier_upper = ONEHALF;

        real_capacity = real_capacity * MULTIPLIER_2;

        #ifdef SAFETY
            stack->capacity = real_capacity + (sizeof(int*) / sizeof(int));
            LOG("LINE %d: real_capacity = %d\n", __LINE__, real_capacity);
        #else
            stack->capacity = real_capacity;
            LOG("LINE %d: real_capacity = %d\n", __LINE__, real_capacity);            
        #endif

        int* check_ptr = (int*) realloc(stack->data, (stack->capacity) * sizeof(int));

        if(check_ptr != NULL)
        {
            stack->data = check_ptr;
        }
        else
        {
            return ERR_OUT_MEMORY;
        }
    }

    if(flag_multiplier_down == ONEHALF)
    {
        flag_multiplier_upper = TWO;

        real_capacity = real_capacity * MULTIPLIER_1;

        #ifdef SAFETY
            stack->capacity = real_capacity + (sizeof(int*) / sizeof(int));
            LOG("LINE %d: real_capacity = %d\n", __LINE__, real_capacity);
        #else
            stack->capacity = real_capacity;
            LOG("LINE %d: real_capacity = %d\n", __LINE__, real_capacity);            
        #endif

        int* check_ptr = (int*) realloc(stack->data, (stack->capacity) * sizeof(int));

        if(check_ptr != NULL)
        {
            stack->data = check_ptr;
        }
        else
        {
            return ERR_OUT_MEMORY;
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

    #ifdef SAFETY
        int real_capacity = stack->capacity - sizeof(int*) / sizeof(int);
        LOG("LINE %d: real_capacity = %d\n", __LINE__, real_capacity);
    #else
        int real_capacity = stack->capacity;
        LOG("LINE %d: real_capacity = %d\n", __LINE__, real_capacity);
    #endif

    if(flag_multiplier_upper == TWO)
    {
        flag_multiplier_down = ONEHALF;

        real_capacity = real_capacity / MULTIPLIER_2;

        #ifdef SAFETY
            stack->capacity = real_capacity + (sizeof(int*) / sizeof(int));
            LOG("LINE %d: real_capacity = %d\n", __LINE__, real_capacity);
        #else
            stack->capacity = real_capacity;
            LOG("LINE %d: real_capacity = %d\n", __LINE__, real_capacity);            
        #endif

        int* check_ptr = (int*) realloc(stack->data, stack->capacity * sizeof(int));
        
        if(check_ptr != NULL)
        {
            stack->data = check_ptr;
        }
        else
        {
            return ERR_OUT_MEMORY;
        }
    }

    if(flag_multiplier_upper == ONEHALF)
    {
        flag_multiplier_down = TWO;

        real_capacity = real_capacity / MULTIPLIER_1;

        #ifdef SAFETY
            stack->capacity = real_capacity + (sizeof(int*) / sizeof(int));
            LOG("LINE %d: real_capacity = %d\n", __LINE__, real_capacity);
        #else
            stack->capacity = real_capacity;
            LOG("LINE %d: real_capacity = %d\n", __LINE__, real_capacity);            
        #endif

        int* check_ptr = (int*) realloc(stack->data, stack->capacity * sizeof(int));
        
        if(check_ptr != NULL)
        {
            stack->data = check_ptr;
        }
        else
        {
            return ERR_OUT_MEMORY;
        }
    }
}

//===================================================================

int stack_pop(stack_t* stack)
{
    if(stack == NULL)
    {
        return ERR_NULL_POINTER;
    }

    LOG("stack->count = %d\n", stack->count);

    if(stack->count == 0)
    {
        exit(ERR_STACK_UNDERFLOW);
    }
    --(stack->count);
    LOG("stack->count = %d\n", stack->count);

    #ifdef SAFETY
        int real_capacity = stack->capacity - sizeof(int*) / sizeof(int);
        LOG("LINE %d: real_capacity = %d\n", __LINE__, real_capacity);
    #else
        int real_capacity = stack->capacity;
        LOG("LINE %d: real_capacity = %d\n", __LINE__, real_capacity);
    #endif

    if(stack->count <= real_capacity / 2)
    {
        stack_resize_decrease(stack);
    }

    #ifdef SAFETY
        stack_hash((char*) stack->data + sizeof(canary_begin_array_), stack->count);
    #endif

    previous_hash = hash;

    return *((char*) stack->data + sizeof(canary_begin_array_) + stack->count * sizeof(int));
}

//===================================================================

int stack_peek(const stack_t* stack)
{
    if(stack == NULL)
    {
        return ERR_NULL_POINTER;
    }

    LOG("stack->count = %d\n", stack->count);

    if(stack->count == 0)
    {
        return ERR_STACK_UNDERFLOW;
    }

    #ifdef SAFETY
        stack_hash((char*) stack->data + sizeof(canary_begin_array_), stack->count);
    #endif

    previous_hash = hash;

    return *((char*) stack->data + sizeof(canary_begin_array_) + (stack->count - 1) * sizeof(int));
}

//===================================================================

int stack_push(stack_t* stack, int value)
{
    if(stack == NULL)
    {
        return ERR_NULL_POINTER;
    }

    LOG("stack->count = %d\n", stack->count);
    LOG("stack->capacity = %d\n", stack->capacity);

    #ifdef SAFETY
        int real_capacity = stack->capacity - sizeof(int*) / sizeof(int);
        LOG("LINE %d: real_capacity = %d\n", __LINE__, real_capacity);
    #else
        int real_capacity = stack->capacity;
        LOG("LINE %d: real_capacity = %d\n", __LINE__, real_capacity);
    #endif

    if(stack->count >= real_capacity)
    {
        stack_resize_increase(stack);
    }

    *((char*) stack->data + sizeof(canary_begin_array_) + stack->count * sizeof(int)) = value;

    LOG("value = %d\n", value);

    ++(stack->count);

    LOG("stack->count = %d\n", stack->count);
    LOG("stack->capacity = %d\n", stack->capacity);

    #ifdef SAFETY
        stack_hash((char*) stack->data + sizeof(canary_begin_array_), stack->count);
    #endif

    previous_hash = hash;
}

//===================================================================

int stack_dump(stack_t* stack)
{
    if(stack == NULL)
    {
        return ERR_NULL_POINTER;
    }
    
    #ifdef SAFETY
        int real_capacity = stack->capacity - sizeof(int*) / sizeof(int);
        LOG("LINE %d: real_capacity = %d\n", __LINE__, real_capacity);
    #else
        int real_capacity = stack->capacity;
        LOG("LINE %d: real_capacity = %d\n", __LINE__, real_capacity);
    #endif

    printf("-----------------------------------------------------------\n");

    #ifdef SAFETY
        printf("\t\t       Canaries and hash \n");
        printf("\tValues \t\t\t\t    Addresses\n");
        printf("canary_1_ = %p \t\t\t %p\n", canary_1_, &canary_1_);
        printf("canary_2_ = %p \t\t\t %p\n", canary_2_, &canary_2_);
        printf("canary_3_ = %p \t\t\t %p\n", canary_3_, &canary_3_);
        printf("canary_begin_array_ = %p \t %p\n", canary_begin_array_, &canary_begin_array_);
        printf("  hash    = %d \t\t\t %p\n", hash, &hash);
        printf("\n");

        printf("-----------------------------------------------------------\n");

    #endif

    printf("\n");
    printf("stack->count = %d\n", stack->count);
    printf("real_capacity = %d\n", real_capacity);
    printf("\n");

    printf("-----------------------------------------------------------\n");

    printf("\n");
    printf("Elements of stack \t\t\t   Addresses\n");

    #ifdef SAFETY
        for(int idx = 0; idx < stack->count; ++idx)
        {
            printf("stack->data[%d] = %d\t\t\t", idx,  *((char*) stack->data + sizeof(canary_begin_array_) + idx * sizeof(int)));
            printf("%p\n", (char*) stack->data + sizeof(canary_begin_array_) + idx * sizeof(int));
        }
    #else
        for(int idx = 0; idx < stack->count; ++idx)
        {
            printf("stack->data[%d] = %d\t\t\t", idx,  *((char*) stack->data + idx * sizeof(int)));
            printf("%p\n", (char*) stack->data + idx * sizeof(int));
        }
    #endif

    printf("\n");

    printf("-----------------------------------------------------------\n");
}

//===================================================================

int stack_verify(stack_t* stack)
{
    if(stack == NULL)
    {
        return ERR_NULL_POINTER;
    }

    #ifdef SAFETY
        if((stack->canary_1 != canary_1_) || (stack->canary_2 != canary_2_) ||         \
        (stack->canary_3 != canary_3_) ||                                              \
        (memcmp(stack->data, &canary_begin_array_, sizeof(canary_begin_array_)) != 0))
        {
            return ERR_STACK_ATTACKED;
        }
    #endif

    #ifdef SAFETY
        int real_capacity = stack->capacity - sizeof(int*) / sizeof(int);
        LOG("LINE %d: real_capacity = %d\n", __LINE__, real_capacity);
    #else
        int real_capacity = stack->capacity;
        LOG("LINE %d: real_capacity = %d\n", __LINE__, real_capacity);
    #endif

    if(stack->count > real_capacity)
    {
        return ERR_STACK_OVERFLOW;
    }

    if((stack->count < 0) || (stack->capacity < 0))
    {
        return ERR_NEGATIVE_COUNT;
    }

    #ifdef SAFETY
        for(int idx = 0; idx < stack->count; ++idx)
        {
            if(*((char*) stack->data + sizeof(canary_begin_array_) + idx * sizeof(int)) == POISON)
            {
                return ERR_INC_INPUT;
            }
        }
    #else
        for(int idx = 0; idx < stack->count; ++idx)
        {
            if(*((char*) stack->data + idx * sizeof(int)) == POISON)
            {
                return ERR_INC_INPUT;
            }
        }
    #endif

    #ifdef SAFETY
        if(hash != previous_hash)
        {
            return ERR_DATA_ATTACKED;
        }
    #endif
    
    return 0;
}

//===================================================================

void stack_hash(char *key, size_t len)
{
    uint32_t i = 0;

    for(i = 0; i < len; ++i)
    {
        hash += *(key + i * sizeof(int));
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

}
