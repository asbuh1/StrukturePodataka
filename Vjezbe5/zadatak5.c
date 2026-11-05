/*
5. Napisati program koji iz datoteke čita postfiks izraz i zatim korištenjem stoga računa
rezultat.Stog je potrebno realizirati preko vezane liste.
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 1024

#define ARG_ERR (-1)
#define FILE_ERR (-2)
#define DATA_ERR (-3)
#define MEM_ALLOC_ERR (-4)
#define LOGIC_ERR (-5)

struct _Element;
typedef struct _Element* Position;
typedef struct _Element
{
	char* num;
	Position next;
}Element;

int reader(char* filename);
char* pop(Position head);
int push(Position head, char* val);
Position createElement(char* val);
int readFile(Position headNum, Position headString, char* buffer);
int calcMath(char operation, int a, int b);
Position makeHead();
int makeString(Position head, char* operation);
int calcNumber(Position head, char* operation);
int freeStacks(Position headNum, Position headHead);
int printResult(char* buffer, Position headNum, Position headString);

int main()
{

	char* filename = "test.txt";

	int status = 0;
	if ((status = reader(filename)) < 0) return status;

	return 0;
}

// Pop funkcija za stog, vraca vrijednost gornjeg elementa sa stoga i brise ga
char* pop(Position head)
{
	if (head == NULL || head->next == NULL)
	{
		printf("Greska s argumentima! (pop)");
		return NULL;
	}

	Position temp = head->next;
	head->next = temp->next;
	char* value = NULL;
	value = temp->num;
	free(temp);

	return value;
}

// Push funkcija za stog, stavlja element na vrh stoga
int push(Position head, char* val)
{
	if (head == NULL || val == NULL)
	{
		printf("Greska s argumentima! (push)");
		return ARG_ERR;
	}

	Position newElement = NULL;
	if ((newElement = createElement(val)) == NULL) return MEM_ALLOC_ERR;

	newElement->next = head->next;
	head->next = newElement;

	return 0;
}

// Stvara prazan head
Position makeHead()
{
	Position tempHead = (Position)malloc(sizeof(Element));
	tempHead->next = NULL;
	tempHead->num = NULL;

	return tempHead;
}

// Stvara element Position i alocira mu memoriju za element.num
Position createElement(char* val)
{
	if (val == NULL)
	{
		printf("Greska s argumentima! (createElement)");
		return NULL;
	}

	Position temp = (Position)malloc(sizeof(Element));

	if (temp == NULL)
	{
		printf("Greska pri alokaciji memorije! (createElement)");
		return NULL;
	}

	temp->next = NULL;
	temp->num = (char*)malloc(strlen(val) + 1);

	if (temp->num == NULL)
	{
		printf("Greska pri alokaciji memorije! (createElement)");
		free(temp);
		return NULL;
	}

	strcpy(temp->num, val);

	return temp;
}

// Usporeduje operatore/ trazi koju operaciju izvesti
int calcMath(char operation, int int1, int int2)
{

	int result = 0;
	if (operation == '+') return int1 + int2;
	else if (operation == '*') return int1 * int2;
	else if (operation == '/')
	{
		if (int2 == 0)
		{
			printf("Pokusaj djeljenja s 0! (calcMath)");
			return LOGIC_ERR;
		}
		return int1 / int2;
	}
	else if (operation == '-') return int1 - int2;
	return 0;

}

// Logika za pretvaranje char* u int, racuanje s int, pa vracanje nazad u char*
int calcNumber(Position head, char* operation)
{
	if (head == NULL || operation == NULL)
	{
		printf("Greska s argumentima!");
		return ARG_ERR;
	}

	int int2 = 0, int1 = 0, result = 0;

	char* checkErr = NULL;
	if ((checkErr = pop(head)) == NULL)
	{
		return ARG_ERR;
	}
	sscanf(checkErr, "%d", &int2);
	free(checkErr);

	if ((checkErr = pop(head)) == NULL) return ARG_ERR;

	sscanf(checkErr, "%d", &int1);
	free(checkErr);

	result = calcMath(operation[0], int1, int2);
	char buffer[MAX_BUFFER] = { 0 };
	sprintf(buffer, "%d", result);

	push(head, buffer);

	return 0;
}

// Skida dva elementa sa stoga, kombinira ih ovisno o operaciji, vraca nazad na stog
int makeString(Position head, char* operation)
{
	if (head == NULL || operation == NULL)
	{
		printf("Greska s argumentima! (makeString)");
		return ARG_ERR;
	}

	char* pos2 = NULL;
	char* pos1 = NULL;
	char buffer[MAX_BUFFER] = { 0 };

	if((pos2 = pop(head)) == NULL) 
	{
		return ARG_ERR;
	}

	if ((pos1 = pop(head)) == NULL)
	{
		free(pos2);
		return ARG_ERR;
	}
	

	strcpy(buffer, "(");
	strcat(buffer, pos1);
	strcat(buffer, operation);
	strcat(buffer, pos2);
	strcat(buffer, ")");

	free(pos1);
	free(pos2);

	push(head, buffer);

	return 0;
}

// Ispisuje Pocetni postfix izraz, convertani infix i rezultat
int printResult(char* buffer, Position headNum, Position headString)
{
	if (buffer == NULL || headNum == NULL || headString == NULL)
	{
		printf("Greska s argumentima! (printResult)");
		return ARG_ERR;
	}
	char* stringInfix = NULL;
	char* solved = NULL;

	if ((stringInfix = pop(headString)) == NULL) return MEM_ALLOC_ERR;
	if ((solved = pop(headNum)) == NULL) return MEM_ALLOC_ERR;
	
	printf("(%s) -> %s -> %s\n", buffer, stringInfix, solved);

	free(stringInfix);
	free(solved);
	return 0;
}

// Cita postfix izraze iz filea
int reader(char* filename)
{
	if (filename == NULL)
	{
		printf("Greska s argumentima! (readFile)");
		return ARG_ERR;
	}

	FILE* file = fopen(filename, "r");

	if (file == NULL)
	{
		printf("Greska pri otvaranju filea! (reader)");
		return FILE_ERR;
	}

	Position headNum = makeHead();
	Position headString = makeHead();

	char buffer[MAX_BUFFER] = { 0 };
	while(fgets(buffer, MAX_BUFFER, file) != NULL)
	{
		
		int lengthofBuffer = (int)strlen(buffer) - 1;
		if (lengthofBuffer > 1 && buffer[lengthofBuffer] == '\n') buffer[lengthofBuffer] = 0;

		int status = 0;
		
		if ((status = readFile(headNum, headString, buffer)) < 0)
		{
			
			freeStacks(headNum, headString);
			if (status == DATA_ERR)
			{
				headNum = makeHead(), headString = makeHead();
				continue;
			}

			fclose(file);
			return status;
		}

		if ((status = printResult(buffer, headNum, headString)) < 0)
		{
			freeStacks(headNum, headString);
			fclose(file);
			return status;
		}
		freeStacks(headNum, headString);
		headNum = makeHead();
		headString = makeHead();
	}

	freeStacks(headNum, headString);
	fclose(file);

	return 0;
}

// Cita sve postfix elemente u fileu, te pomocu stoga izraduje infix izraz i racuna rezultat
int readFile(Position headNum, Position headString, char* buffer)
{
	if (headNum == NULL || headString == NULL || buffer == NULL)
	{
		printf("Greska s argumentima! (readFile)");
		return ARG_ERR;
	}

	int lengthOffset = 0, offset  = 0;

	char readBuffer[MAX_BUFFER] = { 0 };

	int numberOfOperations = 0, numberOfNumbers = 0;
	int status = 0;
	while (sscanf(buffer + lengthOffset, "%s%n", readBuffer, &offset) == 1)
	{
		if (strcmp("(", readBuffer))
		{
			status = 0;

			if (strchr("+-/*", readBuffer[0]) && strlen(readBuffer) == 1)
			{
				numberOfOperations++;

				if(numberOfOperations >= numberOfNumbers)
				{
					printf("Greska u logici postfix izraza! Vise operacija od dopustenog! (readFile)\n");
					return DATA_ERR;
				}

				if ((status = makeString(headString, readBuffer)) < 0) return status;
				if ((status = calcNumber(headNum, readBuffer)) < 0) return status;
			}

			else if (sscanf(readBuffer, "%d", &status) == 1)
			{
				numberOfNumbers++;
				if ((status = push(headString, readBuffer)) < 0) return status;
				if ((status = push(headNum, readBuffer)) < 0) return status;

			}
			else
			{
				printf("Unesen je nepodrzan znak! Zatvaram program!(readFile)\n");
				return DATA_ERR;
			}
		}
		lengthOffset += offset;
	};
	
	if (numberOfNumbers > (numberOfOperations + 1 ))
	{
		printf("Greska! Posfix izraz je krivo unesen! Imate pre malo operacija!(readFile)");
		return DATA_ERR;
	}

	return status;
}

// Oslobada memoriju u Stogovima
int freeStacks(Position headNum, Position headString)
{
	if (headNum != NULL)
	{
		Position curr = headNum->next;
		while (curr != NULL)
		{
			Position next = curr->next;
			free(curr->num);
			free(curr);
			curr = next;
		}
	}

	if (headString != NULL)
	{
		Position curr = headString->next;
		while (curr != NULL)
		{
			Position next = curr->next;
			free(curr->num);
			free(curr);
			curr = next;
		}
	}

	free(headNum);
	free(headString);

	return 0;
}