#include "Header.h"

number_t* calculateOpz(opz_list_el* opzList_head)
{
	number_t* result = (number_t*)malloc(sizeof(number_t));
	number_stack_el* numberStack_head = NULL;
    if (opzList_head == NULL || opzList_head->value->sign)
	{
		free(result);
		return NULL;
	}
	
	opz_list_el_value* data = popFromOpzList(&opzList_head);
	while (data != NULL)
	{
		if (data->number != NULL)
		{
			if (convertToDec(&(data->number)) != SUCCESS) return NULL;
			pushIntoNumberStack(numberStack_head, data->number);
		}
		else if (data->sign != NULL_OPERATOR)
		{
			if (handleOperation(numberStack_head, data->sign) != SUCCESS) return NULL;
		}
	}

	if (numberStack_head != NULL && numberStack_head->next == NULL)
	{
		return popFromNumberStack(numberStack_head);
	}
	else
	{
		printf("ERROR: Unable to calculate RPN because of invalid RPN\n");
		return NULL;
	}
}

func_result_t* convertToDec(number_t** number)
{
	number_t* converted = (number_t*)malloc(sizeof(number_t));
	if (converted == NULL) return FAIL;
	if (number == NULL || (*number)->numberSystem == 10) return SUCCESS;

	converted->asString = '\0';
	converted->numberSystem = CALC_NUMBER_SYSTEM;
	long digitPos = 0; //разряд числа
	while ((*number)->asString)
	{
		long digit = (long)symbolToInt(*((*number)->asString)); //цифра в исходной СС
		long value = digit * pow((double)(*number)->numberSystem, digitPos);
		converted->asString = sum(converted->asString, longToString(value));
	}

	free((*number)->asString);
	free(*number);
	number = &converted;
	return SUCCESS;
}