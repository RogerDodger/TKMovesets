#pragma once

#include <vector>

#include "GameData.hpp"
#include "GameProcess.hpp"
#include "Extractor.hpp"

#include "GameAddresses.h"

enum GameId
{
	GameId_t7 = 0,
	GameId_t8 = 1,
	GameId_ttt2 = 2,
};

class GameExtract
{
private:
	GameExtract() = default;
	~GameExtract() = default;
	GameExtract& operator = (const GameExtract&) = delete;
	GameExtract(const GameExtract&) = delete;

	// Extractor glass, never stores an Extractor*, used for polymorphism
	Extractor* m_extractor = nullptr;
	// Is a thread running
	bool m_threadStarted = false;
	// Player address to extract
	std::vector<gameAddr> m_playerAddress;

	// Callback called whenever the process is re-atached
	void OnProcessAttach();
	// Instantiate an extractor with polymorphism, also destroy the old one
	void InstantiateExtractor();
	// Order an extraction to be pushed to the queue
	void OrderExtraction(gameAddr playerAddress);
	// Latch on to process 
	void Update();
	// Reads the movesets for the players characters' names. Accessible under .characterNames
	void LoadCharacterNames();
public:
	static GameExtract& getInstance() {
		// Todo: mutex here or something?
		static GameExtract s_instance;
		return s_instance;
	}

	// Stores the ID of the currently opened game
	size_t currentGameId = -1;
	// Used to know which process is currently being searched/opened
	std::string currentGameProcess;
	// Currently opened process for extraction
	GameProcess* process = nullptr;
	// Helper to read on address from their game_addresses.txt identifier
	GameData* game = nullptr;
	// Progress of the current extraction, between 0.0f and 100.0f
	float progress{ 0.0f };
	// Contains the character names
	std::string characterNames[4];
	// Max character count of the game
	int characterCount = 2;

	// Is crurently busy with an extraction
	bool IsBusy();
	// Set the process to open
	void SetTargetProcess(const char* processName, size_t gameId);
	// Starts the thread that will later be used for extracton
	void StartThread();
	// Queue a character extraction. -1 of all characters
	void QueueCharacterExtraction(int playerId);
};