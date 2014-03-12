#ifndef STR_API_H
#define STR_API_H

/* 删除字符串尾部的某些字符 */
char * rtrimchr( char * str, char cflag );
char * ltrimchr( char * str, char cflag );

/* 判断一个字符串开头数字的数量 */
int nums(const char * str);

/* 跳过开头的字符集 */
const char * strskpst(const char * str, const char * chars );

/* 跳过开头的非字符集 */
const char * strstpst(const char * str, const char * chars );

const char * getsubstr( char * sdest, int nsize, const char * ssrc, int ncnt, char cflag);

#endif