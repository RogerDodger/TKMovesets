#include "imgui_extras.hpp"
#include "EditorMoveEndProperty.hpp"
#include "Localization.hpp"

// -- Public methods -- //

EditorMoveEndProperty::EditorMoveEndProperty(std::string windowTitleBase, uint32_t t_id, Editor* editor, EditorWindowBase* baseWindow)
{
	windowType = EditorWindowType_MoveEndProperty;
	m_baseWindow = baseWindow;
	InitForm(windowTitleBase, t_id, editor);
}

void EditorMoveEndProperty::OnFieldLabelClick(int listIdx, EditorInput* field)
{
	int id = atoi(field->buffer);
	auto& name = field->name;

	if (name == "requirements_addr") {
		m_baseWindow->OpenFormWindow(EditorWindowType_Requirement, id);
	}
}

void EditorMoveEndProperty::RequestFieldUpdate(EditorWindowType_ winType, int valueChange, int listStart, int listEnd)
{
	switch (winType)
	{
	case EditorWindowType_MoveEndProperty:
		// If a struct was created before this one, we must shfit our own ID
		if (MUST_SHIFT_ID(structureId, valueChange, listStart, listEnd)) {
			// Same shifting logic as in ListCreations
			structureId += valueChange;
			ApplyWindowName();
		}
		break;
	case EditorWindowType_Requirement:
		{
			int listIdx = 0;
			for (auto& item : m_items)
			{
				EditorInput* field = item->identifierMap["requirements_addr"];

				if (field->errored) {
					continue;
				}

				int value = atoi(field->buffer);
				if (MUST_SHIFT_ID(value, valueChange, listStart, listEnd)) {
					// Same shifting logic as in ListCreations
					sprintf_s(field->buffer, field->bufsize, "%d", value + valueChange);
					BuildItemDetails(listIdx);
				}

				++listIdx;
			}
		}
		break;
	}

}