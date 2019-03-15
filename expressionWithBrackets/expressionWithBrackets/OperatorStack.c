#include "Header.h"

char getLastFromOperatorStack(operator_stack_el* head)
{
	if (head == NULL) return NULL_OPERATOR;
	while (head->next) head = head->next;
	return head->sign;
}

func_result_t pushIntoOperatorStack(operator_stack_el** head, char value)
{
	operator_stack_el *tmp = (operator_stack_el*)malloc(sizeof(operator_stack_el));
	if (tmp == NULL) return FAIL;
	tmp->next = *head;
	tmp->sign = value;
	*head = tmp;
	return SUCCESS;
}

char popFromOperatorStack(operator_stack_el** head)
{
	operator_stack_el* out;
	char value;
	if (*head == NULL) return NULL_OPERATOR;
	out = *head;
	*head = (*head)->next;
	value = out->sign;
	free(out);
	return value;
}