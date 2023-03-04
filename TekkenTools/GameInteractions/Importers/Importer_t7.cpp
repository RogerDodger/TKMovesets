#include <string>

#include "helpers.hpp"
#include "Importer_t7.hpp"

#include "Structs_t7.h"

// Contains the same structure as StructsT7 but with gameAddr types instead of ptrs types
// Defined here because i don't want any other file to have access to this shortcut
#define gAddr StructsT7_gameAddr

// -- Static helpers -- //

static char* getMovesetInfos(std::ifstream& file, MovesetHeader* header, uint64_t& movesetSize)
{
	file.read((char*)header, sizeof(MovesetHeader));
	file.seekg(0, std::ios::end);
	movesetSize = file.tellg();
	char* movesetData = (char*)malloc(movesetSize);
	if (movesetData != nullptr) {
		file.seekg(header->offsets.movesetInfoBlock + header->infos.header_size, std::ios::beg);
		file.read(movesetData, movesetSize);
	}
	file.close();
	return movesetData;
}

// -- Private methods -- //

void ImporterT7::ConvertMotaListOffsets(uint64_t motalistsBlock, char* movesetData, gameAddr gameMoveset, gameAddr playerAddress)
{
	MotaList currentMotasList{};
	gameAddr currentMovesetAddr = m_process->readInt64(playerAddress + m_game->addrFile->GetSingleValue("val_motbin_offset"));
	m_process->readBytes(currentMovesetAddr + 0x280, &currentMotasList, sizeof(MotaList));

	MotaList* motaList = (MotaList*)(movesetData + motalistsBlock);

	uint64_t* gameMotaCursor = (uint64_t*)&currentMotasList;
	uint64_t* fileMotaCursor = (uint64_t*)motaList;

	// This is just a list of uint64_t anyway so might as well do this
	for (size_t i = 0; i <= 12; ++i)
	{
		if (fileMotaCursor[i] == 0 || true) {
			// Moveset block was not included in the file: copy the currently used one
			fileMotaCursor[i] = gameMotaCursor[i];
		}
		else
		{
			// Todo: check if this works. I believe we are off.
			fileMotaCursor[i] += gameMoveset;
		}
	}
}

void ImporterT7::ConvertMovesetTableOffsets(const MovesetHeader_offsets& offsets, char* moveset, gameAddr gameMoveset)
{
	gAddr::MovesetTable* table = (gAddr::MovesetTable*)(moveset + offsets.tableBlock);
	gameAddr offset = gameMoveset + offsets.movesetBlock;

	table->reactions += offset;
	table->requirement += offset;
	table->hitCondition += offset;
	table->projectile += offset;
	table->pushback += offset;
	table->pushbackExtradata += offset;
	table->cancel += offset;
	table->groupCancel += offset;
	table->cancelExtradata += offset;
	table->extraMoveProperty += offset;
	table->unknown_0x1f0 += offset;
	table->unknown_0x200 += offset;
	table->move += offset;
	table->voiceclip += offset;
	table->inputSequence += offset;
	table->input += offset;
	table->unknownParryRelated += offset;
	table->cameraData += offset;
	table->throws += offset;
}

