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
PosRe* makeMatchList(PosRe head, char* from, char* to, int* size, char* item, char use);
int printMatchList(PosRe* matchList, int size);
int printTotal(PosRe* matchList, int size, char* word);
int printArticles(PosArt head);
int printReceipt(PosRe head);
int checkDateCompatibility(char** to, char** from);
int simpleSearch(PosArt head, char* item);
int simpleQuestionAction(char* question);
int freeArticles(PosArt head);
int freeArticle(PosArt ad);
int freeReceipts(PosRe head);
int freeReceipt(PosRe ad);
int fileSortReload(PosRe head, int sortReceipt, int sortArticle);
int printAllReceipts(PosRe head);
  

int fillIndexer(PosRe head, PosRe** indexer, int length, int** totalPrice, int** totalArticles);
int saveReceipt(PosRe head, char** filenames, PosArt* articleIndex, int length);
int fileSaveReceipt(PosRe receipt, char* filename);
char** filenameLoader(char* filename, int* length);
// Ocisceno svo freeanje zbog lakseg kontroliranja nakon nadogradnje

int editReceipt(PosRe** indexer, int length, int* totalPrice, int* totalArticles);
int filterByPrice(int* totalPrice, int length, PosRe* index);
int averageSpend(int length, int* totalPrice);
int totalArticlesPeriod(PosRe head, PosRe* indexer, int length);
int totalArticlesPeriodFind(PosRe head, char* numFrom, char* numTo);

int dateChoice(PosRe head, PosRe* indexer, int length, char** numFromP, char** numToP);
int totalArticleAlloc(int** tempIndex, int size);
int editReceiptInput(int* num1, int* num2, char* buffer);

int findMinMax(int* totalIndex, int length, PosRe* index, int isMax, char* wanted);

int callError(int code, char* location);

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

	

	// fileSortReload(head, -1, -1);
	printf("\n");
	// printAllReceipts(head);
	*/
	char** filenames = filenameLoader("racuni.txt", &receiptCount);
	int* totalArticles = NULL;
	int* totalPrice = NULL;

	fileLoader(head, filenames, receiptCount, &articleIndex);
	fillIndexer(head, &indexer, receiptCount, &totalPrice, &totalArticles);
	// specialSearch(head, indexer, receiptCount);

	//totalArticlesPeriod(head, indexer, receiptCount);


	// editReceipt(&indexer, receiptCount, totalPrice, totalArticles);
	// saveReceipt(head, filenames, articleIndex, receiptCount);
	// filterByPrice(totalPrice, receiptCount, indexer);
	// findMax(totalPrice, receiptCount, indexer);
	// findMostUniqueArticles(receiptCount, indexer, totalArticles);
	// averageSpend(receiptCount, totalPrice);
	/*
	fileSortReload(head, 1, 1);
	printAllReceipts(head);

	printf("\n");
	// fileSaveReceipt(indexer[1], "LOLOLOL.txt");
	// printf("TESTING: %s : %d : %d\n", indexer[0]->date, totalPrice[0], totalArticles[0]);
	
	editReceipt(&indexer, receiptCount, totalPrice, totalArticles);
	//printAllReceipts(head);
	 */
}

// UK aritkli u periodu;

// Greske
int callError(int code, char* location)
{
	switch (code)
	{
	case FILE_ERR: 
		printf("Greska pri otvaranju filea! (%s)\n", location);
		return FILE_ERR;
	case ARG_ERR:
		printf("Greska s argumentima! (%s)\n", location);
		return ARG_ERR;
	case MEM_ALLOC_ERR:
		printf("Greska pri alokaciji memorije! (%s)\n", location);
		return MEM_ALLOC_ERR;
	case USR_ERR:
		printf("Greska s podatcima!(%s)\n", location);
		return USR_ERR;
	default:
		break;
	}

	return 0;

}

// Alokator za totalArticlesPeriodFind
int totalArticleAlloc(int ** tempIndex, int size)
{
	if (tempIndex == NULL) return ARG_ERR;

	int* temp = (int*)realloc(*tempIndex, sizeof(int) * size);

	if (temp == NULL) return MEM_ALLOC_ERR;

	*tempIndex = temp;

	return 0;
 
}

