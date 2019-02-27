
#include "Scores.h"
#include "Archery.h"
#include "System/NLogger.h"

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

	/*
	Msg("This is the NEW array^");
	TArray<int> t_arr;
	t_arr.Add(0);
	t_arr.Add(13);
	t_arr.Add(1);
	t_arr.Add(94);
	t_arr.Add(47);
	for (int i = 0; i < t_arr.Num(); i++) Msg("%d", t_arr[i]);
	Msg("_END_OF_ARRAY_");

	Msg("This is the 4th element of the array and should be 94. That element is %d", *(&t_arr[0]+3));

	// WRITE

	FILE *t_scoreFile;

	fopen_s(&t_scoreFile, SCORE_FILE, "wb");

	for (int i = 0; i < t_arr.Num(); i++)
		for (int j = 0; j < 4; j++) fprintf_s(t_scoreFile, "%c", *((char*)(&t_arr[0] + i)) + j);

	fclose(t_scoreFile);

	// READ

	char t_buffer[512];
	TArray<int> t_arr_out;

	fopen_s(&t_scoreFile, SCORE_FILE, "rb");

	if (t_scoreFile) {
		for (int i = 0; i < 5 * sizeof(int); i++) {
			if (fscanf_s(t_scoreFile, "%c", t_buffer + i, 1) == EOF) break;
		}

		for (int i = 0; i < 5; i++) {
			int* pScore = (int*)(t_buffer) + i;
			t_arr_out.Add(*pScore);
		}

		fclose(t_scoreFile);
	}

	Msg("redone array^");
	for (int i = 0; i < t_arr_out.Num(); i++) Msg("%d", t_arr_out[i]);
	Msg("end of array");
	*/

	return arr;
}
