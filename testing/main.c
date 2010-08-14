#include <stdio.h>
#include <signal.h>
#include "../pointit/pointit.h"

void sigtrap() {
    pointit_destroy();
    exit(0);
}

int main(void) {
    if (pointit_init() != 0) {
        printf("An error ocurred while starting pointit\n");
        return 0;
    }

    signal(SIGINT, sigtrap);
    signal(SIGTERM, sigtrap);
    
    for (;;) {
        pointit_detect();
        
        //if (pointit_get_diff_x() || pointit_get_diff_y())
            printf("Detected (%d %d)\n", pointit_get_x() , pointit_get_y());
    }

    pointit_destroy();
    return 0;
}
