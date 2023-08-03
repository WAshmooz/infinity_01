/*******************************************************************************
****** Author: HRD31
****** Date: 24.07.2023
****** Version: 2
*******************************************************************************/
#ifndef __AUX_FUNCS_H__
#define __AUX_FUNCS_H__

/******************************************************************************
Description:     	Performs specific action on data.
Return value:    	Returns 0 for success, 1 for fail
*******************************************************************************/
typedef int(*action_func_t)(void *data, void *user_params);

/*******************************************************************************
Description:     	Checks if data applies to specific criteria.
Return value:    	Returns 1 for true, 0 for false
*******************************************************************************/
typedef int (*is_match_func_t)(const void *data, const void *user_params);

/******************************************************************************
Description:     	Performs sort function for sorting.
Return value:    	(lhs < rhs)
******************************************************************************/
typedef int (*is_before_t) (const void *lhs, const void *rhs);

typedef void (*PrintElement) (const void *element);

#endif    /*__AUX_FUNCS_H__*/


