#ifndef EVENT_H
#define EVENT_H

#include <windows.h>
#include "list.h"

typedef struct 
{
	linked_list_t * list;
}event_list_t;

typedef struct 
{
	linked_list_t * all_event;
	event_list_t el;
}event_t;

void event_init(event_t * event);
void event_free(event_t * event);
int  event_load(event_t * event, const char * file);

event_list_t * event_check(event_t * event, SYSTEMTIME * time);
const char *   event_list_next(event_list_t * el);

#endif