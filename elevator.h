#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "list.h"

typedef enum Direction {
    DOWN = -1,
    MOTIONLESS,
    UP
}Direction;

typedef struct ElevatorData {
    uint8_t id;
    uint8_t currentFloor;
    List* destinations;
    Direction direction;
}ElevatorData;

typedef struct ElevatorInfo {
    uint8_t id;
    uint8_t currentFloor;
    uint8_t destination;
}ElevatorInfo;

typedef struct ElevatorCollection {
    ElevatorData** data;
    uint8_t amount;
}ElevatorCollection;

typedef struct InfoCollection {
    ElevatorInfo** data;
    uint8_t amount;
}InfoCollection;

ElevatorCollection* newElevatorCollection(uint8_t amount);
InfoCollection* newInfoCollection(uint8_t amount);

void freeElevatorCollection(ElevatorCollection** collection);
void freeInfoCollection(InfoCollection** collection);

InfoCollection* status(ElevatorCollection* elevators);
void printStatus(InfoCollection* status);

int update(ElevatorCollection* elevators, uint8_t id, uint8_t currentFloor, uint8_t destination);
void setDirection(ElevatorData* elevator);

void pickup(ElevatorCollection* elevators, uint8_t pickupFloor, Direction moveDirection);
uint8_t getPickupElevatorId(ElevatorCollection* elevators);
int getPickupCost(ElevatorData* elevator);

void step(ElevatorCollection* elevators);
int getPickupInput(uint8_t* pickupFloor, Direction* moveDirection);
int getDestinationFloorInput(ElevatorCollection* elevators, uint8_t id, uint8_t* destinationFloor);

#endif
