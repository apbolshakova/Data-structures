#ifndef TYPES
#define TYPES

typedef enum FuncResult_ { FAIL, SUCCESS } func_result_t;
typedef enum Bool_ { FALSE, TRUE } bool_t;

typedef struct Operation_ //�������� ��� �������
{
	char type;
	int beginIndex;
	int endIndex;
	char* data;
	struct Operation_* next;
} operation_t;

typedef struct Version_ //������
{
	int verNum;
	bool_t isDeleted; //������ �������, ���� � �������� NOT_DEFINED_PARENT, ���� ��� ��������
	struct Version_* parentPtr;
	int childNum;
	struct Version_** child;
	operation_t* operation; //�������� ������ ��������� � ���� ������
} version_t;

typedef struct General_ //������� ���������� � ������
{
	char* name;
	int lastCreatedVersion;
	version_t* root; //������ ������ ������
} general_t;

#endif