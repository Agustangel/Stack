
#include <stdio.h>
#include <assert.h>
#include <logger.h>
#include <stdlib.h>

#include "stack.h"


int main()
{
    logger_init(0, "stack.log");
    logger_set_level(INFO);

    stack_t stack;

    int init_size = 5;

    int ret = stack_init(&stack, 5);
    HANDLE_ERROR(ret, ERR_OUT_MEMORY, "ERROR: memory failure.\n");
    HANDLE_ERROR(ret, ERR_NULL_POINTER, "ERROR: memory failure.\n");

    STACK_OK(&stack);
    stack_push(&stack, 1);

    STACK_OK(&stack);
    stack_push(&stack, 2);

    STACK_OK(&stack);
    stack_push(&stack, 11);

    STACK_OK(&stack);
    stack_push(&stack, 32);

    STACK_OK(&stack);
    stack_pop(&stack);
    STACK_OK(&stack);

    //stack_dump(&stack);

    logger_finalize(file);

    return 0;
}