// Funkcija za trazenje svih racuna u periodu, trazi posebno spominjajne artikala + kolicinu
int totalArticlesPeriodFind(PosRe head, char* numFrom, char* numTo)
{

	if (head == NULL || numFrom == NULL || numTo == NULL) return ARG_ERR;

	int* amount = NULL;
	char** articleName = NULL;
	int length = 0, z = 0, status = 0;
	
	int* priceTot = NULL;
	int* nOfUniqueMentions = NULL;

	char** tempArticleName = NULL;

	PosRe* matchList = makeMatchList(head, numFrom, numTo, &length, "fdf", 'i');
	if (matchList == NULL) return MEM_ALLOC_ERR;

	for(int i = 0; i < length; i++)
	{
		PosArt curr = matchList[i]->articles->next;
		printf("%s", curr->name);
		while (curr != NULL)
		{
			int flag = 0;
			for(int b = 0; b < z; b++)
			{
				if (z == 0) break;

				if (strcmp(articleName[b], curr->name) == 0)
				{
					flag = 1;
					amount[b] += curr->amount;
					priceTot[b] += curr->price;
					nOfUniqueMentions[b]++;
					break;
				}
			}
			if (flag == 0)
			{
				z++;
				tempArticleName = (char**)realloc(articleName, sizeof(char*) * z);
				if (tempArticleName == NULL) return MEM_ALLOC_ERR;

				if ((status = totalArticleAlloc(&amount, z)) < 0)
				{
					free(tempArticleName);
					return MEM_ALLOC_ERR;
				}
			
				if ((status = totalArticleAlloc(&nOfUniqueMentions, z)) < 0)
				{
					free(tempArticleName);
					free(amount);
					return MEM_ALLOC_ERR;
				}

				if ((status = totalArticleAlloc(&priceTot, z)) < 0)
				{
					free(tempArticleName);
					free(amount);
					free(nOfUniqueMentions);
					return MEM_ALLOC_ERR;
				}

				articleName = tempArticleName;
				
				amount[z - 1] = curr->amount;
				articleName[z - 1] = curr->name;

				priceTot[z - 1] = curr->price;
				nOfUniqueMentions[z - 1] = 1;
			}
			curr = curr->next;
		}
	}

	printf("\nPronadeni su sljedeci artikli za period (%s)-(%s)\n", numFrom, numTo);
	printf("NAZIV KOL PROSJECNA CIJENA\n");
	for(int i = 0; i < z; i++)
	{
		printf("%s %d %.2f\n", articleName[i], amount[i], (float)priceTot[i] / nOfUniqueMentions[i]);
	}

	return 0;
}

// Birac datuma 
int dateChoice(PosRe head, PosRe* indexer, int length, char ** numFromP, char** numToP)
{
	
	if (head == NULL || indexer == NULL || numFromP == NULL || numToP == NULL) return ARG_ERR;

	int status = 0;
	if ((status = printDates(head)) < 0) return status;

	printf("Napisite razdoblje od kojeg zelite brojati!:");

	char* numFrom = NULL;
	char* numTo = NULL;

	while (1)
	{
		free(numFrom);
		free(numTo);

		puts("\n");
		printf("Od: \n");

		numFrom = choiceEngine(head, indexer, length);
		printf("Do: \n");
		numTo = choiceEngine(head, indexer, length);

		if (numFrom == NULL || numTo == NULL) return MEM_ALLOC_ERR;

		int isCompatible = checkDateCompatibility(&numTo, &numFrom);
		if (isCompatible == 0) break;
	}

	*numToP = numTo;
	*numFromP = numFrom;

	return 0;
}

