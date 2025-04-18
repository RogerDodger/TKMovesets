#pragma once

#include <set>
#include <vector>

#include "ThreadedClass.hpp"
#include "Helpers.hpp"

// Moveset info, most of which is read straight from the file. Use for display.
struct movesetInfo
{
	int color; // Color is defined by moveset flags
	std::wstring filename;
	std::string name;
	std::wstring wname;
	FasterStringComp origin;
	int32_t characterId;
	std::string target_character;
	std::string original_character;
	std::string version_string;

	uint64_t date;
	std::string date_str;

	std::string extraction_date_str;

	uint64_t size;
	std::string sizeStr;

	std::string hash;

	uint64_t extractionDate;
	time_t modificationDate;
	uint16_t gameId;
	uint16_t minorVersion;
	bool modified; // Was modified with the editor
	bool editable;
	bool onlineImportable; // Moveset over a certain size can't be used online
	
	bool is_valid;
};

class LocalStorage : public ThreadedClass
{
private:
	// Contains the list of movesets found in the extraction directory
	std::vector<movesetInfo*> localMovesets;
	// A set containing the list of moveset files in the configured extraction dir. Used to determine when to fetch new movesets info
	std::set<std::wstring> m_extractedMovesetFilenames;
	// .extractedMovesets garbage, because it can be accessed in another thread while we remove items for it.
	std::vector<movesetInfo*> m_garbage;
	// If true, will clear the moveset list and refresh it in the next run
	bool m_refreshMovesets = false;

	// Function ran in the parallel thread, used to list files regularly
	void Update() override;
public:
	// Contains the list of movesets found in the extraction directory
	std::vector<movesetInfo*> extractedMovesets;
	// Contains a list of new movesets that need to be loaded into shared memory
	std::vector<movesetInfo*> newMovesets;

	~LocalStorage();

	// Stops the thread started above
	void StopThreadAndCleanup() override;
	// Reads movesets from their configured extraction dir. Accessible under .extractedMovesets
	void ReloadMovesetList();
	// Frees the content of .garbage. Must be called by the display thread after being finished with .extractedMovesets
	void CleanupUnusedMovesetInfos();
	// Delete a moveset's file entirely, returns false if deletion failed
	bool DeleteMoveset(const wchar_t* filename);
	// Force movesets to refresj
	void RefreshMovesets();
};