/*******************************************************************************
****** Author: HRD31
****** Date: 20.11.2023
****** Version: 1
*******************************************************************************/
#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>  /* fprintf */
#include <arpa/inet.h> /*uint16_t*/

#define UNUSED(x) (void)(x)

enum {ILRD_SUCCESS = 0};
enum {ILRD_FALSE = 1};

enum {ERR_SYS = 1};


/*******************************************************************************
Description:        Macro Function checks the 'condition',
                    FALSE  - prints message and returns with error number
                            indicator 'err_type' from function that the 
                            Macro placed in.
                    TRUE - nothing happen.
*******************************************************************************/
#define RETURN_IF_ERROR(condition, msg, err_type) \
do { \
    if (condition) { \
        fprintf(stderr, "Error: %s (Line %d)\n", (msg), __LINE__); \
        return (err_type); \
    } \
} while (0)

/*******************************************************************************
Description:        Function checks the 'condition',
                    FALSE  - prints message and exit the program with error
                            number indicator 'err_type'.
                    TRUE - nothing happen.
*******************************************************************************/
void ExitIfError(int condition, const char *msg, int err_type);

/*******************************************************************************
Description:        Error handling utility function that prints a formatted 
                    message to standard output and exits the program. 
                    It takes a variable number of arguments similar to printf 
                    for formatting the message.
Note:               This function can accept additional arguments in a variable 
                    number and with variable types, similar to how printf or 
                    fprintf functions work in C. 
*******************************************************************************/
void Err_N_Die(const char *fmt, ...);

/*******************************************************************************
Description:     Initialize Socket Adress of Server / Client
*******************************************************************************/
void initializeSocketAddress(struct sockaddr_in *sockaddr, uint16_t port);

/*******************************************************************************
Description:    This function generates a timestamp and prints the action along 
                with the timestamp in the format: [timestamp] action. Useful for 
                creating log entries with timestamps for various actions or 
                events.
                Timestamp Format (%Y-%m-%d %H:%M:%S): This format specifies the 
                year, month, day, hour, minute, and second components of the 
                timestamp in a specific order. For example:
*******************************************************************************/
void LogAction(const char *action); 


#endif    /*__UTILS_H__*/