// Daje prosjecnu potrosnju/cijenu po artiklu u nekom periodu
int totalArticlesPeriod(PosRe head, PosRe* indexer, int length)
{

	char funcName[] = "totalArticlesPeriod";
	if (head == NULL || indexer == NULL) return callError(ARG_ERR, funcName);

	int remember = 0, status = 0;
	char* numFrom = NULL;
	char* numTo = NULL;
	while (1)
	{
		if (remember == 0)
		{
			if((status = dateChoice(head, indexer, length, &numFrom, &numTo)) < 0) callError(status, funcName);
		}

		if ((status = totalArticlesPeriodFind(head, numFrom, numTo)) < 0)
		{
			free(numFrom);
			free(numTo);
			callError(status, funcName);
		}

		remember = 0;

		if ((status = simpleQuestionAction("\nZelite li ponovno pokrenuti program? d/n: \n")) < 0) return callError(status, funcName);
		if (status != 500) break;

		if ((status = simpleQuestionAction("\nZelite li zapamtiti raspon brojeva? d/n: \n")) < 0) return callError(status, funcName);
		if (status == 500) remember = 1;

	}
	return 0;
}

// Prosjecna potrosnja po racunu
int averageSpend(int length, int* totalPrice)
{
	if (totalPrice == NULL) return callError(ARG_ERR, "averageSpend");

	int total = 0;
	for(int i = 0; i < length; i++)
	{
		total += totalPrice[i];
	}
	double avg = (double)total / length;
	printf("\nProsjecna potrosnja po racunu je: %.2lf\n", avg);
	return 0;
}

// Moze - pronaci najskuplji/najjeftiniji racun za bilo sto sto ima svoju index listu; kolicinu unikatnih artikala po racunu, cijenu racuna, bilo akkave prosjeke
int findMinMax(int* totalIndex, int length, PosRe* index, int isMax, char* wanted)
{
	if (totalIndex == NULL || index == NULL || wanted == NULL) return callError(ARG_ERR, "findMinMax");

	int wantIndex = 0, status = 0;
	
	if (isMax == 1)
	{
		for (int i = 1; i < length; i++) if (totalIndex[wantIndex] < totalIndex[i]) wantIndex = i;
	}
	else if (isMax == 0)
	{
		for (int i = 1; i < length; i++) if (totalIndex[wantIndex] > totalIndex[i]) wantIndex = i;
	}
	printf("\nIspisujem %s!\n", wanted);
	if ((status = printReceipt(index[wantIndex]) ) < 0) return callError(status, "findMinMax");

	return 0;
}

// Filtrira po uk. cijenama racuna
int filterByPrice(int* totalPrice, int length, PosRe* index)
{
	if (totalPrice == NULL || index == NULL) return callError(ARG_ERR, "filterByPrice");

	int max = 0, min = 0;

	printf("\nIzaberite najmanju cijenu: \n");
	scanf("%d", &min);
	printf("Dodajte najvecu cijenu: \n");
	scanf("%d", &max);
	
	printf("Ispisujem sve racune sa odgovorajucim cijenama!\n");

	for(int i = 0; i < length; i++)
	{
		int price = totalPrice[i];
		if(price >= min && price <= max)
		{
			printReceipt(index[i]);
			printf("\nUkupno: %d\n", price);
		}
	}
	return 0;
}

// Trazi informacije za uredivanje artikla na racunu
int editReceiptInput(int * amount, int* price, char* buffer)
{
	if (amount == NULL || price == NULL || buffer == NULL) return ARG_ERR;

	printf("Naziv:\n");
	scanf("%s", buffer);

	printf("Kolicina:\n");
	scanf("%d", amount);

	printf("Cijena:\n");
	scanf("%d", price);

	return 0;
}

// Birac za broj
int getNumChoice()
{
	char buffer[MAX_BUFFER] = { 0 };
	int choice = 0;
	fgets(buffer, MAX_BUFFER, stdin);
	while (sscanf(buffer, "%d", &choice) != 1)
	{
		printf("Upisite broj!:\n");
		fgets(buffer, MAX_BUFFER, stdin);
	}
	return choice;
}

