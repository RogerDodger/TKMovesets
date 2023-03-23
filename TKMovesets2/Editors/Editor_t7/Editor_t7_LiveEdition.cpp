#include "Editor_t7.hpp"

// Live edition callbacks that will only be called if live edition is enabled, the moveset is loaded in memory AND if the modified field is valid

void EditorT7::Live_OnMoveEdit(int id, EditorInput* field)
{
	std::string& name = field->name;

	if (name == "anim_name") {
		return ;
	}

	uint64_t blockStart = live_loadedMoveset + m_header->offsets.movesetBlock;
	gameAddr move = (uint64_t)m_infos->table.move + blockStart + id * sizeof(Move);

	if (name == "vulnerability") {
		m_process->writeInt32(move + offsetof(Move, vuln), (uint32_t)atoi(field->buffer));
	}
	else if (name == "hitlevel") {
		m_process->writeInt32(move + offsetof(Move, hitlevel), (uint32_t)strtol(field->buffer, nullptr, 16));
	}
	else if (name == "transition") {
		m_process->writeInt16(move + offsetof(Move, transition), (uint16_t)atoi(field->buffer));
	}
	else if (name == "moveId_val1") {
		m_process->writeInt16(move + offsetof(Move, moveId_val1), (uint16_t)atoi(field->buffer));
	}
	else if (name == "moveId_val2") {
		m_process->writeInt16(move + offsetof(Move, moveId_val2), (uint16_t)atoi(field->buffer));
	}
	else if (name == "anim_len") {
		m_process->writeInt32(move + offsetof(Move, anim_len), (uint32_t)atoi(field->buffer));
	}
	else if (name == "airborne_start") {
		m_process->writeInt32(move + offsetof(Move, airborne_start), (uint32_t)atoi(field->buffer));
	}
	else if (name == "airborne_end") {
		m_process->writeInt32(move + offsetof(Move, airborne_end), (uint32_t)atoi(field->buffer));
	}
	else if (name == "ground_fall") {
		m_process->writeInt32(move + offsetof(Move, ground_fall), (uint32_t)atoi(field->buffer));
	}
	else if (name == "hitbox_location") {
		m_process->writeInt32(move + offsetof(Move, hitbox_location), (uint32_t)strtol(field->buffer, nullptr, 16));
	}
	else if (name == "first_active_frame") {
		m_process->writeInt32(move + offsetof(Move, first_active_frame), (uint32_t)atoi(field->buffer));
	}
	else if (name == "last_active_frame") {
		m_process->writeInt32(move + offsetof(Move, last_active_frame), (uint32_t)atoi(field->buffer));
	}
	else if (name == "distance") {
		m_process->writeInt16(move + offsetof(Move, distance), (uint16_t)atoi(field->buffer));
	}

	else if (name == "cancel_addr") {
		int id = atoi(field->buffer);
		gameAddr cancelAddr = blockStart + (uint64_t)m_infos->table.cancel + id * sizeof(Cancel);
		m_process->writeInt64(move + offsetof(Move, cancel_addr), cancelAddr);
	}
	else if (name == "hit_condition_addr") {
		int id = atoi(field->buffer);
		gameAddr hitConditionAddr = blockStart + (uint64_t)m_infos->table.hitCondition + id * sizeof(HitCondition);
		m_process->writeInt64(move + offsetof(Move, hit_condition_addr), hitConditionAddr);
	}
	else if (name == "extra_properties_addr") {
		int id = atoi(field->buffer);
		gameAddr propAddr = blockStart + (uint64_t)m_infos->table.extraMoveProperty + id * sizeof(ExtraMoveProperty);
		m_process->writeInt64(move + offsetof(Move, extra_move_property_addr), propAddr);
	}
	else if (name == "move_start_extraprop_addr") {
		int id = atoi(field->buffer);
		gameAddr propAddr = blockStart + (uint64_t)m_infos->table.moveBeginningProp + id * sizeof(OtherMoveProperty);
		m_process->writeInt64(move + offsetof(Move, move_start_extraprop_addr), propAddr);
	}
	else if (name == "move_end_extraprop_addr") {
		int id = atoi(field->buffer);
		gameAddr propAddr = blockStart + (uint64_t)m_infos->table.moveEndingProp + id * sizeof(OtherMoveProperty);
		m_process->writeInt64(move + offsetof(Move, move_end_extraprop_addr), propAddr);
	}
	else if (name == "voiceclip_addr") {
		int id = atoi(field->buffer);
		gameAddr voiceclipAddr = blockStart + (uint64_t)m_infos->table.voiceclip + id * sizeof(Voiceclip);
		m_process->writeInt64(move + offsetof(Move, voicelip_addr), voiceclipAddr);
	}

	else if (name == "cancel_addr_2") {
		int id = atoi(field->buffer);
		gameAddr cancelAddr = blockStart + (uint64_t)m_infos->table.cancel + id * sizeof(Cancel);
		m_process->writeInt64(move + offsetof(Move, _0x28_cancel_addr), cancelAddr);
	}
	else if (name == "cancel_addr_3") {
		int id = atoi(field->buffer);
		gameAddr cancelAddr = blockStart + (uint64_t)m_infos->table.cancel + id * sizeof(Cancel);
		m_process->writeInt64(move + offsetof(Move, _0x38_cancel_addr), cancelAddr);
	}
	else if (name == "cancel_addr_4") {
		int id = atoi(field->buffer);
		gameAddr cancelAddr = blockStart + (uint64_t)m_infos->table.cancel + id * sizeof(Cancel);
		m_process->writeInt64(move + offsetof(Move, _0x48_cancel_addr), cancelAddr);
	}

	else if (name == "_0x34_int") {
		m_process->writeInt32(move + offsetof(Move, _0x34_int), (uint32_t)atoi(field->buffer));
	}
	else if (name == "_0x44_int") {
		m_process->writeInt32(move + offsetof(Move, _0x44_int), (uint32_t)atoi(field->buffer));
	}
	else if (name == "_0x56_short") {
		m_process->writeInt16(move + offsetof(Move, _0x56_short), (uint16_t)atoi(field->buffer));
	}
	else if (name == "_0x5C_short") {
		m_process->writeInt16(move + offsetof(Move, _0x5C_short), (uint16_t)atoi(field->buffer));
	}
	else if (name == "_0x5E_short") {
		m_process->writeInt16(move + offsetof(Move, _0x5E_short), (uint16_t)atoi(field->buffer));
	}
	else if (name == "_0x98_int") {
		m_process->writeInt32(move + offsetof(Move, _0x98_int), (uint32_t)atoi(field->buffer));
	}
	else if (name == "_0xA8_short") {
		m_process->writeInt16(move + offsetof(Move, _0xA8_short), (uint16_t)atoi(field->buffer));
	}
	else if (name == "_0xAC_short") {
		m_process->writeInt16(move + offsetof(Move, _0xAC_short), (uint16_t)atoi(field->buffer));
	}
}

