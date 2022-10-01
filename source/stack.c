
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <logger.h>
#include <stdint.h>

#include "stack.h"

static unsigned long long* canary_1_ = (unsigned long long*) 0xDEADBEAF;
static unsigned long long* canary_2_ = (unsigned long long*) 0xBACBEDEF;
static unsigned long long* canary_3_ = (unsigned long long*) 0xAD666FED;
static unsigned long long* canary_begin_array_ = (unsigned long long*) 0xBAD0BEDA;

static uint32_t previous_hash_stack = 0;
static uint32_t previous_hash_data  = 0;
static uint32_t hash_stack = 0;
static uint32_t hash_data  = 0;

static int flag_multiplier_upper = MULTIPLIER_LARGE;
static int flag_multiplier_down  = MULTIPLIER_SMALL;

static double multiplier = 0.0;

static int ret_ = 0;

//===================================================================
int stack_init(stack_t* stack, int init_size)
{
    stack->error_name = 0;

    if(stack == NULL)
    {
        fprintf(stderr, RED "ERROR: " RESET "INVALID POINTER TO STACK.\n");
        exit(ERR_NULL_POINTER);
    }

    if(init_size <= 0)
    {
        fprintf(stderr, RED "ERROR: " RESET "INCORRECT SIZE OF STACK.\n");
        exit(ERR_INC_INPUT);
    }

    #ifdef SAFETY
        stack->capacity = init_size + sizeof(canary_begin_array_) / sizeof(elem_t);
    #else
        stack->capacity = init_size;
    #endif

    stack->count = 0;

    stack->data = (elem_t*) calloc(stack->capacity, sizeof(elem_t));
    if(stack->data == NULL)
    {
        stack->error_name |= 1 << ERR_OUT_MEMORY;

        #ifdef SAFETY
            update_hash(stack);
        #endif

        STACK_OK(stack);

        return ERR_OUT_MEMORY;
    }
    
    #ifdef SAFETY
        memcpy(stack->data, &canary_begin_array_, sizeof(canary_begin_array_));

        for(int idx = 0; idx < init_size; ++idx)
        {
            *((char*) stack->data + sizeof(canary_begin_array_) + idx * sizeof(elem_t)) = POISON;
        }
    #else
        for(int idx = 0; idx < init_size; ++idx)
        {
            *((char*) stack->data + idx * sizeof(elem_t)) = POISON;
        }
    #endif

    #ifdef SAFETY
        stack->canary_1 = canary_1_;
        stack->canary_2 = canary_2_;
        stack->canary_3 = canary_3_;
        unsigned long long* canary_begin_array = canary_begin_array_;

        update_hash(stack);
    #endif

    LOG("LINE %d: FUNCTION %s: stack->capacity = %d\n", __LINE__, __PRETTY_FUNCTION__, stack->capacity);

    STACK_OK(stack);

    return 0;
}

//===================================================================

int stack_destroy(stack_t* stack)
{
    STACK_OK(stack);

    free(stack->data);

    stack = NULL;
}

//===================================================================

void update_hash(stack_t* stack)
{
    hash_stack = hash_FAQ6(stack, sizeof(stack_t));
    hash_data  = hash_FAQ6(stack->data, sizeof(canary_begin_array_) + stack->count * sizeof(elem_t));
    previous_hash_stack = hash_stack;
    previous_hash_data  = hash_data;
}

//===================================================================

