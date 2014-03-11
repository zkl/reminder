#include "file_api.h"

int file_readline(FILE * fp, char * buf, int size)
{
	char * sp = buf;

	int n;
	for(;sp - buf < size && !feof(fp);sp++)
	{
		n = fread(sp, sizeof(char), 1, fp);
		if(n <= 0)
			break;

		if(*sp == '\n')
			break;
	}

	*sp = 0;
	return sp-buf;
}

void file_monitor_init(struct file_monitor_t * fm, const char * file)
{
	strcpy(fm->file, file);
	stat(file, &fm->st);
}

void file_monitor_reset(struct file_monitor_t * fm)
{
	stat(fm->file, &fm->st);
}

int file_monitor_check(struct file_monitor_t * fm)
{
	struct stat st;
	stat(fm->file, &st);

	if(st.st_mtime != fm->st.st_mtime)
		return 1;
	
	return 0;
}