void ImporterT7::ConvertMovesOffsets(char* moveset, gameAddr gameMoveset, const gAddr::MovesetTable* offsets, const MovesetHeader_offsets& blockOffsets)
{
	size_t i;
	gameAddr blockOffset = gameMoveset + blockOffsets.movesetBlock;

	i = 0;
	for (gAddr::Move* move = (gAddr::Move*)(moveset + blockOffsets.movesetBlock + offsets->move); i < offsets->moveCount; ++i, ++move)
	{
		move->name_addr += gameMoveset + blockOffsets.nameBlock;
		move->anim_name_addr += gameMoveset + blockOffsets.nameBlock;
		move->anim_addr += gameMoveset + blockOffsets.animationBlock;

		FROM_INDEX(move->cancel_addr, blockOffset + offsets->cancel, Cancel);
		FROM_INDEX(move->hit_condition_addr, blockOffset + offsets->hitCondition, HitCondition);
		FROM_INDEX(move->voicelip_addr, blockOffset + offsets->voiceclip, Voiceclip);
		FROM_INDEX(move->extra_move_property_addr, blockOffset + offsets->extraMoveProperty, ExtraMoveProperty);
	}

	// Convert projectile ptrs
	i = 0;
	for (gAddr::Projectile* projectile = (gAddr::Projectile*)(moveset + blockOffsets.movesetBlock + offsets->projectile); i < offsets->projectileCount; ++i, ++projectile)
	{
		// One projectile actually has both at 0 for some reason ?
		FROM_INDEX(projectile->cancel_addr, blockOffset + offsets->cancel, Cancel);
		FROM_INDEX(projectile->hit_condition_addr, blockOffset + offsets->hitCondition, HitCondition);
	}

	// Convert cancel ptrs
	i = 0;
	for (gAddr::Cancel* cancel = (gAddr::Cancel*)(moveset + blockOffsets.movesetBlock + offsets->cancel); i < offsets->cancelCount; ++i, ++cancel)
	{
		FROM_INDEX(cancel->requirement_addr, blockOffset + offsets->requirement, Requirement);
		FROM_INDEX(cancel->extradata_addr, blockOffset + offsets->cancelExtradata, CancelExtradata);
	}
	i = 0;

	for (gAddr::Cancel* groupCancel = (gAddr::Cancel*)(moveset + blockOffsets.movesetBlock + offsets->groupCancel); i < offsets->groupCancelCount; ++i, ++groupCancel)
	{
		FROM_INDEX(groupCancel->requirement_addr, blockOffset + offsets->requirement, Requirement);
		FROM_INDEX(groupCancel->extradata_addr, blockOffset + offsets->cancelExtradata, CancelExtradata);
	}

	// Convert reaction ptrs
	i = 0;
	for (gAddr::Reactions* reaction = (gAddr::Reactions*)(moveset + blockOffsets.movesetBlock + offsets->reactions); i < offsets->reactionsCount; ++i, ++reaction)
	{
		FROM_INDEX(reaction->front_pushback, blockOffset + offsets->pushback, Pushback);
		FROM_INDEX(reaction->backturned_pushback, blockOffset + offsets->pushback, Pushback);
		FROM_INDEX(reaction->left_side_pushback, blockOffset + offsets->pushback, Pushback);
		FROM_INDEX(reaction->right_side_pushback, blockOffset + offsets->pushback, Pushback);
		FROM_INDEX(reaction->front_counterhit_pushback, blockOffset + offsets->pushback, Pushback);
		FROM_INDEX(reaction->downed_pushback, blockOffset + offsets->pushback, Pushback);
		FROM_INDEX(reaction->block_pushback, blockOffset + offsets->pushback, Pushback);
	}

	// Convert input sequence ptrs
	i = 0;
	for (gAddr::InputSequence* inputSequence = (gAddr::InputSequence*)(moveset + blockOffsets.movesetBlock + offsets->inputSequence); i < offsets->inputSequenceCount; ++i, ++inputSequence)
	{
		FROM_INDEX(inputSequence->input_addr, blockOffset + offsets->input, Input);
	}

	// Convert throws ptrs
	i = 0;
	for (gAddr::ThrowData* throws = (gAddr::ThrowData*)(moveset + blockOffsets.movesetBlock + offsets->throws); i < offsets->throwsCount; ++i, ++throws)
	{
		FROM_INDEX(throws->cameradata_addr, blockOffset + offsets->cameraData, CameraData);
	}

	// Convert hit conditions ptrs
	i = 0;
	for (gAddr::HitCondition* hitCondition = (gAddr::HitCondition*)(moveset + blockOffsets.movesetBlock + offsets->hitCondition); i < offsets->hitConditionCount; ++i, ++hitCondition)
	{
		FROM_INDEX(hitCondition->requirement_addr, blockOffset + offsets->requirement, Requirement);
		FROM_INDEX(hitCondition->reactions_addr, blockOffset + offsets->reactions, Reactions);
	}

	// Convert pushback ptrs
	i = 0;
	for (gAddr::Pushback* pushback = (gAddr::Pushback*)(moveset + blockOffsets.movesetBlock + offsets->pushback); i < offsets->pushbackCount; ++i, ++pushback)
	{
		FROM_INDEX(pushback->extradata_addr, blockOffset + offsets->pushbackExtradata, PushbackExtradata);
	}

	// Convert ??? ptrs
	i = 0;
	for (gAddr::unknown_0x200* unknown = (gAddr::unknown_0x200*)(moveset + blockOffsets.movesetBlock + offsets->unknown_0x200); i < offsets->unknown_0x200_size; ++i, ++unknown)
	{
		FROM_INDEX(unknown->requirements_addr, blockOffset + offsets->cancel, Requirement);
	}
}