// Omogucuje uredivanje racuna
int editReceipt(PosRe** indexer, int length, int* totalPrice, int* totalArticles)
{
	if (indexer == NULL || totalPrice == NULL || totalArticles == NULL) return callError(ARG_ERR, "editReceipt");

	printf("\nIzaberite koji racun zelite urediti!:\n");
	printf("BR. DATUM BRRAZART UKCIJENA\n");
	for(int i = 0; i < length; i++)
	{
		printf("%d. %s %d %d\n", i+1,(*indexer)[i]->date, totalArticles[i], totalPrice[i]);
	}

	printf("----------------------------\n");
	printf("0 - izlaz\n\n");

	int choice = getNumChoice(), status = 0;
	
	if (choice <= 0) return 0;

	printf("Izabrali ste sljedeci racun!:\n");
	printf("br - uredi, > br - dodaj novi, 0 - izlaz\n");
	PosRe choiceRe = (*indexer)[choice - 1];
	printReceipt(choiceRe);
	puts("\n");

	length = totalArticles[choice - 1];

	char buffer[MAX_BUFFER] = { 0 };

	PosArt choiceArtHeadBack = choiceRe->articles;
	PosArt choiceArtHead = NULL;

	choice = getNumChoice();

	if((choice -1) < length && choice > 0)
	{
		for(int i = 0; i < choice - 1; i++) choiceArtHeadBack = choiceArtHeadBack->next;

		choiceArtHead = choiceArtHeadBack->next;
		choiceArtHeadBack->next = choiceArtHead->next;
		free(choiceArtHead->name);

		int tempAmount = 0, tempPrice = 0;

		if((status = editReceiptInput(&tempAmount, &tempPrice, buffer)) < 0) return callError(status, "editReceipt");

		char* temp = (char*)malloc(strlen(buffer) + 1);
		if (temp == NULL) return callError(MEM_ALLOC_ERR, "editReceipt");

		strcpy(temp, buffer);

		choiceArtHead->name = temp, choiceArtHead->amount = tempAmount, choiceArtHead->price = tempPrice;
	
		if ((status = insertArticle(choiceRe->articles, choiceArtHead, 1)) < 0) return callError(status, "editReceipt");

	}
	else if (choice > length)
	{
	int tempAmount = 0, tempPrice = 0;

	if ((status = editReceiptInput(&tempAmount, &tempPrice, buffer)) < 0) return callError(status, "editReceipt");
	
	PosArt newArticle = makeArticle(buffer, tempPrice, tempAmount);
	if (newArticle == NULL) return callError(MEM_ALLOC_ERR, "editReceipt");

	if ((status = insertArticle(choiceRe->articles, newArticle, 1)) < 0) return callError(status, "editReceipt");
	}
	return 0;
}

// cuva indekse od svih racuna u obicnoj listi zbog lakseg biranja datuma, printanja, rada s nekim specificnim podatcima
int fillIndexer(PosRe head, PosRe** indexer, int length, int** totalPrice, int** totalArticles)
{
	free(*indexer);
	free(*totalPrice);
	free(*totalArticles);

	PosRe* tempRec = (PosRe*)malloc(length * sizeof(PosRe));
	int * tempPrice = (int*)malloc(sizeof(int) * length);
	int* tempArt = (int*)malloc(sizeof(int) * length);

	if (tempRec == NULL) return callError(MEM_ALLOC_ERR, "fillIndexer");
	if(tempPrice == NULL)
	{
		free(tempRec);
		return callError(MEM_ALLOC_ERR, "fillIndexer");
	}
	if (tempArt == NULL)
	{
		free(tempRec);
		free(tempPrice);
		return callError(MEM_ALLOC_ERR, "fillIndexer");
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
			(*totalArticles)[i]++;
			(*totalPrice)[i] += currArt->amount * currArt->price;
			currArt = currArt->next;
		}
		i++;
	}
	return 0;
}

// Pisac racuna na file
int fileSaveReceipt(PosRe receipt, char* filename)
{
	if (receipt == NULL || filename == NULL) return ARG_ERR;

	FILE* file = fopen(filename, "w");
	if (file == NULL) return FILE_ERR;

	fprintf(file, "%s\n", receipt->date);

	for (PosArt curr = receipt->articles->next; curr != NULL; curr = curr->next) fprintf(file, "%s %d %d\n", curr->name, curr->amount, curr->price);
	
	fclose(file);
	return 0;
}

