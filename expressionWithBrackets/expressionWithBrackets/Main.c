#include "Header.h"

int main (void)
{
	opz_list_el* opzListHead = getOpz();
	if (opzListHead != NULL)
	{
		char* result = calculateOpz(opzListHead); //���������� ������-��������� ����������
        //write result to output.txt
	}
	_getch();
	return 0;
}