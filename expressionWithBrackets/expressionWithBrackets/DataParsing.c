#include "Header.h"

opz_list_el* getOpz()
{
	opz_list_el* opzList_head = NULL;
	operator_stack_el* operartorStack_head = NULL;
	char curChar = '\0';
	bool_t prevIsNum = FALSE;

	FILE *input = fopen(DATA_PATH, "r");
	if (input == NULL) return NULL;
	if (fgetc(input) == EOF || feof(input))
	{
		fclose(input);
		return NULL;
	}
	fclose(input);
	input = fopen(DATA_PATH, "r");
	char* temp = (char*)calloc(SIZE_OF_STRING_TO_READ, sizeof(char));
	while (fgets(temp, SIZE_OF_STRING_TO_READ * sizeof(char), input) != NULL)
		if (handleDataFromString(temp, &opzList_head, &operartorStack_head, &prevIsNum) != SUCCESS)
		{
			deleteOperatorStack(&operartorStack_head);
			return NULL;
		}
	free(temp);
	fclose(input);

	if (popRestOfOperatorStackIntoOpz(&opzList_head, &operartorStack_head) != SUCCESS)
	{
		deleteOperatorStack(&operartorStack_head);
		return NULL;
	}
	deleteOperatorStack(&operartorStack_head);
	return opzList_head;
}

func_result_t handleDataFromString(char* curChar, opz_list_el** opzList_head,
	operator_stack_el** operartorStack_head, bool_t* prevIsNum)
{
	while (*curChar != '=')
	{
		if (handleOpzListValue(opzList_head, operartorStack_head, &curChar, prevIsNum) != SUCCESS)
		{
			printf("ERROR: impossible to build correct RPN.\n");
			return FAIL;
		}
		if (*(++curChar) == '\0')
		{
			printf("ERROR: '=' sign not found.\n");
			return FAIL;
		}
	}
	return SUCCESS;
}

func_result_t handleOpzListValue(opz_list_el** opzList_headPtr,
	operator_stack_el** operatorStack_headPtr, char** curChar, bool_t* prevIsNum)
{
	if (*curChar == NULL) return FAIL;
	opz_list_el_value* elValue = (opz_list_el_value*)malloc(sizeof(opz_list_el_value));
	if (elValue == NULL) return FAIL;

	if (isHexDigit(**curChar) &&
		handleNumber(opzList_headPtr, curChar, prevIsNum) != SUCCESS)
		return FAIL;
	else if
		(isOperator(**curChar) &&
			handleOperator(opzList_headPtr, operatorStack_headPtr, **curChar, prevIsNum) != SUCCESS)
		return FAIL;
	else if
		(**curChar == '(' &&
			handleOpeningBracket(operatorStack_headPtr, **curChar, prevIsNum) != SUCCESS)
		return FAIL;
	else if
		(**curChar == ')' &&
			handleClosingBracket(opzList_headPtr, operatorStack_headPtr, prevIsNum) != SUCCESS)
		return FAIL;

	return SUCCESS;
}

func_result_t handleNumber(opz_list_el** opzList_headPtr, char** curChar, bool_t* prevIsNum)
{
	number_t* number = (number_t*)malloc(sizeof(number_t));
	if (number == NULL) return FAIL;
	number->sign = POSITIVE;
	number->asString = '\0';
	number->numberSystem = 0;
	number->stringLen = 0;

	char* sav = *curChar;
	while (**curChar != '_' && isHexDigit(**curChar))
	{
		number->stringLen++;
		(*curChar)++;
	}
	if (**curChar != '_' || (getNumberSystem(*curChar, &(number->numberSystem)) != SUCCESS))
	{
		printf("ERROR: found number with invalid or not mentioned number system.\n");
		return FAIL;
	}

	*curChar = sav;
	number->asString = (char*)calloc(number->stringLen + 1, sizeof(char));
	if (number->asString == NULL)
	{
		free(number);
		return FAIL;
	}
	number->asString += number->stringLen + 1;
	*(number->asString) = '\0';
	number->asString--;
	while (**curChar != '_')
	{
		if (!isValidDigit(number->numberSystem, **curChar))
		{
			number->asString = NULL;
			free(number->asString);
			number = NULL;
			free(number);
			printf("ERROR: found incorrect number (digits aren't fit number system.)\n");
			return FAIL;
		}
		*(number->asString) = **curChar;
		(*curChar)++;
		number->asString--; //число в памяти с конца
	}
	number->asString++;

	(*curChar)++;
	if (number->numberSystem >= 10) (*curChar)++;

	pushIntoOpzList(opzList_headPtr, NULL_OPERATOR, number);
	*prevIsNum = TRUE;
	return SUCCESS;
}

