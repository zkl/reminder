#include <stdio.h>
#include "reminder.h"

void reminder_runcmd(const char * cmd)
{
	char buf[2048];
	sprintf(buf, "start %s", cmd);
	system(buf);
}

reminder_t * reminder_create()
{
	reminder_t * reminder = (reminder_t *)malloc(sizeof(reminder_t));
	config_init(&reminder->config);
	event_init(&reminder->events);
	return reminder;
}

void reminder_delete(reminder_t * reminder)
{
	config_free(&reminder->config);
	event_free(&reminder->events);
	free(reminder);
}

void reminder_load_config(reminder_t * reminder, const char * file)
{
	file_monitor_init(&reminder->cf, file);

	config_free(&reminder->config);
	config_init(&reminder->config);
	config_load(&reminder->config, file);
}

void reminder_load_remind(reminder_t * reminder, const char * file)
{
	file_monitor_init(&reminder->ef, file);
	
	event_free(&reminder->events);
	event_init(&reminder->events);
	event_load(&reminder->events, file);
}

void reminder_exec(reminder_t * reminder)
{
	SYSTEMTIME ntm;
	SYSTEMTIME otm;
	event_list_t * el;
	const char * cmd;
	
	GetLocalTime(&otm);
	while(1)
	{
		GetLocalTime(&ntm);
		el = event_check(&reminder->events, &ntm, &otm);
		
		do
		{
			cmd = event_list_next(el);
			if(cmd == 0 )
				break;

			reminder_runcmd(cmd);
		}while(1);

		if(file_monitor_check(&reminder->ef))
			reminder_load_remind(reminder, reminder->ef.file);

		if(file_monitor_check(&reminder->cf))
			reminder_load_config(reminder, reminder->cf.file);

		Sleep(1000);
		otm = ntm;
	}
}
