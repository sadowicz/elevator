#include "list.h"

List* createList() {
    List* list = (List*)malloc(sizeof(List));
    if(!list)
        return NULL;

    ListItem* head = (ListItem*)calloc(1, sizeof(ListItem));
    ListItem* tail = (ListItem*)calloc(1, sizeof(ListItem));

    if(!head || !tail)
        return NULL;

    head->next = tail;
    tail->prev = head;

    list->head = head;
    list->tail = tail;

    return list;
}

void deleteList(List** list) {
    clearList(*list);
    free((*list)->head);
    free((*list)->tail);
    free(*list);
    *list = NULL;
}

ListItem* front(List* list) {
    return list->head->next;
}

ListItem* back(List* list) {
    return list->tail->prev;
}

ListItem* find(List* list, uint8_t value) {
    ListItem* seeker = list->head;

    while(seeker->next != list->tail) {
        if(seeker->next->data == value)
            return seeker->next;

        seeker = seeker->next;
    }

    return NULL;
}

int isEmpty(List* list) {
    return (list->head == list->tail);
}

int insertAfter(ListItem* after, uint8_t data) {
    ListItem* nextItem = (ListItem*)malloc(sizeof(ListItem));
    if(!nextItem)
        return 1;

    nextItem->data = data;
    nextItem->next = after->next;
    nextItem->prev = after;

    after->next = nextItem;

    return 0;
}

int insertFront(List* list, uint8_t data) {
    return insertAfter(list->head, data);
}

int insertBefore(ListItem* before, uint8_t data) {
    ListItem* prevItem = (ListItem*)malloc(sizeof(ListItem));
    if(!prevItem)
        return 1;

    prevItem->data = data;
    prevItem->next = before;
    prevItem->prev = before->prev;

    before->prev = prevItem;

    return 0;
}

int insertBack(List* list, uint8_t data) {
    return insertBefore(list->tail, data);
}

int removeAfter(ListItem* after) {
    if(!after->next)
        return 1;

    ListItem* removed = after->next;
    after->next = removed->next;
    removed->next->prev = after;
    free(removed);

    return 0;
}

int removeFront(List* list) {
    return removeAfter(list->head);
}

int removeBefore(ListItem* before) {
    if(!before->prev)
        return 1;

    ListItem* removed = before->prev;
    before->prev = removed->prev;
    removed->prev->next =before;
    free(removed);

    return 0;
}

int removeBack(List* list) {
    return removeBefore(list->tail);
}

void clearList(List* list) {
    while(!removeFront(list));
}