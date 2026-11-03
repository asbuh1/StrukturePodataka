/*6. Napisati program koji čita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji
predstavljaju pojedini račun. Na početku svake datoteke je zapisan datum u kojem vremenu je
račun izdat u formatu YYYY-MM-DD. Svaki sljedeći red u datoteci predstavlja artikl u formatu
naziv, količina, cijena. Potrebno je formirati vezanu listu računa sortiranu po datumu. Svaki čvor
vezane liste sadržava vezanu listu artikala sortiranu po nazivu artikla. Nakon toga potrebno je
omogućiti upit kojim će korisnik saznati koliko je novaca sveukupno potrošeno na specifični
artikl u određenom vremenskom razdoblju i u kojoj je količini isti kupljen.*/

#define _CRT_SECURE_NO_WARNINGS

#define MAX_BUFFER 1024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _Articles;
typedef struct _Articles* PosArt;
typedef struct _Articles
{
	char* name;
	int price;
	PosArt next;
}Article;

struct _Receipt;
typedef struct _Receipt* PosRe;
typedef struct _Receipt
{
	char* date;
	PosArt articles;
	PosRe next;
}Receipt;

PosRe makeHeadReceipt();
PosArt makeHeadArticle();
int readEngine(char* filename);
int readReceipt(char* filename, PosRe Receipt);
PosRe makeReceipt();
int insertRecipe(PosRe head, PosRe receipt);
int printRacuniDatum(PosRe head);
int insertElementReceipt(PosRe rec1, PosRe rec2);
int insertElementArticle(PosArt rec1, PosArt rec2);
PosArt makeArticle(char* name, int price);
int insertArticle(PosArt head, PosArt article);
int searchEngine(PosRe head);
int search(PosArt head, char* searchName, int* count, int* priceTotal);
int specialSearch(PosRe head);
int printDates(PosRe head);
int checkChoiceValidity(PosRe head, char* choice);
char * choiceEngine(PosRe head);
PosRe* makeMatchList(PosRe head, char* from, char* to, int* size, char* item);
int printMatchList(PosRe* matchList, int size);
int printTotal(PosRe* matchList, int size, char* word);
int printArticles(PosArt head);
int printReceipt(PosRe head);
int checkDateCompatibility(char* to, char* from);
int simpleSearch(PosArt head, char* item);
int main()
{
	readEngine("racuni.txt");
	return 0;
}

int printDates(PosRe head)
{
	printf("Ispisujem popis datuma! :\n");
	for (PosRe curr = head->next; curr != NULL; curr = curr->next) printf("%s\n", curr->date);
	printf("-----------------------\n");
	return 0;

}

int checkChoiceValidity(PosRe head, char* choice)
{
	for (PosRe curr = head->next; curr != NULL; curr = curr->next) if (strcmp(choice, curr->date) == 0) return 1;
	return 0;
}

char * choiceEngine(PosRe head)
{
	char odabir[50] = { 0 };

	while (1)
	{	
		scanf("%s", odabir);
		if (checkChoiceValidity(head, odabir) == 1) break;
		else
		{
			printf("Datum koji ste odabrali nije ponuden! Pokusajte opet!\n");
		}
	}

	char* returnedValue = (char*)malloc(strlen(odabir) * sizeof(char) + 1);
	sscanf(odabir, "%s", returnedValue);

	return returnedValue;
}

int checkDateCompatibility(char* to, char* from)
{
	if (strcmp(from, to) > 0)
	{
		printf("Nije podrzan format! Napisite ponovno!\n");
		return 1;
	}
	return 0;
}

int specialSearch(PosRe head)
{
	printDates(head);
	
	printf("Napisite razdoblje od kojeg zelite brojati!:");
	char* numFrom = NULL;
	char* numTo = NULL;
	while (1) {
		printf("Od: \n");

		numFrom = choiceEngine(head);

		printf("Do: \n");
		numTo = choiceEngine(head);

		if (checkDateCompatibility(numTo, numFrom) == 0) break;
		free(numFrom);
		free(numTo);
	}
		
	char trazeno[50] = { 0 };
	printf("Koji artikal trazite? \n");
	scanf("%s", trazeno);

	printf("Prikazuje se pronadeni podatci za razdoblje (%s) - (%s)\n", numFrom, numTo);

	int size = 0;
	PosRe* cobanizacija = makeMatchList(head, numFrom, numTo, &size, trazeno);

	printTotal(cobanizacija, size, trazeno);

	printMatchList(cobanizacija, size);
	

	return 0;
}

PosRe* makeMatchList(PosRe head, char* from, char* to, int* size, char* item)
{
	PosRe* matchList = NULL;
	int counter = 0;
	for(PosRe curr = head->next; curr != NULL; curr = curr->next)
	{
	if((strcmp(from, curr->date) <= 0 )&& ((strcmp(to, curr->date) >= 0)) && simpleSearch(curr->articles, item) == 1)
	{
		//printf("TEST ZA ZIVCE");
		counter++;
		PosRe* temp = (PosRe*)realloc(matchList, counter * sizeof(PosRe));
		if (temp == NULL) return NULL;
		temp[counter - 1] = curr;
		matchList = temp;
	}
	}
	*size = counter;

	return matchList;
}

int printMatchList(PosRe* matchList, int size)
{
	for(int i = 0; i < size; i++)
	{
		printReceipt(matchList[i]);
	}

	return 0;
}

int printReceipt(PosRe receipt)
{
	printf("\n-----------------------");
	printf("\n%s\n", receipt->date);
	printArticles(receipt->articles);
	printf("-----------------------");
	return 0;
}
int printArticles(PosArt head)
{
	int nCounter = 0;
	printf("Br. Naziv Cijena\n");
	for(PosArt curr = head->next; curr != NULL; curr = curr->next)
	{
		nCounter++;
		printf("%d. %s %d\n", nCounter, curr->name, curr->price);
	}
	return 0;
}

