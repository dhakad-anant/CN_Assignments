#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

/* #if !defined(__bool_true_false_are_defined) && !defined(__cplusplus)
typedef int bool;
#define true 1
#define false 0
#define __bool_true_false_are_defined
#endif */

#include <limits.h>
#include <ctype.h>

#define mxSize 1000


/* Defining the stack struct */
typedef struct Stack{
    int top; // pointer to top element.
    int capacity; // capacity of the stack(i.e., max size that the stack can grow upto)
    float* array;
} Stack;

/*  Stack Operations */
Stack* createStack(int capacity){
    Stack* stack = (Stack*) malloc(sizeof(Stack));
    
    if (!stack){
        printf("Error: couldn't create a stack!\n");
        return NULL;
    }
 
    stack->top = -1;
    stack->capacity = capacity;
    stack->array = (float*) malloc(stack->capacity * sizeof(float));
 
    if (!stack->array){
        printf("Error: couldn't create a array for stack!\n");
        return NULL;
    }

    return stack;
}
 
bool isEmpty(Stack* stack){
    return stack->top == -1 ;
}
 
float peek(Stack* stack){
    return stack->array[stack->top];
}
 
float pop(Stack* stack){
    if (!isEmpty(stack))
        return stack->array[stack->top--];

    printf("Error: Trying to pop from an empty stack\n");
    return -1;
}
 
void push(Stack* stack,float op){
    stack->array[++stack->top] = op;
}
/*  Stack Operations. Ends*/


/**
 * @brief Evaluates a postfix expression and
 * returns the answers as a float value.
 * 
 * @param exp Input postfix expression
 * @return float 
 */
float evaluatePostfix(char exp[]){
    // Create a stack of capacity equal to expression size
    Stack* stack = createStack(strlen(exp));

    // See if stack was created successfully
    if (!stack) return INT_MIN;
    
    // Scanning all characters one by one from left to right.
    for(int i=0; exp[i] != '\n'; i++){
        // If the character is blank space then continue
        if(exp[i]==' ')continue;

        // If the scanned character is an
        // operand (number here),extract the full number
        // Push it to the stack.
        else if(isdigit(exp[i])){
            char tmp[mxSize];
            bzero(tmp, sizeof(tmp));

            int j=i;
            int idx=0;
            while(exp[j] != ' '){
                tmp[idx++] = exp[j++];
            }
            i = j-1;

            float num = (float)atof(tmp);
            push(stack, num);
        }

        // If the scanned character is an operator, pop two
        // elements from stack apply the operator
        else{
            float val1 = pop(stack);
            float val2 = pop(stack);
            float res;
            switch (exp[i]){
                case '-': res = val2 - val1; break;
                case '+': res = val2 + val1; break;
                case '*': res = val2 * val1; break;
                case '/': res = val2 / val1; break;
            }
            push(stack, res);
        }
    }
    
    return pop(stack);
}


// End of header file
#endif