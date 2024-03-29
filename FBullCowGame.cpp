#pragma once
#include "FBullCowGame.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <ctime>
#include <map>
#define TMap std::map

using int32 = int;

FBullCowGame::FBullCowGame() {	Reset(); } //default constructor


int32 FBullCowGame::GetCurentTry() const {	return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const {	return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bGameIsWon; }

int32 FBullCowGame::GetMaxTries() const {
	TMap<int32, int32> WordLengthToMaxTries{ {3,4}, {4,7},{5,10},{6,15},{7,20},{8,25},{9,30},{10,35} }; // TODO finish map. user testing required.
	return WordLengthToMaxTries[MyHiddenWord.length()];
}

void FBullCowGame::Reset()
{
	constexpr int32 MAX_TRIES = 8;
	const FString HIDDEN_WORD = "planet";

	MyHiddenWord = HIDDEN_WORD;
	MyCurrentTry = 1;
	bGameIsWon = false;
	return;
}

// recieves a VALID guess, increments turn, and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount;
	int32 WordLength = MyHiddenWord.length(); // assuming same length as guess

	// loop through all letters in the hidden word
	
	for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++) {
		// compare letters against the guess
		for (int32 GChar = 0; GChar < WordLength; GChar++) {
			// if they match then
			if (Guess[GChar] == MyHiddenWord[MHWChar]) {
				if (MHWChar == GChar) { // if they're in the same place
					BullCowCount.Bulls++; // incriment bulls
				}
				else {
					BullCowCount.Cows++; // incriment cows
				}
			}
		}
	}
	if (BullCowCount.Bulls == WordLength) {
		bGameIsWon = true;
	}
	else {
		bGameIsWon = false;
	}
	return BullCowCount;
}

// randomizer code
void FBullCowGame::Randomize()
{
	std::vector<FString> Word;
	int32 NumberOfLines = 0;
	FString Line = "";
	int32 Spot = 0;
	std::srand(std::time(NULL));
	// open the file isograms.txt
	std::ifstream File("isograms.txt");
	
	// vector the file
	while (std::getline(File, Line))
	{
		Word.push_back(Line);
		++NumberOfLines; // count line in the file
	}
	// randomly pull a line
	Spot = rand() % NumberOfLines;

	File.close();

	MyHiddenWord = Word[Spot];

	return;
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (!IsIsogram(Guess)) // if the guess isn't an isogram
	{
		return EGuessStatus::Not_Isogram;
	}
	else if (!IsLowercase(Guess)) // if the guess isn't all lowercase
	{
		return EGuessStatus::Not_Lowercase;
	}
	else if (Guess.length() != GetHiddenWordLength()) // if the guess length is wrong
	{
		return EGuessStatus::Wrong_Length;
	}
	else
	{
		return EGuessStatus::OK;
	}

}

bool FBullCowGame::IsIsogram(FString Word) const
{
	// treat 0 and 1 letter words as isograms
	if (Word.length() <= 1) { return true; }
	
	TMap<char, bool> LetterSeen; // setup our map
	for (auto Letter : Word) // for all letters of the word
	{
		Letter = tolower(Letter);
		
		if (LetterSeen[Letter]) {// if the letter is in the map
			return false; // we do NOT have an isogram
		}
		else {
			LetterSeen[Letter] = true; // add the letter to the map as seen
		}
	}

	return true; // for example in cases where /0 is entered
}

bool FBullCowGame::IsLowercase(FString Word) const
{
	for (auto Letter : Word) // if not a lower letter
	{
		if (!islower(Letter)) {
			return false;
		}
	}
	return true;
}
