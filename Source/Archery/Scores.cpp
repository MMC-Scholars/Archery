
#include "Scores.h"

#define SCORE_FILE "./scores.scores"

void ArcheryScores::writeScores(TArray<int> arr) {
	FILE *scoreFile;

	fopen_s(&scoreFile, SCORE_FILE, "wb");

	ArcheryScores::numScores = arr.Num();

	for (int i = 0; i < arr.Num(); i++) {
//		fprintf_s(scoreFile, "%c", (char*) arr[i]);
	}

	fclose(scoreFile);
}

TArray<int> ArcheryScores::readScores() {
	FILE *scoreFile;
	char buffer[512];
	TArray<int> arr;

	fopen_s(&scoreFile, SCORE_FILE, "rb");
	
	ArcheryScores::numScores = ArcheryScores::numScores ? ArcheryScores::numScores : 0;

	for (int i = 0; i < ArcheryScores::numScores * sizeof(int); i++) {
		fscanf(scoreFile, "%c", buffer + i);
		arr.Add( *(buffer + i) );
	}
	
	fclose(scoreFile);

	return arr;
}
