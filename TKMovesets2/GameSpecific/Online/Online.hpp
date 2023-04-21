#pragma once

#include <windows.h>

#include "GameProcess.hpp"
#include "GameData.hpp"
#include "LocalStorage.hpp"
#include "BaseGameSpecificClass.hpp"

#include "constants.h"

class DLLCONTENT Online : public BaseGameSpecificClass
{
protected:
	// Stores a handle to the shared memory
	HANDLE m_memoryHandle = nullptr;
	// Contains whether or not we have injected our DLL
	bool m_injectedDll = false;
	// Ptr to the shared memory
	void* m_orig_sharedMemPtr = nullptr;

	// Calls a function of the MovesetLoader inside of the remote process. Returns false if failure was encountered somewhere.
	bool CallMovesetLoaderFunction(const char* functionName, bool waitEnd=false);
	// Returns the name of the shared memory to look after
	virtual const TCHAR* GetSharedMemoryName() = 0;
public:
	// Contains a copy of the basic moveset informations we have loaded
	std::vector<movesetInfo>* displayedMovesets;
	// True if we are currently injecting a DLL
	bool isInjecting = false;

	Online(GameProcess* process, GameData* game, uint16_t gameId, uint16_t minorVersion);
	~Online();

	// Load the shared memory handle
	bool LoadSharedMemory();
	// Returns true if the shared memory has been found and loaded
	bool IsMemoryLoaded();
	// Injects the DLL into the current process, not waiting for it to be completed
	void InjectDll();
	// Injects the DLL into the current process, waiting for it to be completed. Returns true/false if the library was successfully loaded or not.
	bool InjectDllAndWaitEnd();
	// Called when a moveset is successfully loaded in the game's memory by the importer
	virtual void OnMovesetImport(movesetInfo* displayedMoveset, gameAddr movesetAddr, unsigned int playerId, uint8_t characterId) = 0;
};