#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
void handler(int signo, siginfo_t *info, void *context)
{
    struct sigaction oldact;

    if (sigaction(SIGSEGV, NULL, &oldact) == -1
        || (oldact.sa_flags & SA_UNSUPPORTED)
        || !(oldact.sa_flags & SA_EXPOSE_TAGBITS))
    {
        _exit(EXIT_FAILURE);
    }
    _exit(EXIT_SUCCESS);
}
*/

int main(void)
{
	/* struct sigaction act = { 0 }; */
	struct sigaction act;
	
	/*
	act.sa_flags = SA_SIGINFO | SA_UNSUPPORTED | SA_EXPOSE_TAGBITS;
	act.sa_sigaction = &handler;
	if (sigaction(SIGSEGV, &act, NULL) == -1) {
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	
	raise(SIGSEGV);
	*/
	return 0;
}
