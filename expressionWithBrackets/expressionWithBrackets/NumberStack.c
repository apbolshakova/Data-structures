#include "Header.h"

func_result_t pushIntoNumberStack(number_stack_el** head, number_t* number)
{
	number_stack_el *tmp = (number_stack_el*)malloc(sizeof(number_stack_el));
	if (tmp == NULL) return FAIL;
	tmp->next = *head;
	tmp->data = number;
	*head = tmp;
	return SUCCESS;
}

number_t* popFromNumberStack(number_stack_el** head)
{
	number_stack_el* out;
	number_t* data = NULL;
	if (*head == NULL) return data;
	out = *head;
	*head = (*head)->next;
	data = out->data;
	free(out);
	return data;
}