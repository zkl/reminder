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

const char * event_em_analise(struct event_em_t * em, const char * buf);
int  event_em_compare(struct event_em_t * em, SYSTEMTIME * ntime, SYSTEMTIME * otime);
long event_em_cmptime(SYSTEMTIME * ntime, SYSTEMTIME * otime);
int  event_analise(event_t * event, char * buf);

int event_analise(event_t * event, char * buf)
{
	struct event_em_t * em;
	const char * ps = strskpst(buf, "\t \r\n");
	if(strlen(ps) == 0 || ps[0] == '#')
		return 0;

	em = (struct event_em_t *)malloc(sizeof(struct event_em_t));
	ps = event_em_analise(em, ps);
	
	if(ps != 0)
	{
		linked_list_insert(event->all_event, 0, em);
		
		if(strlen(buf) >= 1024) //  命令长度太长 
			return -1;
		else
			strcpy(em->cmd, ps);
	}
	else
	{
		free(em);
		return -1;
	}

	return 0;
}

const char * event_em_analise(struct event_em_t * em, const char * buf)
{
	int ret = 0;
	int year, month, day, hour, minute, index, find;
	const char * weekday[] = {"Sun", "Mon", "Tues", "Wed", "Thu", "Fir", "Sat"};
	const char * monthstr[]= {"Jan", "Feb", "Mar", "Apl", "May", "Jnu", "Jul",
		"Aug", "Sep", "Oct", "Nov", "Dec"};
	
	char words[100];

	/* 初始化 */
	em->hour = -1;
	em->day  = -1;
	em->year = -1;
	em->minute = -1;
	em->second = 0;
	
	for(index=0; index < 12; index++)
		em->mounth[index] = 0;
	
	for(index=0; index < 7; index++)
		em->week[index] = 0;
	
	for(index=0; 1; index++)
	{
		buf = getsubstr(words, 100, buf, 0, ' ');
		if(buf == 0 || strlen(buf) == 0)
			return 0;

		if(stricmp(words, "RUN") == 0)
			break;

		ret = sscanf(words, "%d:%d", &hour, &minute);
		if(ret == 2)
		{
			em->hour = hour;
			em->minute = minute;
			continue;
		}

		ret = sscanf(words, "%d-%d-%d", &year, &month, &day);
		if(ret == 3 && month < 13 && day < 32)
		{
			em->year = year;
			em->mounth[month-1] = 1;
			em->day = day;
			continue;
		}

		ret = sscanf(words, "%d", &day);
		if(ret == 1 && strlen(words) <=2 && nums(words) == strlen(words))
		{
			if(em->day != -1)
			{
				printf("重复设置日期，使用最后一次设置(%d)为准\n", day);
			}
			em->day = day;
			continue;
		}

		ret = sscanf(words, "%d", &year);
		if(ret == 1 && strlen(words) <= 4 && nums(words) == strlen(words))
		{
			if(em->year != -1)
				printf("重复设置年份，使用最后一次设置(%d)为准\n", year);
			
			em->year = year;
			continue;
		}

		find = 0;
		for(index = 0;index<12; index++)
		{
			if(stricmp(words, monthstr[index]) == 0)
			{
				find = 1;
				em->mounth[index] = 1;
			}
		}

		if(find)
			continue;

		find = 0;
		for(index = 0;index<7; index++)
		{
			if(stricmp(words, weekday[index]) == 0)
			{
				find = 1;
				em->week[index] = 1;
			}
		}

		if(find)
			continue;

		/* 未知的时间格式 */
		return 0;
	}

	find = 0;
	for(index = 0;index<12; index++)
	{
		if(em->mounth[index] == 1)
		{
			find = 1;
			break;
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
		if(em->week[index] == 1)
		{
			find = 1;
			break;
		}
	}

	if(!find)
	{
		for(index=0; index < 7; index++)
			em->week[index] = 1;
	}

	return buf;
}

int event_em_compare(struct event_em_t * em, SYSTEMTIME * ntime, SYSTEMTIME * otime)
{
	int index, find;
	SYSTEMTIME tm;

	/* 比较月份是否存在 */
	find = -1;
	index = otime->wMonth-1;
	while(1)
	{
		if(em->mounth[index] == 1) // 找到最大的匹配月份
			find = index;

		if(index == ntime->wMonth-1)
			break;

		index = (index+1)%12;
	};

	if(find == -1)
		return 0;

	tm.wMonth = find+1;

	/* 比较星期是否存在 */
	find = -1;
	index = otime->wDayOfWeek;
	while(1)
	{
		if(em->week[index] == 1)
			find = index;

		if(index == ntime->wDayOfWeek)
			break;

		index = (index+1)%7;
	};

	if(find == -1)
		return 0;

	tm.wDayOfWeek = find;
	tm.wMilliseconds = 0;
	
	tm.wYear = em->year > -1 ? em->year : ntime->wYear;
	tm.wDay  = em->day  > -1 ? em->day  : ntime->wDay;
	tm.wHour = em->hour > -1 ? em->hour : ntime->wHour;
	tm.wMinute = em->minute > -1 ? em->minute : ntime->wMinute;
	tm.wSecond = em->second > -1 ? em->second : ntime->wSecond;

	if(event_em_cmptime(ntime, &tm) >= 0 && event_em_cmptime(otime, &tm) < 0)
		return 1;
	else
		return 0;
}

// 如果大于0说明ntime > otime 等于0 ntime == otime 小于0 ntime < otime
long event_em_cmptime(SYSTEMTIME * ntime, SYSTEMTIME * otime)
{
	long diff = 0;
	diff = ntime->wYear-otime->wYear;
	diff += ntime->wMonth+diff*12-otime->wMonth;
	diff += ntime->wDay+diff*31-otime->wDay; 
	diff += ntime->wHour+diff*24-otime->wHour;
	diff += ntime->wHour+diff*24-otime->wHour;
	diff += ntime->wMinute+diff*60-otime->wMinute;
	diff += ntime->wSecond+diff*60-otime->wSecond;
	return diff;
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
	FILE * fp = fopen(file, "rb");
	char mbuf[2048];
	int  line_num = 0;

	if(fp == 0)
		return -1;

	line_num = 0;
	while(!feof(fp))
	{
		line_num++;
		file_readline(fp, mbuf, 2048);
		if(event_analise(event, mbuf) != 0)
			printf("配置错误 第 %3d 行 %s\n", line_num, mbuf);
	}
	fclose(fp);
	return 0;
}

event_list_t * event_check(event_t * event, SYSTEMTIME * ntime, SYSTEMTIME * otime)
{
	linked_list_node_t * node = linked_list_first(event->all_event);

	linked_list_free(event->el.list);
	linked_list_create(&event->el.list);

	for(node; node; node = linked_list_next(node))
	{
		struct event_em_t * em = (struct event_em_t *)linked_list_data(node);
		
		if(event_em_compare(em, ntime, otime))
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