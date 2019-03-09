#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"

#define END_OF_EXPRESSION '='

/*��������� �����*/
typedef struct Number_
{
	char* asString;
	int stringLen;
	int numberSystem;
} number_t;

/*�������� ������ ���*/
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
/*�������� ������ ���*/

/*��������� ����� ��������, ������������ ��� ��������� ���*/
typedef struct OperatorStackEl_
{
	char sign;
	struct OperatorStackEl_* next;
} operator_stack_el;

/*TODO: ��������� ����� ��� �������� ���*/

opz_list_el_value* getOpzElValue(operator_stack_el** operatorStack_head, char** curChar)
{
	if (curChar == NULL) return NULL;
	opz_list_el_value* elValue = (opz_list_el_value*)malloc(sizeof(opz_list_el_value));

	if      (isDigit(**curChar))    elValue = handleNumber();
	else if (isOperator(**curChar)) elValue = handleOperator();
	else if (**curChar == '(')      elValue = handleOpeningBracket();
	else if (**curChar == ')')      elValue = handleClosingBracket();

	return elValue;
}

opz_list_el* getOpz() //������ ������� ������, ������ �� � ��� � ���������� ��������� �� ������ ����������� ������
{
	opz_list_el* opzList_head = NULL;
	operator_stack_el* operartorStack_head = NULL;
	char* curChar = '\0';

	if (!scanf("%c", &curChar)) return opzList_head;
	while (curChar != END_OF_EXPRESSION)
	{
		opz_list_el_value* value = getOpzElValue(&operartorStack_head, &curChar); //���������� �������� ��� ������ ���� � ������ ���
		pushIntoOpzList(opzList_head, value);
		if (!scanf("%c", &curChar)) return NULL; //��������� ��� ����� ����� => ������. TODO: ���������� ��� ������??
		
	}
	//����������� ������ ������, ���� ����, ��� ������ ������� ��������� opz_list:
	/*
	����������� ������ - ��������� � ����
	����������� ������ - ����������� �� ����� �� �� ������ ����������� ������ ������������
	����� ������������ � ��������� � ��������� number_t
	������ - ����������� ��� �������� � ������� ����������� �� ������ ����������� ������
	����� ������ - ����������� �� �� ����� ��������
	*/
}

int main(void)
{
	printf("������� ��������� ��� �������� (������ ������������� ������ ���������): \n");
	opz_list_el* opzListHead = getOpz(); //�������� ���
	calculateOpz(opzListHead); //���������� ��������� �� ���
	return 0;
}