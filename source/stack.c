
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <logger.h>
#include <stdint.h>

#include "stack.h"

static unsigned long long* canary_1_ = (unsigned long long*) 0xDEADBEAF;
static unsigned long long* canary_2_ = (unsigned long long*) 0xBACBEDEF;
static unsigned long long* canary_3_ = (unsigned long long*) 0xAD666FED;
static unsigned long long* canary_begin_array_ = (unsigned long long*) 0xBAD0BEDA;

static int flag_multiplier_upper = TWO;
static int flag_multiplier_down  = ONEHALF;

static uint32_t hash = 0;
static uint32_t previous_hash = 0;

//===================================================================
int stack_init(stack_t* stack, int init_size)
{
    stack->error_name = 0;

    if(stack == NULL)
    {
        stack->error_name = ERR_NULL_POINTER;

        return ERR_NULL_POINTER;
    }

    if(init_size <= 0)
    {
        fprintf(stderr, RED "ERROR: " RESET "INCORRECT SIZE OF STACK.\n");
        exit(ERR_INC_INPUT);
    }

    #ifdef SAFETY
        stack->canary_1 = (unsigned long long*) 0xDEADBEAF;
        stack->canary_2 = (unsigned long long*) 0xBACBEDEF;
        stack->canary_3 = (unsigned long long*) 0xAD666FED;
    
        unsigned long long* canary_begin_array = (unsigned long long*) 0xBAD0BEDA;
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
        stack->error_name = ERR_OUT_MEMORY;

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
        stack->error_name = ERR_NULL_POINTER;

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
        stack->error_name = ERR_NULL_POINTER;
        
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
            stack->error_name = ERR_OUT_MEMORY;

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
            stack->error_name = ERR_OUT_MEMORY;

            return ERR_OUT_MEMORY;
        }
    }
}

//===================================================================

int stack_resize_decrease(stack_t* stack)
{
    if(stack == NULL)
    {
        stack->error_name = ERR_NULL_POINTER;

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
            stack->error_name = ERR_OUT_MEMORY;

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
            stack->error_name = ERR_OUT_MEMORY;

            return ERR_OUT_MEMORY;
        }
    }
}

//===================================================================