// Trazi originalne racune iz njihovih originalnih lokacija i sprema ih nazad na mjesta
int saveReceipt(PosRe head, char** filenames, PosArt* articleIndex, int length)
{
	if (head == NULL || filenames == NULL || articleIndex == NULL) return callError(ARG_ERR, "saveReceipt");
	int status = 0;

	for (int i = 0; i < length; i++)
	{
		PosRe back = head, curr = head->next;
		while (curr != NULL)
		{
			if (curr->articles == articleIndex[i])
			{
				printf("%s : %s\n", filenames[i], curr->date);
				back->next = curr->next;

				if((status = fileSaveReceipt(curr, filenames[i])) < 0) return callError(status,"saveReceipt");

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
// Ucitaje i otvara sve filenameove iz racuni.txt
char** filenameLoader(char* filename, int* length)
{
	if (filename == NULL || length == NULL) return NULL;

	FILE* file = fopen(filename, "r");

	if (file == NULL) return NULL;

	char buffer[MAX_BUFFER] = { 0 }, readBuffer[MAX_BUFFER] = { 0 };

	char* temp = NULL;
	char** tempFilenames = NULL;
	char** filenames = NULL;
	int receiptCount = 0;

	while (fgets(buffer, MAX_BUFFER, file) != NULL)
	{
		receiptCount++;

		sscanf(buffer, "%s", readBuffer);

		temp = (char*)malloc(strlen(readBuffer) + 1);
		if (temp == NULL)
		{
			for (int i = 0; i < receiptCount - 1; i++) free(filenames[i]);
			free(filenames);
			fclose(file);
			return NULL;
		}
		tempFilenames = (char**)realloc(filenames, receiptCount * sizeof(char*));

		if(tempFilenames == NULL)
		{

			for (int i = 0; i < receiptCount - 1; i++) free(filenames[i]);
			free(filenames);
			fclose(file);
			free(temp);
			return NULL;
		}

		strcpy(temp, readBuffer);
		
		filenames = tempFilenames;
		filenames[receiptCount - 1] = temp;
	}
	*length = receiptCount;
	fclose(file);
	return filenames;
}

// Ispisuje sve racune
int printAllReceipts(PosRe head)
{
	if (head == NULL) return ARG_ERR;
	printf("\n");
	for (PosRe curr = head->next; curr != NULL; curr = curr->next) printReceipt(curr);
	return 0;
}

// Ponovno ucitaje file da bi ga mogao sortirati po novom
int fileSortReload(PosRe head, int sortReceipt, int sortArticle)
{
	if (head == NULL) return ARG_ERR;
	int status = 0;

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

			if ((status = insertArticle(headArticles, currArticle, sortArticle))<0) return status;

			currArticle = nextArticle;
		}

		nextReceipt = currReceipt->next;
		currReceipt->next = NULL;

		if ((status = insertRecipe(head, currReceipt, sortReceipt)) < 0) return status;
		
		currReceipt = nextReceipt;

	}
	return 0;
}

// Mijenja logiku , filenameovi iz racuni.txt dolaze iz odvojene funkciju

int fileLoader(PosRe head, char** filenames, int length, PosArt** articleIndexer)
{
	if (filenames == NULL || head == NULL) return callError(ARG_ERR, "fileLoader");
	
	int status = 0;

	for (int i = 0; i < length; i++)
	{
		if ((status = readReceipt(filenames[i], head, articleIndexer, i + 1)) < 0)
		{
			freeReceipts(head);
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

// Stvara prazan Receipt i sprema head artikla u indeks artikla
PosRe makeReceipt(char* dateBuffer, int index, PosArt** articleIndex)
{
	if (dateBuffer == NULL || articleIndex == NULL) return NULL;

	char* tempDate = (char*)malloc(strlen(dateBuffer) * sizeof(char) + 1);

	if (tempDate == NULL) return NULL;
	
	sscanf(dateBuffer, "%s", tempDate);

	PosRe receipt = makeHeadReceipt();
	PosArt* tempBuffer = (PosArt*)realloc(*articleIndex, sizeof(PosArt) * index);

	if (tempBuffer == NULL) return NULL;
	
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
	if (name == NULL || price < 0) return NULL;

	PosArt article = (PosArt)malloc(sizeof(Article));
	if (article == NULL) return NULL;

	char* temp = (char*)malloc(strlen(name) + 1);
	if (temp == NULL) return NULL;

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
	if (rec1 == NULL || rec2 == NULL) return ARG_ERR;

	rec2->next = rec1->next;
	rec1->next = rec2;
	return 0;
}

// Ubacuje element iza, u Article
int insertElementArticle(PosArt rec1, PosArt rec2)
{
	if (rec1 == NULL || rec2 == NULL) return ARG_ERR;
	
	rec2->next = rec1->next;
	rec1->next = rec2;
	return 0;
}

// Sortira racune prema datumu
int insertRecipe(PosRe head, PosRe inserted, int sortReceipt)
{
	if (head == NULL || inserted == NULL) return ARG_ERR;

	PosRe curr = head;
	while (curr->next != NULL && (sortReceipt * strcmp(curr->next->date, inserted->date)) < 0) curr = curr->next;

	int status = 0;
	if ((status = insertElementReceipt(curr, inserted)) < 0) return status;

	return 0;

}

// Sortira artikle prema nazivu
int insertArticle(PosArt head, PosArt article, int sortArticles)
{
	if (head == NULL || article == NULL) return ARG_ERR;

	int status = 0;
	PosArt curr = head;

	if (sortArticles == 3) 
	{
		while (curr->next != NULL && (curr->next->price < article->price)) curr = curr->next;
	}
	else if (sortArticles == 4)
	{
		while (curr->next != NULL && (curr->next->price > article->price)) curr = curr->next;
	}

	else 
	{
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
	}

	return insertElementArticle(curr, article);
}

// Cita Racun i artikle iz filea
int readReceipt(char* filename, PosRe head, PosArt** articleIndex, int index)
{
	if (filename == NULL || head == NULL) return callError(ARG_ERR, "readReceipt");

	int sortReceipt = 1, sortArticle = 1, status = 0;

	FILE* file = fopen(filename, "r");

	if (file == NULL) return callError(FILE_ERR, "readReceipt");
	
	char buffer[MAX_BUFFER] = { 0 };
	fgets(buffer, MAX_BUFFER, file);
	PosRe receipt = makeReceipt(buffer, index, articleIndex);

	if(receipt == NULL)
	{
		fclose(file);
		return callError(MEM_ALLOC_ERR, "readReceipt");
	}

	if ((status = insertRecipe(head, receipt, sortReceipt)) < 0)
	{
		freeReceipt(receipt);
		fclose(file);
		return callError(status, "readReceipt");
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
			return callError(MEM_ALLOC_ERR, "readReceipt");
		}

		if ((status = insertArticle(receipt->articles, article, sortArticle)) < 0)
		{
			freeArticle(article);
			fclose(file);
			return callError(status, "readReceipt");
		}
	}

	fclose(file);
	return 0;
}

// Ispisuje datume sa racuna
int printDates(PosRe head)
{
	if (head == NULL) return ARG_ERR;
	int index = 0;

	printf("Ispisujem popis datuma! :\n");
	for (PosRe curr = head->next; curr != NULL; curr = curr->next) printf("%d. %s\n", index++ + 1, curr->date);
	printf("-----------------------\n");
	return 0;

}

// Provjerava postoji li korisnikov odabir datuma
int checkChoiceValidity(PosRe head, char* choice, int length, PosRe* indexer)
{
	if (head == NULL || choice == NULL) return ARG_ERR;
	
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
	if (question == NULL) return ARG_ERR;
	
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
	if (to == NULL || from == NULL) return ARG_ERR;
	
	char* dateTo = *to;
	char* dateFrom = *from;

	if (strcmp(dateFrom, dateTo) > 0)
	{
		*to = dateFrom;
		*from = dateTo;
	}
	return 0;
}

// Ispisuje listu sa trazenim artiklima
int specialSearchListPrint(PosRe head, char* numFrom, char* numTo)
{
	if (head == NULL || numFrom == NULL || numTo == NULL) return ARG_ERR;
	
	char trazeno[MAX_LINE] = { 0 };
	printf("Koji artikal trazite? \n");
	scanf("%s", trazeno);

	int size = 0;
	PosRe* matchList = makeMatchList(head, numFrom, numTo, &size, trazeno, 's');

	if (matchList == NULL) return USR_ERR;

	printf("Prikazuje se pronadeni podatci za razdoblje (%s) - (%s)\n", numFrom, numTo);

	int status = 0;
	if ((status = printTotal(matchList, size, trazeno)) < 0)
	{
		free(matchList);
		return status;
	}
	if ((status = printMatchList(matchList, size)) < 0)
	{
		free(matchList);
		return status;
	}
	free(matchList);
	return 0;
}

// Sluzi za biranje datuma
char* choiceEngine(PosRe head, PosRe* indexer, int length)
{
	if (head == NULL || indexer == NULL) return NULL;
	
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
	if (head == NULL) return callError(ARG_ERR, "specialSearch");
	
	int remember = 0, status = 0;
	char* numFrom = NULL;
	char* numTo = NULL;

	while (1)
	{
		if (remember == 0)
		{
			if ((status = dateChoice(head, indexer, length, &numFrom, &numTo)) < 0) return callError(status, "specialSearch");
		}
		remember = 0;
		if ((status = specialSearchListPrint(head, numFrom, numTo)) < 0) return callError(status, "specialSearch");

		if ((status = simpleQuestionAction("\nZelite li ponovno pokrenuti program? d/n: \n")) < 0) return callError(status, "specialSearch");
		if (status != 500) break;

		if ((status = simpleQuestionAction("\nZelite li zapamtiti raspon brojeva? d/n: \n")) < 0) return callError(status, "specialSearch");
		if (status == 500) remember = 1;
	}
	return 0;
}

// Jednostavna provjera za postojanje naziva u artiklima
int simpleSearch(PosArt head, char* item)
{
	if (head == NULL || item == NULL) return ARG_ERR;
	
	for (PosArt curr = head->next; curr != NULL; curr = curr->next) if (strcmp(item, curr->name) == 0) return 1;

	return 0;
}

// Provjera za artikle koja racuna ukupni broj ponavljanja i cijenu za specifican niz
int search(PosArt head, char* searchName, int* count, int* priceTotal)
{
	if (head == NULL || searchName == NULL || count == NULL || priceTotal == NULL) return ARG_ERR;

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
PosRe* makeMatchList(PosRe head, char* from, char* to, int* size, char* item, char use)
{
	if (head == NULL || item == NULL || from == NULL || to == NULL || size == NULL) return NULL;
	
	PosRe* matchList = NULL;
	int counter = 0;
	for (PosRe curr = head->next; curr != NULL; curr = curr->next)
	{
		int status = 0;
		
		status = (use == 's') ? simpleSearch(curr->articles, item) : 1;

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
	if (size == 0 || matchList == NULL) return ARG_ERR;

	int status = 0;
	for (int i = 0; i < size; i++) if ((status = printReceipt(matchList[i])) < 0) return status;
	return 0;
}

// Ispisuje formatiran racun
int printReceipt(PosRe receipt)
{
	if (receipt == NULL) return ARG_ERR;
	
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
	if (head == NULL) return ARG_ERR;
	
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
	if (matchList == NULL || size == 0 || word == NULL) return ARG_ERR;
	
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
int freeArticles(PosArt head)
{
	PosArt curr = head;
	while (curr != NULL)
	{
		PosArt next = curr->next;
		freeArticle(curr);
		curr = next;
	}
	return 0;
}
// Cisti Artikal
int freeArticle(PosArt ad)
{
	if (ad == NULL) return 0;
	free(ad->name);
	free(ad);
	return 0;
}

// Cisti racun iz memorije
int freeReceipts(PosRe head)
{
	PosRe curr = head;
	while (curr != NULL)
	{
		PosRe next = curr->next;
		freeReceipt(curr);
		curr = next;
	}
	return 0;
}
// Cisti racun
int freeReceipt(PosRe ad)
{	
	if (ad == NULL) return 0;
	free(ad->date);
	freeArticles(ad->articles);
	free(ad);
	return 0;
}