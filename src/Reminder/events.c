#include <malloc.h>
#include <string.h>
#include "strapi.h"
#include "list.h"
#include "file_api.h"
#include "events.h"

struct event_em_t
{
	int  second;
	int  minute;
	int  hour;
	int  day;
	int  year;
	char mounth[12];
	char week[7];
	char cmd[1024];
};

char * event_em_analise(struct event_em_t * em, char * buf);
int event_em_compare(struct event_em_t * em, SYSTEMTIME * time);
int event_analise(event_t * event, char * buf);

int event_analise(event_t * event, char * buf)
{
	struct event_em_t * em;
	
	ltrimchr(buf, ' ');
	if(strlen(buf) == 0 || buf[0] == '#')
		return 0;

	em = (struct event_em_t *)malloc(sizeof(struct event_em_t));
	buf = event_em_analise(em, buf);
	
	if(buf != 0)
	{
		linked_list_insert(event->all_event, 0, em);
		
		if(strlen(buf) >= 1024) //  命令长度太长 
			return -1;
		else
			strcpy(em->cmd, buf);
	}
	else
	{
		free(em);
		return -1;
	}

	return 0;
}

char * event_em_analise(struct event_em_t * em, char * buf)
{
	int ret = 0;

	int year, month, day, hour, minute, index, find;
	const char * weekday[] = {"Sun", "Mon", "Tues", "Wed", "Thu", "Fir", "Sat"};
	const char * monthstr[]= {"Jan", "Feb", "Mar", "Apl", "May", "Jnu", "Jul",
		"Aug", "Sep", "Oct", "Nov", "Dec"};

	char * est;
	est = strstr(buf, "RUN");
	if(est == 0)
		return 0;

	*est = 0;
	ltrimchr(est+3, ' ');

	em->hour = -1;
	em->day  = -1;
	em->year = -1;
	em->minute = -1;
	em->second = 0;
	
	for(index=0; index < 12; index++)
		em->mounth[index] = 0;
	
	for(index=0; index < 7; index++)
		em->week[index] = 0;
	

	ret = sscanf(buf, "%d:%d", &hour, &minute);
	if(ret == 2)
	{
		em->hour = hour;
		em->minute = minute;
	}

	ret = sscanf(buf, "%d-%d-%d", &year, &month, &day);
	if(ret == 3 && month < 13 && day < 32)
	{
		em->year = year;
		em->mounth[month-1] = 1;
		em->day = day;
	}

	find = 0;
	for(index = 0;index<12; index++)
	{
		if(strstr(buf, monthstr[index]) != 0)
		{
			em->mounth[index] = 1;
			find = 1;
		}
	}

	if(!find)
	{
		for(index=0; index < 12; index++)
			em->mounth[index] = 1;
	}

	find = 0;
	for(index = 0;index<7; index++)
	{
		if(strstr(buf, weekday[index]) != 0)
			em->week[index] = 1;
	}
	if(!find)
	{
		for(index=0; index < 7; index++)
			em->week[index] = 1;
	}

	return est + 3;
}

int event_em_compare(struct event_em_t * em, SYSTEMTIME * time)
{
	if(em->year >= 0 && em->year != time->wYear)
		return 0;

	if(em->mounth[time->wMonth-1] != 1)
		return 0;

	if(em->week[time->wDayOfWeek] != 1)
		return 0;

	if(em->day >= 0 && em->day != time->wDay)
		return 0;

	if(em->hour >= 0 && em->hour != time->wHour)
		return 0;

	if(em->minute >= 0 && em->minute != time->wMinute)
		return 0;

	if(em->second >= 0 && em->second != time->wSecond)
		return 0;

	return 1;
}

void event_init(event_t * event)
{
	linked_list_create(&event->all_event);
	linked_list_create(&event->el.list);
}

void event_free(event_t * event)
{
	linked_list_node_t * node;
	for(node = linked_list_first(event->all_event); node;
		node = linked_list_next(node))
	{
		free(linked_list_data(node));
	}

	linked_list_free(event->all_event);
	linked_list_free(event->el.list);
}


int event_load(event_t * event, const char * file)
{
	FILE * fp = fopen(file, "r");
	char mbuf[2048];
	int  line_num = 0;

	if(fp == 0)
		return -1;

	line_num = 0;
	while(!feof(fp))
	{
		line_num++;
		file_readline(fp, mbuf, 2048);
		if(strlen(mbuf) == 0)
			continue;

		if(event_analise(event, mbuf) != 0)
			printf("配置错误 第 %3d 行 %s\n", line_num, mbuf);
	}
	fclose(fp);
	return 0;
}

event_list_t * event_check(event_t * event, SYSTEMTIME * time)
{
	linked_list_node_t * node = linked_list_first(event->all_event);

	linked_list_free(event->el.list);
	linked_list_create(&event->el.list);

	for(node; node; node = linked_list_next(node))
	{
		struct event_em_t * em = (struct event_em_t *)linked_list_data(node);
		
		if(event_em_compare(em, time))
			linked_list_insert(event->el.list, 0, em);
	}

	return &event->el;
}

const char * event_list_next(event_list_t * el)
{
	struct event_em_t * em;
	linked_list_node_t * node = linked_list_first(el->list);
	
	if(node == 0)
		return 0;

	em = (struct event_em_t *)linked_list_data(node);
	linked_list_remove(el->list, node);

	return em->cmd;
}