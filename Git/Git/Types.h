#ifndef TYPES
#define TYPES

typedef enum FuncResult_ { FAIL, SUCCESS } status_t;
typedef enum Bool_ { FALSE, TRUE } bool_t;

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
	//bool_t isDeleted; TODO: maybe delete and add function that returns TRUE if ver is deleted
	struct Version_* parentPtr; //версия удалена, если её родитель NOT_DEFINED_PARENT
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

typedef struct Path_ //путь до определённой версии от корня, линейный список указателей на версии
{
	version_t* ver;
	struct Path_* next;
} path_t;
#endif