// -- Public methods -- //

ImportationErrcode ImporterT7::Import(const char* filename, gameAddr playerAddress, bool applyInstantly, float& progress)
{
	// Read file data
	std::ifstream file(filename, std::ios::binary);

	if (file.fail()) {
		return ImportationFileReadErr;
	}

	// Variables that will store the moveset size & the moveset itself in our own memory
	uint64_t s_moveset;
	char* moveset;

	// Header of the moveset that will contain our own information about it
	MovesetHeader header;

	// Table that contains offsets and amount of cancels, move, requirements, etc...
	gAddr::MovesetTable* offsets;

	// Moveset allocated IN-GAME. 
	gameAddr gameMoveset;

	// -- File reading & allocations -- //


	// Allocate a copy of the moveset locally. This is NOT in the game's memory
	moveset = getMovesetInfos(file, &header, s_moveset);
	if (moveset == nullptr) {
		return ImportationAllocationErr;
	}


	// Allocate our moveset in the game's memory, but we aren't gonna write on that for a while.
	// The idea is to write on our moveset in our own memory (should be faster), then write it all at once on gameMoveset with a single m_process->writeBytes()
	gameMoveset = m_process->allocateMem(s_moveset);
	if (gameMoveset == 0) {
		free(moveset);
		return ImportationGameAllocationErr;
	}

	// -- Conversions -- //


	// Get the table address
	offsets = (gAddr::MovesetTable*)(moveset + header.offsets.tableBlock);


	//Convert move offets into ptrs
	ConvertMovesOffsets(moveset, gameMoveset, offsets, header.offsets);

	// Turn our table offsets into ptrs. Do this only at the end because we actually need those offsets above
	ConvertMovesetTableOffsets(header.offsets, moveset, gameMoveset);

	// Turn our mota offsets into mota ptrs, or copy the currently loaded character's mota for each we didn't provide
	ConvertMotaListOffsets(header.offsets.motalistsBlock, moveset, gameMoveset, playerAddress);


	// -- Allocation &Conversion finished -- //

	// Finally write our moveset to the game's memory
	printf("Moveset allocated at %llx\n", gameMoveset);
	m_process->writeBytes(gameMoveset, moveset, s_moveset);

	// Todo: write moveset to player
	// Todo: Write move 32769 to player

	// -- Cleanup -- //

	free(moveset);
	return ImportationSuccessful;
}

bool ImporterT7::CanImport()
{
	gameAddr playerAddress = m_game->ReadPtr("p1_addr");
	// We'll just read through a bunch of values that wouldn't be valid if a moveset wasn't loaded
	// readInt64() may return -1 if the read fails so we have to check for this value as well.

	if (playerAddress == 0 || playerAddress == -1) {
		return false;
	}

	gameAddr currentMove = m_process->readInt64(playerAddress + 0x220);
	if (currentMove == 0 || currentMove == -1) {
		return false;
	}

	gameAddr animAddr = m_process->readInt64(currentMove + 0x10);
	if (animAddr == 0 || animAddr == -1) {
		return false;
	}

	uint8_t animType = m_process->readInt8(animAddr);
	return animType == 0x64 || animType == 0xC8;
}
