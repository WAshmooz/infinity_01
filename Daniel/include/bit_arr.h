/*******************************************************************************
****** Author: HRD31
****** Date: 29.05.2023
****** Version: 1
*******************************************************************************/
#ifndef __BIT_ARR_H__
#define __BIT_ARR_H__

#include <stddef.h> /* size_t */

typedef size_t bit_array_t;

/*******************************************************************************
general notes:  LSB is in index 0 
                undefined behaviour if index is out of range ( (< 0) or (> 63) )
*******************************************************************************/

/*******************************************************************************
description:  return bit array with all the bits set
*******************************************************************************/
bit_array_t BitArrSetAll(bit_array_t bit_arr);


/*******************************************************************************
description:  return bit array with all the bits off
*******************************************************************************/
bit_array_t BitArrResetAll(bit_array_t bit_arr);


/*******************************************************************************
description:  return bit array with bit in index i on.
*******************************************************************************/
bit_array_t BitArrSetOn(bit_array_t bit_arr, size_t idx);


/*******************************************************************************
description:   return bit array with bit in index i off.
*******************************************************************************/
bit_array_t BitArrSetOff(bit_array_t bit_arr, size_t idx);


/*******************************************************************************
description:  changes the set value of a bit by index "idx" in given bit array
              to the given "boolean_val".
*******************************************************************************/ 
bit_array_t BitArrSetBit(bit_array_t bit_arr, size_t idx, int boolean_val);


/*******************************************************************************
description:  get the  value of the bit by index "idx" in given bit array.
return value: boolean 1/0. 
*******************************************************************************/
int BitArrGetVal(bit_array_t bit_arr, size_t idx);


/*******************************************************************************
description:  change the value of a bit in index "idx" in given "bit_arr".
*******************************************************************************/ 
bit_array_t BitArrFlipBit(bit_array_t bit_arr, size_t idx);


/*******************************************************************************
description:  reverses all bits in given "bit_arr".
arguments:    bit array pointer.
*******************************************************************************/
bit_array_t BitArrMirror(bit_array_t bit_arr);


/*******************************************************************************
description:  rotate right "shifts" times the bits in given bit array.
arguments:    shifts- number of rotations.
*******************************************************************************/
bit_array_t BitArrRotateRight(bit_array_t bit_arr, size_t shifts);


/*******************************************************************************
description:  rotate left n times the bits in given bit array.
arguments:    bit array pointer, size_t number of rotations.
*******************************************************************************/
bit_array_t BitArrRotateLeft(bit_array_t bit_arrr, size_t n);

/*******************************************************************************
description:  returns number of set bits in given "bit_arr"
*******************************************************************************/
size_t BitArrCountOn(bit_array_t bit_arr);

/*******************************************************************************
description:  returns number of unset (off) bits in given "bit_arr" 
*******************************************************************************/
size_t BitArrCountOff(bit_array_t bit_arr);


/*******************************************************************************
description:  return string representation of the given bit array.
return value : pointer to dest
notes:         dest size must be large enough ( < 64 )
*******************************************************************************/
char *BitArrToString(bit_array_t bit_arr, char *dest);


#endif    /*__BIT_ARR_H__*/
