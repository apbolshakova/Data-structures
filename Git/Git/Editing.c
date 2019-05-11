#include "Header.h"

func_result_t add(int i, char* data)
{
	if (!indexIsCorrect(i)) 
	{
		printf("ERROR: Invalid index for starting position.\n");
		return FAIL;
	}

	//������� ��������
	operation_t* opBuf = (operation_t*)malloc(sizeof(operation_t));
	if (!opBuf)
	{
		printf("ERROR: Memory allocation problem.\n");
		return FAIL;
	}
	opBuf->type = '+';
	opBuf->beginIndex = i;
	opBuf->endIndex = INVALID_INDEX;
	opBuf->data = data;

	//�������� �������� � �������� ������ ������
	if (!pushIntoOpList(opBuf) == FAIL)
	{
		printf("ERROR: Unable to push into operation list.\n");
		return FAIL;
	}
	return SUCCESS;
}

bool_t indexIsCorrect(int i)
{
	int textLen = getTextLen();
	if (i > textLen) return FALSE;
	return TRUE;
}

int getTextLen()
{ 
	int result = 0;
	//TODO: finish; ������, ��� ����� ��� ����� ���� ������ (���� ������ �� ������)
	/*result += getLenDiffFromVer(buf->operation); //������� ��������������� ������� - �����

	version_t* ver = getVerPtr(buf->parentVerNum);
	while (verNum != NOT_DEFINED_PARENT) //���� ������� ��������
	{
		//�������������� ����� �������� �� ���� ������
 		//����� ������� ������ - �������� �������
	}*/
		
	return result;
}