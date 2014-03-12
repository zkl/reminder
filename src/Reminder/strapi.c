#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strapi.h"


/***************************************************************
* 函数名称 : rtrimchr
* 作者     : zhang
* 描述     : 删除字符串尾部的某字符的串
* 参数     : char *str  要操作的串
             char cflag 要删除的字符
* 返回值   : 无
* 修改历史 : 
***************************************************************/
char * rtrimchr( char *str, char cflag )
{
	int i;
	int len;

	len = (int)strlen(str) - 1;

	for( i = len; i >= 0 && str[i] == cflag; i-- )
		str[i] = 0;

	return str;
}

/***************************************************************
* 函数名称 : ltrimchr
* 作者     : rdc-zhangz  
* 描述     : 删除字符串头部的某字符的串
* 参数     : char *str  要操作的串
             char cflag 要删除的字符
* 返回值   : 无
* 修改历史 : 
***************************************************************/
char * ltrimchr( char *str, char cflag )
{
	int i ;
	int len;
	
	len = (int)strlen( str );

	for( i = 0 ; i < len && str[i] == cflag; i++ );
	if( i > 0 )
		strcpy( str , str + i );
	
	return str;
}

int nums(const char * str)
{
	return strskpst(str, "0123456789") - str;
}

const char * strskpst(const char * str, const char * chars )
{
	while(*str)
	{
		if(strchr(chars, *str) == 0)
			break;

		str++;
	}
	return str;
}

const char * strstpst(const char * str, const char * chars )
{
	while(*str)
	{
		if(strchr(chars, *str) != 0)
			break;

		str++;
	}
	return str;
}


/***************************************************************
* 函数名称 : getsubstr
* 作者     : rdc-zhangz  
* 描述     : 根据分隔符和序号从指定字符串中取相应子串(源串不变)
* 参数     : char *sdest  - 取到的子串
*            int nsize    - 子串的空间大小
*            char *ssrc   - 源串
*            int ncnt     - 子串序号(从0开始)
*            char cflag   - 分隔符(不能为0x00)
* 返回值   : 结束为止
* 修改历史 : 
***************************************************************/
const char * getsubstr( char * sdest, int nsize, const char * ssrc, int ncnt, char cflag)
{
	char buf[2];

	const char *es = 0;
	const char *ps = 0;

	sprintf(buf, "%c", cflag);

	ps = strskpst(ssrc, buf);
	for(ncnt; ncnt > 0; ncnt--)
	{
		ps = strstpst(ps, buf);
		ps = strskpst(ps, buf);
	}

	es = strstpst(ps, buf);

	if(es - ps > nsize)
		return 0;

	if(es != ps)
		strncpy(sdest, ps, es-ps);

	sdest[es-ps] = 0;

	return es;
}
