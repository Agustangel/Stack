
#include <stdio.h>
#include <assert.h>
#include <logger.h>
#include <stdlib.h>

#include "stack.h"

/* TODO:
    1) */
int main()
{
    logger_init(0, "stack.log");
    logger_set_level(INFO);

    stack_t stack;

    int init_size = 5;

    int ret = stack_init(&stack, init_size);
    
    stack_push(&stack, 1);
    stack_push(&stack, 2);
    stack_push(&stack, 11);
    stack_push(&stack, 32);
    stack_peek(&stack);

    // stack_pop(&stack);
    // stack_pop(&stack);
    // stack_pop(&stack);
    // stack_pop(&stack);
    // stack_pop(&stack);
    // stack_pop(&stack);
    
    stack_dump(&stack);

    stack_destroy(&stack);

    logger_finalize(file);

    return 0;
}
