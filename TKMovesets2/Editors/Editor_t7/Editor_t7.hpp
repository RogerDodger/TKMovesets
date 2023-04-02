#pragma once

#include <vector>

#include "Editor.hpp"
#include "Helpers.hpp"

#include "Structs_t7.h"

using namespace StructsT7;

struct StructIterators
{
	StructIterator<StructsT7_gameAddr::Move> moves;
	StructIterator<Requirement> requirements;
	StructIterator<StructsT7_gameAddr::HitCondition> hit_conditions;
	StructIterator<StructsT7_gameAddr::Cancel> cancels;
	StructIterator<StructsT7_gameAddr::Cancel> grouped_cancels;
	StructIterator<StructsT7_gameAddr::Reactions> reactions;
	StructIterator<StructsT7_gameAddr::Pushback> pushbacks;
	StructIterator<PushbackExtradata> pushback_extras;
	StructIterator<CancelExtradata> cancel_extras;
	StructIterator<ExtraMoveProperty> extra_move_properties;
	StructIterator<StructsT7_gameAddr::OtherMoveProperty> move_start_properties;
	StructIterator<StructsT7_gameAddr::OtherMoveProperty> move_end_properties;
	StructIterator<StructsT7_gameAddr::Projectile> projectiles;
	StructIterator<StructsT7_gameAddr::InputSequence> input_sequences;
	StructIterator<StructsT7_gameAddr::ThrowCamera> throw_datas;
	StructIterator<Input> inputs;
	StructIterator<Voiceclip> voiceclips;
	StructIterator<CameraData> camera_datas;

	StructIterator<MvlDisplayable> mvl_displayables;
	StructIterator<MvlPlayable> mvl_playables;
	StructIterator<MvlInput> mvl_inputs;
};

class DLLCONTENT EditorT7 : public Editor
{
private:
	// Stores the moveset table containing lists offsets & count, aliases too
	MovesetInfo* m_infos = nullptr;
	// Stores absolute pointers to the moveset table within the moveset
	MovesetTable* m_absoluteTable = nullptr;
	// Contains iterators for the various structure lists
	StructIterators m_iterators;
	// Contains a ptr to the head of the displayable movelist
	MvlHead* m_mvlHead = nullptr;

public:
	using Editor::Editor;

	void LoadMovesetPtr(Byte* t_moveset, uint64_t t_movesetSize) override;
	void LoadMoveset(Byte* t_moveset, uint64_t t_movesetSize) override;
	void ReloadDisplayableMoveList(std::vector<DisplayableMove*>* ref = nullptr) override;
	uint16_t GetCurrentMoveID(uint8_t playerId) override;
	std::map<std::string, EditorInput*> GetFormFields(EditorWindowType_ type, uint16_t id, VectorSet<std::string>& drawOrder) override;
	std::vector<std::map<std::string, EditorInput*>> GetFormFieldsList(EditorWindowType_ type, uint16_t id, VectorSet<std::string>& drawOrder) override;
	std::vector<std::map<std::string, EditorInput*>> GetFormFieldsList(EditorWindowType_ type, uint16_t id, VectorSet<std::string>& drawOrder, int listSize) override;
	bool ValidateField(EditorWindowType_ fieldType, EditorInput* field) override;
	void SaveItem(EditorWindowType_ type, uint16_t id, std::map<std::string, EditorInput*>& inputs) override;

	// Extra iterators setup
	void SetupIterators_DisplayableMovelist();

	// Moves
	std::map<std::string, EditorInput*> GetMoveInputs(uint16_t id, VectorSet<std::string>& drawOrder);
	uint64_t CreateMoveName(const char* newName);
	void SaveMoveName(const char* newName, gameAddr move_name_addr);
	void SaveMove(uint16_t id, std::map<std::string, EditorInput*>& inputs);
	bool ValidateMoveField(EditorInput* field);

