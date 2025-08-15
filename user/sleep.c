#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) 
{
    // Check if the correct number of arguments is provided
    if (argc != 2) {
        printf("Usage: sleep <ticks>\n");
        exit(1);
    }

    // Convert the argument to an integer
    int ticks = atoi(argv[1]);
    if (ticks < 0) {
        printf("Sleep: ticks must be a positive integer\n");
        exit(1);
    }

    // Call the sleep function with the specified number of ticks
    sleep(ticks);
    exit(0);
}