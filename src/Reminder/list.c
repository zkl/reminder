#include <malloc.h>
#include <string.h>
#include "list.h"


/*******************************************************************************
** ��  ���� v 1.1     
** ��  �ܣ� ��������
** ��  �Σ� 
** ����ֵ��                                   
** ��  ע�� ��Ҫ���� linked_list_delete ɾ������
*******************************************************************************/
void linked_list_create(linked_list_t ** list)
{
	*list = (linked_list_t *)malloc(sizeof(linked_list_t));
	memset(*list, 0, sizeof(linked_list_t));
}

/* ɾ���б� */
void linked_list_free(linked_list_t * list)
{
	linked_list_node_t * node = list->head;
	while(list->head)
	{
		node = node->next;
		free(list->head);
		list->head = node;
	}
	free(list);
}

/* ��ȡ���� */
void * linked_list_data(linked_list_node_t * node)
{
	return node->d;
}

int linked_list_size(linked_list_t * list)
{
	return list->size;
}

/* ��һ��λ�� */
linked_list_node_t * linked_list_first(linked_list_t * list)
{
	return list->head;
}

/* ���һ��λ�� */
linked_list_node_t * linked_list_last (linked_list_t * list)
{
	return list->last;
}

/* ��һ��һ��λ�� */
linked_list_node_t * linked_list_next (linked_list_node_t * node)
{
	return node->next;
}

/* ��һ��һ��λ�� */
linked_list_node_t * linked_list_previous(linked_list_node_t * node)
{
	return node->prev;
}

/*******************************************************************************
** ��  ���� v 1.1     
** ��  �ܣ� ɾ��һ���ڵ�
** ��  �Σ�
** ����ֵ�� ������һ���ڵ�                      
** ��  ע��   
*******************************************************************************/
linked_list_node_t * linked_list_remove(linked_list_t * list, 
	linked_list_node_t * node)
{
	linked_list_node_t * next_node = node->next;

	if(node == list->last)
		list->last = node->prev;

	if(node == list->head)
		list->head = node->next;

	if(node->prev)
		node->prev->next = node->next;

	if(node->next)
		node->next->prev = node->prev;

	list->size--;

	free(node);

	return next_node;
}

/*******************************************************************************
** ��  ���� v 1.1     
** ��  �ܣ� ��������
** ��  �Σ�
** ����ֵ�� �����²���Ľڵ�                                
** ��  ע��   
*******************************************************************************/
linked_list_node_t * linked_list_insert(linked_list_t * list, 
	linked_list_node_t * node, void * data)
{
	linked_list_node_t * new_node = (linked_list_node_t *)malloc(
		sizeof(linked_list_node_t));

	new_node->d = data;
	new_node->prev = 0;
	new_node->next = 0;

	if(node == 0)
	{
		new_node->prev = 0;
		new_node->next = list->head;

		if(list->head)
		{	
			list->head->prev = new_node;
		}

		list->head = new_node;

		if(list->last == 0)
			list->last = new_node;
	}
	else
	{
		new_node->next = node->next;
		new_node->prev = node;

		node->next = new_node;

		if(new_node->next) // ������һ���ڵ�
			new_node->next->prev = new_node;
		else
			list->last = new_node;
	}

	list->size++;

	return new_node;
}