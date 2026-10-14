/*
2. Definirati strukturu osoba (ime, prezime, godina rođenja) i napisati program koji:
A. dinamički dodaje novi element na početak liste,
B. ispisuje listu,
C. dinamički dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. briše određeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 50

#define MEMORY_ALLOC_ERR (-1)
#define ARG_ERR (-2)

struct _Person;
typedef struct _Person* Position;
typedef struct _Person
{
	char name[MAX_LENGTH],
		surname[MAX_LENGTH];
	int birthYear;
	Position next;
}Person;


Position createPerson(char* name, char* surname, int birthYear);
int insertPosition(Position pos1, Position pos2);
int addToFront(Position head, char* name, char* surname, int birthYear);
int printList(Position head);
int addToBack(Position head, char* name, char* surname, int birthYear);
int findPersonBySurname(Position head, char* surname);
int deletePersonBySurname(Position head, char* surname);
int freeList(Position head);

int main()
{

	Person head = { 0 };
	Position headP = &head;

	int status = 0;

	if ((status = addToFront(headP, "Jako", "Dobro", 2000)) < 0) return status;
	if ((status = addToFront(headP, "Vrlo", "Dobro", 3000)) < 0) return status;
	if ((status = addToBack(headP, "Vrlo", "Lose", 2000)) < 0) return status;

	printf("TEST 1: \n");
	
	if ((status = printList(headP)) < 0) return status;

	if ((status = addToFront(headP, "iv", "ivic", 1000)) < 0) return status;
	if ((status = addToBack(headP, "Dobro", "Lose", 2000)) < 0) return status;

	printf("TEST 2: \n");

	if ((status = printList(headP)) < 0) return status;
	if ((status = findPersonBySurname(headP, "Lose")) < 0) return status;
	if ((status = deletePersonBySurname(headP, "Dobro")) < 0) return status;

	printf("Izbrisano prezime Dobro: \n");

	if ((status = printList(headP)) < 0) return status;
	if ((status = deletePersonBySurname(headP, "Lose")) < 0) return status;

	printf("Izbrisano prezime Lose: \n");
	if ((status = printList(headP)) < 0) return status;

	if ((status = freeList(headP)) < 0) return status;
}

// Alocira memoriju i stavlja podatke u newPerson
Position createPerson(char* name, char* surname, int birthYear)
{

	Position newPerson = NULL;
	newPerson = (Position)malloc(sizeof(Person));

	if (newPerson == NULL) return NULL;

	strcpy(newPerson->name, name);
	strcpy(newPerson->surname, surname);
	newPerson->birthYear = birthYear;

	newPerson->next = NULL;

	return newPerson;
}

// Povezuje pokazivace elemenata vezane liste
int insertPosition(Position pos1, Position pos2)
{
	pos2->next = pos1->next;
	pos1->next = pos2;

	return 0;
}

// Pripaja element na pocetak vezane liste
int addToFront(Position head, char* name, char* surname, int birthYear)
{

	if (head == NULL || name == NULL || surname == NULL)
	{
		printf("Greska u argumentima!(addToFront)");
		return ARG_ERR;
	}

	Position newPerson = createPerson(name, surname, birthYear);

	if (newPerson == NULL)
	{
		printf("Greška pri alokaciji memorije! (addToFront)");
		return MEMORY_ALLOC_ERR;
	}

	insertPosition(head, newPerson);

	return 0;
}

// Ispisuje sve elemente vezanog liste u obliku |ime|prezime|godina|
int printList(Position head)
{

	if (head == NULL)
	{
		printf("Greska u argumentima! (printList)");
		return ARG_ERR;
	}

	Position printPosition = NULL;
	printPosition = head->next;

	while (printPosition != NULL)
	{
		printf("|%s|%s|%d|\n", printPosition->name, printPosition->surname, printPosition->birthYear);
		printPosition = printPosition->next;
	}

	return 0;
}
// Pripaja element na kraj vezane liste
int addToBack(Position head, char* name, char* surname, int birthYear)
{

	if (head == NULL || name == NULL || surname == NULL)
	{
		printf("Greska u argumentima! (addToBack)");
		return ARG_ERR;
	}

	Position newPerson = createPerson(name, surname, birthYear);

	if (newPerson == NULL)
	{
		printf("Greska pri alokaciji memorije! (addToBack)");
		return MEMORY_ALLOC_ERR;
	}

	Position lastPosition = NULL;
	lastPosition = head;
	while (lastPosition->next != NULL)
	{
		lastPosition = lastPosition->next;
	}

	insertPosition(lastPosition, newPerson);

	return 0;
}
// Trazi i ispisuje sve elemente vezane liste sa trazenim prezimenom
int findPersonBySurname(Position head, char* surname)
{

	if (head == NULL || surname == NULL)
	{
		printf("Greska u arugmentima! (findPersonBySurname)");
		return ARG_ERR;
	}

	Position search = NULL;
	search = head->next;

	int matches = 0;
	Position* foundMatching = NULL;
	Position* newFound = NULL;

	while (search != NULL)
	{
		if (strcmp(search->surname, surname) == 0)
		{
			matches++;
			newFound = (Position*)realloc(foundMatching, matches * sizeof(Position));

			if (newFound == NULL)
			{
				printf("Greska pri alokaciji memorije! (findPersonBySurame)");
				free(foundMatching);
				return MEMORY_ALLOC_ERR;
			}
			foundMatching = newFound;
			foundMatching[matches - 1] = search;
		}
		search = search->next;
	}

	if (matches == 0) printf("Nije pronadeno niti jedno prezime: %s u listi!", surname);
	else
	{
		printf("Pronadeno: \n");
		for (int i = 0; i < matches; i++)
		{
			printf("%d.|%s|%s|%d|\n", i + 1, foundMatching[i]->name, foundMatching[i]->surname, foundMatching[i]->birthYear);
		}
	}
	free(foundMatching);
	return 0;
}


// Odspaja i brise sve elemente sa odredenim prezimenom iz vezane liste
int deletePersonBySurname(Position head, char* surname)
{

	if (head == NULL || surname == NULL)
	{
		printf("Greska u argumentima! (deletePersonBySurname)");
		return ARG_ERR;
	}

	Position current = NULL,
		previous = NULL;
	int deletedCount = 0;

	current = head->next;
	previous = head;

	while (current != NULL)
	{
		if (strcmp(current->surname, surname) == 0)
		{
			previous->next = current->next;
			free(current);
			current = previous->next;
			deletedCount++;
		}
		else
		{
			previous = current;
			current = current->next;
		}
	};

	if(deletedCount == 0)
	{
		printf("Nije pronaden niti jedan element sa prezimenom: %s za izbrisati", surname);
	}

	return 0;
}

// Cisti memoriju koju je lista zauzimala / brise sve clanove liste
int freeList(Position head)
{
	if (head == NULL)
	{
		printf("Greska s argumentima! (freeList)");
		return ARG_ERR;
	}

	Position current = NULL,
		next = NULL;

	next = head->next;

	while(next != NULL)
	{
		current = next;
		next = current->next;
		free(current);
	}

	head->next = NULL;

	return 0;
}