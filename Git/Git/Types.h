#ifndef TYPES
#define TYPES

typedef enum FuncResult_ { FAIL, SUCCESS } func_result_t;

typedef struct Change //�������� ��� �������
{
	char type;
	int beginIndex;
	int endIndex;
	char* data;
} change_t;

typedef struct Buf //������������ ������ �� ����� ����������� � ��
{
	char* text;
	int textLen;
	int verNum;
	int parentVer;
	change_t* unsavedChange;
} buf_t;

#endif