func_result_t getNumberSystem(char* curChar, int* numberSystem)
{
	if (*curChar == '_') curChar++;
	else return FAIL;

	if (isDigit(*curChar)) *numberSystem = *curChar - '0';
	if (isDigit(*(++curChar)))
	{
		*numberSystem = (*numberSystem) * 10 + *curChar - '0';
		if (isDigit(*(++curChar)) || *numberSystem > MAX_NUMBER_SYSTEM) return FAIL;

	}

	if (!isValidNumberSystem(*numberSystem)) return FAIL;
	return SUCCESS;
}

func_result_t handleOperator(opz_list_el** opzList_headPtr,
	operator_stack_el** operatorStack_headPtr, char curChar, bool_t* prevIsNum)
{
	if (!(*prevIsNum))
	{
		printf("ERROR: Invalid RPN.\n");
		return FAIL;
	}
	if (curChar == '+' || curChar == '-')
	{
		char lastOperator = getLastFromOperatorStack(*operatorStack_headPtr);
		while (lastOperator && lastOperator != '(')
		{
			lastOperator = popFromOperatorStack(operatorStack_headPtr);
			if (pushIntoOpzList(opzList_headPtr, lastOperator, NULL) != SUCCESS)
			{
				printf("ERROR: RPN list stack overflow.\n");
				return FAIL;
			}
			lastOperator = getLastFromOperatorStack(*operatorStack_headPtr);
		}
	}
	else if (curChar == '*' || curChar == '/')
	{
		char lastOperator = getLastFromOperatorStack(*operatorStack_headPtr);
		while (lastOperator == '*' || lastOperator == '/')
		{
			lastOperator = popFromOperatorStack(operatorStack_headPtr);
			if (pushIntoOpzList(opzList_headPtr, lastOperator, NULL) != SUCCESS)
			{
				printf("ERROR: RPN list stack overflow.\n");
				return FAIL;
			}
			lastOperator = getLastFromOperatorStack(*operatorStack_headPtr);
		}
	}

	if (pushIntoOperatorStack(operatorStack_headPtr, curChar) != SUCCESS)
	{
		printf("ERROR: operator stack overflow.\n");
		return FAIL;
	}
	*prevIsNum = FALSE;
	return SUCCESS;
}

func_result_t handleOpeningBracket(operator_stack_el** operatorStack_headPtr, char curChar, bool_t* prevIsNum)
{
	if (pushIntoOperatorStack(operatorStack_headPtr, curChar) != SUCCESS)
	{
		printf("ERROR: operator stack overflow.\n");
		return FAIL;
	}
	*prevIsNum = FALSE;
	return SUCCESS;
}

func_result_t handleClosingBracket(opz_list_el** opzList_headPtr,
	operator_stack_el** operatorStack_headPtr, bool_t* prevIsNum)
{
	char lastOperator = NULL_OPERATOR;
	do
	{
		lastOperator = popFromOperatorStack(operatorStack_headPtr);
		if (lastOperator == NULL_OPERATOR)
		{
			printf("ERROR: number of opening and closing brackets is unbalanced.\n");
			return FAIL;
		}
		if (lastOperator != '(')
		{
			if (pushIntoOpzList(opzList_headPtr, lastOperator, NULL) != SUCCESS)
			{
				printf("ERROR: RPN list stack overflow.\n");
				return FAIL;
			}
		}
	} while (lastOperator != '(');
	*prevIsNum = TRUE;
	return SUCCESS;
}
