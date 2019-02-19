
#include "Scores.h"

#define SCORE_FILE "./scores.scores"

void ArcheryScores::writeScores(TArray<int> arr) {
	FILE *scoreFile;

	fopen_s(&scoreFile, SCORE_FILE, "wb");

	//TODO THIS

}

TArray<int> ArcheryScores::readScores() {
	FILE *scoreFile;
	
	fopen_s(&scoreFile, SCORE_FILE, "rb");

	//TODO THIS
	
}
