#ifndef ELEVATOR_H
#define ELEVATOR_H

typedef struct ElevatorInfo {
    uint8_t id;
    uint8_t currentFloor;
    uint8_t destination;
}ElevatorInfo;

typedef struct ElevatorCollection {
    ElevatorInfo** data;
    uint8_t amount;
};

#endif