int printTotal(PosRe* matchList, int size, char* word)
{
	int totalPrice = 0, counter = 0;
	for(int i = 0; i < size; i++)
	{
		int onePrice = 0, oneCounter = 0;
		if (search(matchList[i]->articles, word, &oneCounter, &onePrice) == 100)
		{
			totalPrice += onePrice, counter += oneCounter;
		}
	}
	printf("\nNa %s je u navedenom periodu potroseno (%d), te je artikal kupljen (%d) puta", word, totalPrice, counter);
	return 0;
}

int searchEngine(PosRe head)
{
	char trazeno[50] = { 0 };
	printf("Upisite naziv predmeta kojeg trazite!: ");
	scanf("%s", trazeno);


	
	int brojac = 0, cijena = 0, number = 0;
	for(PosRe curr = head->next; curr != NULL; curr = curr->next)
	{
		
		if (search(curr->articles, trazeno, &number, &cijena) == 100)
		{
			brojac++;
			printf("BR.%d Datum : %s Ukupno: %d, Cijena: %d\n", brojac, curr->date, number, cijena);
		}
	}
	return 0;
}

int simpleSearch(PosArt head, char* item)
{
for(PosArt curr = head->next; curr != NULL; curr = curr->next)
{
	if (strcmp(item, curr->name) == 0) return 1;
}
return 0;
}

int search(PosArt head, char* searchName, int* count, int* priceTotal)
{
	PosArt curr = head;
	int brojac = 0, cijena = 0;
	
		while (curr->next != NULL)
		{

			if (strcmp(curr->next->name, searchName)  == 0)
			{
				
				brojac++;
				cijena += curr->next->price;
			}
			curr = curr->next;
		}
		(*count) = brojac, (* priceTotal) = cijena;
		if (brojac > 0) return 100;
		return 0;
}

int insertElementReceipt(PosRe rec1, PosRe rec2)
{
	rec2->next = rec1->next;
	rec1->next = rec2;
	return 0;
}
int insertElementArticle(PosArt rec1, PosArt rec2)
{
	rec2->next = rec1->next;
	rec1->next = rec2;
	return 0;
}

int readEngine(char* filename)
{
	FILE* file = fopen(filename, "r");
	char buffer[MAX_BUFFER] = { 0 };
	PosRe head = makeHeadReceipt();


	while (fscanf(file, "%s", buffer) != EOF)
	{
		char* fileArticles = (char*)malloc(strlen(buffer) * sizeof(char) + 1);
		strcpy(fileArticles, buffer);

		readReceipt(fileArticles, head);
	}
	// searchEngine(head);
	// printDates(head);
	specialSearch(head);
	
	return 0;

}
int printRacuniDatum(PosRe head)
{
	for (PosRe curr = head->next; curr != NULL; curr = curr->next)
	{
		printf("\nPARCIJALNI TEST: %s", curr->date);

		for(PosArt tren = curr->articles->next; tren != NULL; tren = tren->next)
		{
			printf("\n%s %d", tren->name, tren->price);
		}
	};
	return 0;
}

PosRe makeReceipt()
{
	PosRe receipt = (PosRe)malloc(sizeof(Receipt));
	receipt->articles = NULL;
	receipt->next = NULL;
	receipt->date = NULL;
	return receipt;
}


PosArt makeArticle(char* name, int price)
{
	PosArt article = (PosArt)malloc(sizeof(Article));
	char* temp = (char*)malloc(strlen(name) + 1);
	strcpy(temp, name);
	article->name = temp;
	article->price = price;
	article->next = NULL;
	return article;
}

int insertRecipe(PosRe head, PosRe inserted)
{
		PosRe curr = head;
	

		while (curr->next != NULL && (strcmp(curr->next->date, inserted->date)) < 0)
		{
			curr = curr->next;
		}

		insertElementReceipt(curr, inserted);
	
	return 0;

}

int insertArticle(PosArt head, PosArt article)
{
	PosArt curr = head;
	while (curr->next != NULL && (strcmp(curr->next->name, article->name)) < 0) curr = curr->next;
	insertElementArticle(curr, article);

	return 0;
}

int readReceipt(char* filename, PosRe head)
{

	FILE* file = fopen(filename, "r");

	char buffer[MAX_BUFFER] = { 0 };

	fgets(buffer, MAX_BUFFER, file);
	char* date = (char*)malloc(strlen(buffer) * sizeof(char) + 1);

	PosRe receipt = makeReceipt();
	sscanf(buffer, "%s", date);
	// strcpy(date, buffer);
	receipt->date = date;
	

	insertRecipe(head, receipt);
	PosArt temp = makeHeadArticle();
	receipt->articles = temp;
	while(fgets(buffer, MAX_BUFFER, file) != NULL)
	{
		char name[50] = { 0 };
		int price = 0;
		sscanf(buffer, "%s %d", name, &price);
		
		PosArt article = makeArticle(name, price);
		insertArticle(temp, article);
	}

	return 0;
}

PosRe makeHeadReceipt()
{
	PosRe tempHead = (PosRe)malloc(sizeof(Receipt));
	tempHead->next = NULL;
	tempHead->articles = NULL;
	tempHead->date = NULL;
	return tempHead;
}

PosArt makeHeadArticle()
{
	PosArt tempHead = (PosArt)malloc(sizeof(Article));
	tempHead->next = NULL;
	tempHead->price = 0;
	tempHead->name = NULL;
	return tempHead;
}



