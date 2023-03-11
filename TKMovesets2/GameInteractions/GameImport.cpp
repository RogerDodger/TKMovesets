#include <set>
#include <chrono>
#include <thread>
#include <format>
#include <iostream>
#include <filesystem>

#include "GameImport.hpp"
#include "Games.hpp"
#include "Importer.hpp"
#include "helpers.hpp"

#include "constants.h"
#include "GameAddresses.h"

/// Class storing an importation queue and an Importer* instance to feed it to

// -- Private methods -- //

void GameImport::OnProcessAttach()
{
	InstantiateFactory();
	m_plannedImportations.clear();
}


void GameImport::InstantiateFactory()
{
	if (m_importer != nullptr) {
		delete m_importer;
	}

	// Every game has its own extraction subtleties so we use polymorphism to manage that
	m_importer = Games::FactoryGetImporter(currentGameId, process, game);
}

void GameImport::PreProcessDetach()
{
	if (free_unused_movesets) {
		m_importer->CleanupUnusedMovesets();
	}
}

void GameImport::RunningUpdate()
{
	bool errored = false;
	while (IsBusy())
	{
		if (!errored) {
			ImportationErrcode_ err;

			// Two different ways to import movesets, from filename or from actual moveset data
			if (m_plannedFileImportations.size() > 0) {
				auto& [filename, playerAddress] = m_plannedFileImportations[0];
				err = m_importer->Import(filename.c_str(), playerAddress, apply_instantly, progress);
				m_plannedFileImportations.erase(m_plannedFileImportations.begin());
			}
			else {
				auto& [moveset, movesetSize, playerAddress] = m_plannedImportations[0];
				err = m_importer->Import(moveset, movesetSize, playerAddress, apply_instantly, progress);
				m_plannedImportations.erase(m_plannedImportations.begin());
			}

			if (free_unused_movesets) {
				m_importer->CleanupUnusedMovesets();
			}
			if (err != ImportationErrcode_Successful) {
				m_errors.push_back(err);
				errored = true;
			}
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
		if (free_unused_movesets && process->IsAttached()) {
			m_importer->CleanupUnusedMovesets();
		}
		delete m_importer;
	}

	delete process;
	delete game;
}

bool GameImport::CanStart()
{
	if (m_importer == nullptr) {
		return false;
	}
	// Per-game definition
	return m_importer->CanImport();
}

bool GameImport::IsBusy()
{
	// There are still playerAddresss to extract from
	return m_plannedFileImportations.size() > 0 || m_plannedImportations.size() > 0;
}

void GameImport::QueueCharacterImportation(byte* moveset, uint64_t movesetSize)
{
	// It is safe to call this function even while an extraction is ongoing
	gameAddr playerAddress = m_importer->GetCharacterAddress(currentPlayerId);
	m_plannedImportations.push_back(std::tuple<byte*, uint64_t, gameAddr>(moveset, movesetSize, playerAddress));
}

void GameImport::QueueCharacterImportation(std::string filename)
{
	// It is safe to call this function even while an extraction is ongoing
	gameAddr playerAddress = m_importer->GetCharacterAddress(currentPlayerId);
	m_plannedFileImportations.push_back(std::pair<std::string, gameAddr>(filename, playerAddress));
}


uint8_t GameImport::GetCharacterCount()
{
	if (m_importer != nullptr) {
		return m_importer->characterCount;
	}
	return 2;
}