
#include "Scores.h"
#include "Archery.h"

void ArcheryScores::writeScores(TArray<int> arr) {
	FILE *scoreFile;

	fopen_s(&scoreFile, SCORE_FILE, "wb");

	for (int i = 0; i < arr.Num(); i++) {

		for (int j = 0; j < 4; j++) {

			fprintf_s(scoreFile, "%c", * ((char*)  (&arr[0] + i)) + j );

		}

	}

	fclose(scoreFile);
}

TArray<int> ArcheryScores::readScores(int numScores) {
	FILE *scoreFile;
	char buffer[512];
	TArray<int> arr;

	fopen_s(&scoreFile, SCORE_FILE, "rb");

	if (scoreFile) {
		for (int i = 0; i < numScores * sizeof(int); i++) {
			if (fscanf_s(scoreFile, "%c", buffer + i, 1) == EOF) break;
		}

		for (int i = 0; i < numScores; i++) {
			int* pScore = (int*)(buffer + sizeof(int) * i);
			arr.Add(*pScore);
		}

		fclose(scoreFile);
	}

	return arr;
}
