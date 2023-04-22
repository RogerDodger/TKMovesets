#include <set>
#include <thread>
#include <format>
#include <iostream>
#include <filesystem>

#include "GameImport.hpp"
#include "Games.hpp"
#include "Importer.hpp"
#include "helpers.hpp"

#include "constants.h"
#include "GameTypes.h"

/// Class storing an importation queue and an Importer* instance to feed it to

// -- Private methods -- //

void GameImport::OnProcessAttach()
{
	InstantiateFactory();
	m_plannedImportations.clear();
}

void GameImport::OnProcessDetach()
{
	// There is no way to know if the moveset is still valid after detaching: reset it
	lastLoadedMoveset = 0;
}

void GameImport::InstantiateFactory()
{
	if (m_importer != nullptr) {
		delete m_importer;
	}

	game->gameKey = currentGame->dataString;
	game->minorGameKey = currentGame->minorDataString;
	// Every game has its own importation subtleties so we use polymorphism to manage that
	m_importer = Games::FactoryGetImporter(currentGame, process, game);
}

void GameImport::PreProcessDetach()
{
	m_importer->CleanupUnusedMovesets();
}

void GameImport::RunningUpdate()
{
	bool errored = false;
	while (IsBusy())
	{
		// Two different ways to import movesets, from filename or from actual moveset data
		if (!errored && m_plannedImportations.size() > 0)
		{
			ImportationErrcode_ err;

			auto& [moveset, movesetSize, filename, playerAddress, settings] = m_plannedImportations[0];
			if (moveset == nullptr) {
				err = m_importer->Import(filename.c_str(), playerAddress, settings, progress);
			}
			else {
				err = m_importer->Import(moveset, movesetSize, playerAddress, settings, progress);
			}

			if (settings & ImportSettings_FreeUnusedMovesets) {
				m_importer->CleanupUnusedMovesets();
			}

			if (err != ImportationErrcode_Successful) {
				m_errors.push_back(err);
				errored = true;
			}
			else {
				lastLoadedMoveset = m_importer->lastLoaded.address;
			}

			m_plannedImportations.erase(m_plannedImportations.begin());
		}
	}
}

// -- Public methods -- //

ImportationErrcode_ GameImport::GetLastError()
{
	if (m_errors.size() > 0) {
		ImportationErrcode_ err = m_errors[0];
		m_errors.erase(m_errors.begin());
	}
	return ImportationErrcode_Successful;
}

void GameImport::StopThreadAndCleanup()
{
	// Order thread to stop
	m_threadStarted = false;
	m_t.join();

	if (m_importer != nullptr) {
		if (process->IsAttached()) {
			m_importer->CleanupUnusedMovesets();
		}
		delete m_importer;
	}

	delete process;
	delete game;
}

bool GameImport::CanStart(bool cached)
{
	if (m_importer == nullptr) {
		return false;
	}
	if (cached) {
		return m_canStart;
	}
	// Per-game definition
	return m_importer->CanImport();
}

bool GameImport::IsBusy()
{
	// There are still movesets to import
	return m_plannedImportations.size() > 0;
}

void GameImport::QueueCharacterImportation(const Byte* moveset, uint64_t movesetSize, ImportSettings settings)
{
	// It is safe to call this function even while an extraction is ongoing
	gameAddr playerAddress = m_importer->GetCharacterAddress(currentPlayerId);
	m_plannedImportations.push_back({
		.moveset = moveset,
		.movesetSize = movesetSize,
		.filename = L"",
		.playerAddress = playerAddress,
		.settings = settings
	});
}

void GameImport::QueueCharacterImportation(std::wstring filename, ImportSettings settings)
{
	// It is safe to call this function even while an extraction is ongoing
	gameAddr playerAddress = m_importer->GetCharacterAddress(currentPlayerId);
	m_plannedImportations.push_back({
		.moveset = nullptr,
		.movesetSize = 0,
		.filename = filename,
		.playerAddress = playerAddress,
		.settings = settings
	});
}

uint8_t GameImport::GetCharacterCount()
{
	// todo: If the importer gets deallocated while we do this, things can go bad
	if (m_importer != nullptr) {
		return m_importer->characterCount;
	}
	// Return 1 by default on purpose, easier to debug stuff that way
	return 1;
}

gameAddr GameImport::GetCurrentPlayerMovesetAddr()
{
	// todo: If the importer gets deallocated while we do this, things can go bad
	if (m_importer != nullptr) {
		return m_importer->GetMovesetAddress(currentPlayerId);
	}
	return 0;
}