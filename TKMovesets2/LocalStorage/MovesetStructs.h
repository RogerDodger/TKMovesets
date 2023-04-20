#pragma once

#include <stdint.h>

enum TKMovesetProperty_
{
	TKMovesetProperty_END = 0x0
};
// Custom moveset properties used when loading into a game
struct TKMovesetProperty
{
	int id;
	int value;
};

typedef uint64_t MovesetFlags;
enum MovesetFlags_
{
	// Used to show which movesets the user has modified (knowing if it's an original or not can be useful)
	MovesetFlags_MOVESET_MODIFIED = (1 << 0),
};

// This structure should really not change or it would break compatibility with all previous movesets
// If you have any change in mind that might be important, do it now
// Useful infos regarding the moveset : extracting date, game origin, game id, extractor version
struct TKMovesetHeader
{
	const char _signature[4] = { 'T', 'K', 'M', '2' };
	// Version string of the extractor at the time of the extraction
	char version_string[28];
	// Array of bytes where you can store a unique identifier/hash for the game version, allowing you to detect which version the moveset was extracted form
	Byte gameVersionHash[32];
	// ID of the game that the moveset was extracted from
	uint16_t gameId;
	// Minor version of the game that the moveset was extracted from
	uint16_t minorVersion;
	// Contains the size of the header, aligned on 8 bytes
	uint32_t header_size;
	// Offset to the list of blocks
	uint32_t block_list;
	// Size of the list of blocks
	uint32_t block_list_size;
	// Absolute offset of the block offset list
	uint32_t moveset_data_start;
	// Stores a hash of the moveset data 
	uint32_t crc32;
	// Original crc32 of the moveset data at the time of extraction
	uint32_t orig_crc32;
	// Date of last modification
	uint64_t date;
	// Date of initial file extraction
	uint64_t extraction_date;
	// ID of the extracted character, used internally to make some moves that require it work
	uint32_t characterId;
	// Flags used for storing useful data. Currently unused. Todo : see what we can do with this?
	MovesetFlags flags;
	// Origin (Game name)
	char origin[32];
	// Target character to play on
	char target_character[32];
};