void EditorT7::Live_OnCancelEdit(int id, EditorInput* field)
{
	std::string& name = field->name;
	auto& buffer = field->buffer;
	
	uint64_t blockStart = live_loadedMoveset + m_header->offsets.movesetBlock;
	gameAddr cancel = blockStart + (uint64_t)m_infos->table.cancel + id * sizeof(Cancel);

	if (name == "command") {
		uint64_t command = (uint64_t)strtoll(field->buffer, nullptr, 16);
		m_process->writeInt64(cancel + offsetof(Cancel, command), command);
	}
	else if (name == "requirements_addr") {
		int id = atoi(field->buffer);
		gameAddr reqAddr = blockStart + (uint64_t)m_infos->table.requirement + id * sizeof(Requirement);
		m_process->writeInt64(cancel + offsetof(Cancel, requirements_addr), reqAddr);
	}
	else if (name == "extradata_addr") {
		int id = atoi(field->buffer);
		gameAddr extradataAddr = blockStart + (uint64_t)m_infos->table.cancelExtradata + id * sizeof(CancelExtradata);
		m_process->writeInt64(cancel + offsetof(Cancel, extradata_addr), extradataAddr);
	}
	else if (name == "detection_start") {
		m_process->writeInt16(cancel + offsetof(Cancel, detection_start), atoi(field->buffer));
	}
	else if (name == "detection_end") {
		m_process->writeInt16(cancel + offsetof(Cancel, detection_end), atoi(field->buffer));
	}
	else if (name == "starting_frame") {
		m_process->writeInt16(cancel + offsetof(Cancel, starting_frame), atoi(field->buffer));
	}
	else if (name == "move_id") {
		m_process->writeInt16(cancel + offsetof(Cancel, move_id), atoi(field->buffer));
	} else if (name == "cancel_option") {
		m_process->writeInt16(cancel + offsetof(Cancel, cancel_option), atoi(field->buffer));
	}

}

