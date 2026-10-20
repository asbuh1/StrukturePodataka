/*
2. Definirati strukturu osoba (ime, prezime, godina rođenja) i napisati program koji:
A. dinamički dodaje novi element na početak liste,
B. ispisuje listu,
C. dinamički dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. briše određeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.

3. Prethodnom zadatku dodati funkcije:
A. dinamički dodaje novi element iza određenog elementa,
B. dinamički dodaje novi element ispred određenog elementa,
C. sortira listu po prezimenima osoba,
D. upisuje listu u datoteku,
E. čita listu iz datoteke.
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 50
#define MAX_BUFFER 1024

#define MEMORY_ALLOC_ERR (-1)
#define ARG_ERR (-2)
#define PERSON_NOT_FOUND_ERR (-3)
#define FILE_ERR (-4)

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
Position* findPersonBySurname(Position head, char* surname);
int deletePersonBySurname(Position head, char* surname);
int freeList(Position head);

int addBehind(Position head, char* findSurname, char* name, char* surname, int birthYear);
int addInFront(Position head, char* findSurname, char* name, char* surname, int birthYear);
int sortListInsertion(Position head);
int sortListBubble(Position head);
int createFileFromList(Position head, char* filename);
int readListFromFile(Position head, char* filename);

Position selectPerson(Position head, char* surname);

int main()
{

	Person head = { 0 };
	Position headP = &head;

	int status = 0;

	if ((status = readListFromFile(headP, "Jagoda.txt")) < 0) return status;

	printList(headP);
	if ((status = addBehind(headP, "Zakic", "Joza", "Enic", 1000)) < 0) return status;

	printf("TEST2 \n");

	printList(headP);

	printf("TEST3 \n");
	if ((status = addInFront(headP, "Anic", "Jakov", "Jakovic", 3000)) < 0) return status;
	printList(headP);
	printf("SORTIRANO\n");
	if ((status = sortListBubble(headP)) < 0) return status;

	printList(headP);

	if ((status = createFileFromList(headP, "Jabuka.txt")) < 0) return status;
	freeList(headP);

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

// Trazi i ispisuje sve elemente vezane liste sa trazenim prezimenom te vraca listu sa svim pronadenim elementima
Position* findPersonBySurname(Position head, char* surname)
{

	if (head == NULL || surname == NULL)
	{
		printf("Greska u arugmentima! (findPersonBySurname)");
		return NULL;
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
				return NULL;
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

	return foundMatching;
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

	if (deletedCount == 0)
	{
		printf("Nije pronaden niti jedan element sa prezimenom: %s za izbrisati", surname);
	}

	return 0;
}

// Pomocna funkcija za addBehind i addInFront, iz pronadenih prezimena sa findPersonBySurname vraca jedno
Position selectPerson(Position head, char* findSurname)
{
	if (head == NULL || findSurname == NULL)
	{
		printf("Greska u argumentima! (selectPerson)");
		return NULL;
	}

	Position curr = NULL;
	Position* found = NULL;
	if ((found = findPersonBySurname(head, findSurname)) == NULL) return NULL;

	printf("Odaberite clana: ");
	int choice = 0;
	scanf("%d", &choice);
	curr = found[choice - 1];
	free(found);
	return curr;
}

// Spaja novi clan u listi iza
int addBehind(Position head, char* findSurname, char* name, char* surname, int birthYear)
{
	if (head == NULL || findSurname == NULL || name == NULL || surname == NULL)
	{
		printf("Greska u argumentima! (addBehind)");
		return ARG_ERR;
	}

	Position curr = NULL;
	Position novi = NULL;

	if ((curr = selectPerson(head, findSurname)) == NULL) return PERSON_NOT_FOUND_ERR;
	if ((novi = createPerson(name, surname, birthYear)) == NULL)
	{
		printf("Greska pri alokacijig novog clana! (addBehind)");
		return MEMORY_ALLOC_ERR;
	};


	insertPosition(curr, novi);
	return 0;

}

// Spaja novi clan u listu ispred trazenog
int addInFront(Position head, char* findSurname, char* name, char* surname, int birthYear)
{

	if (head == NULL || findSurname == NULL || name == NULL || surname == NULL)
	{
		printf("Greska u argumentima! (addBehind)");
		return ARG_ERR;
	}

	Position curr = NULL, prev = NULL;

	if ((curr = selectPerson(head, findSurname)) == NULL) return PERSON_NOT_FOUND_ERR;

	// D:
	for (prev = head; prev->next != curr; prev = prev->next);


	Position novi = NULL;

	if ((novi = createPerson(name, surname, birthYear)) == NULL)
	{
		printf("Greska pri alokacijig novog clana! (addBehind)");
		return MEMORY_ALLOC_ERR;
	};

	novi->next = curr;
	prev->next = novi;

	return 0;
}

// Insertion sort za vezanu listu
int sortListInsertion(Position head)
{
	if (head == NULL)
	{
		printf("Greska s argumentima! (sortListBubble)");
		return ARG_ERR;
	}

	Position curr = NULL, next = NULL;

	curr = head->next;
	head->next = NULL;

	while (curr != NULL)
	{
		next = curr->next;

		Position temp = NULL;
		temp = head;
		while (temp->next != NULL && strcmp(temp->next->surname, curr->surname) < 0) temp = temp->next;
		insertPosition(temp, curr);

		curr = next;
	}
	return 0;
}

// Bubble sort za vezanu listu
int sortListBubble(Position head)
{
	if (head == NULL)
	{
		printf("Greska s argumentima! (sortListBubble)");
		return ARG_ERR;
	}

	Position prevElement = NULL, currElement = NULL, end = NULL;
	Position current = head;
	while (current->next != end)
	{
		prevElement = current;
		currElement = current->next;

		while (currElement->next != end)
		{
			if (strcmp(currElement->surname, currElement->next->surname) > 0)
			{
				Position temp = NULL;

				temp = currElement->next;
				currElement->next = temp->next;
				temp->next = currElement;
				prevElement->next = temp;

				currElement = temp;
			}
			prevElement = currElement;
			currElement = currElement->next;
		}
		end = currElement;
	}
	return 0;
}

// Stvara i upsije podatke iz liste u file
int createFileFromList(Position head, char* filename)
{
	if (head == NULL || filename == NULL)
	{
		printf("Greska s argumentama(createFileFromList)!");
		return ARG_ERR;
	}

	FILE* file = fopen(filename, "w");

	if (file == NULL)
	{
		printf("Greska sa fileom!(createFileFromList)");
		return FILE_ERR;
	}

	fprintf(file, "IME PREZIME GODINA RODENJA\n");
	Position start = NULL;
	for (start = head->next; start != NULL; start = start->next)
	{
		fprintf(file, "%s %s %d\n", start->name, start->surname, start->birthYear);
	}
	fclose(file);
	return 0;
}

// Stvara/Cita vezanu listu iz filea
int readListFromFile(Position head, char* filename)
{
	if (head == NULL || filename == NULL)
	{
		printf("Greska s argumentama(readListFromFile)!");
		return ARG_ERR;
	}

	FILE* file = fopen(filename, "r");

	if (file == NULL)
	{
		printf("Greska pri otvaranju filea!(readListFromFile)");
		return FILE_ERR;
	}

	char buffer[MAX_BUFFER] = { 0 };

	fgets(buffer, MAX_BUFFER, file);

	char name[MAX_LENGTH] = { 0 }, surname[MAX_LENGTH] = { 0 };
	int birthYear = 0;

	while (fscanf(file, "%s %s %d", name, surname, &birthYear) > 0)
	{
		addToBack(head, name, surname, birthYear);
	}

	fclose(file);
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

	while (next != NULL)
	{
		current = next;
		next = current->next;
		free(current);
	}

	head->next = NULL;

	return 0;
}