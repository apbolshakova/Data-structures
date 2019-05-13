#include "Header.h"

func_res_t handleAdd()
{
	int i = 0;
	printf("Enter starting index or any negative number to cancel (no number -> index = 0): ");
	scanf("%i", &i);
	if (i < 0) return SUCCESS;
	printf("Enter text to add and press %s when you are done:\n", PROCEED_BTN);
	char* data = getData(); //TODO: fix when first char in output is missing after this func
	if (add(i, data) == FAIL)
	{
		printf("ERROR: unable to save an add operation.\n");
		return FAIL;
	}
	return SUCCESS;
}

char* getData()
{
	char* data = (char*)calloc(TEMP_LEN, sizeof(char));
	char ch = '\0';
	while (ch != PROCEED_BTN_CODE)
	{
		ch = _getch();
		if (ch == '\r') ch = '\n';
		printf("%c", ch);
		strncat(data, &ch, 1);
		if (strlen(data) % (TEMP_LEN - 1) == 0)
			data = (char*)realloc(data, (strlen(data) + TEMP_LEN) * sizeof(char));
	}
	data[strlen(data) - 1] = '\0';
	printf("\n");
	return data;
}

func_res_t add(int i, char* data)
{
	if (!indexIsCorrect(i)) 
	{
		printf("ERROR: invalid index for starting position.\n");
		return FAIL;
	}

	//создать операцию
	operation_t* opBuf = (operation_t*)malloc(sizeof(operation_t));
	if (!opBuf)
	{
		printf("ERROR: memory allocation problem.\n");
		return FAIL;
	}
	opBuf->type = '+';
	opBuf->beginIndex = i;
	opBuf->endIndex = INVALID_INDEX;
	opBuf->data = data;
	opBuf->next = NULL;

	//добавить операцию в линейный список буфера
	pushIntoOpList(&(buf->operation), opBuf);
	return SUCCESS;
}

func_res_t handleRemoving()
{
	int i = 0;
	printf("Enter starting index or any negative number to cancel (no number -> index = 0): ");
	scanf("%i", &i);
	if (i < 0) return SUCCESS;

	int j = 0;
	printf("Enter ending index or any negative number to cancel (no number -> index = 0): ");
	scanf("%i", &j);
	if (j < 0) return SUCCESS;

	if (remove(i, j) == FAIL)
	{
		printf("ERROR: unable to save a removing operation.\n");
		return FAIL;
	}
	return SUCCESS;
}

func_res_t remove(int i, int j)
{
	if (j < i || !indexIsCorrect(i) || !indexIsCorrect(j))
	{
		printf("ERROR: invalid indexes for removing.\n");
		return FAIL;
	}

	//создать операцию
	operation_t* opBuf = (operation_t*)malloc(sizeof(operation_t));
	if (!opBuf)
	{
		printf("ERROR: memory allocation problem.\n");
		return FAIL;
	}
	opBuf->type = '-';
	opBuf->beginIndex = i;
	opBuf->endIndex = j;
	opBuf->data = NULL;
	opBuf->next = NULL;

	//добавить операцию в линейный список буфера
	pushIntoOpList(&(buf->operation), opBuf);

	return SUCCESS;
}

bool_t indexIsCorrect(int i)
{
	int textLen = getTextLen();
	if (i > textLen) return FALSE;
	return TRUE;
}