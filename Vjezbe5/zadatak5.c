/*
5. Napisati program koji iz datoteke čita postfiks izraz i zatim korištenjem stoga računa
rezultat.Stog je potrebno realizirati preko vezane liste.
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int zbroj(int a, int b);
int oduzmi(int a, int b);
int pomnozi(int a, int b);
int podjeli(int a, int b);
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

int zbroj(int a, int b) { return a + b; }
int oduzmi(int a, int b) { return a - b; }
int pomnozi(int a, int b) { return a * b; }
int podjeli(int a, int b) { return a / b; }

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

int readEngine(char* filename, Position head, Position calcStog)
{
	
	FILE* file = fopen(filename, "r");
	char buffer[1024] = { 0 };
	fgets(buffer, 1024, file);

	printf("(%s) ->", buffer);
	rewind(file);
	Position jaje = makeHead();

	char demons[50] = { 0 };
	char* new = NULL;
	while(fscanf(file, "%s", demons) != EOF)
	{
	if(strcmp("(", demons))
	{
		new = (char*)malloc((int)strlen(demons) * sizeof(char) + 1);
		strcpy(new, demons);

		// printf("TESTIRAM : %s\n", new);

		if(strchr("+-/*", new[0]))
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

				int int2 = 0, int1 = 0;
				int newa = 0;

				sscanf(pop(jaje), "%d", &int2);
				sscanf(pop(jaje), "%d", &int1);
				
			
				if (new[0] == '+') newa = int2 + int1;
				else if (new[0] == '*') newa = int2 * int1;
				else if (new[0] == '/') newa = int2 / int1;
				else if (new[0] == '-') newa = int2 - int1;

				char* returnedElement = (char*)malloc(brojacZnamenki(newa) * sizeof(char) + 1);
				sprintf(returnedElement,"%d",newa);

			
			push(head, newElement);
			push(jaje, returnedElement);

		}
		else
		{
			push(head, new);

			push(jaje, new);
		}
	}

	}
	printf(" %s = %s", pop(head), pop(jaje));

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