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

char* pop(Position head);
int push(Position head, char* val);
Position createElement(char* val);
int readFile(char* filename, Position headNum, Position headString);
int calcMath(char operation, int a, int b);
Position makeHead();
int makeString(Position head, char* operation);
int calcNumber(Position head, char* operation);
int freeStacks(Position headNum, Position headHead);

int main()
{
	Position headNum = makeHead();
	Position headString = makeHead();

	char* filename = "test.txt";

	int status = 0;
	if ((status = readFile(filename, headNum, headString)) < 0) return status;

	char* stringInfix = NULL;
	char* solved = NULL;

	if ((stringInfix = pop(headString)) == NULL)
	{
		free(headString);
		return MEM_ALLOC_ERR;
	}

	if ((solved = pop(headNum)) == NULL)
	{
		free(headNum);
		return MEM_ALLOC_ERR;
	}

	printf(" %s -> %s", stringInfix, solved);

	free(stringInfix);
	free(solved);
	
	freeStacks(headNum, headString);
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
			printf("Pokusaj djeljenja s 0!");
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
	if(head == NULL || operation == NULL)
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

	char* returnedElement = (char*)malloc(strlen(buffer) * sizeof(char) + 1);

	if (returnedElement == NULL) 
	{
		printf("Greska pri alokaciji memorije! (calcNumber)!");
		return MEM_ALLOC_ERR;
	}
	sprintf(returnedElement, "%d", result);

	push(head, returnedElement);
	free(returnedElement);

	return 0;
}

// Skida dva elementa sa stoga, kombinira ih ovisno o operaciji, vraca nazad na stog
int makeString(Position head, char* operation)
{
	if(head == NULL || operation == NULL)
	{
		printf("Greska s argumentima! (makeString)");
		return ARG_ERR;
	}

	char* pos2 = pop(head);
	char* pos1 = pop(head);
	if (pos2 == NULL || pos1 == NULL) return -1;

	char* newElement = (char*)malloc(strlen(pos2) + strlen(pos1) + strlen(operation) + 3 * sizeof(char));

	if(newElement == NULL)
	{
		printf("Greska pri alokaciji memorije! (makeString)");
		return MEM_ALLOC_ERR;
	}

	strcpy(newElement, "(");
	strcat(newElement, pos1);
	strcat(newElement, operation);
	strcat(newElement, pos2);
	strcat(newElement, ")");

	free(pos1);
	free(pos2);

	push(head, newElement);
	free(newElement);

	return 0;
}

// Cita sve postfix elemente u fileu, te pomocu stoga izraduje infix izraz i racuna rezultat
int readFile(char* filename, Position headNum, Position headString)
{
	if (filename == NULL || headNum == NULL || headString == NULL)
	{
		printf("Greska s argumentima! (readFile)");
		return ARG_ERR;
	}

	FILE* file = fopen(filename, "r");
	if(file == NULL)
	{
		printf("Greska pri otvaranju filea! (readFile)");
		return FILE_ERR;
	}


	char buffer[MAX_BUFFER] = { 0 };

	fgets(buffer, MAX_BUFFER, file);
	printf("(%s) ->", buffer);
	rewind(file);

	char* new = NULL;

	while (fscanf(file, "%s", buffer) != EOF)
	{
		if (strcmp("(", buffer))
		{
			int status = 0;
			new = (char*)malloc((int)strlen(buffer) * sizeof(char) + 1);
			if (new == NULL)
			{
				freeStacks(headNum, headString);
				printf("Greska pri alokaciji memorije!");
				fclose(file);
				return MEM_ALLOC_ERR;
			}
			strcpy(new, buffer);

			if (strchr("+-/*", new[0]) && strlen(new) == 1)
			{

				char* pos2 = NULL;
				char* pos1 = NULL;

				if ((status = makeString(headString, new)) < 0)
				{
					freeStacks(headNum, headString);
					free(new);
					fclose(file);
					return status;
				}
				
				if ((status = calcNumber(headNum, new)) < 0)
				{
					freeStacks(headNum, headString);
					free(new);
					fclose(file);
					return status;
				}

			}
			else if (sscanf(new, "%d", &status) == 1)
			{
				if ((status = push(headString, new)) < 0)
				{
					freeStacks(headNum, headString);
					free(new);
					fclose(file);
					return status;
				}

				if ((status = push(headNum, new)) < 0)
				{
					freeStacks(headNum, headString);
					free(new);
					fclose(file);
					return status;
				}

			}
			else
			{
				printf("Unesen je nepodrzan znak! Zatvaram program!");
				freeStacks(headNum, headString);
				return DATA_ERR;
			}
			free(new);
		}
		
	}
	fclose(file);
	return 0;
}

// Oslobada memoriju u Stogovima
int freeStacks(Position headNum, Position headString)
{
	if(headNum != NULL)
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