void EditorT7::Live_OnGroupedCancelEdit(int id, EditorInput* field)
{
	std::string& name = field->name;
	auto& buffer = field->buffer;

	uint64_t blockStart = live_loadedMoveset + m_header->offsets.movesetBlock;
	gameAddr cancel = blockStart + (uint64_t)m_infos->table.groupCancel + id * sizeof(Cancel);

	if (name == "command") {
		uint64_t command = (uint64_t)strtoll(field->buffer, nullptr, 16);
		m_process->writeInt64(cancel + offsetof(Cancel, command), command);
	}
	else if (name == "requirements_addr") {
		int id = atoi(field->buffer);
		gameAddr reqAddr = blockStart + (uint64_t)m_infos->table.requirement + id * sizeof(Requirement);
		m_process->writeInt64(cancel + offsetof(Cancel, requirements_addr), reqAddr);
	}
	else if (name == "extradata_addr") {
		int id = atoi(field->buffer);
		gameAddr extradataAddr = blockStart + (uint64_t)m_infos->table.cancelExtradata + id * sizeof(CancelExtradata);
		m_process->writeInt64(cancel + offsetof(Cancel, extradata_addr), extradataAddr);
	}
	else if (name == "detection_start") {
		m_process->writeInt32(cancel + offsetof(Cancel, detection_start), atoi(field->buffer));
	}
	else if (name == "detection_end") {
		m_process->writeInt32(cancel + offsetof(Cancel, detection_end), atoi(field->buffer));
	}
	else if (name == "starting_frame") {
		m_process->writeInt32(cancel + offsetof(Cancel, starting_frame), atoi(field->buffer));
	}
	else if (name == "move_id") {
		m_process->writeInt16(cancel + offsetof(Cancel, move_id), atoi(field->buffer));
	}
	else if (name == "cancel_option") {
		m_process->writeInt16(cancel + offsetof(Cancel, cancel_option), atoi(field->buffer));
	}

}

void EditorT7::Live_OnVoiceclipEdit(int id, EditorInput* field)
{
	std::string& name = field->name;
	auto& buffer = field->buffer;

	uint64_t blockStart = live_loadedMoveset + m_header->offsets.movesetBlock;
	gameAddr voiceclip = blockStart + (uint64_t)m_infos->table.voiceclip + id * sizeof(Voiceclip);

	if (name == "id") {
		m_process->writeInt32(voiceclip + offsetof(Voiceclip, id), (uint32_t)strtol(field->buffer, nullptr, 16));
	}
}

void EditorT7::Live_OnCancelExtraEdit(int id, EditorInput* field)
{
	std::string& name = field->name;
	auto& buffer = field->buffer;

	uint64_t blockStart = live_loadedMoveset + m_header->offsets.movesetBlock;
	gameAddr cancelExtra = blockStart + (uint64_t)m_infos->table.cancelExtradata + id * sizeof(CancelExtradata);

	if (name == "value") {
		m_process->writeInt32(cancelExtra + offsetof(CancelExtradata, value), (uint32_t)strtol(field->buffer, nullptr, 16));
	}
}