elem_t* stack_realloc_internal(stack_t* stack)
{
    STACK_OK(stack);

    #ifdef SAFETY
        int real_capacity = stack->capacity - sizeof(canary_begin_array_) / sizeof(elem_t);
        LOG("In LINE %d, FUNCTION %s: real_capacity = %d\n", __LINE__, __PRETTY_FUNCTION__, real_capacity);
    #else
        int real_capacity = stack->capacity;
        LOG("In LINE %d, FUNCTION %s: real_capacity = %d\n", __LINE__, __PRETTY_FUNCTION__, real_capacity);
    #endif

    real_capacity = real_capacity * multiplier;

    #ifdef SAFETY
        stack->capacity = real_capacity + sizeof(canary_begin_array_) / sizeof(elem_t);
        LOG("In LINE %d, FUNCTION %s: real_capacity = %d\n", __LINE__, __PRETTY_FUNCTION__, real_capacity);
        elem_t* check_ptr = (elem_t*) realloc(stack->data, sizeof(canary_begin_array_) + real_capacity * sizeof(elem_t));
    #else
        stack->capacity = real_capacity;
        LOG("In LINE %d, FUNCTION %s: real_capacity = %d\n", __LINE__, __PRETTY_FUNCTION__, real_capacity);
        elem_t* check_ptr = (elem_t*) realloc(stack->data, real_capacity * sizeof(elem_t));         
    #endif

    if(check_ptr != NULL)
    {
        stack->data = check_ptr;

        #ifdef SAFETY
            update_hash(stack);
        #endif

        return check_ptr;
    }

    #ifdef SAFETY
        update_hash(stack);
    #endif

    STACK_OK(stack);

    return NULL;
}

//===================================================================

int stack_resize_increase(stack_t* stack)
{
    STACK_OK(stack);
    
    if(flag_multiplier_down == MULTIPLIER_LARGE)
    {
        flag_multiplier_upper = MULTIPLIER_SMALL;
        multiplier = MULTIPLIER_2;

        elem_t* check_ptr = stack_realloc_internal(stack);
        if(check_ptr != NULL)
        {
            stack->data = check_ptr;

            #ifdef SAFETY
                update_hash(stack);
            #endif

            STACK_OK(stack);

            return 0;
        }

        multiplier = MULTIPLIER_3;
        
        check_ptr = stack_realloc_internal(stack);
        if(check_ptr != NULL)
        {
            stack->data = check_ptr;

            #ifdef SAFETY
                update_hash(stack);
            #endif

            STACK_OK(stack);

            return 0;
        }

        stack->error_name |= 1 << ERR_OUT_MEMORY;
            
        #ifdef SAFETY
            update_hash(stack);
        #endif

        STACK_OK(stack);

        return ERR_OUT_MEMORY;         
    }

    if(flag_multiplier_down == MULTIPLIER_SMALL)
    {
        flag_multiplier_upper = MULTIPLIER_LARGE;
        multiplier = MULTIPLIER_1;

        elem_t* check_ptr = stack_realloc_internal(stack);
        if(check_ptr != NULL)
        {
            stack->data = check_ptr;

            #ifdef SAFETY
                update_hash(stack);
            #endif

            STACK_OK(stack);

            return 0;            
        }

        multiplier = MULTIPLIER_2;

        check_ptr = stack_realloc_internal(stack);
        if(check_ptr != NULL)
        {
            stack->data = check_ptr;

            #ifdef SAFETY
                update_hash(stack);
            #endif

            STACK_OK(stack);

            return 0;  
        }

        multiplier = MULTIPLIER_3;

        check_ptr = stack_realloc_internal(stack);
        if(check_ptr != NULL)
        {
            stack->data = check_ptr;

            #ifdef SAFETY
                update_hash(stack);
            #endif

            STACK_OK(stack);

            return 0; 
        }

        stack->error_name |= 1 << ERR_OUT_MEMORY;

        #ifdef SAFETY
            update_hash(stack);
        #endif

        STACK_OK(stack);

        return ERR_OUT_MEMORY;
    }
}

//===================================================================

