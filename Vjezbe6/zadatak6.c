/*6. Napisati program koji čita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji
predstavljaju pojedini račun. Na početku svake datoteke je zapisan datum u kojem vremenu je
račun izdat u formatu YYYY-MM-DD. Svaki sljedeći red u datoteci predstavlja artikl u formatu
naziv, količina, cijena. Potrebno je formirati vezanu listu računa sortiranu po datumu. Svaki čvor
vezane liste sadržava vezanu listu artikala sortiranu po nazivu artikla. Nakon toga potrebno je
omogućiti upit kojim će korisnik saznati koliko je novaca sveukupno potrošeno na specifični
artikl u određenom vremenskom razdoblju i u kojoj je količini isti kupljen.*/

#define _CRT_SECURE_NO_WARNINGS

#define MAX_BUFFER 1024
#define MAX_LINE 50


#define FILE_ERR (-1)
#define ARG_ERR (-2)
#define MEM_ALLOC_ERR (-3)
#define USR_ERR (-4)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _Articles;
typedef struct _Articles* PosArt;
typedef struct _Articles
{
	char* name;
	int amount;
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
int fileLoader(PosRe head, char** filenames, int length, PosArt** articleIndexer);
int readReceipt(char* filename, PosRe head, PosArt** articleIndex, int index);
PosRe makeReceipt(char* dateBuffer, int index, PosArt** articleIndex);


int insertRecipe(PosRe head, PosRe inserted, int sortReceiptP);
int insertElementReceipt(PosRe rec1, PosRe rec2);
int insertElementArticle(PosArt rec1, PosArt rec2);
PosArt makeArticle(char* name, int price, int amount);
int insertArticle(PosArt head, PosArt article, int sortArticles);
int search(PosArt head, char* searchName, int* count, int* priceTotal);
int specialSearchListPrint(PosRe head, char* numFrom, char* numTo);
int specialSearch(PosRe head, PosRe* indexer, int length);
int printDates(PosRe head);
int checkChoiceValidity(PosRe head, char* choice, int length, PosRe* indexer);
char* choiceEngine(PosRe head, PosRe* indexer, int length);
PosRe* makeMatchList(PosRe head, char* from, char* to, int* size, char* item);
int printMatchList(PosRe* matchList, int size);
int printTotal(PosRe* matchList, int size, char* word);
int printArticles(PosArt head);
int printReceipt(PosRe head);
int checkDateCompatibility(char** to, char** from);
int simpleSearch(PosArt head, char* item);
int simpleQuestionAction(char* question);
int freeArticle(PosArt head);
int freeReceipt(PosRe head);

int fileSortReload(PosRe head, int sortReceipt, int sortArticle);
int printAllReceipts(PosRe head);
  

int fillIndexer(PosRe head, PosRe** indexer, int length, int** totalPrice, int** totalArticles);
int saveReceipt(PosRe head, char** filenames, PosArt* articleIndex, int length);
int fileSaveReceipt(PosRe receipt, char* filename);
char** filenameLoader(char* filename, int* length);
// Ocisceno svo freeanje zbog lakseg kontroliranja nakon nadogradnje

int editReceipt(PosRe** indexer, int length, int* totalPrice, int* totalArticles);


int main()
{
	/*
	1. Uredite racun - spremaj preko liste koja ce se napraviti pri pocetnom pokretanju, imat ce imena svih fileova i istovremeno jedan sa svim article headovima pointerima, poslije ce traziti sve to dok ne slozi pocetni racuni nazad i ispisati ih kakve ih je file ostavio
	2. Postavke Sortiranja
	3. Trazenje racuna
	4. Ispis racuna
	5. Izlaz

	1. - ispisuje sve racune - Trazi da bira racun po indesku - n. broj po indeksu artikla, bira se indeks, ako je n+1 novi element, ako postojeci trazi kolicinu pa cijenu
	2. - Rastuce/Padajuce - odvojeno za artikle i datume - Po imenima/Po cijeni
	3. - 1. Trazenje specificnog artikla, 2. Ispisi top 3 najskuplja racuna 3. Filtriranje po cijeni
	4. - printAllReceipts
	*/

	PosRe head = makeHeadReceipt();

	PosRe* indexer = NULL;
	int receiptCount = 0;
	PosArt* articleIndex = NULL;
	/*fillIndexer(head, &indexer, &length);

	printf("%s\n", indexer[0]->date);

	fileSortReload(head, -1, -1);
	fillIndexer(head, &indexer, &length);
	printf("%s\n", indexer[0]->date);

	specialSearch(head, indexer, length);

	// fileSortReload(head, -1, -1);
	printf("\n");
	// printAllReceipts(head);
	*/
	char** filenames = filenameLoader("racuni.txt", &receiptCount);
	fileLoader(head, filenames, receiptCount, &articleIndex);
	
	int* totalArticles;
	int* totalPrice = NULL;
	fillIndexer(head, &indexer, receiptCount, &totalPrice, &totalArticles);


	fileSortReload(head, 1, 1);
	printAllReceipts(head);

	printf("\n");
	// fileSaveReceipt(indexer[1], "LOLOLOL.txt");
	// printf("TESTING: %s : %d : %d\n", indexer[0]->date, totalPrice[0], totalArticles[0]);
	
	editReceipt(&indexer, receiptCount, totalPrice, totalArticles);
	//printAllReceipts(head);
	saveReceipt(head, filenames, articleIndex, receiptCount);
}

int editReceipt(PosRe** indexer, int length, int* totalPrice, int* totalArticles)
{

printf("Izaberite koji racun zelite urediti!:\n");
printf("BR. DATUM BRRAZART UKCIJENA\n");
for(int i = 0; i < length; i++)
{
	printf("%d. %s %d %d\n", i+1,(*indexer)[i]->date, totalArticles[i], totalPrice[i]);
}

printf("----------------------------\n");
int choice = 0;
scanf("%d", &choice);

printf("Izabrali ste sljedeci racun!:\n");
printf("br - uredi, > br - dodaj novi, 0 - izlaz\n");
PosRe choiceRe = (*indexer)[choice - 1];
printReceipt(choiceRe);
puts("\n");

length = totalArticles[choice - 1];
scanf("%d", &choice);

char buffer[MAX_BUFFER] = { 0 };
char readBuffer[MAX_BUFFER] = { 0 };
int intBuffer = 0;

PosArt choiceArtHeadBack = choiceRe->articles;
PosArt choiceArtHead = NULL;

if((choice -1) < length && choice > 0)
{
	for(int i = 0; i < choice - 1;i++)
	{
		choiceArtHeadBack = choiceArtHeadBack->next;
	}
	choiceArtHead = choiceArtHeadBack->next;

	choiceArtHeadBack->next = choiceArtHead->next;

	printf("Naziv:\n");
	scanf("%s", readBuffer);

	free(choiceArtHead->name);

	char* temp = (char*)malloc(strlen(readBuffer) + 1);
	strcpy(temp, readBuffer);

	choiceArtHead->name = temp;

	printf("Kolicina:\n");
	scanf("%d", &intBuffer);

	choiceArtHead->amount = intBuffer;

	printf("Cijena:\n");
	scanf("%d", &intBuffer);

	choiceArtHead->price = intBuffer;

	insertArticle(choiceRe->articles, choiceArtHead, 1);
}
else if (choice > length)
{
	int br1 = 0, br2 = 0;
	printf("Naziv:\n");
	scanf("%s", readBuffer);

	printf("Kolicina:\n");
	scanf("%d", &br1);

	printf("Cijena:\n");
	scanf("%d", &br2);


	PosArt newArticle = makeArticle(readBuffer, br2, br1);
	

	insertArticle(choiceRe->articles, newArticle, 1);
}


return 0;
}


// Mijenjana logika indexera u koristenje lengtha od filenameReadera
int fillIndexer(PosRe head, PosRe** indexer, int length, int** totalPrice, int** totalArticles)
{
	PosRe* tempRec = (PosRe*)realloc(*indexer, length * sizeof(PosRe));
	int * tempPrice = (int*)malloc(sizeof(int) * length);
	int* tempArt = (int*)malloc(sizeof(int) * length);



	if (tempRec == NULL)
	{
		printf("Greska pri alokaciji!");
		return MEM_ALLOC_ERR;
	}
	if(tempPrice == NULL)
	{
		printf("Greska pri alokaciji!");
		free(tempRec);
		return MEM_ALLOC_ERR;
	}
	if (tempArt == NULL)
	{
		printf("Greska pri alokaciji!");
		free(tempRec);
		free(tempPrice);
		return MEM_ALLOC_ERR;
	}

	*indexer = tempRec;
	*totalPrice = tempPrice;
	*totalArticles = tempArt;

	int i = 0;
	for (PosRe curr = head->next; curr != NULL; curr = curr->next)
	{
		(*indexer)[i] = curr;
		PosArt currArt = curr->articles->next;
		(*totalPrice)[i] = 0;
		(*totalArticles)[i] = 0;

		while (currArt != NULL)
		{
			
			(*totalArticles) = tempArt;
			(*totalArticles)[i]++;
	

			(*totalPrice)[i] += currArt->amount * currArt->price;
			currArt = currArt->next;
		}

		i++;
	}
	
	return 0;
}

int fileSaveReceipt(PosRe receipt, char* filename)
{
	FILE* file = fopen(filename, "w");

	fprintf(file, receipt->date);
	fputc('\n', file);

	for (PosArt curr = receipt->articles->next; curr != NULL; curr = curr->next)
	{
		fprintf(file, "%s %d %d\n", curr->name, curr->amount, curr->price);
	}
	fclose(file);
	return 0;
}


int saveReceipt(PosRe head, char** filenames, PosArt* articleIndex, int length)
{
	for (int i = 0; i < length; i++)
	{
		PosRe back = head;
		PosRe curr = head->next;
		while (curr != NULL)
		{
			if (curr->articles == articleIndex[i])
			{
				printf("%s : %s\n", filenames[i], curr->date);
				back->next = curr->next;

				fileSaveReceipt(curr, filenames[i]);

				freeArticle(curr->articles);
				free(curr->date);
				free(curr);
				break;
			}

			back = curr;
			curr = curr->next;
		}
	}
	return 0;
}

char** filenameLoader(char* filename, int* length)
{
	FILE* file = fopen(filename, "r");

	char buffer[MAX_BUFFER] = { 0 };
	char readBuffer[MAX_BUFFER] = { 0 };

	char* temp = NULL;
	char** tempFilenames = NULL;
	char** filenames = NULL;
	int receiptCount = 0;

	while (fgets(buffer, MAX_BUFFER, file) != NULL)
	{

		receiptCount++;

		sscanf(buffer, "%s", readBuffer);

		temp = (char*)malloc(strlen(readBuffer) + 1);
		tempFilenames = (char**)realloc(filenames, receiptCount * sizeof(char*));

		strcpy(temp, readBuffer);

		if (tempFilenames == NULL)
		{
			printf("Greska pri alokaciji memorije!");
			return NULL;
		}
		filenames = tempFilenames;
		filenames[receiptCount - 1] = temp;
	}
	*length = receiptCount;
	return filenames;
}


int printAllReceipts(PosRe head)
{
	printf("\n");
	for (PosRe curr = head->next; curr != NULL; curr = curr->next) printReceipt(curr);
	return 0;
}



int fileSortReload(PosRe head, int sortReceipt, int sortArticle)
{
	PosRe currReceipt = head->next, nextReceipt = NULL;
	head->next = NULL;

	while (currReceipt != NULL)
	{
		PosArt headArticles = currReceipt->articles;
		PosArt currArticle = headArticles->next, nextArticle = NULL;

		headArticles->next = NULL;

		while (currArticle != NULL)
		{
			nextArticle = currArticle->next;
			currArticle->next = NULL;

			insertArticle(headArticles, currArticle, sortArticle);

			currArticle = nextArticle;
		}


		nextReceipt = currReceipt->next;
		currReceipt->next = NULL;

		insertRecipe(head, currReceipt, sortReceipt);

		currReceipt = nextReceipt;

	}
	return 0;

}
// Mijenja logiku , filenameovi iz racuni.txt dolaze iz odvojene funkciju

int fileLoader(PosRe head, char** filenames, int length, PosArt** articleIndexer)
{
	if (filenames == NULL || head == NULL)
	{
		printf("Greska s argumentima! (readEngine)");
		return ARG_ERR;
	}

	int status = 0;

	// Dodaj obicnu listu sa svim racunima za printanje datuma i toga

	for (int i = 0; i < length; i++)
	{

		if ((status = readReceipt(filenames[i], head, articleIndexer, i + 1)) < 0)
		{
			freeReceipt(head);
			return status;
		}
	}

	return 0;

}

// Stvara head za tip Receipt
PosRe makeHeadReceipt()
{
	PosRe tempHead = (PosRe)malloc(sizeof(Receipt));
	tempHead->next = NULL;
	tempHead->articles = NULL;
	tempHead->date = NULL;
	return tempHead;
}

// Stvara head za tip Article
PosArt makeHeadArticle()
{
	PosArt tempHead = (PosArt)malloc(sizeof(Article));
	tempHead->next = NULL;
	tempHead->price = 0;
	tempHead->name = NULL;
	tempHead->amount = 0;
	return tempHead;
}

// Stvara prazan Receipt
PosRe makeReceipt(char* dateBuffer, int index, PosArt** articleIndex)
{
	// Mijenjan iz identincnog makeHeadReceiptu da stavlja datum sam

	char* tempDate = (char*)malloc(strlen(dateBuffer) * sizeof(char) + 1);

	if (tempDate == NULL)
	{
		printf("Greska pri alokaciji memorije! (makeReceipt)");
		return NULL;
	}

	sscanf(dateBuffer, "%s", tempDate);

	PosRe receipt = makeHeadReceipt();
	PosArt* tempBuffer = (PosArt*)realloc(*articleIndex, sizeof(PosArt) * index);

	if (tempBuffer == NULL)
	{
		printf("Greksa pri alokaciji!");
		return NULL;
	}

	*articleIndex = tempBuffer;

	receipt->articles = makeHeadArticle();

	(*articleIndex)[index - 1] = receipt->articles;

	receipt->next = NULL;
	receipt->date = tempDate;
	return receipt;
}

// Stvara prazan Article
PosArt makeArticle(char* name, int price, int amount)
{
	if (name == NULL || price < 0)
	{
		printf("Greska s argumentima! (makeArticle)");
		return NULL;
	}

	PosArt article = (PosArt)malloc(sizeof(Article));
	char* temp = (char*)malloc(strlen(name) + 1);

	if (article == NULL || temp == NULL)
	{
		printf("Greska pri alokaciji memorije! (makeArticle)");
		return NULL;
	}

	strcpy(temp, name);
	article->name = temp;
	article->amount = amount;
	article->price = price;
	article->next = NULL;
	return article;
}

// Ubacuje element iza, u Receipt
int insertElementReceipt(PosRe rec1, PosRe rec2)
{
	if (rec1 == NULL || rec2 == NULL)
	{
		printf("Greska s argumentima! (insertElementReceipt)");
		return ARG_ERR;
	}

	rec2->next = rec1->next;
	rec1->next = rec2;
	return 0;
}

// Ubacuje element iza, u Article
int insertElementArticle(PosArt rec1, PosArt rec2)
{
	if (rec1 == NULL || rec2 == NULL)
	{
		printf("Greska s argumentima! (insertElementArticle)");
		return ARG_ERR;
	}

	rec2->next = rec1->next;
	rec1->next = rec2;
	return 0;
}

// Sortira racune prema datumu
int insertRecipe(PosRe head, PosRe inserted, int sortReceipt)
{

	if (head == NULL || inserted == NULL)
	{
		printf("Greska s argumentima! (insertReceipt)");
		return ARG_ERR;
	}

	PosRe curr = head;
	while (curr->next != NULL && (sortReceipt * strcmp(curr->next->date, inserted->date)) < 0) curr = curr->next;

	int status = 0;
	if ((status = insertElementReceipt(curr, inserted)) < 0) return status;

	return 0;

}

// Sortira artikle prema nazivu
int insertArticle(PosArt head, PosArt article, int sortArticles)
{
	if (head == NULL || article == NULL)
	{
		printf("Greska s argumentima! (insertArticle)");
		return ARG_ERR;
	}

	int status = 0;
	PosArt curr = head;
	while (curr->next != NULL && (status = (sortArticles * strcmp(curr->next->name, article->name))) <= 0)
	{
		if (status == 0)
		{
			curr->next->amount += article->amount;
			free(article->name);
			free(article);
			return 0;
		}

		curr = curr->next;
	}
	if ((status = insertElementArticle(curr, article)) < 0) return status;

	return 0;
}

// Cita Racun i artikle iz filea
int readReceipt(char* filename, PosRe head, PosArt** articleIndex, int index)
{
	int sortReceipt = 1, sortArticle = 1;
	if (filename == NULL || head == NULL)
	{
		printf("Greska s argumentima! (readReceipt)");
		return ARG_ERR;
	}

	FILE* file = fopen(filename, "r");

	if (file == NULL)
	{
		printf("Greska pri otvaranju filea! (readReceipt)");
		return FILE_ERR;
	}

	char buffer[MAX_BUFFER] = { 0 };

	fgets(buffer, MAX_BUFFER, file);

	PosRe receipt = makeReceipt(buffer, index, articleIndex);

	int status = 0;

	if ((status = insertRecipe(head, receipt, sortReceipt)) < 0)
	{
		freeReceipt(head);
		fclose(file);
		return status;
	}

	while (fgets(buffer, MAX_BUFFER, file) != NULL)
	{
		char name[MAX_LINE] = { 0 };
		int price = 0, amount = 0;

		sscanf(buffer, "%s %d %d", name, &amount, &price);

		PosArt article = makeArticle(name, price, amount);

		if (article == NULL)
		{
			fclose(file);
			freeReceipt(head);
			return MEM_ALLOC_ERR;
		}

		if ((status = insertArticle(receipt->articles, article, sortArticle)) < 0)
		{
			fclose(file);
			freeReceipt(head);
			return status;
		}
	}

	fclose(file);
	return 0;
}

// Ispisuje datume sa racuna
int printDates(PosRe head)
{
	if (head == NULL)
	{
		printf("Greska s argumentima! (printDates)");
		return ARG_ERR;
	}

	int index = 0;

	printf("Ispisujem popis datuma! :\n");
	for (PosRe curr = head->next; curr != NULL; curr = curr->next) printf("%d. %s\n", index++ + 1, curr->date);
	printf("-----------------------\n");
	return 0;

}

// Provjerava postoji li korisnikov odabir datuma
int checkChoiceValidity(PosRe head, char* choice, int length, PosRe* indexer)
{
	if (head == NULL || choice == NULL)
	{
		printf("Greska s argumentima! (checkChoiceValidity)");
		return ARG_ERR;
	}

	int choiceNum = 0;
	if (sscanf(choice, "%d", &choiceNum) != 1)
	{
		printf("Odaberite broj!\n");
		return 0;
	}
	else if (choiceNum <= 0 || choiceNum > length)
	{
		printf("Odaberite broj unutar trazenog raspona!\n");
		return 0;
	}

	return (choiceNum);
}

// Gradi jednostavna pitanja za d/n odgovore

int simpleQuestionAction(char* question)
{
	if (question == NULL)
	{
		printf("Greska s argumentima! (simpleQuestionAction)");
		return ARG_ERR;
	}

	char doAction = '\0';
	while (1)
	{

		printf(question);

		scanf(" %c", &doAction);

		if (doAction == 'd' || doAction == 'D') return 500;
		else if (doAction == 'n' || doAction == 'N') return 0;

	}
	return 0;
}

// Provjerava kakvi su datumi u odnosu jedan na drugog
int checkDateCompatibility(char** to, char** from)
{
	char* dateTo = *to;
	char* dateFrom = *from;

	if (dateTo == NULL || dateFrom == NULL)
	{
		printf("Greska s argumentima! (checkDateCompatibility)");
		return ARG_ERR;
	}

	if (strcmp(dateFrom, dateTo) > 0)
	{
		printf("Testiram promjenu umjesto greske!");

		*to = dateFrom;
		*from = dateTo;

	}

	return 0;
}

// Ispisuje listu sa trazenim artiklima
int specialSearchListPrint(PosRe head, char* numFrom, char* numTo)
{
	if (head == NULL || numFrom == NULL || numTo == NULL)
	{
		printf("Greska s argumentima! (specialSearchListPrint)");
		return ARG_ERR;
	}

	char trazeno[MAX_LINE] = { 0 };
	printf("Koji artikal trazite? \n");
	scanf("%s", trazeno);



	int size = 0;
	PosRe* matchList = makeMatchList(head, numFrom, numTo, &size, trazeno);

	if (matchList == NULL)
	{
		printf("Nije pronaden niti jedan artikal! Gasim program!\n");
		return USR_ERR;
	}

	printf("Prikazuje se pronadeni podatci za razdoblje (%s) - (%s)\n", numFrom, numTo);

	int status = 0;
	if ((status = printTotal(matchList, size, trazeno)) < 0) return status;
	if ((status = printMatchList(matchList, size)) < 0) return status;

	free(matchList);
	return 0;
}

// Sluzi za biranje datuma
char* choiceEngine(PosRe head, PosRe* indexer, int length)
{
	if (head == NULL)
	{
		printf("Greska s argumentima! (choiceEngine)");
		return NULL;
	}

	char choice[MAX_LINE] = { 0 };
	int isValid = 0;
	while (1)
	{
		scanf("%s", choice);

		if ((isValid = checkChoiceValidity(head, choice, length, indexer)) < 0) return NULL;

		if (isValid >= 1) break;

	}

	char* returnedValue = indexer[isValid - 1]->date;

	return returnedValue;
}

// Pokrece funkciju za biranje i ispis racuna
int specialSearch(PosRe head, PosRe* indexer, int length)
{
	if (head == NULL)
	{
		printf("Greska s argumentima! (specialSearch)");
		return ARG_ERR;
	}

	int remember = 0;
	char* numFrom = NULL;
	char* numTo = NULL;

	while (1)
	{
		if (remember == 0)
		{
			int status = 0;
			if ((status = printDates(head)) < 0) return status;

			printf("Napisite razdoblje od kojeg zelite brojati!:");

			numFrom = NULL;
			numTo = NULL;

			while (1)
			{
				puts("\n");
				printf("Od: \n");

				numFrom = choiceEngine(head, indexer, length);
				printf("Do: \n");
				numTo = choiceEngine(head, indexer, length);

				if (numFrom == NULL || numTo == NULL) return MEM_ALLOC_ERR;

				int isCompatible = checkDateCompatibility(&numTo, &numFrom);
				if (isCompatible == 0) break;

			}
		}

		int status = 0;
		remember = 0;
		if ((status = specialSearchListPrint(head, numFrom, numTo)) < 0) return status;

		if ((status = simpleQuestionAction("\nZelite li ponovno pokrenuti program? d/n: \n")) < 0) return status;
		if (status != 500) break;

		if ((status = simpleQuestionAction("\nZelite li zapamtiti raspon brojeva? d/n: \n")) < 0) return status;
		if (status == 500) remember = 1;

	}
	return 0;
}

// Jednostavna provjera za postojanje naziva u artiklima
int simpleSearch(PosArt head, char* item)
{
	if (head == NULL || item == NULL)
	{
		printf("Greska s argumentima! (simpleSearch)");
		return ARG_ERR;
	}

	for (PosArt curr = head->next; curr != NULL; curr = curr->next)
	{
		if (strcmp(item, curr->name) == 0) return 1;
	}
	return 0;
}

// Provjera za artikle koja racuna ukupni broj ponavljanja i cijenu za specifican niz
int search(PosArt head, char* searchName, int* count, int* priceTotal)
{
	if (head == NULL || searchName == NULL || count == NULL || priceTotal == NULL)
	{
		printf("Greska s argumentima! (search)");
		return ARG_ERR;
	}

	PosArt curr = head;
	int counter = 0, price = 0;

	while (curr->next != NULL)
	{
		if (strcmp(curr->next->name, searchName) == 0)
		{
			counter += curr->next->amount;
			price += curr->next->price * curr->next->amount;
		}
		curr = curr->next;
	}
	(*count) = counter, (*priceTotal) = price;
	if (counter > 0) return 100;
	return 0;
}

// Stvara listu sa trazenim artiklima
PosRe* makeMatchList(PosRe head, char* from, char* to, int* size, char* item)
{
	if (head == NULL || item == NULL || from == NULL || to == NULL || size == NULL)
	{
		printf("Greska s argumentima! (makeMatchList)");
		return NULL;
	}

	PosRe* matchList = NULL;
	int counter = 0;
	for (PosRe curr = head->next; curr != NULL; curr = curr->next)
	{
		int status = simpleSearch(curr->articles, item);
		if (status < 0) return NULL;

		if ((strcmp(from, curr->date) <= 0) && ((strcmp(to, curr->date) >= 0)) && status == 1)
		{
			counter++;
			PosRe* temp = (PosRe*)realloc(matchList, counter * sizeof(PosRe));
			if (temp == NULL)
			{
				free(matchList);
				return NULL;
			}
			temp[counter - 1] = curr;
			matchList = temp;
		}
	}

	if (counter == 0) return NULL;

	*size = counter;

	return matchList;
}

// Ispisuje listu sa trazenim artiklima
int printMatchList(PosRe* matchList, int size)
{
	if (size == 0 || matchList == NULL)
	{
		printf("Greska s argumentima! (printMatchList)");
		return ARG_ERR;
	}
	int status = 0;
	for (int i = 0; i < size; i++)
	{
		if ((status = printReceipt(matchList[i])) < 0) return status;
	}
	return 0;
}

// Ispisuje formatiran racun
int printReceipt(PosRe receipt)
{
	if (receipt == NULL)
	{
		printf("Greska s argumentima! (printReceipt)");
		return ARG_ERR;
	}

	printf("\n-----------------------");
	printf("\n%s\n", receipt->date);
	int status = 0;
	if ((status = printArticles(receipt->articles)) < 0) return status;
	printf("-----------------------");
	return 0;
}

// Ispisuje artikle
int printArticles(PosArt head)
{
	if (head == NULL)
	{
		printf("Greska s argumentima! (printArticles)");
		return ARG_ERR;
	}

	int nCounter = 0;
	printf("Br. Naziv Kolicina Cijena\n");
	for (PosArt curr = head->next; curr != NULL; curr = curr->next)
	{
		nCounter++;
		printf("%d. %s %d %d\n", nCounter, curr->name, curr->amount, curr->price);
	}
	return 0;
}

// Ispisuje broj pojavljivanja i ukupnu cijenu u racunima
int printTotal(PosRe* matchList, int size, char* word)
{
	if (matchList == NULL || size == 0 || word == NULL)
	{
		printf("Greska s argumentima! (printTotal)");
		return ARG_ERR;
	}

	int totalPrice = 0, counter = 0;
	for (int i = 0; i < size; i++)
	{
		int onePrice = 0, oneCounter = 0;
		int status = search(matchList[i]->articles, word, &oneCounter, &onePrice);
		if (status < 0) return status;

		if (status == 100) totalPrice += onePrice, counter += oneCounter;

	}
	printf("\nNa %s je u navedenom periodu potroseno (%d), te je artikal kupljen (%d) puta", word, totalPrice, counter);
	return 0;
}

// Cisti artikle iz memorije
int freeArticle(PosArt head)
{
	PosArt curr = head;
	while (curr != NULL)
	{
		PosArt next = curr->next;
		free(curr->name);
		free(curr);
		curr = next;
	}
	return 0;
}

// Cisti racun iz memorije
int freeReceipt(PosRe head)
{
	PosRe curr = head;
	while (curr != NULL)
	{
		PosRe next = curr->next;
		free(curr->date);
		freeArticle(curr->articles);
		free(curr);
		curr = next;
	}
	return 0;
}