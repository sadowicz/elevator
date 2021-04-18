#include "list.h"

ListItem* createList() {
    return (ListItem*)calloc(1, sizeof(ListItem));
}

void deleteList(ListItem** list) {
    clearList(*list);
    free(*list);
    *list = NULL;
}

ListItem* front(ListItem* list) {
    return list->next;
}

int isEmpty(ListItem* list) {
    return !(list->next);
}

int insert(ListItem* after, uint8_t data) {
    ListItem* nextItem = (ListItem*)malloc(sizeof(ListItem));
    if(!nextItem)
        return 1;

    nextItem->data = data;
    nextItem->next = after->next;
    after->next = nextItem;

    return 0;
}

int insertFront(ListItem* list, uint8_t data) {
    return insert(list, data);
}

int remove(ListItem* after) {
    if(isEmpty(after))
        return 1;

    ListItem* removed = after->next;
    after->next = removed->next;
    free(removed);

    return 0;
}

int removeFront(ListItem* list) {
    return remove(list);
}

void clearList(ListItem* list) {
    while(!removeFront(list));
}