	// Voiceclipss
	std::vector<std::map<std::string, EditorInput*>> GetVoiceclipListInputs(uint16_t id, VectorSet<std::string>& drawOrder);
	void SaveVoiceclip(uint16_t id, std::map<std::string, EditorInput*>& inputs);

	// Extra properties
	std::vector<std::map<std::string, EditorInput*>> GetExtrapropListInputs(uint16_t id, VectorSet<std::string>& drawOrder);
	void SaveExtraproperty(uint16_t id, std::map<std::string, EditorInput*>& inputs);

	// Other properties (start)
	std::vector<std::map<std::string, EditorInput*>> GetMoveStartPropertyListInputs(uint16_t id, VectorSet<std::string>& drawOrder);
	void SaveMoveStartProperty(uint16_t id, std::map<std::string, EditorInput*>& inputs);
	bool ValidateOtherMoveProperty(EditorInput* field);

	// Other properties (end)
	std::vector<std::map<std::string, EditorInput*>> GetMoveEndPropertyListInputs(uint16_t id, VectorSet<std::string>& drawOrder);
	void SaveMoveEndProperty(uint16_t id, std::map<std::string, EditorInput*>& inputs);

	// Cancels
	std::vector<std::map<std::string, EditorInput*>> GetCancelListInputs(uint16_t id, VectorSet<std::string>& drawOrder);
	void SaveCancel(uint16_t id, std::map<std::string, EditorInput*>& inputs);
	bool ValidateCancelField(EditorInput* field);

	// Grouped Cancels
	std::vector<std::map<std::string, EditorInput*>> GetGroupedCancelListInputs(uint16_t id, VectorSet<std::string>& drawOrder);
	void SaveGroupedCancel(uint16_t id, std::map<std::string, EditorInput*>& inputs);
	bool ValidateGroupedCancelField(EditorInput* field);

	// Cancel extras
	std::map<std::string, EditorInput*> GetCancelExtraInput(uint16_t id, VectorSet<std::string>& drawOrder);
	void SaveCancelExtra(uint16_t id, std::map<std::string, EditorInput*>& inputs);

	// Requirements
	std::vector<std::map<std::string, EditorInput*>> GetRequirementListInputs(uint16_t id, VectorSet<std::string>& drawOrder);
	void SaveRequirement(uint16_t id, std::map<std::string, EditorInput*>& inputs);

	// Hit conditions
	std::vector<std::map<std::string, EditorInput*>> GetHitConditionListInputs(uint16_t id, VectorSet<std::string>& drawOrder);
	void SaveHitCondition(uint16_t id, std::map<std::string, EditorInput*>& inputs);
	bool ValidateHitConditionField( EditorInput* field);
	
	// Reactions
	std::map<std::string, EditorInput*> GetReactionsInputs(uint16_t id, VectorSet<std::string>& drawOrder);
	void SaveReactions(uint16_t id, std::map<std::string, EditorInput*>& inputs);
	bool ValidateReactionsField(EditorInput* field);

	// Pushback
	std::map<std::string, EditorInput*> GetPushbackInputs(uint16_t id, VectorSet<std::string>& drawOrder);
	void SavePushback(uint16_t id, std::map<std::string, EditorInput*>& inputs);
	bool ValidatePushbackField(EditorInput* field);

	// Pushback extradata
	std::vector<std::map<std::string, EditorInput*>> GetPushbackExtraListInputs(uint16_t id, int listSize, VectorSet<std::string>& drawOrder);
	void SavePushbackExtra(uint16_t id, std::map<std::string, EditorInput*>& inputs);

	// Input Sequence
	std::map<std::string, EditorInput*> GetInputSequenceInputs(uint16_t id, VectorSet<std::string>& drawOrder);
	void SaveInputSequence(uint16_t id, std::map<std::string, EditorInput*>& inputs);
	bool ValidateInputSequenceField(EditorInput* field);

	// Input list
	std::vector<std::map<std::string, EditorInput*>> GetInputListInputs(uint16_t id, int listSize, VectorSet<std::string>& drawOrder);
	void SaveInput(uint16_t id, std::map<std::string, EditorInput*>& inputs);