void EditorT7::Live_OnExtrapropertyEdit(int id, EditorInput* field)
{
	std::string& name = field->name;
	auto& buffer = field->buffer;

	uint64_t blockStart = live_loadedMoveset + m_header->offsets.movesetBlock;
	gameAddr prop = blockStart + (uint64_t)m_infos->table.extraMoveProperty + id * sizeof(ExtraMoveProperty);

	if (name == "starting_frame") {
		m_process->writeInt32(prop + offsetof(ExtraMoveProperty, starting_frame), atoi(field->buffer));
	}
	else if (name == "id") {
		m_process->writeInt32(prop + offsetof(ExtraMoveProperty, id), (uint32_t)strtoll(field->buffer, nullptr, 16));
	}
	else
	{
		if (name == "value_hex") {
			m_process->writeInt32(prop + offsetof(ExtraMoveProperty, value_unsigned), (uint32_t)strtoll(field->buffer, nullptr, 16));
		}
		else if (name == "value_unsigned") {
			m_process->writeInt32(prop + offsetof(ExtraMoveProperty, value_unsigned), (uint32_t)atoi(field->buffer));
		}
		else if (name == "value_signed") {
			m_process->writeInt32(prop + offsetof(ExtraMoveProperty, value_unsigned), (int32_t)atoi(field->buffer));
		}
		else if (name == "value_float") {
			m_process->writeFloat(prop + offsetof(ExtraMoveProperty, value_unsigned), atof(field->buffer));
		}
	}
}

void EditorT7::Live_OnMoveBeginPropEdit(int id, EditorInput* field)
{
	std::string& name = field->name;
	auto& buffer = field->buffer;

	uint64_t blockStart = live_loadedMoveset + m_header->offsets.movesetBlock;
	gameAddr prop = blockStart + (uint64_t)m_infos->table.moveBeginningProp + id * sizeof(OtherMoveProperty);

	if (name == "requirements_addr") {
		int id = atoi(field->buffer);
		gameAddr reqAddr = blockStart + (uint64_t)m_infos->table.requirement + id * sizeof(Requirement);
		m_process->writeInt64(prop + offsetof(OtherMoveProperty, requirements_addr), reqAddr);
	}
	else if (name == "extraprop") {
		m_process->writeInt32(prop + offsetof(OtherMoveProperty, extraprop), (uint32_t)strtoll(field->buffer, nullptr, 16));
	}
	else if (name == "value") {
		m_process->writeInt32(prop + offsetof(OtherMoveProperty, value), (uint32_t)atoi(field->buffer));
	}
}

void EditorT7::Live_OnMoveEndPropEdit(int id, EditorInput* field)
{
	std::string& name = field->name;
	auto& buffer = field->buffer;

	uint64_t blockStart = live_loadedMoveset + m_header->offsets.movesetBlock;
	gameAddr prop = blockStart + (uint64_t)m_infos->table.moveEndingProp + id * sizeof(OtherMoveProperty);

	if (name == "requirements_addr") {
		int id = atoi(field->buffer);
		gameAddr reqAddr = blockStart + (uint64_t)m_infos->table.requirement + id * sizeof(Requirement);
		m_process->writeInt64(prop + offsetof(OtherMoveProperty, requirements_addr), reqAddr);
	}
	else if (name == "extraprop") {
		m_process->writeInt32(prop + offsetof(OtherMoveProperty, extraprop), (uint32_t)strtoll(field->buffer, nullptr, 16));
	}
	else if (name == "value") {
		m_process->writeInt32(prop + offsetof(OtherMoveProperty, value), (uint32_t)atoi(field->buffer));
	}
}

