/*******************************************************************************
****** Author: HRD31
****** Date: 18.07.2023
****** Version: 1
****** Description: Calculator header file.
*******************************************************************************/
#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__


typedef enum 
{
    SUCCESS = 0,
    SYNTAX_ERROR = 1,
    MATH_ERROR = 2,
    SYSTEM_ERROR = 3
} status_t;

/*******************************************************************************
Description:     	Calculates the value of an expression pointed to by a string 
                    "exp" and saves the result in "result".
Return value:       returns an enum according to final result.
Time Complexity: 	O(n). 
Note:            	exp must consist of only digits ('0 - 9', '.') and the 
                    following operators '(', ')' , '+' , '-' , '*' , '^' , '/'.
                    
*******************************************************************************/
status_t Calculator(const char *exp, double *result);

#endif    /*__CALCULATOR_H__*/
