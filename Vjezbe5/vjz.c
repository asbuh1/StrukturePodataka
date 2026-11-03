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

int readEngine(char* filename, Position head, Position calcStog);

/*int zbroj(int a, int b);
int oduzmi(int a, int b);
int pomnozi(int a, int b);
int podjeli(int a, int b);
*/
int brojacZnamenki(int prijatelj);

int testing(Position head);
Position makeHead();



int main()
{
	Position head = makeHead();
	Position head2 = makeHead();

	readEngine("test.txt", head, head2);
	return 0;
}

/*int zbroj(int a, int b) { return a + b; }
int oduzmi(int a, int b) { return a - b; }
int pomnozi(int a, int b) { return a * b; }
int podjeli(int a, int b) { return a / b; }*/

int brojacZnamenki(int prijatelj)
{
	if (prijatelj == 0) return 1;
	int brojac = 0;
	if (prijatelj < 0) brojac++;

	while (prijatelj != 0)
	{
		prijatelj /= 10;
		brojac++;
	}
	return brojac;
}


Position makeHead()
{
	Position lala = (Position)malloc(sizeof(Element));
	lala->next = NULL;
	lala->num = NULL;

	return lala;
}

int readFile(char* filename, Position stringHead, Position calcStog, Position calcStackHead)
{
	FILE* file = fopen(filename, "r");

	char buffer[MAX_BUFFER] = { 0 };
	char* new = NULL;

	while (fscanf(file, "%s", buffer) != EOF)
	{
		if (strcmp("(", buffer))
		{
			int temp = 0;
			new = (char*)malloc((int)strlen(buffer) * sizeof(char) + 1);
			strcpy(new, buffer);

			// printf("TESTIRAM : %s\n", new);

			if (strchr("+-/*", new[0]))
			{
				// printf("OPERATOR");

				char* pos2 = pop(stringHead);
				char* pos1 = pop(stringHead);

				char* newElement = (char*)malloc(strlen(pos2) + strlen(pos1) + strlen(new) + 3 * sizeof(char));

				strcpy(newElement, "(");
				strcat(newElement, pos1);
				strcat(newElement, new);
				strcat(newElement, pos2);
				strcat(newElement, ")");

				int int2 = 0, int1 = 0, result = 0;

				sscanf(pop(calcStackHead), "%d", &int2);
				sscanf(pop(calcStackHead), "%d", &int1);

				if (new[0] == '+') result = int2 + int1;
				else if (new[0] == '*') result = int2 * int1;
				else if (new[0] == '/') result = int1 / int2;
				else if (new[0] == '-') result = int1 - int2;

				char* returnedElement = (char*)malloc(brojacZnamenki(result) * sizeof(char) + 1);
				sprintf(returnedElement, "%d", result);

				push(stringHead, newElement);
				push(calcStackHead, returnedElement);

			}
			else if (sscanf(new, "%d", &temp) == 1)
			{
				push(stringHead, new);
				push(calcStackHead, new);
			}
			else
			{
				printf("Unesen je nepodrzan znak!");
				return DATA_ERR;
			}
		}

	}

}

int readEngine(char* filename, Position head, Position calcStog)
{
	if (filename == NULL || head == NULL || calcStog == NULL)
	{
		printf("Greska s argumentima (readEngine)");
		return ARG_ERR;
	}

	FILE* file = fopen(filename, "r");

	if(file == NULL)
	{
		printf("Greska pri otvaranju filea! (readEngine)");
		return FILE_ERR;
	}

	char buffer[MAX_BUFFER] = { 0 };
	fgets(buffer, MAX_BUFFER, file);

	printf("(%s) ->", buffer);
	Position calcStackHead = makeHead();
	fclose(file);

	readFile(filename, head, calcStog, calcStackHead);

	char demons[50] = { 0 };
	char* new = NULL;
	while (fscanf(file, "%s", demons) != EOF)
	{
		if (strcmp("(", demons))
		{
			int temp = 0;
			new = (char*)malloc((int)strlen(demons) * sizeof(char) + 1);
			strcpy(new, demons);

			// printf("TESTIRAM : %s\n", new);

			if (strchr("+-/*", new[0]))
			{
				// printf("OPERATOR");

				char* pos2 = pop(head);
				char* pos1 = pop(head);

				char* newElement = (char*)malloc(strlen(pos2) + strlen(pos1) + strlen(new) + 3 * sizeof(char));

				strcpy(newElement, "(");
				strcat(newElement, pos1);
				strcat(newElement, new);
				strcat(newElement, pos2);
				strcat(newElement, ")");

				int int2 = 0, int1 = 0, result = 0;

				sscanf(pop(calcStackHead), "%d", &int2);
				sscanf(pop(calcStackHead), "%d", &int1);

				if (new[0] == '+') result = int2 + int1;
				else if (new[0] == '*') result = int2 * int1;
				else if (new[0] == '/') result = int1 / int2;
				else if (new[0] == '-') result = int1 - int2;

				char* returnedElement = (char*)malloc(brojacZnamenki(result) * sizeof(char) + 1);
				sprintf(returnedElement, "%d", result);

				push(head, newElement);
				push(calcStackHead, returnedElement);

			}
			else if (sscanf(new, "%d", &temp) == 1)
			{
				push(head, new);
				push(calcStackHead, new);
			}
			else
			{
				printf("Unesen je nepodrzan znak!");
				return DATA_ERR;
			}
		}

	}
	printf(" %s = %s", pop(head), pop(calcStackHead));

	return 0;
}

int testing(Position head)
{
	for (Position curr = head->next; curr != NULL; curr = curr->next) printf("\nTESTIRANJE: %s", curr->num);
	return 0;
}

Position createElement(char* val)
{
	Position temp = (Position)malloc(sizeof(Element));

	temp->next = NULL;
	temp->num = (char*)malloc(strlen(val) + 1);
	strcpy(temp->num, val);

	return temp;
}

char* pop(Position head)
{
	Position temp = head->next;
	head->next = temp->next;
	char* value = NULL;
	value = temp->num;
	free(temp);

	return value;
}

int push(Position head, char* val)
{
	Position newElement = createElement(val);

	newElement->next = head->next;
	head->next = newElement;

	return 0;
}