void EditorT7::Live_OnRequirementEdit(int id, EditorInput* field)
{
	std::string& name = field->name;
	auto& buffer = field->buffer;

	uint64_t blockStart = live_loadedMoveset + m_header->offsets.movesetBlock;
	gameAddr requirement = blockStart + (uint64_t)m_infos->table.requirementCount + id * sizeof(Requirement);

	if (name == "condition") {
		m_process->writeInt32(requirement + offsetof(Requirement, condition), atoi(field->buffer));
	}
	else if (name == "param") {
		m_process->writeInt32(requirement + offsetof(Requirement, param), atoi(field->buffer));
	}
}

void EditorT7::Live_OnHitConditionPropEdit(int id, EditorInput* field)
{
	std::string& name = field->name;
	auto& buffer = field->buffer;

	uint64_t blockStart = live_loadedMoveset + m_header->offsets.movesetBlock;
	gameAddr hitCondition = blockStart + (uint64_t)m_infos->table.hitCondition + id * sizeof(HitCondition);

	if (name == "requirements_addr") {
		int id = atoi(field->buffer);
		gameAddr reqAddr = blockStart + (uint64_t)m_infos->table.requirement + id * sizeof(Requirement);
		m_process->writeInt64(hitCondition + offsetof(HitCondition, requirements_addr), reqAddr);
	}
	else if (name == "reactions_addr") {
		int id = atoi(field->buffer);
		gameAddr reactAddr = blockStart + (uint64_t)m_infos->table.reactions + id * sizeof(Reactions);
		m_process->writeInt64(hitCondition + offsetof(HitCondition, reactions_addr), reactAddr);
	}
	else if (name == "damage") {
		m_process->writeInt32(hitCondition + offsetof(HitCondition, damage), (uint32_t)atoi(field->buffer));
	}
	else if (name == "_0xC_int") {
		m_process->writeInt32(hitCondition + offsetof(HitCondition, _0xC_int), (uint32_t)atoi(field->buffer));
	}
}

void EditorT7::Live_OnPushbackEdit(int id, EditorInput* field)
{
	std::string& name = field->name;
	auto& buffer = field->buffer;

	uint64_t blockStart = live_loadedMoveset + m_header->offsets.movesetBlock;
	gameAddr pushback = blockStart + (uint64_t)m_infos->table.hitCondition + id * sizeof(Pushback);

	if (name == "duration") {
		m_process->writeInt16(pushback + offsetof(Pushback, duration), (uint16_t)atoi(field->buffer));
	}
	else if (name == "displacement") {
		m_process->writeInt16(pushback + offsetof(Pushback, displacement), (uint16_t)atoi(field->buffer));
	}
	else if (name == "num_of_loops") {
		m_process->writeInt32(pushback + offsetof(Pushback, num_of_loops), (uint32_t)atoi(field->buffer));
	}
	else if (name == "extradata_addr") {
		int id = atoi(field->buffer);
		gameAddr extraAddr = blockStart + (uint64_t)m_infos->table.pushbackExtradata + id * sizeof(PushbackExtradata);
		m_process->writeInt64(pushback + offsetof(Pushback, extradata_addr), extraAddr);
	}
}


void EditorT7::Live_OnPushbackExtraEdit(int id, EditorInput* field)
{
	std::string& name = field->name;
	auto& buffer = field->buffer;

	uint64_t blockStart = live_loadedMoveset + m_header->offsets.movesetBlock;
	uint64_t pushbackExtra = blockStart + (uint64_t)m_infos->table.pushbackExtradata + id * sizeof(PushbackExtradata);

	if (name == "horizontal_offset") {
		m_process->writeInt16(pushbackExtra + offsetof(PushbackExtradata, horizontal_offset), (uint16_t)atoi(field->buffer));
	}
}

