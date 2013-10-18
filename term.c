#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>

static int s_in_fd;
static struct termios s_old_tc;

static void
term_init(void)
{
    struct termios tty;

    tcgetattr(s_in_fd, &s_old_tc);
    memcpy(&tty, &s_old_tc, sizeof(s_old_tc));

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_iflag = IGNBRK;
    tty.c_lflag = 0;
    tty.c_cflag |= CLOCAL | CREAD;
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    tty.c_cflag &= ~(PARENB | PARODD);
    tcsetattr(s_in_fd, TCSANOW, &tty);
}

static void
term_exit(void)
{
    printf("Bye!\n");
    tcsetattr(s_in_fd, TCSANOW, &s_old_tc);
}

static void
sig_exit(int sig)
{
    printf("signal %d received\n", sig);
    tcsetattr(s_in_fd, TCSANOW, &s_old_tc);
    exit(0);
}

int main(int argc, char *argv[])
{
    int i, loop = 100;

    if (argc > 1)
	loop = atoi(argv[1]);
    atexit(term_exit);
    signal(SIGINT, sig_exit);
    printf("veeeeeeeeeeeeeeeeerryyyyyyyyyyyyy"
	   "loooooooooooooooooooooooooooooooong"
	   "printttttttttttttttttttttttttttttttttttttttttttt"
	   "to fill stdout stream buffer\n");
    term_init();
    for (i = 0; i < loop; ++i)
    {
	printf("i = %d\n", i);
	usleep(10 * 1000);
    }

    return 0;
}
