
#include "Scores.h"
#include "Archery.h"

void ArcheryScores::writeScores(TArray<int> arr) {
	FILE *scoreFile;

	fopen_s(&scoreFile, SCORE_FILE, "wb");

	for (int i = 0; i < arr.Num(); i++) {
		fprintf_s(scoreFile, "%c", * (char*) (&arr[0] + i));
	}

	fclose(scoreFile);
}

TArray<int> ArcheryScores::readScores(int numScores) {
	FILE *scoreFile;
	char buffer[512];
	TArray<int> arr;

	fopen_s(&scoreFile, SCORE_FILE, "rb");

	if (scoreFile) {
		for (int i = 0; i < numScores; i++) {
			if (fscanf_s(scoreFile, "%c", buffer + i, 1) == EOF) break;

			arr.Add(*(buffer + i));
		}

		fclose(scoreFile);
	}

	return arr;
}