	// Projectiles
	std::map<std::string, EditorInput*> GetProjectileInputs(uint16_t id, VectorSet<std::string>& drawOrder);
	void SaveProjectile(uint16_t id, std::map<std::string, EditorInput*>& inputs);
	bool ValidateProjectileField(EditorInput* field);

	// Throw Camera
	std::map<std::string, EditorInput*> GetThrowCameraInputs(uint16_t id, VectorSet<std::string>& drawOrder);
	void SaveThrowCamera(uint16_t id, std::map<std::string, EditorInput*>& inputs);
	bool ValidateThrowCameraField(EditorInput* field);

	// Camera data
	std::map<std::string, EditorInput*> GetCameraDataInputs(uint16_t id, VectorSet<std::string>& drawOrder);
	void SaveCameraData(uint16_t id, std::map<std::string, EditorInput*>& inputs);

	//  Movelist : Displayables
	std::vector<std::map<std::string, EditorInput*>> GetMovelistDisplayablesInputs(uint16_t id, VectorSet<std::string>& drawOrder);
	void SaveMovelistDisplayable(uint16_t id, std::map<std::string, EditorInput*>& inputs);
	bool ValidateMovelistDisplayableField(EditorInput* field);

	//  Movelist : Playables
	std::map<std::string, EditorInput*> GetMovelistPlayableInputs(uint16_t id, VectorSet<std::string>& drawOrder);
	void SaveMovelistPlayable(uint16_t id, std::map<std::string, EditorInput*>& inputs);

	// Movelist: Inputs
	std::vector<std::map<std::string, EditorInput*>> GetMovelistInputListInputs(uint16_t id, int listSize, VectorSet<std::string>& drawOrder);
	void SaveMovelistInput(uint16_t id, std::map<std::string, EditorInput*>& inputs);
	
	// -- Interactions -- //
	// Sets the current move of a player
	void SetCurrentMove(uint8_t playerId, gameAddr playerMoveset, size_t moveId) override;
	// Starts a thread that extracts the moveset's animations
	void OrderAnimationsExtraction(const std::string& characterFilename) override;
	// Saves all the moveset animations in our library. Explicit copy of the parameters because this runs in another thread and the source data might be freed in parallel.
	void ExtractAnimations(Byte* moveset, std::string characterFilename, TKMovesetHeader_offsets offsets, std::map<gameAddr, uint64_t> animOffsetToNameOffset);
	// Imports an animation into the moveset and applies it to a move. Returns the name of the imported anim.
	std::string ImportAnimation(const char* filepath, int moveid) override;

	// -- Command Utils -- //
	std::string GetCommandStr(const char* direction, const char* button) override;
	std::string GetCommandStr(const char* commandBuf) override;
	void GetInputSequenceString(int id, std::string& outStr, int& outSize) override;
	std::string GetDisplayableMovelistInputStr(const char* directions, const char* buttons) override;
	int GetDisplayableMovelistEntryColor(EditorInput* field) override;
	bool IsMovelistDisplayableEntryCombo(EditorInput* field) override;
	bool IsMovelistDisplayableEntryCategory(EditorInput* field) override;
	// -- Utils --//
	bool IsCommandInputSequence(const char* buffer) override;
	bool IsCommandGroupedCancelReference(const char* buffer) override;
	int GetCommandInputSequenceID(const char* buffer) override;
	bool IsPropertyThrowCameraRef(const char* buffer) override;
	bool IsPropertyProjectileRef(const char* buffer) override;
	bool IsVoicelipValueEnd(const char* buffer) override;
	unsigned int GetStructureCount(EditorWindowType_ type) override;
	unsigned int GetMotaAnimCount(int motaId) override;

