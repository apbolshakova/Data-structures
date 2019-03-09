#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"

typedef enum Bool_ { FALSE, TRUE } bool_t;

typedef enum FuncResult_ { FAIL, SUCCESS } func_result_t;

typedef struct Number_
{
	char* asString;
	int stringLen;
	int numberSystem;
} number_t;

/*Линейный список ОПЗ*/
typedef union OpzElValue_
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

bool_t isDigit (char symbol)
{
	if ('0' <= symbol && symbol <= '9') return TRUE;
	else return FALSE;
}

bool_t ifOperator (char symbol)
{
	if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/') return TRUE;
	else return FALSE;
}

func_result_t handleNumber(opz_list_el** opzList_headPtr, char** curChar)
{
	//распарсить число в структуру и добавить в список ОПЗ
}

func_result_t handleOparator(opz_list_el** opzList_headPtr, operator_stack_el** operatorStack_headPtr, char** curChar)
{
	//вытащить из стека все операции с большим приоритетом до первой открывающей скобки не включительно
	//пишем полученный оператор в стек
}

func_result_t handleOpeningBracket(operator_stack_el** operatorStack_headPtr)
{
	//положить скобку в стек
}

func_result_t handleClosingBracket(opz_list_el** opzList_headPtr, operator_stack_el** operatorStack_headPtr)
{
	//вытащить из стека в ОПЗ все операции до открывающей скобки включительно
}

func_result_t handleOpzListValue(opz_list_el** opzList_headPtr, operator_stack_el** operatorStack_headPtr, char** curChar)
{
	if (curChar == NULL) return NULL;
	opz_list_el_value* elValue = (opz_list_el_value*)malloc(sizeof(opz_list_el_value));

	if (isDigit(**curChar))
	{
		if (handleNumber(opzList_headPtr, curChar) != SUCCESS)
		{
			printf("ERROR: found incorrect number.\n");
			return FAIL;
		};
	}
	else if (isOperator(**curChar))
	{
		if (handleOperator(opzList_headPtr, operatorStack_headPtr, curChar) != SUCCESS) //TODO: узнать, почему может быть ошибка
		{
			printf("ERROR: problem with operators stack.\n");
			return FAIL;
		}
	}
	else if (**curChar == '(')
	{
		if (handleOpeningBracket(operatorStack_headPtr) != SUCCESS)
		{
			printf("ERROR: problem with operators stack.\n");
			return FAIL;
		}
	}
	else if (**curChar == ')')
	{
		if (handleClosingBracket(opzList_headPtr, operatorStack_headPtr) != SUCCESS)
		{
			printf("ERROR: number of opening and closing brackets is unbalanced.\n");
			return FAIL;
		}
	}
	return SUCCESS;
}

opz_list_el* getOpz () //читает входные данные, парсит их в ОПЗ и возвращает указатель на голову полученного списка (вернёт NULL в случае ошибки)
{
	opz_list_el* opzList_head = NULL;
	operator_stack_el* operartorStack_head = NULL;
	char* curChar = '\0';

	if (!scanf("%c", &curChar)) return opzList_head;
	while (curChar != '=')
	{
		if (handleOpzListValue(&opzList_head, &operartorStack_head, &curChar) != SUCCESS) //добавляет новые элементы в список ОПЗ в соот-вии с семантикой
		{
			printf("ERROR: impossible to build correct RPN.\n");
			return NULL;
		}
		if (!scanf("%c", &curChar))
		{
			printf("ERROR: '=' sign not found.\n");
			return NULL;
		}
		
	}
	if (popRestOfOperatorStack(&opzList_head, &operartorStack_head) != SUCCESS) return NULL; //вытащить всё, что осталось в стеке операторов

	return opzList_head;
}

int main (void)
{
	printf("Enter expression to calculate (it must end with '='):\n");
	opz_list_el* opzListHead = getOpz(); //получить опз
	calculateOpz(opzListHead); //поссчитать выражение по опз
	return 0;
}