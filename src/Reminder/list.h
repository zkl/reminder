/*******************************************************************************
**                                                  
** ��    ��:  v 1.3.0                                                  
** ��    ��:  ������������ݽṹ
** ��    ��:  Sat. 3 Mar 2012  �޸��� Wed. 26 Feb 2014 
** ��    Ȩ:  
** ��    ��:  ������ 
** ��    ע:                                              
*******************************************************************************/

#ifndef LIST_H
#define LIST_H

typedef void * DATA;

typedef struct _linked_list_node_t_
{
	DATA d;
	struct _linked_list_node_t_ * prev;
	struct _linked_list_node_t_ * next;
}LIST_NODE, * LNDE, linked_list_node_t;

typedef struct _linked_list_
{
	int size;
	struct _linked_list_node_t_ * head;
	struct _linked_list_node_t_ * last;
}LIST, linked_list_t;


/*******************************************************************************
**                      Wed. 26 Feb 2014  ����                                */

/* �����б� */
void   linked_list_create(linked_list_t ** list);
void   linked_list_free(linked_list_t * list);
int    linked_list_size(linked_list_t * list);

/* ��ȡ���� */
void * linked_list_data(linked_list_node_t * node);

/* ��һ��λ�� */
linked_list_node_t * linked_list_first(linked_list_t * list);

/* ���һ��λ�� */
linked_list_node_t * linked_list_last (linked_list_t * list);

/* ��һ��һ��λ�� */
linked_list_node_t * linked_list_next (linked_list_node_t * node);

/* ��һ��һ��λ�� */
linked_list_node_t * linked_list_previous(linked_list_node_t * node);

/* ɾ���ڵ� */
linked_list_node_t * linked_list_remove(linked_list_t * list, 
	linked_list_node_t * node);

/* ����ڵ� */
linked_list_node_t * linked_list_insert(linked_list_t * list, linked_list_node_t * node, 
	void * data);

/******************************************************************************/

#endif