	// -- Creation / Deletion -- //
	// Create a new structure or structure list
	int32_t CreateNew(EditorWindowType_ type) override;
	template<typename T> int32_t CreateNewGeneric(T* struct_1, T* struct_2, size_t tableListOffset);
	//
	int32_t CreateInputSequence();
	int32_t CreateInputList();
	//
	int32_t CreateNewRequirements();
	//
	int32_t CreateNewMove();
	int32_t CreateNewVoiceclipList();
	int32_t CreateNewCancelList();
	int32_t CreateNewGroupedCancelList();
	int32_t CreateNewCancelExtra();
	//
	int32_t CreateNewExtraProperties();
	int32_t CreateNewMoveBeginProperties();
	int32_t CreateNewMoveEndProperties();
	//
	int32_t CreateNewHitConditions();
	int32_t CreateNewReactions();
	int32_t CreateNewPushback();
	int32_t CreateNewPushbackExtra();
	//
	int32_t CreateNewProjectile();
	//
	int32_t CreateNewThrowCamera();
	int32_t CreateNewCameraData();

	// -- List Creation / Deletion -- //
	void ModifyListSize(EditorWindowType_ type, int listId, int oldSize, int newSize) override;
	template<typename T> void ModifyGenericMovesetListSize(int listId, int oldSize, int newSize, size_t tableListOffset);
	template<typename T> void ModifyGenericMovelistListSize(int listId, int oldSize, int newSize, uint32_t listStart_offset);
	//
	void ModifyRequirementListSize(int listId, int oldSize, int newSize);
	//
	void ModifyCancelListSize(int listId, int oldSize, int newSize);
	void ModifyGroupedCancelListSize(int listId, int oldSize, int newSize);
	//
	void ModifyExtraPropertyListSize(int listId, int oldSize, int newSize);
	void ModifyStartPropertyListSize(int listId, int oldSize, int newSize);
	void ModifyEndPropertyListSize(int listId, int oldSize, int newSize);
	//
	void ModifyHitConditionListSize(int listId, int oldSize, int newSize);
	void ModifyPushbackExtraListSize(int listId, int oldSize, int newSize);
	//
	void ModifyInputListSize(int listId, int oldSize, int newSize);
	//
	void ModifyVoiceclipListSize(int listId, int oldSize, int newSize);
	// Movelist
	void ModifyMovelistInputSize(int listId, int oldSize, int newSize);
	void ModifyMovelistDisplayableSize(int listId, int oldSize, int newSize);

	// -- Live edition -- //
	// Called whenever a field is edited. Returns false if a re-import is needed.
	void Live_OnFieldEdit(EditorWindowType_ type, int id,EditorInput* field) override;
	//
	void Live_OnMoveEdit(int id, EditorInput* field);
	void Live_OnVoiceclipEdit(int id, EditorInput* field);
	//
	void Live_OnCancelEdit(int id, EditorInput* field);
	void Live_OnGroupedCancelEdit(int id, EditorInput* field);
	void Live_OnCancelExtraEdit(int id, EditorInput* field);
	//
	void Live_OnExtrapropertyEdit(int id, EditorInput* field);
	void Live_OnMoveBeginPropEdit(int id, EditorInput* field);
	void Live_OnMoveEndPropEdit(int id, EditorInput* field);
	//
	void Live_OnRequirementEdit(int id, EditorInput* field);
	//
	void Live_OnHitConditionPropEdit(int id, EditorInput* field);
	void Live_OnPushbackEdit(int id, EditorInput* field);
	void Live_OnPushbackExtraEdit(int id, EditorInput* field);
	//
	void Live_OnInputSequenceEdit(int id, EditorInput* field);
	void Live_OnInputEdit(int id, EditorInput* field);
	//
	void Live_OnReactionsEdit(int id, EditorInput* field);
	//
	void Live_OnProjectileEdit(int id, EditorInput* field);
	//
	void Live_OnThrowCameraEdit(int id, EditorInput* field);
	void Live_OnCameraDataEdit(int id, EditorInput* field);

	// Movelist text conversion
	std::string GetMovelistDisplayableText(uint32_t offset);
};
