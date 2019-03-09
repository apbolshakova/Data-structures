#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"

typedef enum Bool_ { FALSE, TRUE } bool_t;

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
/*Линейный список ОПЗ*/

/*структура стека символов, используемая при получении ОПЗ*/
typedef struct OperatorStackEl_
{
	char sign;
	struct OperatorStackEl_* next;
} operator_stack_el;

/*TODO: структура стека для подсчёта опз*/

bool_t isDigit(char symbol)
{
	if ('0' <= symbol && symbol <= '9') return TRUE;
	else return FALSE;
}

bool_t ifOperator(char symbol)
{
	if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/') return TRUE;
	else return FALSE;
}

void handleOpzListValue(opz_list_el** opzList_headPtr, operator_stack_el** operatorStack_headPtr, char** curChar)
{
	if (curChar == NULL) return NULL;
	opz_list_el_value* elValue = (opz_list_el_value*)malloc(sizeof(opz_list_el_value));

	if      (isDigit(**curChar))    handleNumber(opzList_headPtr, operatorStack_headPtr, curChar);
	else if (isOperator(**curChar)) handleOperator(opzList_headPtr, operatorStack_headPtr);
	else if (**curChar == '(') handleOpeningBracket(opzList_headPtr, operatorStack_headPtr);
	else if (**curChar == ')') handleClosingBracket(opzList_headPtr, operatorStack_headPtr);
}

opz_list_el* getOpz() //читает входные данные, парсит их в ОПЗ и возвращает указатель на голову полученного списка
{
	opz_list_el* opzList_head = NULL;
	operator_stack_el* operartorStack_head = NULL;
	char* curChar = '\0';

	if (!scanf("%c", &curChar)) return opzList_head;
	while (curChar != '=')
	{
		handleOpzListValue(&opzList_head, &operartorStack_head, &curChar); //добавляет новые элементы в список ОПЗ в соот-вии с семантикой
		if (!scanf("%c", &curChar)) return NULL; //выражение без знака равно => ошибка. TODO: возвращать код ошибки??
		
	}
	//динамически читать строку, пока есть, при первом проходе формируем opz_list:
	/*
	открывающая скобка - скидываем в стек
	закрывающая скобка - вытаскиваем из стека всё до первой открывающей скобки включительно
	число обрабатываем и скидываем в экземпляр number_t
	символ - вытаскиваем все операции с большим приоритетом до первой открывающей скобки
	конец строки - вытаскиваем всё из стека символов
	*/
}

int main(void)
{
	printf("Введите выражение для подсчёта (должно заканчиваться знаком равенства): \n");
	opz_list_el* opzListHead = getOpz(); //получить опз
	calculateOpz(opzListHead); //поссчитать выражение по опз
	return 0;
}