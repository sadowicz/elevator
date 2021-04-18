#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <inttypes.h>

typedef struct ListItem {
    uint8_t data;
    struct ListItem* next;
}ListItem;

ListItem* createList();
void deleteList(ListItem** list);

ListItem* front();
int isEmpty(ListItem* list);

int insert(ListItem* after, uint8_t data);
int insertFront(ListItem* list, uint8_t data);

int remove(ListItem* after);
int removeFront(ListItem* list);
void clearList(ListItem* list);

#endif
