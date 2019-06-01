#include "Common.h"

status_t handleAdd()
{
	int i = 0;
	printf("Enter starting index or any negative number to cancel (no number -> index = 0): ");
	fflush(stdin);
	scanf_s("%i", &i);
	if (i < 0) return SUCCESS;
	printf("Enter text to add and press %s when you are done:\n", PROCEED_BTN);
	char* data = getDataFromInput(NULL);
	if (add(i, data, ALL, NULL) == FAIL)
	{
		printf("ERROR: unable to save an add operation.\n");
		return FAIL;
	}
	return SUCCESS;
}

status_t add(int i, char* data, int n, version_t* ver)
{
	if (!indexIsCorrect(i)) 
	{
		printf("ERROR: invalid index for starting position.\n");
		return FAIL;
	}
	//create new operation
	operation_t* opBuf = (operation_t*)malloc(sizeof(operation_t));
	if (!opBuf)
	{
		printf("ERROR: memory allocation problem.\n");
		return FAIL;
	}
	opBuf->type = '+';
	opBuf->beginIndex = i;
	opBuf->endIndex = INVALID_INDEX;
	opBuf->next = NULL;
	if (n == ALL) opBuf->data = data;
	else
	{
		char* str = (char*)calloc(n, sizeof(char));
		if (!str)
		{
			free(opBuf);
			printf("ERROR: memory allocation problem.\n");
			return FAIL;
		}
		strncpy(str, data, n);
		str[n] = '\0';
		opBuf->data = str;
	}
	//add operation into buffer's list
	if (!ver) pushIntoOpList(&(buf->operation), opBuf);
	else pushIntoOpList(&(ver->operation), opBuf);
	return SUCCESS;
}

status_t handleRemoving()
{
	fflush(stdin);
	int i = 0;
	printf("Enter starting index or any negative number to cancel (no number -> index = 0): ");
	scanf_s("%i", &i);
	if (i < 0) return SUCCESS;

	int j = 0;
	printf("Enter ending index or any negative number to cancel (no number -> index = 0): ");
	scanf_s("%i", &j);
	if (j < 0) return SUCCESS;

	if (remove(i, j) == FAIL)
	{
		printf("ERROR: unable to save a removing operation.\n");
		return FAIL;
	}
	return SUCCESS;
}

status_t remove(int i, int j)
{
	if (j < i || !indexIsCorrect(i) || !indexIsCorrect(j))
	{
		printf("ERROR: invalid indexes for removing.\n");
		return FAIL;
	}

	//create new operation
	operation_t* opBuf = (operation_t*)malloc(sizeof(operation_t));
	if (!opBuf)
	{
		printf("ERROR: memory allocation problem.\n");
		return FAIL;
	}
	int stringLen = getMaxTextLen(NULL);
	char* text = (char*)calloc(stringLen + 1, sizeof(char));
	if (getCurText(text, stringLen, NULL, NULL) == FAIL)
	{
		free(text);
		return FAIL;
	}

	opBuf->type = '-';
	opBuf->beginIndex = i;
	opBuf->endIndex = j;
	opBuf->data = (char*)calloc(j - i + 1, sizeof(char));
	strncpy(opBuf->data, &(text[i]), j - i);
	opBuf->next = NULL;

	//add operation into buffer's list
	pushIntoOpList(&(buf->operation), opBuf);
	
	return SUCCESS;
}

status_t handleEditing()
{
	fflush(stdin);
	int i = 0;
	printf("Enter starting index or any negative number to cancel (no number -> index = 0): ");
	scanf_s("%i", &i);
	if (i < 0) return SUCCESS;

	int j = 0;
	printf("Enter ending index or any negative number to cancel (no number -> index = 0): ");
	scanf_s("%i", &j);
	if (j < 0) return SUCCESS;

	int len = j - i;
	if (len < 0)
	{
		printf("ERROR: invalid indexes.\n");
		return FAIL;
	}

	printf("Enter text for replacing or press %s when you are done:\n", PROCEED_BTN);
	char* data = getDataFromInput(&len); 
	if (edit(i, j, data) == FAIL)
	{
		printf("ERROR: unable to save an edit operation.\n");
		return FAIL;
	}
	return SUCCESS;
}

status_t edit(int i, int j, char* data)
{
	if (remove(i, j) == FAIL)
	{
		printf("ERROR: unable to save a removing operation.\n");
		return FAIL;
	}
	if (add(i, data, ALL, NULL) == FAIL)
	{
		printf("ERROR: unable to save an add operation.\n");
		return FAIL;
	}
	return SUCCESS;
}

char* getDataFromInput(int* dataLen)
{
	int len = ANY;
	if (dataLen) len = *dataLen;
	char* data = (char*)calloc(TEMP_LEN, sizeof(char));
	char ch = '\0';
	while (ch != PROCEED_BTN_CODE && len)
	{
		ch = _getch();
		printf("%c", ch);
		if (ch == BACKSPACE)
		{
			data[strlen(data) - 1] = '\0';
			if (len != ANY) len++;
			fflush(stdin);
			continue;
		}
		strncat(data, &ch, 1);
		if (strlen(data) % (TEMP_LEN - 1) == 0)
			data = (char*)realloc(data, (strlen(data) + TEMP_LEN) * sizeof(char));
		if (len != ANY) len--;
	}
	if (len == ANY) data[strlen(data) - 1] = '\0';
	printf("\n");
	fflush(stdin);
	return data;
}

bool_t indexIsCorrect(int i)
{
	int textLen = getTextLen(NULL);
	if (i > textLen) return FALSE_;
	return TRUE_;
}