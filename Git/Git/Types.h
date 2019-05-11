#ifndef TYPES
#define TYPES

typedef enum FuncResult_ { FAIL, SUCCESS } func_result_t;
typedef enum Bool_ { FALSE, TRUE } bool_t;

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
	bool_t isDeleted; //версия удалена, если её родитель NOT_DEFINED_PARENT, флаг для удобства
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

#endif