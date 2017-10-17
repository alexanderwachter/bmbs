#ifndef _LIST_H_
#define _LIST_H_

typedef struct _list_
{
    void* _data;
    struct _list_* _next;
} List; 

void list_operations(void);

#endif