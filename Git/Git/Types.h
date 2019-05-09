#ifndef TYPES
#define TYPES

typedef enum FuncResult_ { FAIL, SUCCESS } func_result_t;
typedef enum Bool_ { FALSE, TRUE } bool_t;

typedef struct operation_ //�������� ��� �������
{
	char type;
	int beginIndex;
	int endIndex;
	char* data;
} operation_t;

typedef struct Version_ //������
{
	int verNum;
	bool_t isDeleted; //������ �������, ���� � �������� NOT_DEFINED_PARENT, ���� ��� ��������
	int parentVerNum;
	struct Version_* parentPtr;
	operation_t* operation; //�������� ������ ��������� � ���� ������
} version_t;

typedef struct General_ //������� ���������� � ������
{
	char* name;
	int lastCreatedVersion;
	version_t* root; //������ ������ ������
} general_t;

#endif