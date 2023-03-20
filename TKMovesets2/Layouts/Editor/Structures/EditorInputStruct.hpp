#pragma once

#include "Editor.hpp"
#include "EditorFormList.hpp"

class EditorInputStruct : public EditorFormList
{
	void BuildItemDetails(int listIdx) override;
public:
	EditorInputStruct(std::string windowTitleBase, uint32_t t_id, Editor* editor, EditorWindowBase* baseWindow, int listSize);
};