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

        if(!isEmpty(elevators->data[i]->destinations))
            status->data[i]->destination = front(elevators->data[i]->destinations)->data;
        else
            status->data[i]->destination = 0;
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

    ElevatorData* elevator = elevators->data[id];

    elevator->currentFloor = currentFloor;

    ListItem* destinationFloor = find(elevator->destinations, currentFloor);
    if(destinationFloor) {
        removeAfter(destinationFloor->prev);
        //CHOOSE DESTINATION FLOOR (optional)
        uint8_t newDestination;
        if(!getDestinationFloorInput(elevators, id, &newDestination))
            update(elevators, id, elevators->data[id]->currentFloor, newDestination);
    }

    if(destination < 255 && !find(elevator->destinations, destination))
        insertBack(elevator->destinations, destination);

    setDirection(elevator);

    return 0;
}

void setDirection(ElevatorData* elevator) {
    if(isEmpty(elevator->destinations) || elevator->currentFloor == elevator->destinations->head->next->data)
        elevator->direction = MOTIONLESS;
    else if(elevator->currentFloor > elevator->destinations->head->next->data)
        elevator->direction = DOWN;
    else
        elevator->direction = UP;
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

    while(destination != elevator->destinations->tail) {
        cost += abs(destination->data - destination->next->data);
        destination = destination->next;
    }

    return cost;
}

void step(ElevatorCollection* elevators) {

    //UPDATE ELEVATORS BY 1 IN ELEVATOR DIRECTION
    for(int i = 0; i < elevators->amount; i++) {
        uint8_t newFloor = elevators->data[i]->currentFloor + elevators->data[i]->direction;
        update(elevators, i, newFloor, 255);
    }

    //PICK UP (optional)
    uint8_t pickupFloor;
    Direction direction;

    while(!getPickupInput(&pickupFloor, &direction))
        pickup(elevators, pickupFloor, direction);
}

int getPickupInput(uint8_t* pickupFloor, Direction* moveDirection) {
    printf("Do you want to pick-up elevator? (y/n): ");
    char pickupResponse;
    scanf(" %c", &pickupResponse);
    if(pickupResponse != 'y') {
        if(pickupResponse != 'n')
            fprintf(stderr, "Invalid input format\n");
        return 1;
    }

    printf("Your floor: ");
    int floor;
    scanf(" %d", &floor);
    *pickupFloor = (uint8_t)floor;
    char directionBuffer[5] = {0};
    printf("Select move direction (up/down): ");
    scanf(" %s", directionBuffer);

    if(!strcmp(directionBuffer, "up"))
        *moveDirection = UP;
    else if(!strcmp(directionBuffer, "down"))
        *moveDirection = DOWN;
    else {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }

    return 0;
}

int getDestinationFloorInput(ElevatorCollection* elevators, uint8_t id, uint8_t* destinationFloor) {
    printf("Do you want to choose floor in elevator (id: %d)? (y/n): ", id);
    char destinationResponse;
    scanf(" %c", &destinationResponse);
    if(destinationResponse != 'y') {
        if(destinationResponse != 'n')
            fprintf(stderr, "Invalid input format");
        return 1;
    }

    printf("Select destination floor: ");
    int floor;
    scanf(" %d", &floor);
    *destinationFloor = (uint8_t)floor;

    return 0;
}