#define _CRT_SECURE_NO_WARNINGS

/*структура числа*/
typedef struct Number
{
	char* asString;
	int numberSystem;
	int stringLen;
} NUMBER;

/*линейный список opzItems*/
typedef union OpzItem
{
	char sign;
	NUMBER* number;
} OPZ_ITEM;

/*структура стека символов из getOPZ*/
typedef struct CharStackItem
{
	char sign;
	//TODO закончить
};

/*структура стека дл€ подсчЄта опз*/

void getOPZ()
{
	//динамически читать строку, пока есть, при первом проходе формируем список opzItems:
	/*
	открывающа€ скобка - скидываем в стек
	закрывающа€ скобка - вытаскиваем из стека всЄ до первой открывающей скобки включительно
	число обрабатываем и скидываем в экземпл€р NUM
	символ - вытаскиваем все операции с большим приоритетом до первой открывающей скобки
	конец строки - вытаскиваем всЄ из стека символов
	*/
}

int main(void)
{
	getReadyForCalculationOPZ(); //получить опз
	calculateOPZ(); //поссчитать выражение по опз
	return 0;
}