int stack_resize_decrease(stack_t* stack)
{
    STACK_OK(stack);

    #ifdef SAFETY
        int real_capacity = stack->capacity - sizeof(canary_begin_array_) / sizeof(elem_t);
        LOG("In LINE %d, FUNCTION %s: real_capacity = %d\n", __LINE__, __PRETTY_FUNCTION__, real_capacity);
    #else
        int real_capacity = stack->capacity;
        LOG("In LINE %d, FUNCTION %s: real_capacity = %d\n", __LINE__, __PRETTY_FUNCTION__, real_capacity);
    #endif

    if(flag_multiplier_upper == MULTIPLIER_LARGE)
    {
        flag_multiplier_down = MULTIPLIER_SMALL;

        real_capacity = real_capacity / MULTIPLIER_2;

        #ifdef SAFETY
            stack->capacity = real_capacity + sizeof(canary_begin_array_) / sizeof(elem_t);
            LOG("In LINE %d, FUNCTION %s: real_capacity = %d\n", __LINE__, __PRETTY_FUNCTION__, real_capacity);
        #else
            stack->capacity = real_capacity;
            LOG("In LINE %d, FUNCTION %s: real_capacity = %d\n", __LINE__, __PRETTY_FUNCTION__, real_capacity);            
        #endif

        elem_t* check_ptr = (elem_t*) realloc(stack->data, stack->capacity * sizeof(elem_t));
        if(check_ptr != NULL)
        {
            stack->data = check_ptr;

            #ifdef SAFETY
                update_hash(stack);
            #endif

            STACK_OK(stack);

            return 0; 
        }

        stack->error_name |= 1 << ERR_OUT_MEMORY;

        #ifdef SAFETY
            update_hash(stack);
        #endif  

        STACK_OK(stack);

        return ERR_OUT_MEMORY;  
    }

    if(flag_multiplier_upper == MULTIPLIER_SMALL)
    {
        flag_multiplier_down = MULTIPLIER_LARGE;

        real_capacity = real_capacity / MULTIPLIER_1;

        #ifdef SAFETY
            stack->capacity = real_capacity + sizeof(canary_begin_array_) / sizeof(elem_t);
            LOG("In LINE %d, FUNCTION %s: real_capacity = %d\n", __LINE__, __PRETTY_FUNCTION__, real_capacity);
        #else
            stack->capacity = real_capacity;
            LOG("In LINE %d, FUNCTION %s: real_capacity = %d\n", __LINE__, __PRETTY_FUNCTION__, real_capacity);            
        #endif

        elem_t* check_ptr = (elem_t*) realloc(stack->data, stack->capacity * sizeof(elem_t));
        if(check_ptr != NULL)
        {
            stack->data = check_ptr;

            #ifdef SAFETY
                update_hash(stack);
            #endif

            STACK_OK(stack);

            return 0; 
        }

        stack->error_name |= 1 << ERR_OUT_MEMORY;

        #ifdef SAFETY
            update_hash(stack);
        #endif  

        STACK_OK(stack);

        return ERR_OUT_MEMORY; 
    }

    return 0; 
}

//===================================================================

int stack_pop(stack_t* stack)
{
    STACK_OK(stack);

    LOG("In LINE %d, FUNCTION %s: stack->count = %d\n",  __LINE__, __PRETTY_FUNCTION__, stack->count);

    if(stack->count == 0)
    {
        stack->error_name |= 1 << ERR_STACK_UNDERFLOW;

        #ifdef SAFETY
            update_hash(stack);
        #endif

        return ERR_STACK_UNDERFLOW;
    }

    --(stack->count);
    LOG("In LINE %d, FUNCTION %s: stack->count = %d\n", __LINE__, __PRETTY_FUNCTION__, stack->count);

    #ifdef SAFETY
        update_hash(stack);
    #endif

    #ifdef SAFETY
        int real_capacity = stack->capacity - sizeof(canary_begin_array_) / sizeof(elem_t);
        LOG("In LINE %d, FUNCTION %s: real_capacity = %d\n", __LINE__, __PRETTY_FUNCTION__, real_capacity);
    #else
        int real_capacity = stack->capacity;
        LOG("In LINE %d, FUNCTION %s: real_capacity = %d\n", __LINE__, __PRETTY_FUNCTION__, real_capacity);
    #endif

    if(stack->count <= real_capacity / 2)
    {
        stack_resize_decrease(stack);
    }

    #ifdef SAFETY
        update_hash(stack);
    #endif

    #ifdef SAFETY
        STACK_OK(stack);
        return *((char*) stack->data + sizeof(canary_begin_array_) + stack->count * sizeof(elem_t));
    #else
        STACK_OK(stack);
        return *((char*) stack->data + stack->count * sizeof(elem_t));
    #endif
}

