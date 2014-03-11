#ifndef REMINDER_H
#define REMINDER_H

#include "config.h"
#include "events.h"
#include "file_api.h"

typedef struct reminder_t
{
	config_t config;
	event_t  events;
	struct file_monitor_t cf;
	struct file_monitor_t ef;
}reminder_t;

void reminder_create(reminder_t * reminder);
void reminder_delete(reminder_t * reminder);
void reminder_load_config(reminder_t * reminder, const char * file);
void reminder_load_remind(reminder_t * reminder, const char * file);
void reminder_exec(reminder_t * reminder);

#endif