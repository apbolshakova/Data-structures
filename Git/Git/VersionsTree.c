#include "Header.h"

func_result_t initVerTree()
{
	//TODO
}

void deleteVerTree()
{
	//TODO
}

//��������� �� ������������ ������ � ������ ������, ������ � ��������� ����� ������ ���������� � generalInfo
func_result_t buildVerTree() 
{
	if (!exists(getNameOfVerFile(ROOT_VER)))
	{
		initVerTree();
	}
	else
	{
		//load existing tree
	}
}