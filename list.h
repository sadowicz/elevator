#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <inttypes.h>

typedef struct ListItem {
    uint8_t data;
    struct ListItem* next;
    struct ListItem* prev;
}ListItem;

typedef struct List {
    ListItem* head;
    ListItem* tail;
}List;

List* createList();
void deleteList(List** list);

ListItem* front(List* list);
ListItem* back(List* list);
ListItem* find(List* list, uint8_t value);
int isEmpty(List* list);

int insertAfter(ListItem* after, uint8_t data);
int insertFront(List* list, uint8_t data);
int insertBefore(ListItem* before, uint8_t data);
int insertBack(List* list, uint8_t data);

int removeAfter(ListItem* after);
int removeFront(List* list);
int removeBefore(ListItem* before);
int removeBack(List* list);
void clearList(List* list);

#endif
