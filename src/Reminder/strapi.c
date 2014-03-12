#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strapi.h"


/***************************************************************
* �������� : rtrimchr
* ����     : zhang
* ����     : ɾ���ַ���β����ĳ�ַ��Ĵ�
* ����     : char *str  Ҫ�����Ĵ�
             char cflag Ҫɾ�����ַ�
* ����ֵ   : ��
* �޸���ʷ : 
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
* �������� : ltrimchr
* ����     : rdc-zhangz  
* ����     : ɾ���ַ���ͷ����ĳ�ַ��Ĵ�
* ����     : char *str  Ҫ�����Ĵ�
             char cflag Ҫɾ�����ַ�
* ����ֵ   : ��
* �޸���ʷ : 
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
* �������� : getsubstr
* ����     : rdc-zhangz  
* ����     : ���ݷָ�������Ŵ�ָ���ַ�����ȡ��Ӧ�Ӵ�(Դ������)
* ����     : char *sdest  - ȡ�����Ӵ�
*            int nsize    - �Ӵ��Ŀռ��С
*            char *ssrc   - Դ��
*            int ncnt     - �Ӵ����(��0��ʼ)
*            char cflag   - �ָ���(����Ϊ0x00)
* ����ֵ   : ����Ϊֹ
* �޸���ʷ : 
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
