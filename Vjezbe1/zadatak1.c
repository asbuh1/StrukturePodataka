#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAX 50
#define MAXSCORE 57

typedef struct _Student {

	char name[MAX];
	char surname[MAX];
	float score;

}Student;

int lineCounter(char*);
int loadData(char*, Student**, int);
float relativeScore(float, int);
int printData(Student*, int);

int main() {

	char filename[MAX] = "tekst.txt";

	Student* students = NULL;

	int length = lineCounter(filename);

	if (length <= 0)
	{
		printf("Greska pri racunanju duljine! (main:lineCounter)\n");
		return -1;
	}

	if (loadData(filename, &students, length) != 0)
	{
		printf("Greska pri ucitavanju podataka! (main:loadData)\n");
		free(students);
		return -1;
	};

	if (printData(students, length) == -1)
	{
		printf("Greska u ispisu (main:printData)");
		free(students);
		return -1;
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
		return -1;
	}

	int counter = 0;

	while (fscanf(file, "%*s %*s %*f") != EOF)
	{
		counter++;
	};

	fclose(file);
	return counter;
}

// Otvara file, alocira memoriju za studente s obzirom na length, te upisuje podatke iz teksta u studente
int loadData(char* filename, Student** students, int length) {


	FILE* file = fopen(filename, "r");

	if (!file)
	{
		printf("Greska pri citanju filea! (loadData)\n");
		return -1;
	}

	*students = (Student*)malloc(length * sizeof(Student));

	if (*students == NULL)
	{
		printf("Greska pri alokaciji memorije! (loadData)\n");
		fclose(file);
		return -1;
	}


	for (int i = 0; i < length; i++)
	{
		if (fscanf(file, "%s %s %f", (*students)[i].name, (*students)[i].surname, &(*students)[i].score) != 3)
		{
			printf("Greska u parametrima! (loadData)\n");
			fclose(file);
			return -1;
		};
	}
	fclose(file);
	return 0;
}

// Računa relativne bodove prema formuli "relatvan_br_bodova = br_bodova/max_br_bodova*100"
float relativeScore(float score, int maxScore) {

	if (score < 0 || maxScore <= 0)
	{
		printf("Greska u parametrima! (relativeScore)\n");
		return -1;
	}

	else if (score > MAXSCORE)
	{
		printf("Greska u logici (relativeScore)\n");
		return -1;
	}

	return (float)(score / maxScore) * 100;
}

// Ispisuje studente
int printData(Student* students, int length) {
	if (students == NULL || length <= 0)
	{
		printf("Greska u parametrima! (printData)\n");
		return -1;
	}
	double relScore = 0;

	for (int i = 0; i < length; i++)
	{
		relScore = relativeScore(students[i].score, MAXSCORE);
		if (relScore == -1) return -1;

		printf("%d. |%s %s|Broj bodova: %.1f| Relativni broj bodova: %.2f|\n", i + 1, students[i].name, students[i].surname, students[i].score, relScore);
	}
	return 0;
}