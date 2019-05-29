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

typedef struct Operation_ //операция над текстом
{
	char type;
	int beginIndex;
	int endIndex;
	char* data;
	struct Operation_* next;
} operation_t;

typedef struct Version_ //версия
{
	int verNum;
	int parentVerNum;
	struct Version_* parentPtr;
	int childNum;
	struct Version_** child;
	operation_t* operation; //линейный список изменений в этой версии
} version_t;

typedef struct General_ //главная информация о дереве
{
	char* name;
	int lastCreatedVersion;
	version_t* root; //корень дерева версий
} general_t;

typedef struct Path_ //линейный список версий
{
	version_t* ver;
	struct Path_* next;
} verList_t;
#endif