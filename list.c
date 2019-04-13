#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"
#include <assert.h>

typedef struct Node_t* Node;
struct Node_t {
	ListElement data;
	Node next;
};

struct List_t {
	Node head;
	int size;
	Node iterator;
	CopyListElement copy_func;
	FreeListElement free_func;
};

static Node nodeCreate(Node next, ListElement data, CopyListElement copy_func);
static void NodeDestroy(Node node, FreeListElement free_func);
//------------------------------------------------------------------//

static Node nodeCreate(Node next, ListElement data, CopyListElement copy_func) {

	Node new_node = malloc(sizeof(*new_node));
	if (new_node == NULL || data == NULL) {
		return NULL;
	}

	ListElement data_copy = copy_func(data);

	if ( data_copy == NULL) {

		free(new_node);
		return NULL;
	}

	new_node->data = data_copy;
	new_node->next = next;
	return new_node;

}

static void NodeDestroy(Node node, FreeListElement free_func) {

	free_func(node->data);
	free(node);

}


//--------------------------------------------------//
List listCreate(CopyListElement copyElement, FreeListElement freeElement) {

	if (copyElement == NULL || freeElement == NULL) {
		return NULL;
	}

	List new_list = malloc(sizeof(*new_list));
	if (new_list == NULL) {
		return NULL;
	}
	Node dummy = malloc(sizeof(*dummy));
	if (dummy == NULL) {
		free(new_list);
		return NULL;
	}
	dummy->data = NULL;
	dummy->next = NULL;
	new_list->head = dummy;
	new_list->size = 0;
	new_list->iterator = NULL;
	new_list->copy_func = copyElement;
	new_list->free_func = freeElement;

	return new_list;
}

void listDestroy(List list) {

	if (list == NULL || list->free_func == NULL) {
		return;
	}

	listClear(list);
	NodeDestroy(list->head, list->free_func);

	free(list);

}

List listCopy(List list) {
	if (list == NULL) {
		return NULL;
	}

	List new_list = listCreate(list->copy_func, list->free_func);
	if (!new_list) {
		return NULL;
	}

	Node to_copy = list->head->next, previous_node = new_list->head, new_node;
	while (to_copy) {

		new_node = nodeCreate(NULL, to_copy->data, list->copy_func);
		if (new_node == NULL) {
			listDestroy(new_list);
			return NULL;
		}
		previous_node->next = new_node;	//update the "next" in the previous node

		if (list->iterator == to_copy) {
			new_list->iterator = new_node;
		}

		previous_node = new_node;
		to_copy = to_copy->next;

	}
	new_list->size = list->size;
	return new_list;
}


int listGetSize(List list) {

	if (list == NULL) {
		return -1;
	}

	return list->size;

}

ListElement listGetFirst(List list) {

	if (list == NULL) {
		return NULL;
	}

	if (list->size == 0) {
		list->iterator = NULL;
		return NULL;
	}

	list->iterator = list->head->next;
	return list->iterator->data;

}

ListElement listGetNext(List list) {

	if (list == NULL || list->iterator == NULL) {
		return NULL;
	}

	list->iterator = list->iterator->next;
	if (list->iterator == NULL) {
		return NULL;
	}

	return list->iterator->data;
}

ListElement listGetCurrent(List list) {

	if (list == NULL || list->iterator == NULL) {
		return NULL;
	}

	return list->iterator->data;
}

ListResult listInsertLast(List list, ListElement element) {

	if (list == NULL) {
		return LIST_NULL_ARGUMENT;
	}

	Node new_node = nodeCreate(NULL, element, list->copy_func);
	if (new_node == NULL) {
		return LIST_OUT_OF_MEMORY;
	}
	Node tmp = list->head;
	while (tmp->next) {
		tmp = tmp->next;
	}

	list->size++;
	tmp->next = new_node;

	return LIST_SUCCESS;
}

ListResult listRemoveCurrent(List list) {

	if (list == NULL) {
		return LIST_NULL_ARGUMENT;
	}
	if (list->iterator == NULL) {
		return LIST_INVALID_CURRENT;
	}

	Node temp = list->head;
	while (temp->next != list->iterator) {
		temp = temp->next;
	}

	temp->next = list->iterator->next;
	NodeDestroy(list->iterator, list->free_func);
	list->iterator = NULL;
	list->size--;
	return LIST_SUCCESS;
}

ListResult listClear(List list) {

	if (list == NULL) {
		return LIST_NULL_ARGUMENT;
	}

	while (list->head->next) {
		Node to_delete = list->head->next;
		list->head->next = list->head->next->next;
		NodeDestroy(to_delete, list->free_func);
	}
	list->iterator = NULL;
	list->size = 0;

	return LIST_SUCCESS;
}

void* string_copy(void* str){
	char* str_to =  malloc(strlen(str)+1);
	if(!str_to) exit(-1);
	strcpy(str_to,(char*)str);
	str_to[strlen((char*)str)]='\0';
	return str_to;
}
void string_free(void* str){
	free(str);
}
