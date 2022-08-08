
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
/*
    1) Memory realloc (increase and decrease) 
    2) sturct stack_t, stack_push, stack_pop, stack_print, stack_init, stack_destroy
    3) stack_verify (data initialized, no poision, correct fields)
    4) canaries
    5) hash function
    6) conditional compilation
    7) stack_dump
    8) error hanling */

// 2 4 . . .  .  . .

#define MULTIPLIER_1 2
#define MULTIPLIER_2 1.5

#define POISON 0

#define SAFETY 1

#ifdef REMOVE_SAFETY
#undef SAFETY
#endif


typedef struct stack_t
{

    int* canary_1;

    int* data;

    int* canary_2;

    int count;
    int capacity;

    int* canary_3;

} stack_t;


enum flags_multiplier
{
    TWO     = 0,
    ONEHALF = 1
};

enum error_names
{
    ERR_INC_INPUT        = -8,
    ERR_OUT_MEMORY       = -7,
    ERR_STACK_UNDERFLOW  = -6,
    ERR_STACK_OVERFLOW   = -5,
    ERR_STACK_ATTACKED   = -4,
    ERR_NULL_POINTER     = -3,
    ERR_NEGATIVE_COUNT   = -2,
    ERR_DATA_ATTACKED    = -1
};


int stack_init(stack_t* stack, int init_size);
int stack_push(stack_t* stack, int value);
int stack_pop(stack_t* stack);
int stack_peek(const stack_t* stack);
int stack_dump(stack_t* stack);
int stack_resize_decrease(stack_t* stack);
int stack_resize_increase(stack_t* stack);
int stack_destroy(stack_t* stack);
int stack_verify(stack_t* stack);
void stack_hash(int *key, size_t len);

//! Macros STACK_OK
/*! macros to describe the error. */
#define STACK_OK(stack)                                     \
                                                            \
    ret = stack_verify(stack);                              \
                                                            \
    switch (ret)                                            \
    {                                                       \
    case ERR_NULL_POINTER:                                  \
        fprintf(stderr, RED "ERROR: " RESET "line %d. "     \
                "INVALID STRUCTURE POINTER\n", __LINE__);   \
        break;                                              \
                                                            \
    case ERR_STACK_ATTACKED:                                \
        fprintf(stderr, RED "ERROR: " RESET "line %d. "     \
                "STACK ATTACKED\n", __LINE__);              \
        break;                                              \
                                                            \
    case ERR_DATA_ATTACKED:                                 \
        fprintf(stderr, RED "ERROR: " RESET "line %d. "     \
                "STACK DATA ATTACKED\n", __LINE__);         \
        break;                                              \
                                                            \
    case ERR_STACK_OVERFLOW:                                \
        fprintf(stderr, RED "ERROR: " RESET "line %d. "     \
                "STACK OVERFLOW\n", __LINE__);              \
        break;                                              \
                                                            \
    case ERR_STACK_UNDERFLOW:                               \
        fprintf(stderr, RED "ERROR: " RESET "line %d. "     \
                "STACK UNDERFLOW\n", __LINE__);             \
                                                            \
    case ERR_NEGATIVE_COUNT:                                \
        fprintf(stderr, RED "ERROR: " RESET "line %d. "     \
                "NEGATIVE NUMBER OF ITEMS\n", __LINE__);    \
        break;                                              \
                                                            \
    case ERR_INC_INPUT:                                     \
        fprintf(stderr, RED "ERROR: " RESET "line %d. "     \
                "INVALID ELEMENT\n", __LINE__);             \
        break;                                              \
                                                            \
    case ERR_OUT_MEMORY:                                    \
        fprintf(stderr, RED "ERROR: " RESET "line %d. "     \
                "LIMITED DYNAMIC MEMORY\n", __LINE__);      \
    break;                                                  \
                                                            \
    };

