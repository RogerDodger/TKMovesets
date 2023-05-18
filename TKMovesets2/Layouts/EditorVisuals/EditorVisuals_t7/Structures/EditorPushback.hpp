#pragma once

#include "EditorForm.hpp"

class EditorPushback : public EditorForm
{
private:
	void OnFieldLabelClick(int listIdx, EditorInput* field) override;
public:
	using EditorForm::EditorForm;
	void RequestFieldUpdate(EditorWindowType winType, int valueChange, int listStart, int listEnd) override;
};