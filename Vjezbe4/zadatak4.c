/* 4. Napisati program za zbrajanje i množenje polinoma.Koeficijenti i eksponenti se
čitaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nužno sortirani. */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 1024

struct _Polinom;
typedef struct _Polinom* Position;
typedef struct _Polinom {
	int eksponent;
	int koeficijent;
	Position next;
}Polinom;



Position createPolinom(int eksponent, int koeficijent);
int insertPolinom(Position pos1, Position pos2);
int printPolinom(Position head);
int polinomBuilder(Position head, int eksponent, int koeficijent);
int kopirajPolinom(Position headRez, Position polHEAD);
int zbrojiPolinom(Position pol1, Position pol2, Position headRez);
int mnoziPolinom(Position pol1, Position pol2, Position headRez);
Position* readPolinom(char* filename, int* sizeP);
Position selectPolinom(char* filename);
int freeList(Position head);

int main()
{
	Polinom head = { 0 };
	Position headP = &head;

	Position polnymos1 = NULL;

	polnymos1 = selectPolinom("polinom.txt");
	
	printf("Odabir 2.:\n");

	Position polynoms2 = selectPolinom("polinom.txt");

	Polinom rez = { 0 };
	Position rezP = &rez;

	mnoziPolinom(polnymos1, polynoms2, rezP);

	printf("Rezultat mnozenja!: \n");
	printPolinom(rezP);

	Polinom rez2 = { 0 };
	Position rezP2 = &rez2;

	zbrojiPolinom(polnymos1, polynoms2, rezP2);

	printf("\nRezultat zbrajanja!: \n");
	printPolinom(rezP2);

}

Position createPolinom(int eksponent, int koeficijent)
{
	Position newPos = (Position)malloc(sizeof(Polinom));

	newPos->eksponent = eksponent;
	newPos->koeficijent = koeficijent;
	newPos->next = NULL;

	return newPos;
}

int insertPolinom(Position pos1, Position pos2)
{
	pos2->next = pos1->next;
	pos1->next = pos2;

	return 0;
}

int printPolinom(Position head)
{

	Position curr = head->next;

	printf("%dx^%d", curr->koeficijent, curr->eksponent);

	curr = curr->next;

	while (curr != NULL)
	{
		if (curr->koeficijent != 0)
		{
			if (curr->eksponent == 1)
			{
				if (curr->koeficijent > 0) printf("+");
				printf("%dx", curr->koeficijent);
			}
			else if (curr->koeficijent > 0) printf("+");
			printf("%dx^%d", curr->koeficijent, curr->eksponent);
		}

		curr = curr->next;
	}
	return 0;
}

int polinomBuilder(Position head, int eksponent, int koeficijent)
{
	Position newElement = createPolinom(eksponent, koeficijent);
	Position temp = head;

	while (temp->next != NULL && (newElement->eksponent < temp->next->eksponent)) temp = temp->next;
	if (temp->next != NULL && eksponent == temp->next->eksponent)
	{
		temp->next->koeficijent += newElement->koeficijent;
		free(newElement);
	}
	else
	{
		insertPolinom(temp, newElement);
	}
	return 0;


}
int kopirajPolinom(Position headRez, Position polHEAD)
{
	Position curr = polHEAD->next;
	while (curr != NULL)
	{
		polinomBuilder(headRez, curr->eksponent, curr->koeficijent);
		curr = curr->next;
	}
	return 0;
}

int zbrojiPolinom(Position pol1, Position pol2, Position headRez)
{
	kopirajPolinom(headRez, pol1);
	kopirajPolinom(headRez, pol2);

	return 0;
};

int mnoziPolinom(Position pol1, Position pol2, Position headRez)
{
	Position p1 = pol1->next;

	while (p1 != NULL)
	{
		Position p2 = pol2->next;

		while (p2 != NULL)
		{
			int koeficijent = p1->koeficijent * p2->koeficijent;
			int eksponent = p1->eksponent + p2->eksponent;

			polinomBuilder(headRez, eksponent, koeficijent);

			p2 = p2->next;
		}
		p1 = p1->next;
	}
	return 0;
}




Position* readPolinom(char* filename, int* sizeP)
{
	FILE* file = fopen(filename, "r");
	char buffer[MAX_BUFFER] = { 0 };
	int koeficijent, eksponent;

	Position* polynmos = NULL;

	int n = *sizeP;
	while (fgets(buffer, MAX_BUFFER, file) != NULL)
	{
		n++;

		Position head = (Position)malloc(sizeof(Polinom));

		head->eksponent = 0;
		head->koeficijent = 0;
		head->next = NULL;

		polynmos = (Position*)realloc(polynmos, n * sizeof(Position));

		int offset = 0;
		int razmak = 0;
		while (sscanf(buffer + razmak, "%d %d%n", &koeficijent, &eksponent, &offset) == 2)
		{
			razmak += offset;
			polinomBuilder(head, eksponent, koeficijent);
		}
		polynmos[n - 1] = head;
	}
	*sizeP = n;
	fclose(file);
	return polynmos;
}

Position selectPolinom(char* filename)
{
	int size = 0;
	Position* polinoms = readPolinom(filename, &size);

	int odabir = 0;

	for(int i = 0; i < size; i++)
	{
		printf("%d. ", i + 1); 
		printPolinom(polinoms[i]);
		printf("\n");
	}
	printf("\nIzaberi jedan od gore navedenih polinoma: ");
	scanf("%d", &odabir);

	Position choicePolinom = polinoms[odabir - 1];
	for(int i = 0; i < size; i++)
	{
		if (polinoms[i] != choicePolinom) freeList(polinoms[i]);
	}
	free(polinoms);
	return choicePolinom;
}

// Cisti memoriju koju je lista zauzimala / brise sve clanove liste
int freeList(Position head)
{
	if (head == NULL)
	{
		printf("Greska s argumentima! (freeList)");
		return -1;
	}

	Position current = NULL, next = head->next;

	while (next != NULL)
	{
		current = next;
		next = current->next;
		free(current);
	}

	head->next = NULL;

	return 0;
}