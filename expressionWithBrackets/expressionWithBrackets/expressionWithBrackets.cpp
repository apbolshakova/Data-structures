#define _CRT_SECURE_NO_WARNINGS

/*��������� �����*/
typedef struct Number
{
	char* asString;
	int numberSystem;
	int stringLen;
} NUMBER;

/*�������� ������ opzItems*/
typedef union OpzItem
{
	char sign;
	NUMBER* number;
} OPZ_ITEM;

/*��������� ����� �������� �� getOPZ*/
typedef struct CharStackItem
{
	char sign;
	//TODO ���������
};

/*��������� ����� ��� �������� ���*/

void getOPZ()
{
	//����������� ������ ������, ���� ����, ��� ������ ������� ��������� ������ opzItems:
	/*
	����������� ������ - ��������� � ����
	����������� ������ - ����������� �� ����� �� �� ������ ����������� ������ ������������
	����� ������������ � ��������� � ��������� NUM
	������ - ����������� ��� �������� � ������� ����������� �� ������ ����������� ������
	����� ������ - ����������� �� �� ����� ��������
	*/
}

int main(void)
{
	getReadyForCalculationOPZ(); //�������� ���
	calculateOPZ(); //���������� ��������� �� ���
	return 0;
}