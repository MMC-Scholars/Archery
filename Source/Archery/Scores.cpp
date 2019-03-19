
#include "Scores.h"
#include "Archery.h"
#include "System/NLogger.h"

void ArcheryScores::writeScores(TArray<int> arr) {
	FILE *scoreFile;

	fopen_s(&scoreFile, SCORE_FILE, "wb");

	// ensure the correct number of scores are being written to score file
	while (arr.Num() < NUM_HIGH_SCORES) arr.Add(0);

	for (int i = 0; i < NUM_HIGH_SCORES; i++) {

		for (int j = 0; j < 4; j++) {

			fprintf_s(scoreFile, "%c", *(((char*)&arr[i]) + j));

		}

	}

	fclose(scoreFile);
}

TArray<int> ArcheryScores::readScores() {
	FILE *scoreFile;
	char buffer[512];
	TArray<int> arr;

	fopen_s(&scoreFile, SCORE_FILE, "rb");

	if (scoreFile) {

		// add raw data to buffer
		for (int i = 0; i < NUM_HIGH_SCORES * sizeof(int); i++) {
			if (fscanf_s(scoreFile, "%c", buffer + i, 1) == EOF) break;
		}

		// read raw data to integers
		for (int i = 0; i < NUM_HIGH_SCORES; i++) {
			int* pScore = (int*)(buffer + sizeof(int) * i);
			arr.Add(*pScore);
		}

		fclose(scoreFile);
	}

	// sort scores from least to greatest
	arr.Sort();

	return arr;
}
