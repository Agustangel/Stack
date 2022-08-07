

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
    ERR_INC_INPUT        = -5,
    ERR_OUT_MEMORY       = -4,
    ERR_STACK_UNDERFLOW  = -3,
    ERR_STACK_OVERFLOW   = -2,
    ERR_NULL_POINTER     = -1
};

//! Macros HANDLE_ERROR
/*! macros to describe the error. */
/*! \param retval - return value to be tested.
/*! \param errcode - const describing a possible error. 
/*! \param msg - error message. */
#define HANDLE_ERROR(retval, errcode, msg) \
    if (retval == errcode)                 \
    {                                      \
        printf(msg);                       \
        exit(errcode);                     \
    };


int stack_init(stack_t* stack, int init_size);
int stack_push(stack_t* stack, int value);
int stack_pop(stack_t* stack);
int stack_peek(const stack_t* stack);
int stack_dump(stack_t* stack);
int stack_resize_decrease(stack_t* stack);
int stack_resize_increase(stack_t* stack);
int stack_destroy(stack_t* stack);

