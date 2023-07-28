/*******************************************************************************
****** Author: HRD31
****** Date: 18.07.2023
****** Version: 1
****** Description: Calculator header file.
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "stack.h"
#include "cal.h"

#define LUT_SIZE 256 
#define STACK_NUM_SIZE 200 
#define STACK_OPER_SIZE 200 
#define ERROR_STATE 10 

#define NUM_COL 	 0 
#define OPERATOR_COL 1 

#define PRIORITY_COL 0 
#define OPERATOR_COL 1 

#define DIFFERENCE_IN_ASCI 6	/*Difference between '*' and '/' in ASCI*/
#define OFFSET 42 /* '*' */ 

#define GET_PRIORITY(lut_priority_table) (lut_priority_table.priority) 



	
typedef enum
{
    EXPECT_OPERAND = 0,
    EXPECT_OPERATOR = 1,
    NUM_OF_STATES
}state_t;


typedef struct
{
    stack_t *operand_stack;
    stack_t *operator_stack;
    const char *input;
    void *ptr_lut_operator;
} managerial_struct_t;

typedef struct
{
    handle_func_t handle_func;
    state_t next_state;
} lut_cell_t;

typedef lut_cell_t lut_t[NUM_OF_STATES][LUT_SIZE];


/***	My Structs	***/
typedef enum
{
    PLUS_MINUS = 1,
    MULTI_DIV =  2,
    POW = 		 3,
    DUMMY = 	 0,
    NUM_OF_PRIORITY
}priority_t;


typedef struct
{
    managerial_struct_t operator_fun;
    priority_t priority;
}priority_table_t;

typedef priority_table_t priority_table_size_t[DIFFERENCE_IN_ASCI];

/******************************************************************************
Description:     	Performs sort function for sorting.
Return value:    	(lhs < rhs)
******************************************************************************/
typedef status_t (*handle_func_t) (managerial_struct_t *mangerial_struct);

/******************************************************************************
Description:     	Calculates the value of an expression pointed to by a string 
                    "exp" and saves the result in "result".
Return value:       returns an enum according to final result.
Time Complexity: 	O(n). 
Note:            	exp must consist only digits ('0 - 9', '.') and the following
                    operators '(', ')' , '+' , '-' , '*' , '^' , '/'.
                    
******************************************************************************/

/************** Helper function for Syntax Error **************/
status_t RerurnErrorSyntax(priority_table_t *mangerial_struct)
{
	return SYNTAX_ERROR;
}

/*********************** Num Functions ***********************/
status_t ApplyNumHandle(priority_table_t *mangerial_struct)
{
	char *ptr;
    double ret;	
	ret = strtod(managerial_struct->input, &ptr);
	
	managerial_struct->input = ptr;
    StackPush(managerial_struct->operand_stack, ret);
	return SUCCESS;
}	
/******************** Operator Functions ********************/
status_t OperatorFunc(priority_table_t *mangerial_struct)
{
	double operand1 = 0, operand2 = 0;	
	
	SetTo(mangerial_struct, &operand1, &operand2);
	
	if(lut_priority_table[][])
	
}

GET_PRIORITY(lut_priority_table[][])







void SetTo(priority_table_t *mangerial_struct, double *operand1, double *operand2) 
{
	operand1 = *(double*)StackPeek(managerial_struct->operand_stack);
	StackPop(managerial_struct->operand_stack);
	
	operand2 = *(double*)StackPeek(managerial_struct->operand_stack);
	StackPop(managerial_struct->operand_stack);
}

status_t AddFun(double *operand1, double *operand2) 
{
    StackPush(managerial_struct->operand_stack, operand1 + operand2);
    
    return SUCCESS;
}

status_t MinusFun(priority_table_t *mangerial_struct, double *operand1, double *operand2)  
{
    StackPush(managerial_struct->operand_stack, operand2 - operand1);
    
    return SUCCESS;
}

