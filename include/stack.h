#pragma once
#include <stdint.h>

/*! enumeration colors and their corresponding ANSI values. */
#define BLACK "\033[0;30m"
#define BLUE "\033[0;34m"
#define GREEN "\033[0;32m"
#define CYAN "\033[0;36m"
#define RED "\033[0;31m"
#define PURPLE "\033[0;35m"
#define BROWN "\033[0;33m"
#define YELLOW "\e[0;33m"
#define RESET "\033[0m"

#define MULTIPLIER_1 2
#define MULTIPLIER_2 1.5
#define MULTIPLIER_3 1.2


#ifdef PTR
    #define POISON NULL
#else
    #define POISON -101
#endif


#define SAFETY 1

#ifdef REMOVE_SAFETY
#undef SAFETY
#endif

#ifdef PTR
    typedef void* elem_t;
#endif

#ifdef INT
    typedef int elem_t;
#endif

#ifdef DOUBLE
    typedef double elem_t;
#endif

#ifdef CHAR
    typedef char elem_t;
#endif


typedef struct stack_t
{
    #ifdef SAFETY
        unsigned long long* canary_1;
    #endif

    elem_t* data;

    #ifdef SAFETY
        unsigned long long* canary_2;
    #endif

    int count;
    int capacity;

    int error_name;

    #ifdef SAFETY
        unsigned long long* canary_3;
    #endif

} stack_t;


enum flags_multiplier
{
    MULTIPLIER_SMALL  = 1,
    MULTIPLIER_LARGE  = 2
};

enum stk_error_names
{
    ERR_STK_INC_ERRNAME      = 10,
    ERR_STK_BAD_POINTER      = 9,
    ERR_STK_INC_INPUT        = 8,
    ERR_STK_OUT_MEMORY       = 7,
    ERR_STK_STACK_UNDERFLOW  = 6,
    ERR_STK_STACK_OVERFLOW   = 5,
    ERR_STK_STACK_ATTACKED   = 4,
    ERR_STK_DATA_ATTACKED    = 3,
    ERR_STK_NULL_POINTER     = 2,
    ERR_STK_NEGATIVE_COUNT   = 1,
};


int stack_init(stack_t* stack, int init_size);
int stack_push(stack_t* stack, int value);
int stack_pop(stack_t* stack);
int stack_peek(stack_t* stack);
int stack_dump(stack_t* stack);
elem_t* stack_realloc_internal(stack_t* stack);
int stack_resize_decrease(stack_t* stack);
int stack_resize_increase(stack_t* stack);
int stack_destroy(stack_t* stack);
int stack_verify(stack_t* stack);
uint32_t hash_FAQ6(const void* key, size_t len);
void update_hash(stack_t* stack);



//! Macros STACK_ERROR
/*! macros to print the error. */
#define STACK_ERROR(ERROR_NAME)                                                     \
{                                                                                   \
    fprintf(stderr, RED "ERROR:" RESET "line %d. " #ERROR_NAME "\n", __LINE__);     \
};

//! Macros STACK_OK
/*! macros to describe the error. */
#define STACK_OK(stack)                                                             \
                                                                                    \
    ret_ = stack_verify(stack);                                                     \
                                                                                    \
    switch (ret_)                                                                   \
    {                                                                               \
    case (1 << ERR_STK_INC_INPUT):                                                  \
        STACK_ERROR(ERR_STK_INC_INPUT);                                             \
        break;                                                                      \
                                                                                    \
    case (1 << ERR_STK_OUT_MEMORY):                                                 \
        STACK_ERROR(ERR_STK_OUT_MEMORY);                                            \
        break;                                                                      \
                                                                                    \
    case (1 << ERR_STK_STACK_UNDERFLOW):                                            \
        STACK_ERROR(ERR_STK_STACK_UNDERFLOW);                                       \
        break;                                                                      \
                                                                                    \
    case (1 << ERR_STK_STACK_OVERFLOW):                                             \
        STACK_ERROR(ERR_STK_STACK_OVERFLOW);                                        \
        break;                                                                      \
                                                                                    \
    case (1 << ERR_STK_STACK_ATTACKED):                                             \
        STACK_ERROR(ERR_STK_STACK_ATTACKED);                                        \
        break;                                                                      \
                                                                                    \
    case (1 << ERR_STK_DATA_ATTACKED):                                              \
        STACK_ERROR(ERR_STK_DATA_ATTACKED);                                         \
        break;                                                                      \
                                                                                    \
    case (1 << ERR_STK_NULL_POINTER):                                               \
        STACK_ERROR(ERR_STK_NULL_POINTER);                                          \
        break;                                                                      \
                                                                                    \
    case (1 << ERR_STK_NEGATIVE_COUNT):                                             \
        STACK_ERROR(ERR_STK_NEGATIVE_COUNT);                                        \
        break;                                                                      \
                                                                                    \
    case (1 << ERR_STK_BAD_POINTER):                                                \
        STACK_ERROR(ERR_STK_BAD_POINTER);                                           \
        break;                                                                      \
                                                                                    \
    case (1 << ERR_STK_INC_ERRNAME):                                                \
        STACK_ERROR(ERR_STK_INC_ERRNAME);                                           \
        break;                                                                      \
    };                                                          