void EditorT7::Live_OnInputSequenceEdit(int id, EditorInput* field)
{
	std::string& name = field->name;
	auto& buffer = field->buffer;

	uint64_t blockStart = live_loadedMoveset + m_header->offsets.movesetBlock;
	uint64_t inputSequence = blockStart + (uint64_t)m_infos->table.inputSequence + id * sizeof(InputSequence);

	if (name == "reactions_addr") {
		int id = atoi(field->buffer);
		uint64_t inputAddr = blockStart + (uint64_t)m_infos->table.input + id * sizeof(Input);
		m_process->writeInt64(inputSequence + offsetof(InputSequence, input_addr), inputAddr);
	}
	else if (name == "input_window_frames") {
		m_process->writeInt16(inputSequence + offsetof(InputSequence, input_window_frames), (uint16_t)atoi(field->buffer));
	}
	else if (name == "input_amount") {
		m_process->writeInt16(inputSequence + offsetof(InputSequence, input_amount), (uint16_t)atoi(field->buffer));
	}
	else if (name == "_0x4_int") {
		m_process->writeInt16(inputSequence + offsetof(InputSequence, _0x4_int), (uint32_t)atoi(field->buffer));
	}
}

void EditorT7::Live_OnInputEdit(int id, EditorInput* field)
{
	std::string& name = field->name;
	auto& buffer = field->buffer;

	uint64_t blockStart = live_loadedMoveset + m_header->offsets.movesetBlock;
	uint64_t input = blockStart + (uint64_t)m_infos->table.input + id * sizeof(Input);

	if (name == "button") {
		m_process->writeInt32(input + offsetof(Input, button), (uint32_t)strtoll(field->buffer, nullptr, 16));
	}
	else if (name == "direction")
	{
		m_process->writeInt32(input + offsetof(Input, direction), (uint32_t)strtoll(field->buffer, nullptr, 16));
	}

}

void EditorT7::Live_OnFieldEdit(EditorWindowType_ type, int id, EditorInput* field)
{
#ifdef BUILD_TYPE_DEBUG
	printf("Live_OnFieldEdit, type %d id %d, field name [%s], buffer [%s], loaded moveset %llx\n", type, id, field->name.c_str(), field->buffer, live_loadedMoveset);
#endif
	if (live_loadedMoveset == 0) {
		return ;
	}

	switch (type)
	{
	case EditorWindowType_Move:
		Live_OnMoveEdit(id, field);
		break;
	case EditorWindowType_Voiceclip:
		Live_OnVoiceclipEdit(id, field);
		break;

	case EditorWindowType_Cancel:
		Live_OnCancelEdit(id, field);
		break;
	case EditorWindowType_CancelExtradata:
		Live_OnCancelExtraEdit(id, field);
		break;
	case EditorWindowType_GroupedCancel:
		Live_OnGroupedCancelEdit(id, field);
		break;
	case EditorWindowType_Extraproperty:
		Live_OnExtrapropertyEdit(id, field);
		break;
	case EditorWindowType_MoveBeginProperty:
		Live_OnMoveBeginPropEdit(id, field);
		break;
	case EditorWindowType_MoveEndProperty:
		Live_OnMoveEndPropEdit(id, field);
		break;

	case EditorWindowType_Requirement:
		Live_OnRequirementEdit(id, field);
		break;

	case EditorWindowType_HitCondition:
		Live_OnHitConditionPropEdit(id, field);
		break;
	case EditorWindowType_Reactions:
		break;
	case EditorWindowType_Pushback:
		Live_OnPushbackEdit(id, field);
		break;
	case EditorWindowType_PushbackExtradata:
		Live_OnPushbackExtraEdit(id, field);
		break;

	case EditorWindowType_InputSequence:
		Live_OnInputSequenceEdit(id, field);
		break;
	case EditorWindowType_Input:
		Live_OnInputEdit(id, field);
		break;

	case EditorWindowType_Projectile:
		break;

	case EditorWindowType_CameraData:
		break;
	case EditorWindowType_ThrowData:
		break;
	}

	return ;
}