//===================================================================

int stack_peek(stack_t* stack)
{
    STACK_OK(stack);

    LOG("In LINE %d, FUNCTION %s: stack->count = %d\n", __LINE__, __PRETTY_FUNCTION__, stack->count);

    if(stack->count == 0)
    {
        stack->error_name |= 1 << ERR_NULL_POINTER;

        #ifdef SAFETY
            update_hash(stack);
        #endif

        STACK_OK(stack);

        return ERR_NULL_POINTER;
    }

    #ifdef SAFETY
        update_hash(stack);
    #endif

    #ifdef SAFETY
        STACK_OK(stack);
        return *((char*) stack->data + sizeof(canary_begin_array_) + (stack->count - 1) * sizeof(elem_t));
    #else
        STACK_OK(stack);
        return *((char*) stack->data + (stack->count - 1) * sizeof(elem_t));
    #endif
}

//===================================================================

int stack_push(stack_t* stack, elem_t value)
{
    STACK_OK(stack);

    #ifdef SAFETY
        int real_capacity = stack->capacity - sizeof(canary_begin_array_) / sizeof(elem_t);
        LOG("In LINE %d, FUNCTION %s: real_capacity = %d\n", __LINE__, __PRETTY_FUNCTION__, real_capacity);
    #else
        int real_capacity = stack->capacity;
        LOG("In LINE %d, FUNCTION %s: real_capacity = %d\n", __LINE__, __PRETTY_FUNCTION__, real_capacity);
    #endif

    if(stack->count >= real_capacity)
    {
        stack_resize_increase(stack);
    }

    elem_t* skip_canary_data = stack->data;
    #ifdef SAFETY
        skip_canary_data = (elem_t*) ((char*) stack->data + sizeof(canary_begin_array_));
    #endif

    #ifdef DOUBLE
        *((char*) skip_canary_data + stack->count * sizeof(elem_t)) = value;
        LOG("In LINE %d, FUNCTION %s: value = %lf\n", __LINE__, __PRETTY_FUNCTION__, value);
    #elif CHAR
        *((char*) skip_canary_data + stack->count * sizeof(elem_t)) = value;
        LOG("In LINE %d, FUNCTION %s: value = %c\n", __LINE__, __PRETTY_FUNCTION__, value);
    #else
        *((char*) skip_canary_data + stack->count * sizeof(elem_t)) = value;
        LOG("In LINE %d, FUNCTION %s: value = %d\n", __LINE__, __PRETTY_FUNCTION__, value);
    #endif

    ++(stack->count);

    LOG("In LINE %d, FUNCTION %s: stack->count = %d\n", __LINE__, __PRETTY_FUNCTION__, stack->count);
    LOG("In LINE %d, FUNCTION %s: real_capacity = %d\n", __LINE__, __PRETTY_FUNCTION__, real_capacity);

    #ifdef SAFETY
        update_hash(stack);
    #endif

    STACK_OK(stack);
}

//===================================================================

