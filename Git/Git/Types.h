#ifndef TYPES
#define TYPES

typedef enum FuncResult_ { FAIL, SUCCESS } func_res_t;
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
	//bool_t isDeleted; TODO: maybe delete and add function that returns TRUE if ver is deleted
	struct Version_* parentPtr; //������ �������, ���� � �������� NOT_DEFINED_PARENT
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

typedef struct Path_ //���� �� ����������� ������ �� �����, �������� ������ ������� ������, -1 - �����
{
	int verNum;
	struct Path_* next;
} path_t;
#endif