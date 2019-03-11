#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "conio.h"

#define DATA_PATH "input.txt"
#define SIZE_OF_STRING_TO_READ 512
#define MIN_NUMBER_SYSTEM 2
#define MAX_NUMBER_SYSTEM 16
#define NULL_OPERATOR '\0'
#define INVALID_DIGIT -1

typedef enum Bool_ { FALSE, TRUE } bool_t;
typedef enum FuncResult_ { FAIL, SUCCESS } func_result_t;

typedef struct Number_
{
	char* asString;
	int stringLen;
	int numberSystem;
} number_t;

/*Линейный список ОПЗ*/
typedef struct OpzElValue_
{
	char sign;
	number_t* number;
} opz_list_el_value;

typedef struct OpzEl_
{
	opz_list_el_value* value;
	struct OpzEl_* next;
} opz_list_el;

/*структура стека символов, используемая при получении ОПЗ*/
typedef struct OperatorStackEl_
{
	char sign;
	struct OperatorStackEl_* next;
} operator_stack_el;

/*TODO: структура стека для подсчёта опз*/




int symbolToInt(char symbol)
{
	if ('0' <= symbol && symbol <= '9') return symbol - '0';
	if ('A' <= symbol && symbol <= 'F') return symbol - 'A' + 10;
	return INVALID_DIGIT;
}

bool_t isDigit (char symbol)
{
	if ('0' <= symbol && symbol <= '9') return TRUE;
	else return FALSE;
}

bool_t isHexDigit(char symbol)
{
	if ( isDigit(symbol) || ('A' <= symbol && symbol <= 'F') )  return TRUE;
	else return FALSE;
}

bool_t isValidNumberSystem(int num)
{
	if (MIN_NUMBER_SYSTEM <= num && num <= MAX_NUMBER_SYSTEM) return TRUE;
	else return FALSE;
}

bool_t isValidDigit(int numberSystem, char symbol)
{
	int digit = symbolToInt(symbol);
	if (digit == INVALID_DIGIT || digit >= numberSystem) return FALSE;
	return TRUE;
}

bool_t isOperator (char symbol)
{
	if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/') return TRUE;
	else return FALSE;
}

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

func_result_t popRestOfOperatorStackIntoOpz(opz_list_el** opz_head, operator_stack_el** operartor_head)
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

func_result_t getNumberSystem (char* curChar, int* numberSystem)
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

func_result_t handleNumber(opz_list_el** opzList_headPtr, char** curChar)
{
	number_t* number = (number_t*)malloc(sizeof(number_t));
	if (number == NULL) return FAIL;
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
	number->asString = (char*)calloc(number->stringLen, sizeof(char));
	if (number->asString == NULL)
	{
		free(number);
		return FAIL;
	}
	number->asString += number->stringLen;
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
	return SUCCESS;
}

func_result_t handleOperator(opz_list_el** opzList_headPtr, operator_stack_el** operatorStack_headPtr, char curChar)
{
	if (curChar == '+' || curChar == '-')
	{
		char lastOperator = getLastFromOperatorStack(*operatorStack_headPtr);
		while ( lastOperator == '*' || lastOperator == '/')
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
	return SUCCESS;
}

func_result_t handleOpeningBracket(operator_stack_el** operatorStack_headPtr, char curChar)
{
	if (pushIntoOperatorStack(operatorStack_headPtr, curChar) != SUCCESS)
	{
		printf("ERROR: operator stack overflow.\n");
		return FAIL;
	}
	return SUCCESS;
}

func_result_t handleClosingBracket(opz_list_el** opzList_headPtr, operator_stack_el** operatorStack_headPtr)
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
	return SUCCESS;
}

func_result_t handleOpzListValue(opz_list_el** opzList_headPtr, operator_stack_el** operatorStack_headPtr, char** curChar)
{
	if (*curChar == NULL) return FAIL;
	opz_list_el_value* elValue = (opz_list_el_value*)malloc(sizeof(opz_list_el_value));
	if (elValue == NULL) return FAIL;

	if (isHexDigit(**curChar) && 
		handleNumber(opzList_headPtr, curChar) != SUCCESS) 
		return FAIL;
	else if 
		(isOperator(**curChar) &&
		handleOperator(opzList_headPtr, operatorStack_headPtr, **curChar) != SUCCESS) 
		return FAIL;
	else if 
		(**curChar == '(' && 
		handleOpeningBracket(operatorStack_headPtr, **curChar) != SUCCESS) 
		return FAIL;
	else if 
		(**curChar == ')' &&
		handleClosingBracket(opzList_headPtr, operatorStack_headPtr) != SUCCESS) 
		return FAIL;
	return SUCCESS;
}

func_result_t handleDataFromString(char* curChar, opz_list_el** opzList_head, operator_stack_el** operartorStack_head)
{
	while (*curChar != '=')
	{
		if (handleOpzListValue(opzList_head, operartorStack_head, &curChar) != SUCCESS) //добавляет новые элементы в список ОПЗ
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

opz_list_el* getOpz () //парсит данные в ОПЗ, возвращает указатель на голову
{
	opz_list_el* opzList_head = NULL;
	operator_stack_el* operartorStack_head = NULL;
	char curChar = '\0';

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
		if (handleDataFromString(temp, &opzList_head, &operartorStack_head) != SUCCESS) 
			return NULL;
	free(temp);
	fclose(input);

	if (popRestOfOperatorStackIntoOpz(&opzList_head, &operartorStack_head) != SUCCESS) return NULL;
	//TODO: если в ф-ии произошла ошибка, очистить память на список и стек
	return opzList_head;
}

int main (void)
{
	opz_list_el* opzListHead = getOpz(); //получить опз
	//calculateOpz(opzListHead); //поссчитать выражение по опз
	_getch();
	return 0;
}