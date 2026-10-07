/* 1. Napisati program koji prvo pročita koliko redaka ima datoteka, tj. koliko ima studenata
zapisanih u datoteci. Nakon toga potrebno je dinamički alocirati prostor za niz struktura
studenata (ime, prezime, bodovi) i učitati iz datoteke sve zapise. Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova/max_br_bodova*100 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 50
#define MAX_SCORE 50
#define MAX_BUFFER 1024

#define FILE_NOT_FOUND (-1)
#define EMPTY_FILE_ERR (-2)
#define MEMORY_ALLOC_ERR (-3)
#define DATA_ERR (-4)
#define ARG_ERR (-5)
#define MATH_ERR (-6)

struct _Student;
typedef struct _Student* Position;
typedef struct _Student {

	char name[MAX_LENGTH];
	char surname[MAX_LENGTH];
	float score;
	Position next;

}Student;

int lineCounter(char*);
int loadData(char*, Student**, int);
float calcRelativeScore(float, int);
int printData(Student*, int);

int main() {

	char filename[MAX_LENGTH] = "tekst.txt";

	Student* students = NULL;

	int length = 0;
	length = lineCounter(filename);

	if (length < 0)
	{
		return length;
	};

	int status = 0;

	status = loadData(filename, &students, length);
	if (status < 0)
	{
		free(students);
		return status;
	};

	status = printData(students, length);
	if (status < 0)
	{
		free(students);
		return status;
	};

	free(students);

	return 0;
}

// Čita retke u datoteci
int lineCounter(char* filename)
{
	FILE* file = fopen(filename, "r");


	if (!file)
	{
		printf("Greska pri citanju filea! (lineCounter)\n");
		return FILE_NOT_FOUND;
	}

	int counter = 0;
	char buffer[MAX_BUFFER] = { 0 };


	while (fgets(buffer, MAX_BUFFER, file))
	{
		counter++;
	}

	fclose(file);

	if (counter == 0)
	{
		printf("File je prazan! (lineCounter)\n");
		return EMPTY_FILE_ERR;
	}

	return counter;
}

// Otvara file, alocira memoriju za studente s obzirom na length, te upisuje podatke iz teksta u studente
int loadData(char* filename, Student** students, int length) {


	FILE* file = fopen(filename, "r");

	if (!file)
	{
		printf("Greska pri citanju filea! (loadData)\n");
		return FILE_NOT_FOUND;
	}

	*students = (Student*)malloc(length * sizeof(Student));

	if (*students == NULL)
	{
		printf("Greska pri alokaciji memorije! (loadData)\n");
		fclose(file);
		return MEMORY_ALLOC_ERR;
	}


	for (int i = 0; i < length; i++)
	{
		if (fscanf(file, "%s %s %f", (*students)[i].name, (*students)[i].surname, &(*students)[i].score) != 3)
		{
			printf("Greska u formatu datateke! (loadData)\n");
			fclose(file);
			return DATA_ERR;
		};
		(*students)[i].next = NULL;
	}
	fclose(file);
	return 0;
}

// Računa relativne bodove prema formuli "relatvan_br_bodova = br_bodova/max_br_bodova*100"
float calcRelativeScore(float score, int maxScore) {

	if (score < 0 || maxScore <= 0)
	{
		printf("Greska u parametrima! (relativeScore)\n");
		return ARG_ERR;
	}

	else if (score > MAX_SCORE)
	{
		printf("Greska u logici (relativeScore)\n");
		return MATH_ERR;
	}

	return (float)(score / maxScore) * 100;
}

// Ispisuje studente u formatu REDNI BROJ |IME PREZIME|BROJ BODOVA|RELATIVAN BROJ BODOVA|
int printData(Student* students, int length) {
	if (students == NULL || length <= 0)
	{
		printf("Greska u parametrima! (printData)\n");
		return ARG_ERR;
	}
	double relScore = 0;

	for (int i = 0; i < length; i++)
	{
		relScore = calcRelativeScore(students[i].score, MAX_SCORE);
		if (relScore < 0) return (int)relScore;

		printf("%d. |%s %s|Broj bodova: %.1f| Relativni broj bodova: %.2f|\n", i + 1, students[i].name, students[i].surname, students[i].score, relScore);
	}
	return 0;
}