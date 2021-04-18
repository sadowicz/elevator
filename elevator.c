#include "elevator.h"

ElevatorCollection* newElevatorCollection(uint8_t amount) {
    ElevatorCollection* collection = (ElevatorCollection*)malloc(sizeof(ElevatorCollection));
    if(!collection)
        return NULL;

    collection->data = (ElevatorData**)malloc(amount * sizeof(ElevatorData*));
    if(!collection->data)
        return NULL;

    collection->amount = amount;

    for(uint8_t i = 0; i < amount; i++) {
        collection->data[i] = (ElevatorData*)calloc(1, sizeof(ElevatorData));

        if(!collection->data[i])
            return NULL;

        collection->data[i]->id = i;
        collection->data[i]->destinations = createList();
    }

    return collection;
}

InfoCollection* newInfoCollection(uint8_t amount) {
    InfoCollection* collection = (InfoCollection*)malloc(sizeof(InfoCollection));
    if(!collection)
        return NULL;

    collection->data = (ElevatorInfo**)malloc(amount * sizeof(ElevatorInfo*));
    if(!collection->data)
        return NULL;

    collection->amount = amount;

    for(uint8_t i = 0; i < amount; i++) {
        collection->data[i] = (ElevatorInfo*)calloc(1, sizeof(ElevatorInfo));

        if(!collection->data[i])
            return NULL;

        collection->data[i]->id = i;
    }

    return collection;
}

void freeElevatorCollection(ElevatorCollection** collection) {
    for(uint8_t i = 0; i < (*collection)->amount; i++) {
        deleteList(&(*collection)->data[i]->destinations);
        free((*collection)->data[i]);
        (*collection)->data[i] = NULL;
    }

    free((*collection)->data);
    (*collection)->data = NULL;

    free(*collection);
    *collection = NULL;
}

void freeInfoCollection(InfoCollection** collection) {
    for(uint8_t i = 0; i < (*collection)->amount; i++) {
        free((*collection)->data[i]);
        (*collection)->data[i] = NULL;
    }

    free((*collection)->data);
    (*collection)->data = NULL;

    free(*collection);
    *collection = NULL;
}

InfoCollection* status(ElevatorCollection* elevators) {

    InfoCollection* status = newInfoCollection(elevators->amount);
    if(!status)
        return NULL;

    for(uint8_t i = 0; i < elevators->amount; i++) {
        status->data[i]->id = elevators->data[i]->id;
        status->data[i]->currentFloor = elevators->data[i]->currentFloor;
        status->data[i]->destination = front(elevators->data[i]->destinations)->data;
    }

    return status;
}

void printStatus(InfoCollection* status) {
    if(!status)
        return;

    for(uint8_t i = 0; i < status->amount; i++) {
        ElevatorInfo* info = status->data[i];
        printf("ID: %d\tFLOOR: %d\tDESTINATION: %d\n", info->id, info->currentFloor, info->destination);
    }
}

int update(ElevatorCollection* elevators, uint8_t id, uint8_t currentFloor, uint8_t destination) {
    if(!elevators || id >= elevators->amount)
        return 1;

    elevators->data[id]->currentFloor = currentFloor;

    if(!find(elevators->data[id]->destinations, destination) && destination > 0)
        insertBack(elevators->data[id]->destinations, destination);

    if(isEmpty(elevators->data[id]->destinations)) {
        if (currentFloor > destination)
            elevators->data[id]->direction = DOWN;
        else if (currentFloor < destination)
            elevators->data[id]->direction = UP;
        else
            elevators->data[id]->direction = MOTIONLESS;
    }

    return 0;
}

void pickup(ElevatorCollection* elevators, uint8_t pickupFloor, Direction moveDirection) {
    uint8_t pickupId = getPickupElevatorId(elevators);
    update(elevators, pickupId, elevators->data[pickupId]->currentFloor, pickupFloor);
}

uint8_t getPickupElevatorId(ElevatorCollection* elevators) {

    int minCost = getPickupCost(elevators->data[0]);
    uint8_t bestElevatorId = 0;

    for(int i = 1; i < elevators->amount; i++) {
        int cost = getPickupCost(elevators->data[i]);
        if(cost < minCost) {
            minCost = cost;
            bestElevatorId = i;
        }
    }

    return bestElevatorId;
}

int getPickupCost(ElevatorData* elevator) {
    int cost = 0;
    ListItem* destination = elevator->destinations->head->next;

    while(destination->next != elevator->destinations->tail) {
        cost += abs(destination->data - destination->next->data);
        destination = destination->next;
    }

    return cost;
}