int stack_dump(stack_t* stack)
{
    STACK_OK(stack);

    #ifdef SAFETY
        int real_capacity = stack->capacity - sizeof(canary_begin_array_) / sizeof(elem_t);
        LOG("In LINE %d, FUNCTION %s: real_capacity = %d\n", __LINE__, __PRETTY_FUNCTION__, real_capacity);
    #else
        int real_capacity = stack->capacity;
        LOG("In LINE %d, FUNCTION %s: real_capacity = %d\n", __LINE__, __PRETTY_FUNCTION__, real_capacity);
    #endif

    printf("-----------------------------------------------------------\n");

    if(stack->error_name != 0)
    {
        int error_tmp = 0;
        int error_number = 10;
        for(int error_idx = 1; error_idx < error_number; ++error_idx)
        {
            error_tmp = stack->error_name;
            error_tmp &= 1 << error_idx;
    
            switch (error_tmp)
            {
            case (1 << ERR_INC_INPUT):
                STACK_ERROR(ERR_INC_INPUT);
                break;

            case (1 << ERR_OUT_MEMORY):
                STACK_ERROR(ERR_OUT_MEMORY);
                break;

            case (1 << ERR_STACK_UNDERFLOW):
                STACK_ERROR(ERR_STACK_UNDERFLOW);
                break;

            case (1 << ERR_STACK_OVERFLOW):
                STACK_ERROR(ERR_STACK_OVERFLOW);
                break;

            case (1 << ERR_STACK_ATTACKED):
                STACK_ERROR(ERR_STACK_ATTACKED);
                break;

            case (1 << ERR_DATA_ATTACKED):
                STACK_ERROR(ERR_DATA_ATTACKED);
                break;

            case (1 << ERR_NULL_POINTER):
                STACK_ERROR(ERR_NULL_POINTER);
                break;

            case (1 << ERR_NEGATIVE_COUNT):
                STACK_ERROR(ERR_NEGATIVE_COUNT);
                break;

            case (1 << ERR_BAD_POINTER):
                STACK_ERROR(ERR_BAD_POINTER);
                break;
                
            case (1 << ERR_INC_ERRNAME):
                STACK_ERROR(ERR_INC_ERRNAME);
                break;
            };
        }
    }
    printf("\n");

    #ifdef SAFETY
        printf("\t\t       Canaries and hash \n");
        printf("\tValues \t\t\t\t    Addresses\n");
        printf("canary_1_ = %p \t\t\t %p\n", canary_1_, &canary_1_);
        printf("canary_2_ = %p \t\t\t %p\n", canary_2_, &canary_2_);
        printf("canary_3_ = %p \t\t\t %p\n", canary_3_, &canary_3_);
        printf("canary_begin_array_ = %p \t %p\n", canary_begin_array_, &canary_begin_array_);
        printf("hash_stack = %d\t\t\t %p\n", hash_stack, &hash_stack);
        printf("hash_data  = %d\t\t\t %p\n", hash_data, &hash_data);
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

    elem_t* skip_canary_data = stack->data;
    #ifdef SAFETY
        skip_canary_data = (elem_t*) ((char*) stack->data + sizeof(canary_begin_array_));
    #endif

    for(int idx = 0; idx < stack->count; ++idx)
    {
        #ifdef DOUBLE
            printf("stack->data[%d] = %lf\t\t\t", idx,  *((char*) skip_canary_data + idx * sizeof(elem_t)));
        #elif CHAR
            printf("stack->data[%d] = %c\t\t\t", idx,  *((char*) skip_canary_data + idx * sizeof(elem_t)));
        #else
            printf("stack->data[%d] = %d\t\t\t", idx,  *((char*) skip_canary_data + idx * sizeof(elem_t)));
        #endif

        printf("%p\n", (char*) stack->data + idx * sizeof(elem_t));
    }

    printf("\n");

    printf("-----------------------------------------------------------\n");

    #ifdef SAFETY
        update_hash(stack);
    #endif

    STACK_OK(stack);
}

//===================================================================

