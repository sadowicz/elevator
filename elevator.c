#include "elevator.h"

ElevatorCollection* newElevatorCollection(uint8_t amount) {
    ElevatorCollection* collection = (ElevatorCollection*)malloc(sizeof(ElevatorCollection));
    if(!collection)
        return NULL;

    collection->data = (ElevatorInfo**)malloc(amount * sizeof(ElevatorInfo*));
    if(!collection->data)
        return NULL;

    collection->amount = amount;

    for(uint8_t i = 0; i < amount; i++) {
        collection->data[i] = newElevator();

        if(!collection->data[i])
            return NULL;

        collection->data[i]->id = i;
    }

    return collection;
}

ElevatorInfo* newElevator() {
    ElevatorInfo* elevator = (ElevatorInfo*)calloc(1, sizeof(ElevatorInfo));
    return elevator;
}

void freeElevatorCollection(ElevatorCollection** collection) {

    for(uint8_t i = 0; i < (*collection)->amount; i++) {
        free((*collection)->data[i]);
        (*collection)->data[i] = NULL;
    }

    free((*collection)->data);
    (*collection)->data = NULL;

    free(*collection);
    collection = NULL;
}

void printStatus(ElevatorCollection* statusCollection) {
    for(uint8_t i = 0; i < statusCollection->amount; i++) {
        ElevatorInfo* info = statusCollection->data[i];
        printf("ID: %d\tFLOOR: %d\tDESTINATION: %d\n", info->id, info->currentFloor, info->destination);
    }
}

int update(ElevatorCollection* elevators, uint8_t id, uint8_t currentFloor, uint8_t destination) {

    if(id >= elevators->amount)
        return 1;

    elevators->data[id]->currentFloor = currentFloor;
    elevators->data[id]->destination = destination;

    return 0;
}