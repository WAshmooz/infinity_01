/*
The volatile keyword in C is used to indicate that a variable's value can be changed 
unexpectedly by external factors outside the program's control. It tells the compiler not to 
make assumptions about the variable's value and to always retrieve it from memory, rather 
than relying on any cached value.

Here's an example to demonstrate the usage of the volatile keyword:
#include <stdio.h>

volatile int sensorValue;

void readSensor() {
	// Simulating external hardware that changes sensorValue
	sensorValue = 10;
}

int main() {
    printf("Initial sensor value: %d\n", sensorValue);

    readSensor();

    printf("Updated sensor value: %d\n", sensorValue);

    return 0;
}
In this example, sensorValue is declared as a volatile int variable. 
The readSensor() function is used to simulate an external hardware device 
that updates the sensorValue. Without the volatile keyword, the compiler 
might optimize the code by assuming that sensorValue does not change between 
the initial and updated printing statements. However, since sensorValue is 
declared as volatile, the compiler is forced to fetch the updated value from 
memory each time it is accessed.

By using volatile, you ensure that the compiler does not optimize away accesses to variables that can be modified by external sources, such as hardware registers, interrupt service routines, or multi-threaded environments. It helps maintain the correctness and reliability of your code when dealing with such variables.
*/

#include <stdio.h>
volatile int sensorValue;

void readSensor() {
	// Simulating external hardware that changes sensorValue
	sensorValue = 10;
}

int main() {
    printf("Initial sensor value: %d\n", sensorValue);

    readSensor();

    printf("Updated sensor value: %d\n", sensorValue);

    return 0;
}

