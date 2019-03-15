#include "Header.h"

int main (void)
{
	opz_list_el* opzListHead = getOpz();
	if (opzListHead != NULL)
	{
		char* result = calculateOpz(opzListHead); //возвращает строку-результат вычислений
        //write result to output.txt
	}
	_getch();
	return 0;
}