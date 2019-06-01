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

typedef struct Operation_ //text operations
{
	char type;
	int beginIndex;
	int endIndex;
	char* data;
	struct Operation_* next;
} operation_t;

typedef struct Version_
{
	int verNum;
	int parentVerNum;
	struct Version_* parentPtr;
	int childNum;
	struct Version_** child;
	operation_t* operation; //list of operations from one version
} version_t;

typedef struct General_ //general info about tree
{
	char* name;
	int lastCreatedVersion;
	version_t* root; //root of version tree
} general_t;

typedef struct Path_ //list of versions
{
	version_t* ver;
	struct Path_* next;
} verList_t;
#endif