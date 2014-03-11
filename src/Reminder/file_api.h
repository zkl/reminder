#ifndef FILE_API_H
#define FILE_API_H
#include <stdio.h>
#include <windows.h>
#include <sys/stat.h>

int file_readline(FILE * fp, char * buf, int size);

struct file_monitor_t 
{
	char file[MAX_PATH];
	struct stat st;
};

void file_monitor_init(struct file_monitor_t * fm, const char * file);
void file_monitor_reset(struct file_monitor_t * fm);
int file_monitor_check(struct file_monitor_t * fm);

#endif