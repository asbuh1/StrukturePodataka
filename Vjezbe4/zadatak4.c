/*
 4. Napisati program za zbrajanje i množenje polinoma.Koeficijenti i eksponenti se
čitaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nužno sortirani.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 1024

#define MEM_ALLOC_ERR (-1)
#define ARG_ERR (-2)
#define FILE_NOT_FOUND_ERR (-3)


struct _Polinom;
typedef struct _Polinom* Position;
typedef struct _Polinom
{
	int exp;
	int coeff;
	Position next;
}Polinom;

Position createPolinomElement(int exp, int coeff);
int insertPolinomElement(Position pos1, Position pos2);
int polinomBuilder(Position head, int exp, int coeff);
int copyPolinom(Position headRes, Position polHead);
int sumPolinoms(Position pol1, Position pol2, Position headRes);
int multiplyPolinoms(Position pol1, Position pol2, Position headRes);

Position* readPolinom(char* filename, int* sizeP);
int selectPolinom(char* filename, Position head, Position* polinoms, int* sizeP);
int printPolinom(Position head);
int printPolinomCheck(Position element, int* isFirstP);
int freeList(Position head);
int freeWrappedListsExclude(Position* list, Position excludedElement, int size);
int printPolinomList(Position* polinoms, int* size);
Position makeHead();

int run(char* filename);
char runOperationPicker();
int runHelpFree(Position pol1, Position pol2, Position res, Position* polinomList, int size);
int runPrintHelp(Position pol1, Position pol2, Position res);


int main()
{

	char* filename = "polinom.txt";

	int status = 0;

	if ((status = run(filename)) < 0) return status;
	return 0;
}

// Pomocna funkcija za trazenje unosa zeljene operacija od korisnika
char runOperationPicker()
{

	char buffer[MAX_BUFFER] = { 0 };
	while (1)
	{
		printf("\nIzaberi operaciju za izvodenje! (*/+): ");
		fgets(buffer, MAX_BUFFER, stdin);

		if (buffer[0] == '*' || buffer[0] == '+') break;
		else
		{
			printf("Operacija nije podrzana! Probajte opet!");
		}
	}
	return buffer[0];
}

// Pomocna funkcija za brisanje memorije u run funkciji
int runHelpFree(Position pol1, Position pol2, Position res, Position * polinomList, int size)
{

	freeWrappedListsExclude(polinomList, NULL, size);
	free(pol1);
	free(pol2);
	free(res);
	free(polinomList);
	return 0;
}

// Pomocna funkcija za racunanje i ispis operacije sa polinomima 
int runPrintHelp(Position pol1, Position pol2, Position res)
{

	if(pol1 == NULL || pol2 == NULL || res == NULL)
	{
		printf("Greska s argumentima! (runPrintHelp)");
		return ARG_ERR;
	}

	char operation = '\0';
	operation = runOperationPicker();

	printf("Izracunato: (");
	printPolinom(pol1);
	printf(") %c (", operation);
	printPolinom(pol2);
	printf(")");
	printf(" = (");

	int status = 0;

	if (operation == '+')
	{
		if ((status = sumPolinoms(pol1, pol2, res)) < 0) return status;
	}
	else if (operation == '*')
	{
		if ((status = multiplyPolinoms(pol1, pol2, res)) < 0) return status;
	}

	printPolinom(res);
	printf(")\n");
	return 0;
}
// Pokrece while petlju za unos i racunanje s polinomima
int run(char* filename)
{

	if(filename == NULL)
	{
		printf("Greska s citanjem filea! (run)");
		return FILE_NOT_FOUND_ERR;
	}

	printf("Ucitan file sa polinomima: %s\n", filename);

	int run_status = 1;
	printf("\n\nIzaberite dva polinoma i operaciju koju zelite izvrisiti (+/*)! Za izlaz iz programa u bilo koji prompt napisite: 'q'\n\n");

	Position pol1 = makeHead();
	Position pol2 = makeHead();
	Position res = makeHead();

	int size = 0;
	Position* polinomList = readPolinom(filename, &size);

	while(run_status)
	{

		printf("\n");
		printPolinomList(polinomList, &size);
		
		int status = 0;

		status = (selectPolinom(filename, pol1, polinomList, &size));

		if (status  < 0 || status == 1)
		{
			
			runHelpFree(pol1, pol2, res, polinomList, size);
			return status;
		}
		
		status = (selectPolinom(filename, pol2, polinomList, &size));
		if (status < 0 || status == 1)
		{
			
			runHelpFree(pol1, pol2, res, polinomList, size);
			return status;
		}

		status = runPrintHelp(pol1, pol2, res);
		if (status < 0)
		{
			runHelpFree(pol1, pol2, res, polinomList, size);
			return status;
		}

		freeList(res);

	}
	runHelpFree(pol1, pol2, res, polinomList, size);
	return 0;
}

