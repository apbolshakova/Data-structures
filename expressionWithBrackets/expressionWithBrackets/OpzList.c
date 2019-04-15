#include "Header.h"

opz_list_el* getLastFromOpzList(opz_list_el* head)
{
	if (head == NULL) return NULL;
	while (head->next) head = head->next;
	return head;
}

func_result_t pushIntoOpzList(opz_list_el** head, char sign, number_t* number)
{
	opz_list_el* tmp = (opz_list_el*)malloc(sizeof(opz_list_el));
	if (tmp == NULL) return FAIL;

	tmp->value = (opz_list_el_value*)malloc(sizeof(opz_list_el_value));
	if (tmp->value == NULL)
	{
		free(tmp);
		tmp = NULL;
		return FAIL;
	}
	if (number != NULL)
	{
		tmp->value->number = number;
		tmp->value->sign = NULL_OPERATOR;
	}
	else if (sign != NULL_OPERATOR)
	{
		tmp->value->sign = sign;
		tmp->value->number = NULL;
	}
	else
	{
		free(tmp->value);
		tmp->value = NULL;
		free(tmp);
		tmp = NULL;
		return FAIL;
	}
	tmp->next = NULL;

	opz_list_el* last = getLastFromOpzList(*head);
	if (last == NULL) *head = tmp;
	else last->next = tmp;
	return SUCCESS;
}

func_result_t popRestOfOperatorStackIntoOpz(opz_list_el** opz_head,
	operator_stack_el** operartor_head)
{
	char symbol = popFromOperatorStack(operartor_head);
	while (symbol != NULL_OPERATOR)
	{
		if (symbol == '(')
		{
			printf("ERROR: number of opening and closing brackets is unbalanced.\n");
			return FAIL;
		}
		pushIntoOpzList(opz_head, symbol, NULL);
		symbol = popFromOperatorStack(operartor_head);
	}
	return SUCCESS;
}

opz_list_el_value* popFromOpzList(opz_list_el** head)
{
	opz_list_el* out;
	opz_list_el_value* value = NULL;
	if (*head == NULL) return value;
	out = *head;
	*head = (*head)->next;
	value = out->value;
	free(out);
	out = NULL;
	return value;
}

void deleteOpzList(opz_list_el** head)
{
	opz_list_el* pv = NULL;
	while (*head)
	{
		pv = *head;
		*head = (*head)->next;
		
		if (pv->value->number)
		{
			if (pv->value->number->asString)
			{
				free(pv->value->number->asString);
				pv->value->number->asString = NULL;
			}
			free(pv->value->number);
			pv->value->number = NULL;
		}
		free(pv);
		pv = NULL;
	}
}