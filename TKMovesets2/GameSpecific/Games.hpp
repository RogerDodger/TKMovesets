#pragma once

#include "GameData.hpp"
#include "GameProcess.hpp"
#include "Importer.hpp"
#include "Extractor.hpp"
#include "Editor.hpp"
#include "Online.hpp"
#include "GameInfo.hpp"

#include "constants.h"

enum GameId_
{
	GameId_T7,
	GameId_T8,
	GameId_TTT2,
};

namespace Games
{
	// Returns a pointer to a struct containing a game's name, process name, character count
	DLLCONTENT const GameInfo* GetGameInfoFromIndex(unsigned int index);
	DLLCONTENT const GameInfo* GetGameInfoFromIdentifier(uint16_t gameId);
	DLLCONTENT const GameInfo* GetGameInfoFromIdentifier(uint16_t gameId, uint16_t minorVersion);

	// Returns the amount of supported games
	DLLCONTENT unsigned int GetGamesCount();
	// Returns the amount of games that upport extraction
	DLLCONTENT unsigned int GetExtractableGamesCount();
	// Returns the amount of games that upport importation
	DLLCONTENT unsigned int GetImportableGamesCount();

	// Returns true if moveset edition on the moveset is allowed
	bool IsGameEditable(uint16_t gameId, uint16_t minorVersion);
	// Returns true if live moveset edition on the moveset is allowed
	bool IsGameLiveEditable(uint16_t gameId, uint16_t minorVersion);

	// Instantiate a new game-specific extractor
	DLLCONTENT Extractor* FactoryGetExtractor(const GameInfo* gameInfo, GameProcess* process, GameData* game);

	// Instantiate a new game-specific importer
	DLLCONTENT Importer* FactoryGetImporter(const GameInfo* gameInfo, GameProcess* process, GameData* game);

	// Instantiate a new game-specific importer
	DLLCONTENT Editor* FactoryGetEditor(const GameInfo* gameInfo, GameProcess* process, GameData* game);

	// Instantiate a new game-specific online handler
	DLLCONTENT Online* FactoryGetOnline(const GameInfo* gameInfo, GameProcess* process, GameData* game);
}