#pragma once

#include "UnrealEngine.h"

namespace ArcheryScores {

	int numScores;

	void writeScores(TArray<int> arr);

	TArray<int> readScores();

}

