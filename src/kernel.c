/****************************************************
 * kernel.c - This is the true home of the main()   *
 * function in the OS. The kernel.c main() function *
 * calls the shell.c int shellUI() function         *
 * (the previous main() from A1), to display the    *
 * command-line to the user. The kernel main() will *
 * also instantiate all the kernel data structures. *
 * Jason Dan | 260718739                            *
 ****************************************************/ 

#include "shell.h"

int main(int argc, char **argv) {
    shellUI();
}