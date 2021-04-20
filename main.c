#include <stdio.h>
#include <errno.h>

#include "elevator.h"

uint8_t strToUint8(char* str);

int main(int argc, char* argv[]) {

    if(argc != 2) {
        fprintf(stderr, "USAGE: %s <amount_of_elevators>\n", argv[0]);
        return EXIT_FAILURE;
    }

    uint8_t elevatorsAmount = strToUint8(argv[1]);
    if(!elevatorsAmount) {
        fprintf(stderr, "Invalid amount of elevators parameter\n");
        return EXIT_FAILURE;
    }

    ElevatorCollection* elevators = newElevatorCollection(elevatorsAmount);
    if(!elevators) {
        perror("");
        return EXIT_FAILURE;
    }

    char continueSimulation = 'y';

    while(continueSimulation == 'y') {
        step(elevators);
        InfoCollection* elevatorsInfo = status(elevators);
        printStatus(elevatorsInfo);
        freeInfoCollection(&elevatorsInfo);

        printf("\nWould you like to continue with simulation? (y/n): ");
        scanf(" %c", &continueSimulation);
        printf("\n");
    }

    freeElevatorCollection(&elevators);

    return EXIT_SUCCESS;
}

uint8_t strToUint8(char* str) {
    char* endptr = NULL;
    errno = 0;
    long number = strtol(str, &endptr, 0);

    if(errno || *endptr || number < 1 || number > 16)
        return 0;

    return (uint8_t)number;
}