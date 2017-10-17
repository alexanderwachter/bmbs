#include "list_operations.h"
#include <string.h>
#include <registration.h>
#include <xprintf.h>
#include <time.h>
#include <com.h>

//#define DEBUG
#ifdef DEBUG
  #define PRINT_LIST(head) print_list(head)
#else
  #define PRINT_LIST
#endif
#define NUM_LIST_ELEMENTS 50
#define NUM_ITERATIONS 100

typedef enum _comparison_ {ST=-1, MATCH=0, GT=1} COMPARISON;
typedef COMPARISON (*compare_function)(void*, void*);

List* list_get_prev(List* head, List* element)
{
    List* it = head;
    for(it=head; it; it = it->_next)
    {
        if(it->_next == element)
            return it;
    }
    return NULL;
}

COMPARISON list_compare(void* data_1, void* data_2)
{
  if(*(int*)data_1 < *(int*)data_2)
    return ST;
  if(*(int*)data_1 == *(int*)data_2)
    return MATCH;
  return GT;
}

int list_append(List* head, List* element)
{
    List* last = head;
    if(!head || !element)
    {
        return -1;
    }
    while(last->_next)
    {
        last = last->_next;
    }
    
    last->_next = element;
    element->_next = NULL;
    return 0;
}

int list_insert(List* after_this, List* element)
{
    if(!after_this || !element)
    {
        return -1;
    }
    
    element->_next = after_this->_next;
    after_this->_next = element;
    return 0;
}

int list_replace(List* head, List* element, List* replacement)
{
    List* prev;
    if(!element || ! replacement)
    {
        return -1;
    }
    
    prev = list_get_prev(head, element);
    if(prev)
    {
        prev->_next = replacement;
    }
    replacement->_next = element->_next;
   
    return 0;
}

int list_delete(List* head, List* element)
{
    List* prev;
    if(!element)
    {
        return -1;
    }
    prev = list_get_prev(head, element);
    if(prev)
    {
        prev->_next = element->_next;
    }
    element->_next = NULL;
    return 0;
}

void print_list(List* head)
{
  int i;
  List *element = head;
  for(i=0; element; i++, element = element->_next)
    xprintf("%d: %d\n", i, *(int*)element->_data);
}

void list_sort(List* head, compare_function cmp_fnk)
{
  List *old_list = head->_next->_next;
  List *new_list;
  List *tmp;
  //delete the old list;
  head->_next->_next = NULL;
  //create new sorted list
  while(old_list)
  {
    for(new_list = head;
        new_list->_next && cmp_fnk(new_list->_next->_data, old_list->_data) < MATCH;
        new_list = new_list->_next);
    tmp = old_list;
    old_list = old_list->_next;

    tmp->_next = new_list->_next;
    new_list->_next = tmp;
  }
}

void list_operations(void)
{
  int data[NUM_LIST_ELEMENTS],i, loop;
  time_t measurement;
  List  *element, *tmp;

  List head = {._data = NULL, ._next = NULL};
  List elements[NUM_LIST_ELEMENTS];

  for(i=0; i < NUM_LIST_ELEMENTS; i++)
  {
    data[i] = i+1;
    elements[i]._data = &data[i];
  }
    
    

  tick(&measurement);
  for(loop = 0 ; loop < NUM_ITERATIONS; loop++)
  {
    //Append all elements
    for(i=0; i < NUM_LIST_ELEMENTS/2; i++)
    {
      list_append(&head, &elements[i]);
      list_append(&head, &elements[NUM_LIST_ELEMENTS - 1 -i]);
    }
      
    PRINT_LIST(&head);
    list_sort(&head, list_compare);
    PRINT_LIST(&head);
    //delete the odd
    for(element=head._next; element;)
    {
      tmp = element->_next;
      if(*(int*)element->_data & 1)
        list_delete(&head, element);
      element = tmp;
    }
    PRINT_LIST(&head);
    //replace the even with the odd
    for(i=1; i < NUM_LIST_ELEMENTS; i+=2)
      list_replace(&head, &elements[i], &elements[i-1]);
    PRINT_LIST(&head);

    for(i=1, element=head._next; element; i+=2, element = element->_next)
    {
      //xprintf("element %d, data:%d\n", i, (int)element->_data);
      if(*(int*)element->_data != i)
      {
        xprintf("Verification FAILED!\n");
        return;
      }
    }

    for(element=&head, i=0; element; element=element->_next, i++);

    if(i-1!=NUM_LIST_ELEMENTS/2)
    {
      xprintf("Missing elements (%d in list). FAIL!\n", i);
      return;
    }
    //delete the list
    head._next = NULL;
  }
  put_result(0, tock(measurement));
  xprintf("DONE\n");
}

/*Register Benchmark:    name          , version major, version minor, entry function */
BMBS_REGISTER_BENCHMARK(ListOperations , 1            , 1            , list_operations)