// Stvara head
Position makeHead()
{

	Position newHead = (Position)malloc(sizeof(Polinom));
	newHead->coeff = 0, newHead->exp = 0;
	newHead->next = NULL;
	return newHead;
}

// Alocira memorija za novi element polinoma
Position createPolinomElement(int exp, int coeff)
{

	Position newPos = (Position)malloc(sizeof(Polinom));
	if (newPos == NULL) return NULL;

	newPos->exp = exp;
	newPos->coeff = coeff;
	newPos->next = NULL;

	return newPos;
}

// Ubacuje element Polinoma
int insertPolinomElement(Position pos1, Position pos2)
{

	pos2->next = pos1->next;
	pos1->next = pos2;

	return 0;
}

// Sortirani unos polinoma
int polinomBuilder(Position head, int exp, int coeff)
{

	if (head == NULL)
	{
		printf("Greska s argumentima! (polinomBuilder)");
		return ARG_ERR;
	}

	Position newElement = createPolinomElement(exp, coeff);
	Position temp = head;

	while (temp->next != NULL && (newElement->exp < temp->next->exp)) temp = temp->next;

	if (temp->next != NULL && exp == temp->next->exp)
	{
		temp->next->coeff += newElement->coeff;
		free(newElement);

		if (temp->next->coeff == 0)
		{
			Position waste = temp->next;
			temp->next = waste->next;
			free(waste);
		}
	}
	else
	{
		insertPolinomElement(temp, newElement);
	}

	return 0;
}

// Kopira svaki element Polinoma
int copyPolinom(Position headRez, Position polHead)
{

	if (headRez == NULL || polHead == NULL || polHead->next == NULL)
	{
		printf("Greska s argumentima! (copyPolinom)");
		return ARG_ERR;
	}

	int status = 0;
	Position curr = polHead->next;

	while (curr != NULL)
	{
		if ((status = polinomBuilder(headRez, curr->exp, curr->coeff)) < 0) return status;
		curr = curr->next;
	}

	return 0;
}

// Zbraja dva polinoma
int sumPolinoms(Position pol1, Position pol2, Position headRes)
{

	if (pol1 == NULL || pol2 == NULL || headRes == NULL)
	{
		printf("Greska s argumentima! (sumPolinoms)");
		return ARG_ERR;
	}
	
	int status = 0;

	if ((status = copyPolinom(headRes, pol1)) < 0) return status;
	if ((status = copyPolinom(headRes, pol2)) < 0) return status;

	return 0;
};

// Mnozi dva polinoma
int multiplyPolinoms(Position pol1, Position pol2, Position headRes)
{

	if (pol1 == NULL || pol2 == NULL || headRes == NULL)
	{
		printf("Greska s argumentima! (multiplyPolinoms)");
		return ARG_ERR;
	}

	Position p1 = pol1->next;

	while (p1 != NULL)
	{
		Position p2 = pol2->next;

		while (p2 != NULL)
		{
			int koeficijent = p1->coeff * p2->coeff;
			int eksponent = p1->exp + p2->exp;

			int status = 0;
			if ((status = polinomBuilder(headRes, eksponent, koeficijent)) < 0) return status;

			p2 = p2->next;
		}
		p1 = p1->next;
	}
	return 0;
}

