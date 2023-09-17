/*******************************************************************************
****** Author: HRD31
****** Date: 14.08.2023
****** Version: 3
*******************************************************************************/
#ifndef __AUX_FUNCS_H__
#define __AUX_FUNCS_H__

#define UNUSED(x) (void)(x)

enum {ILRD_SUCCESS = 0};
enum {ILRD_FALSE = 1};

/*******************************************************************************
Description:     	Performs specific action on data.
Return value:    	Returns 0 for success, 1 for fail
*******************************************************************************/
typedef int(*action_func_t)(void *data, void *user_params);

/*******************************************************************************
Description:     	Checks if data applies to specific criteria.
Return value:    	Returns 1 for true, 0 for false
*******************************************************************************/
typedef int (*is_match_func_t)(const void *data, const void *user_params);

/*******************************************************************************
Description:     	Performs sort function for sorting.
Return value:    	(lhs < rhs)
*******************************************************************************/
typedef int (*is_before_t) (const void *lhs, const void *rhs);

/*******************************************************************************
Description:     	Macro Function checks the 'condition',
                    FALSE  - prints message and returns with error number
                            indicator 'err_type' from function that the 
                            Macro placed in.
                    TRUE - nothing happen.
*******************************************************************************/
#define RETURN_IF_ERROR(condition, msg, err_type) \
do { \
    if (!((condition))) { \
        fprintf(stderr, "Error: %s (Line %d)\n", (msg), __LINE__); \
        return (err_type); \
    } \
} while (0)


/*******************************************************************************
Description:     	Function checks the 'condition',
                    FALSE  - prints message and exit the program with error
                            number indicator 'err_type'.
                    TRUE - nothing happen.
*******************************************************************************/
void ExitIfError(int condition, const char *msg, int err_type);




#endif    /*__AUX_FUNCS_H__*/
