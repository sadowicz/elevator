#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef struct ElevatorInfo {
    uint8_t id;
    uint8_t currentFloor;
    uint8_t destination;
}ElevatorInfo;

typedef struct ElevatorCollection {
    ElevatorInfo** data;
    uint8_t amount;
}ElevatorCollection;

ElevatorCollection* newElevatorCollection(uint8_t amount);
ElevatorInfo* newElevator();

void freeElevatorCollection(ElevatorCollection** collection);

void printStatus(ElevatorCollection* statusCollection);

int update(ElevatorCollection* elevators, uint8_t id, uint8_t currentFloor, uint8_t destination);


#endif