// Cita polinome iz filea i vraca dinamicku listu sa polinomima
Position* readPolinom(char* filename, int* sizeP)
{

	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("Greska pri otvaranju filea! (readPolinom)");
		return NULL;
	}

	char buffer[MAX_BUFFER] = { 0 };
	int coeff = 0, exp = 0;

	Position* polinoms = NULL;

	int size = *sizeP;

	while (fgets(buffer, MAX_BUFFER, file) != NULL)
	{

		size++;

		Position head = makeHead();

		if (head == NULL)
		{
			printf("Greska pri alokaciji memorije! (readPolinom)");
			fclose(file);
			return NULL;
		}

		head->exp = 0;
		head->coeff = 0;
		head->next = NULL;

		Position* temp = (Position*)realloc(polinoms, size * sizeof(Position));

		int status = 0;

		if (temp == NULL)
		{
			printf("Greska pri alokaciji memorije! (readPolinom)");

			if ((status = freeWrappedListsExclude(polinoms, NULL, size - 1)) < 0) return NULL;
			fclose(file);

			return NULL;
		}

		polinoms = temp;

		int distance = 0, offset = 0;

		while (sscanf(buffer + offset, "%d %d%n", &coeff, &exp, &distance) == 2)
		{
			offset += distance;
			if ((status = polinomBuilder(head, exp, coeff)) < 0) return NULL;
		}
		polinoms[size - 1] = head;
	}

	*sizeP = size;
	fclose(file);

	return polinoms;
}

// Ispisuje polinome iz liste
int printPolinomList(Position* polinoms, int * size)
{
	for (int i = 0; i < *size; i++)
	{
		printf("%d. ", i + 1);
		int status = 0;
		if ((status = printPolinom(polinoms[i])) < 0) return status;
		printf("\n");
	}
	return 0;
}

// Stavlja odabrani polinom na head
int selectPolinom(char* filename, Position head, Position* polinoms, int* sizeP)
{

	if (filename == NULL || head == NULL)
	{
		printf("Greska s argumentima (selectPolinom)");
		return ARG_ERR;
	}

	int size = *(sizeP);
	int choice = 0;

	while (1)
	{
		char buffer[MAX_BUFFER] = { 0 };

		printf("\nIzaberi jedan od gore navedenih polinoma: ");
		fgets(buffer, MAX_BUFFER, stdin);
		if (buffer[0] == 'q')
		{
			return 1;
		}
		else if (sscanf(buffer, "%d", &choice) == 1)
		{
			if (choice > 0 && choice <= size) break;
		}
		
		printf("\nLos odabir! Probaj opet!\n");

	}
	Position selectedPolinom = polinoms[choice - 1];

	int status = 0;

	head->next = selectedPolinom->next;

	return 0;
}

// Ispisuje polinom
int printPolinom(Position head)
{

	if (head == NULL)
	{
		printf("Greska s argumentima! (printPolinom)");
		return ARG_ERR;
	}

	if (head->next == NULL)
	{
		printf("0");
		return 0;
	}

	Position curr = head->next;
	int isFirst = 1;

	while (curr != NULL)
	{
		int status = 0;
		if ((status = printPolinomCheck(curr, &isFirst)) < 0) return status;
		curr = curr->next;
	}

	return 0;
}

// Pomocna funkcija za ispis clana polinoma
int printPolinomCheck(Position element, int* isFirstP)
{

	if (element == NULL || isFirstP == NULL)
	{
		printf("Greska s argumentima! (printPolinomCheck)");
		return ARG_ERR;
	}

	int isFirst = *isFirstP;

	if (element->coeff != 0)
	{
		if (element->exp == 1)
		{
			if (element->coeff > 0 && isFirst == 0) printf("+");
			printf("%dx", element->coeff);
		}
		else
		{
			if (element->coeff > 0 && isFirst == 0) printf("+");

			if (element->exp == 0) printf("%d", element->coeff);

			else if (abs(element->coeff) == 1)
			{
				if (element->coeff == -1) printf("-");
				printf("x^%d", element->exp);
			}
			else printf("%dx^%d", element->coeff, element->exp);
		}

		isFirst = 0;
		*isFirstP = isFirst;

	}

	return 0;
}

// Pomocna funkcija koja brise dinamicki niz vezanih listi sa mogucnosti ne brisanja odredenog elementa liste
int freeWrappedListsExclude(Position* list, Position excludedElement, int size)
{

	if (list == NULL || size < 0)
	{
		printf("Greska s argumentima(freeWrappedListExclude)");
		return ARG_ERR;
	}

	for (int i = 0; i < size; i++)
	{
		if (list[i] != excludedElement)
		{
			int status = 0;
			if ((status = freeList(list[i])) < 0) return status;
			free(list[i]);
		}
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










