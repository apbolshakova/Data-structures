#ifndef TYPES
#define TYPES

typedef enum FuncResult_ { FAIL, SUCCESS } status_t;
typedef enum Bool_ { FALSE_, TRUE_ } bool_t;

typedef enum ActionID_
{
	openID,
	closeID,
	printID,
	editID,
	addID,
	removeID,
	mergeID,
	pushID,
	pullID,
	deleteVerID,
	rebaseID,
	countID
};

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

typedef struct Path_ //���� �� ����������� ������ �� �����, �������� ������ ���������� �� ������
{
	version_t* ver;
	struct Path_* next;
} path_t;
#endif