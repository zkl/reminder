#ifndef STR_API_H
#define STR_API_H

/* ɾ���ַ���β����ĳЩ�ַ� */
char * rtrimchr( char * str, char cflag );
char * ltrimchr( char * str, char cflag );

/* �ж�һ���ַ�����ͷ���ֵ����� */
int nums(const char * str);

/* ������ͷ���ַ��� */
const char * strskpst(const char * str, const char * chars );

/* ������ͷ�ķ��ַ��� */
const char * strstpst(const char * str, const char * chars );

const char * getsubstr( char * sdest, int nsize, const char * ssrc, int ncnt, char cflag);

#endif