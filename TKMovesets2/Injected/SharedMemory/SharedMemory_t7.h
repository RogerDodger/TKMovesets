#pragma once

#include "SharedMemory.h"

struct SharedMemT7_Player
{
	// CRC32 of the moveset, currently unused
	uint32_t crc32;
	// Can be null, in which case no moveset is to be loaded
	uint64_t custom_moveset_addr;
	// Size of the uncompressed moveset. Todo: send the compressed moveset.
	uint64_t custom_moveset_size = 0;
	// Original moveset data in full (including header & compression), will be sent to the opponent
	uint64_t custom_moveset_original_data_addr;
	// Original moveset data size
	uint64_t custom_moveset_original_data_size = 0;
	// Nth bit set = Nth bit mota missing
	uint16_t missingMotas;
	// True if moveset is ready to apply, false if moveset needs initialization
	bool is_initialized;
	// Original character ID of the loaded moveset
	uint8_t moveset_character_id;
	// Character ID of the last loaded char
	uint8_t previous_character_id;
	// True to request that main process loads custom moveset to this player based on its character_id
	bool custom_moveset_request;

	void SetMotaMissing(int id) {
		missingMotas |= (1 << id);
	}
	bool isMotaMissing(int id) const {
		return missingMotas & (1 << id);
	}
};

struct SharedMemT7_Char
{
	// Can be null, in which case no moveset is to be loaded
	uint64_t addr = 0;
	// Size of the uncompressed moveset.
	uint64_t size = 0;
	// True if moveset is ready to apply, false if moveset needs initialization
	bool is_initialized;
	// Nth bit set = Nth bit mota missing
	uint16_t missingMotas;

	void SetMotaMissing(int id) {
		missingMotas |= (1 << id);
	}
	bool isMotaMissing(int id) const {
		return missingMotas & (1 << id);
	}
};

struct SharedMemT7_Extraprop
{
	uint32_t playerid;
	uint32_t id;
	uint32_t value;
};

struct SharedMemT7 : SharedMemBase
{
	// List of players to send custom movesets to
	SharedMemT7_Player players[2];
	// Property to play when the exported ExecuteExtraprop() function is being called
	SharedMemT7_Extraprop propToPlay;
	// List of custom movesets, one per character_id
	SharedMemT7_Char chars[60];
};