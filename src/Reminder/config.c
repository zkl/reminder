#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include "config.h"
#include "file_api.h"
#include "strapi.h"

int  config_anlize(config_t * config, char * buf);

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 解析配置语句
** 入  参： config   - 配置模块指针
			配置语句 - 网络模块指针

** 返回值： 成功返回0
** 备  注：
*******************************************************************************/
int config_anlize(config_t * config, char * buf)
{
	char * str_com = strchr(buf, '#');
	const char * str_end;
	
	if(str_com)
		*str_com = 0;

	ltrimchr(buf, ' ');
	ltrimchr(buf, '\t');

	if(strlen(buf) == 0)
		return 0;

	if(config->last == config->max)
	{
		config->max += 15;
		config->keys = (struct config_key_t *)realloc(config->keys,
			sizeof(struct config_key_t) * config->max);
	}

	str_end = getsubstr(config->keys[config->last].key, 128, buf, ' ');
	str_com = buf + (str_end - buf);

	ltrimchr(str_com, ' ');

	strcpy(config->keys[config->last].val, str_com);
	config->last++;

	return 0;
}

void config_init(config_t * config)
{
	config->last = 0;
	config->size = 0;
	config->max  = 15;

	config->keys = (struct config_key_t *)malloc(
		sizeof(struct config_key_t) * config->max);
}

void config_free(config_t * config)
{
	config->max = 0;
	free(config->keys);
}

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 载入配置界面
** 入  参： config   - 配置模块指针

** 返回值： 成功返回0
** 备  注：
*******************************************************************************/
int config_load(config_t * config, const char * file)
{
	FILE * fp = fopen(file, "r");
	char mbuf[2048];
	int line_num;
	
	if(fp == 0)
		return -1;

	line_num = 0;
	while(!feof(fp))
	{
		file_readline(fp, mbuf, 2048);
		if(strlen(mbuf) == 0)
			continue;

		if(config_anlize(config, mbuf) != 0)
			printf("配置错误 %d %s\n", line_num, mbuf);
	}
	fclose(fp);
	return 0;
}

const char * config_get_val(config_t * config, const char * key)
{
	int i = 0;
	for(i=0; i<config->last; i++)
	{
		if(stricmp(config->keys[i].key, key) == 0)
			return config->keys[i].val;
	}
	return 0;
}

int config_save(config_t * config, const char * file);
void config_set_val(config_t * config, const char * key, const char * val);