status_t MultiFun(priority_table_t *mangerial_struct, double *operand1, double *operand2) 
{
    StackPush(managerial_struct->operand_stack, operand1 * operand2);
    
    return SUCCESS;
}

status_t DivFun(priority_table_t *mangerial_struct, double *operand1, double *operand2) 
{
	if (operand1 == 0)
	{
		return MATH_ERROR;
	}
	
    StackPush(managerial_struct->operand_stack, operand2 / operand1);
    
    return SUCCESS;
}

/******************** xxxxx ********************/




/************************************************************/
status_t Calculator(const char *exp, double *result)
{
	size_t num_of_operatorand = STACK_NUM_SIZE, operatorand_size = sizeof(double);
	size_t num_of_operator = STACK_OPER_SIZE, operator_size = sizeof(char);
	stack_t *operand_stack = NULL, *operator_stack = NULL;
	
	stack_t operand_stack = StackCreate(num_of_operatorand, operatorand_size);
	stack_t operator_stack = StackCreate(num_of_operator, operator_size);	
	
	size_t i = 0, j = 0;
	lut_cell_t lut_cell;
	lut_t lut_table;
	priority_table_t managerial_struct;
	priority_table_t lut_priority_table;
	priority_table_cell_t lut_priority_error_cell;
	void *ptr_lut_priority_table = lut_priority_table;
	char *c = exp;
	state_t status status = EXPECT_OPERAND;
	
	/*push dummy in operator_stack*/	
	lut_priority_error_cell.priority[','- OFFSET] = DUMMY;	
	StackPush(operator_stack, lut_priority_error_cell);
		
	/*initialize managerial_struct for the input functions*/	
	managerial_struct.operand_stack = operand_stack;
	managerial_struct.operator_stack = operator_stack;
	managerial_struct.input = exp;
	managerial_struct.ptr_priority_table = ptr_lut_priority_table;	
	
	/*initialize ErrorSyntax to lut_cell*/
	lut_cell.handle_func =&RerurnErrorSyntax;
	lut_cell.next_state = ERROR_STATE;	

	/*initialize all cells in lut_table to be all ErorSyntax*/
	for(i = 0; i < LUT_SIZE; i++)
	{
		for(j = 0; j < NUM_OF_STATES; j++)
		{
			lut_table[i][j] = lut_cell;
		}
	}
	
	/*Arranges the operators section in lut_table*/
	lut_cell.handle_func =&OperatorFunc(managerial_struct);
	lut_cell.next_state = EXPECT_OPERAND;	
	
	lut_table['+'][OPERATOR_COL] = lut_cell;	
	lut_table['-'][OPERATOR_COL] = lut_cell;	
	lut_table['*'][OPERATOR_COL] = lut_cell;	
	lut_table['/'][OPERATOR_COL] = lut_cell;		 
		
	/*Arranges the num section in lut_table*/
	lut_cell.handle_func = &ApplyNumHandle(managerial_struct);
	lut_cell.next_state = EXPECT_OPERATOR;
		
	for(i = 0; i <= 9; i++)
	{
		lut_table[i][NUM_COL] = lut_cell;
	}
	
	/*Arranges the lut_priority_table*/	
	lut_priority_table.operator_fun['*'- OFFSET] = &MultiFun(managerial_struct);
	lut_priority_table.priority['*'- OFFSET] = MULTI_DIV;
	
	lut_priority_table.operator_fun['+'- OFFSET] = &AddFun(managerial_struct);
	lut_priority_table.priority['+'- OFFSET] = PLUS_MINUS;
	
	lut_priority_table.operator_fun['-'- OFFSET] = &MinusFun(managerial_struct);
	lut_priority_table.priority['-'- OFFSET] = PLUS_MINUS;
	
	lut_priority_table.operator_fun['/'- OFFSET] = &DivFun(managerial_struct);
	lut_priority_table.priority['/'- OFFSET] = MULTI_DIV;
	
	/*Start the calculator*/
	lut_table [status][*c - OFFSET]= 
	
}

int main()
{
	return 0;
}