int stack_verify(stack_t* stack)
{
    #ifdef SAFETY
        if((stack == NULL) || (&stack->count == NULL) || (&stack->capacity == NULL) || 
           (&stack->canary_1 == NULL) || (&stack->canary_2 == NULL) || (&stack->canary_3 == NULL) || 
           (&stack->error_name == NULL))
        {
            stack->error_name |= 1 << ERR_NULL_POINTER;
        }
    #else
        if((stack == NULL) || (&stack->count == NULL) || (&stack->capacity == NULL) || 
           (&stack->error_name == NULL))
        {
            stack->error_name |= 1 << ERR_NULL_POINTER;
        }
    #endif

    #ifdef SAFETY
        if((stack->canary_1 != canary_1_) || (stack->canary_2 != canary_2_) ||            
           (stack->canary_3 != canary_3_) ||                                            
           (memcmp(stack->data, &canary_begin_array_, sizeof(canary_begin_array_)) != 0))
        {
            stack->error_name |= 1 << ERR_STACK_ATTACKED;
        }
    #endif

    #ifdef SAFETY
        int real_capacity = stack->capacity - sizeof(int*) / sizeof(int);
        LOG("In LINE %d, FUNCTION %s: real_capacity = %d\n", __LINE__, __PRETTY_FUNCTION__, real_capacity);
        LOG("In LINE %d, FUNCTION %s: stack->count = %d\n", __LINE__, __PRETTY_FUNCTION__, stack->count);
    #else
        int real_capacity = stack->capacity;
        LOG("In LINE %d, FUNCTION %s: real_capacity = %d\n", __LINE__, __PRETTY_FUNCTION__, real_capacity);
    #endif

    if(stack->count > real_capacity)
    {
        stack->error_name |= 1 << ERR_STACK_OVERFLOW;
    }

    if((stack->count < 0) || (stack->capacity < 0))
    {
        stack->error_name |= 1 << ERR_NEGATIVE_COUNT;
    }

    #ifdef SAFETY
        for(int idx = 0; idx < stack->count; ++idx)
        {
            if(*((char*) stack->data + sizeof(canary_begin_array_) + idx * sizeof(elem_t)) == POISON)
            {
                stack->error_name |= 1 << ERR_INC_INPUT;
            }
        }
    #else
        for(int idx = 0; idx < stack->count; ++idx)
        {
            if(*((char*) stack->data + idx * sizeof(elem_t)) == POISON)
            {
                stack->error_name |= 1 << ERR_INC_INPUT;
            }
        }
    #endif

    #ifdef SAFETY
        hash_stack = hash_FAQ6(stack, sizeof(stack_t));
        hash_data  = hash_FAQ6(stack->data, sizeof(canary_begin_array_) + stack->count * sizeof(elem_t));

        if(hash_stack != previous_hash_stack)
        {
            stack->error_name |= 1 << ERR_STACK_ATTACKED;
        }
        if(hash_data != previous_hash_data)
        {
            stack->error_name |= 1 << ERR_DATA_ATTACKED;
        }        
    #endif

    #ifdef SAFETY
    // we don't compare stack with stack->canary_1 because stack points to stack->canary_1
        if((stack == (stack_t*) stack->data) || (stack == (stack_t*) &stack->count) || (stack == (stack_t*) &stack->capacity) ||         \
           (stack == (stack_t*) &stack->canary_2) || (stack == (stack_t*) &stack->canary_3) || (stack == (stack_t*) &stack->error_name))
        {
            stack->error_name |= 1 << ERR_BAD_POINTER;
        }
    #else
        if((stack == (stack_t*) stack->data) || (stack == (stack_t*) &stack->count) || (stack == (stack_t*) &stack->capacity) || \
           (stack == (stack_t*) &stack->error_name))
        {
            stack->error_name |= 1 << ERR_BAD_POINTER;
        }
    #endif

    if((stack->error_name > 0) || (stack->error_name < -10))
    {
        stack->error_name |= 1 << ERR_INC_ERRNAME;
    }

    return 0;
}

//===================================================================

uint32_t hash_FAQ6(const void* key, size_t len)
{
    uint32_t i = 0;
    uint32_t hash = 0;

    for(i = 0; i < len; ++i)
    {
        hash += (uint32_t) *((const char*) key + i);
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}