int stack_pop(stack_t* stack)
{
    if(stack == NULL)
    {
        stack->error_name = ERR_NULL_POINTER;

        return ERR_NULL_POINTER;
    }

    LOG("stack->count = %d\n", stack->count);

    if(stack->count == 0)
    {
        stack->error_name = ERR_STACK_UNDERFLOW;

        return ERR_STACK_UNDERFLOW;
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
        previous_hash = hash;
    #endif

    #ifdef SAFETY
        return *((char*) stack->data + sizeof(canary_begin_array_) + stack->count * sizeof(int));
    #else
        return *((char*) stack->data + stack->count * sizeof(int));
    #endif
}

//===================================================================

int stack_peek(stack_t* stack)
{
    if(stack == NULL)
    {
        stack->error_name = ERR_NULL_POINTER;

        return ERR_NULL_POINTER;
    }

    LOG("stack->count = %d\n", stack->count);

    if(stack->count == 0)
    {
        stack->error_name = ERR_NULL_POINTER;

        return ERR_STACK_UNDERFLOW;
    }

    #ifdef SAFETY
        stack_hash((char*) stack->data + sizeof(canary_begin_array_), stack->count);
        previous_hash = hash;
    #endif

    #ifdef SAFETY
        return *((char*) stack->data + sizeof(canary_begin_array_) + (stack->count - 1) * sizeof(int));
    #else
        return *((char*) stack->data + (stack->count - 1) * sizeof(int));
    #endif
}

//===================================================================

int stack_push(stack_t* stack, int value)
{
    if(stack == NULL)
    {
        stack->error_name = ERR_NULL_POINTER;

        return ERR_NULL_POINTER;
    }

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

    #ifdef SAFETY
        *((char*) stack->data + sizeof(canary_begin_array_) + stack->count * sizeof(int)) = value;
        LOG("value = %d\n", value);
    #else
        *((char*) stack->data + stack->count * sizeof(int)) = value;
        LOG("value = %d\n", value);
    #endif

    ++(stack->count);

    LOG("stack->count = %d\n", stack->count);
    LOG("stack->capacity = %d\n", stack->capacity);

    #ifdef SAFETY
        stack_hash((char*) stack->data + sizeof(canary_begin_array_), stack->count);
        previous_hash = hash;
    #endif
}

//===================================================================

int stack_dump(stack_t* stack)
{
    if(stack == NULL)
    {
        stack->error_name = ERR_NULL_POINTER;

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

    if(stack->error_name != 0)
    {
        switch (stack->error_name)
        {
            STACK_ERROR(ERR_INC_INPUT);
            STACK_ERROR(ERR_OUT_MEMORY)
            STACK_ERROR(ERR_STACK_UNDERFLOW);
            STACK_ERROR(ERR_STACK_OVERFLOW);
            STACK_ERROR(ERR_STACK_ATTACKED);
            STACK_ERROR(ERR_DATA_ATTACKED);
            STACK_ERROR(ERR_NULL_POINTER);
            STACK_ERROR(ERR_NEGATIVE_COUNT);
            STACK_ERROR(ERR_BAD_POINTER);
            STACK_ERROR(ERR_INC_ERRNAME);
        };
    }
    printf("\n");

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
    #ifdef SAFETY
        if((stack == NULL) || (&stack->count == NULL) || (&stack->capacity == NULL)            || \
        (&stack->canary_1 == NULL) || (&stack->canary_2 == NULL) || (&stack->canary_3 == NULL) || \
        (&stack->error_name == NULL))
        {
            stack->error_name = ERR_NULL_POINTER;

            return ERR_NULL_POINTER;
        }
    #else
        if((stack == NULL) || (&stack->count == NULL) || (&stack->capacity == NULL) || \
           (&stack->error_name == NULL))
        {
            stack->error_name = ERR_NULL_POINTER;

            return ERR_NULL_POINTER;
        }
    #endif

    #ifdef SAFETY
        if((stack->canary_1 != canary_1_) || (stack->canary_2 != canary_2_) ||         \
        (stack->canary_3 != canary_3_) ||                                              \
        (memcmp(stack->data, &canary_begin_array_, sizeof(canary_begin_array_)) != 0))
        {
            stack->error_name = ERR_STACK_ATTACKED;

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
        stack->error_name = ERR_STACK_OVERFLOW;

        return ERR_STACK_OVERFLOW;
    }

    if((stack->count < 0) || (stack->capacity < 0))
    {
        stack->error_name = ERR_NEGATIVE_COUNT;

        return ERR_NEGATIVE_COUNT;
    }

    #ifdef SAFETY
        for(int idx = 0; idx < stack->count; ++idx)
        {
            if(*((char*) stack->data + sizeof(canary_begin_array_) + idx * sizeof(int)) == POISON)
            {
                stack->error_name = ERR_INC_INPUT;

                return ERR_INC_INPUT;
            }
        }
    #else
        for(int idx = 0; idx < stack->count; ++idx)
        {
            if(*((char*) stack->data + idx * sizeof(int)) == POISON)
            {
                stack->error_name = ERR_INC_INPUT;

                return ERR_INC_INPUT;
            }
        }
    #endif

    #ifdef SAFETY
        if(hash != previous_hash)
        {
            stack->error_name = ERR_DATA_ATTACKED;

            return ERR_DATA_ATTACKED;
        }
    #endif

    #ifdef SAFETY
        if((stack == stack->data) || (stack == &stack->count) || (stack == &stack->capacity)      || \
        (stack == &stack->canary_1) || (stack == &stack->canary_2) || (stack == &stack->canary_3) || \
        (stack == &stack->error_name))
        {
            stack->error_name = ERR_BAD_POINTER;

            return ERR_BAD_POINTER;
        }
    #else
        if((stack == stack->data) || (stack == &stack->count) || (stack == &stack->capacity) || \
        (stack == &stack->error_name))
        {
            stack->error_name = ERR_BAD_POINTER;

            return ERR_BAD_POINTER;
        }
    #endif

    if((stack->error_name > 0) || (stack->error_name < -10))
    {
        stack->error_name = ERR_INC_ERRNAME;

        return ERR_INC_ERRNAME;
    }

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
