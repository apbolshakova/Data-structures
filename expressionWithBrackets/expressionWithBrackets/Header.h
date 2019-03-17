#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "conio.h"

#include "Constants.h"
#include "Types.h"

int symbolToInt(char symbol);

bool_t isDigit(char symbol);
bool_t isHexDigit(char symbol);
bool_t isValidNumberSystem(int num);
bool_t isValidDigit(int numberSystem, char symbol);
bool_t isOperator(char symbol);

char getLastFromOperatorStack(operator_stack_el* head);
func_result_t pushIntoOperatorStack(operator_stack_el** head, char value);
char popFromOperatorStack(operator_stack_el** head);

opz_list_el* getLastFromOpzList(opz_list_el* head);
func_result_t pushIntoOpzList(opz_list_el** head, char sign, number_t* number);
func_result_t popRestOfOperatorStackIntoOpz(opz_list_el** opz_head,
	operator_stack_el** operartor_head);
opz_list_el_value* popFromOpzList(opz_list_el** head);

func_result_t pushIntoNumberStack(number_stack_el** head, number_t* number);
number_t* popFromNumberStack(number_stack_el** head);

func_result_t getNumberSystem(char* curChar, int* numberSystem);
func_result_t handleNumber(opz_list_el** opzList_headPtr, char** curChar);
func_result_t handleOperator(opz_list_el** opzList_headPtr,
	operator_stack_el** operatorStack_headPtr, char curChar);
func_result_t handleOpeningBracket(operator_stack_el** operatorStack_headPtr, char curChar);
func_result_t handleClosingBracket(opz_list_el** opzList_headPtr,
	operator_stack_el** operatorStack_headPtr);

func_result_t handleOpzListValue(opz_list_el** opzList_headPtr,  //добавляет новые элементы в список ОПЗ
	operator_stack_el** operatorStack_headPtr, char** curChar);
func_result_t handleDataFromString(char* curChar, opz_list_el** opzList_head,
	operator_stack_el** operartorStack_head);
opz_list_el* getOpz(); //парсит данные в ОПЗ, возвращает указатель на голову

number_t* calculateOpz(opz_list_el* opzList_head);