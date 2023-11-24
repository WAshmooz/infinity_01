/*******************************************************************************
****** Author: Daniel Shabso
****** Reviwer: Gaby
****** Date: 15.11.23
****** Version: 1 
****** Description: IPC Ping-Pong
*******************************************************************************/
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <fcntl.h>

#define SEM_KEY 1234

void ChildProcess(int sem_id) 
{
    while (true) 
    {
        struct sembuf sem_wait = {0, -1, 0}; // Wait operation
        struct sembuf sem_signal = {0, 1, 0}; // Signal operation

        semop(sem_id, &sem_wait, 1); // Wait for the semaphore
        std::cout << "PONG" << std::endl;
        std::cout.flush();
        sleep(1); // Sleep to slow down the process for demonstration
        semop(sem_id, &sem_signal, 1); // Signal the semaphore
    }
}

int main() 
{
    pid_t pid;
    int sem_id;

    sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (sem_id == -1) 
    {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    union semun 
    {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } sem_init;

    sem_init.val = 1; // Initialize semaphore value to 1
    if (semctl(sem_id, 0, SETVAL, sem_init) == -1) 
    {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        /* Child process */
        ChildProcess(sem_id);
    } else {
        /* Parent process */
        for (int i = 0; i < 5; ++i) 
        {
            sleep(1); // Sleep to avoid race condition

            struct sembuf sem_wait = {0, -1, 0}; // Wait operation
            struct sembuf sem_signal = {0, 1, 0}; // Signal operation

            semop(sem_id, &sem_wait, 1); // Wait for the semaphore
            std::cout << "PING" << std::endl;
            std::cout.flush();
            semop(sem_id, &sem_signal, 1); // Signal the semaphore
        }
    }

    return 0;
}
