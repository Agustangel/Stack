

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

#define INIT_SIZE 10
#define MULTIPLIER 2

typedef struct stack_t
{

    int canary_1;

    int* data;
    int count;
    int capacity;

    int canary_2;

} stack_t;


enum error_names
{
    ERR_INC_INPUT = -5,
    ERR_OUT_MEMORY  = -4,
    ERR_STACK_UNDERFLOW  = -3,
    ERR_STACK_OVERFLOW   = -2
};


stack_t* stack_create();