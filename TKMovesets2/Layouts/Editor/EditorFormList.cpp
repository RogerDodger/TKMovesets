#include <ImGui.h>
#include <format>
#include <string>

#include "imgui_extras.hpp"
#include "EditorFormList.hpp"
#include "Localization.hpp"

// -- Static helpers -- //
static int GetColumnCount()
{
	float windowWidth = ImGui::GetWindowWidth();

	if (windowWidth > 1200) {
		return 8;
	}
	if (windowWidth > 460) {
		return 4;
	}
	if (windowWidth > 230) {
		return 2;
	}

	return 1;
}

// -- Private methods -- //

void EditorFormList::Apply()
{
	if (!IsFormValid()) {
		return;
	}

	if (m_listSizeChange != 0)
	{
		// If items were added/removed, reallocate entire moveset
		int oldSize = m_listSize - m_listSizeChange;
		m_editor->ModifyListSize(windowType, id, oldSize, m_listSize);
		if (m_listSize != 0) {
			OnResize(m_listSizeChange, oldSize);
		}
		m_listSizeChange = 0;
	}

	unsavedChanges = false;
	justAppliedChanges = true;
	m_requestedClosure = false;

	if (m_listSize == 0) {
		// List is now empty, no nede to kepe this open
		popen = false;
		return;
	}

	// Write into every individual item
	for (uint32_t listIndex = 0; listIndex < m_listSize; ++listIndex) {
		
		m_editor->SaveItem(windowType, id + listIndex, m_items[listIndex]->identifierMaps);
	}

	OnApply();
}

bool EditorFormList::IsFormValid()
{
	for (uint32_t listIndex = 0; listIndex < m_listSize; ++listIndex)
	{
		for (auto& [category, fields] : m_items[listIndex]->categoryMaps) {
			for (auto& field : fields) {
				if (field->errored) {
					return false;
				}
			}
		}
	}
	return true;
}

void EditorFormList::BuildItemDetails(int listIdx)
{
	std::string label = std::format("{} {} ({})", _(std::format("{}.window_name", m_identifierPrefix).c_str()), listIdx, listIdx + id);

	m_items[listIdx]->itemLabel = label;
}

void EditorFormList::RenderListControlButtons(int listIndex)
{
	ImVec2 cursor = ImGui::GetCursorPos();

	const ImVec2 buttonSize(18, 18);
	const float buttonWidth = 25;
	float pos_x = ImGui::GetContentRegionAvail().x + 15 - buttonWidth * 4;

	if (pos_x <= 220) {
		// If too little place, just don't draw controls (for now)
		return;
	}

	ImGui::PushID(this);
	ImGui::PushID(listIndex);

	// No point in having the + on every single item
	if (listIndex + 1 != m_listSize || m_listSize == 1) {
		ImGui::SetCursorPosX(pos_x);
		if (ImGui::Button("+", buttonSize)) {
			VectorSet<std::string> drawOrder;

			m_items.insert(m_items.begin() + listIndex, new FieldItem);
			auto& item = m_items[listIndex];

			item->identifierMaps = m_editor->GetFormFieldsList(windowType, 0, drawOrder, 1)[0];
			item->openStatus = EditorFormTreeview_ForceOpen;

			for (uint8_t category : m_categories)
			{
				std::vector<EditorInput*> inputs;
				for (const std::string& fieldName : drawOrder) {
					EditorInput* field = item->identifierMaps[fieldName];
					EditorFormUtils::SetFieldDisplayText(field, field->fullName);
					if (field->category == category) {
						inputs.push_back(field);
					}
				}
				item->categoryMaps[category] = inputs;
			}
			BuildItemDetails(listIndex);

			++m_listSizeChange;
			++m_listSize;

			unsavedChanges = true;
		}
		ImGui::SameLine();
	}

	// Don't allow first item and ending item to shift up
	if (listIndex != 0 && listIndex + 1 != m_listSize) {
		ImGui::SetCursorPosX(pos_x + buttonWidth);
		if (ImGui::Button("^", buttonSize))
		{
			// Move item UP
			std::iter_swap(m_items.begin() + listIndex, m_items.begin() + listIndex - 1);

			// Ensure that whatever is open/closed will stay that way after being moved. TreeNode are annoying like that.
			if (m_items[listIndex]->openStatus == EditorFormTreeview_Opened) {
				m_items[listIndex]->openStatus = EditorFormTreeview_ForceOpen;
			}
			else if (m_items[listIndex]->openStatus == EditorFormTreeview_Closed) {
				m_items[listIndex]->openStatus = EditorFormTreeview_ForceClose;
			}

			if (m_items[listIndex - 1]->openStatus == EditorFormTreeview_Opened) {
				m_items[listIndex - 1]->openStatus = EditorFormTreeview_ForceOpen;
			}
			else if (m_items[listIndex - 1]->openStatus == EditorFormTreeview_Closed) {
				m_items[listIndex - 1]->openStatus = EditorFormTreeview_ForceClose;
			}

			unsavedChanges = true;
		}
		ImGui::SameLine();
	}

	// Don't allow last 2 items to shift down
	if (listIndex + 2 < m_listSize) {
		ImGui::SetCursorPosX(pos_x + buttonWidth * 2);
		if (ImGui::Button("V", buttonSize))
		{
			// Move item DOWN
			std::iter_swap(m_items.begin() + listIndex, m_items.begin() + listIndex + 1);

			// Ensure that whatever is open/closed will stay that way after being moved. TreeNode are annoying like that.
			if (m_items[listIndex]->openStatus == EditorFormTreeview_Opened) {
				m_items[listIndex]->openStatus = EditorFormTreeview_ForceOpen;
			}
			else if (m_items[listIndex]->openStatus == EditorFormTreeview_Closed) {
				m_items[listIndex]->openStatus = EditorFormTreeview_ForceClose;
			}

			if (m_items[listIndex + 1]->openStatus == EditorFormTreeview_Opened) {
				m_items[listIndex + 1]->openStatus = EditorFormTreeview_ForceOpen;
			}
			else if (m_items[listIndex + 1]->openStatus == EditorFormTreeview_Closed) {
				m_items[listIndex + 1]->openStatus = EditorFormTreeview_ForceClose;
			}

			unsavedChanges = true;
		}
		ImGui::SameLine();
	}

	// Don't show delete button on last item
	if (listIndex + 1 != m_listSize || m_listSize == 1)
	{
		ImGui::SetCursorPosX(pos_x + buttonWidth * 3);
		ImGui::PushStyleColor(ImGuiCol_Button, FORM_DELETE_BTN);
		if (ImGui::Button("X", buttonSize))
		{
			// Delete this item
			--m_listSizeChange;
			--m_listSize;

			// Only delete if we're not at the list end, to avoid destructing the fields before using them
			// Reducing list size will make things invisible on the next render anyway
			if (listIndex < m_listSize) {
				// Shift following items up
				delete m_items[listIndex];
				m_items.erase(m_items.begin() + listIndex);
				unsavedChanges = true;
			}
		}
		ImGui::PopStyleColor();
		ImGui::SameLine();
	}


	ImGui::PopID();
	ImGui::PopID();
	ImGui::SetCursorPos(cursor);
}

// -- Public methods -- //

void EditorFormList::InitForm(std::string windowTitleBase, uint32_t t_id, Editor* editor)
{
	id = t_id;
	m_editor = editor;

	m_identifierPrefix = "edition." + EditorFormUtils::GetWindowTypeName(windowType);

	VectorSet<std::string> drawOrder;

	std::vector<std::map<std::string, EditorInput*>> fieldIdentifierMaps;
	if (m_listSize == 0) {
		fieldIdentifierMaps = editor->GetFormFieldsList(windowType, t_id, drawOrder);
		m_listSize = fieldIdentifierMaps.size();
	} else {
		// Some struct lists may have fixed list sizes: in this case, we'll call a different function
		// For now these two GetFormFields aren't swappable, if a list is not supposed to have a known size ahead of time, don't provide a size
		fieldIdentifierMaps = editor->GetFormFieldsList(windowType, t_id, drawOrder, m_listSize);
	}

	// Tries to find a name to show in the window title
	// Also figure out the categories
	for (const std::string& fieldName : drawOrder) {
		EditorInput* field = fieldIdentifierMaps[0][fieldName];
		m_categories.insert(field->category);
	}

	// Builds the <category : fields> maps & the item labels
	for (uint32_t listIndex = 0; listIndex < m_listSize; ++listIndex)
	{
		m_items.push_back(new FieldItem);
		auto& item = m_items[listIndex];

		item->identifierMaps = fieldIdentifierMaps[listIndex];

		for (uint8_t category : m_categories)
		{
			std::vector<EditorInput*> inputs;
			for (const std::string& fieldName : drawOrder) {
				EditorInput* field = fieldIdentifierMaps[listIndex][fieldName];
				EditorFormUtils::SetFieldDisplayText(field, _(field->fullName.c_str()));
				if (field->category == category) {
					inputs.push_back(field);
				}
			}
			item->categoryMaps[category] = inputs;
		}

		BuildItemDetails(listIndex);
	}

	// Build category names
	for (uint8_t category : m_categories) {
		m_categoryStringIdentifiers[category] = std::format("{}.category_{}", m_identifierPrefix, category);
	}

	m_windowTitleBase = windowTitleBase;
	ApplyWindowName(false);

}

void EditorFormList::RenderInternal()
{
	currentViewport = ImGui::GetWindowViewport();

	// Responsive form that tries to use big widths to draw up to 4 fields (+ 4 labels) per line
	const int columnCount = EditorFormUtils::GetColumnCount();
	m_labelWidthHalf = m_winInfo.size.x / columnCount / 2;

	const float drawWidth = ImGui::GetContentRegionAvail().x;
	ImDrawList* drawlist = ImGui::GetWindowDrawList();

	for (uint32_t listIndex = 0; listIndex < m_listSize; ++listIndex)
	{
		{
			ImVec2 drawStart = m_winInfo.pos + ImGui::GetCursorPos();
			drawStart.y -= ImGui::GetScrollY() + 2;
			int color = m_items[listIndex]->color;
			color = color == 0 ? (listIndex & 1 ? FORM_BG_1 : FORM_BG_2) : color;
			drawlist->AddRectFilled(drawStart, drawStart + ImVec2(drawWidth, ImGui::GetTextLineHeightWithSpacing() + 4), color);
		}

		if (id > 1) {
			// Only non-starting lists may have access to list controls
			RenderListControlButtons(listIndex);
		}

		auto& item = m_items[listIndex];

		// This allows us to force some treenode to open at certain times, without forcing them to be closed
		auto openStatus = item->openStatus;
		if (openStatus == EditorFormTreeview_ForceOpen || openStatus == EditorFormTreeview_ForceClose) {
			ImGui::SetNextItemOpen(openStatus == EditorFormTreeview_ForceOpen);
		}

		ImGui::PushID(listIndex);
		if (!ImGui::TreeNodeExV(this + listIndex, ImGuiTreeNodeFlags_SpanAvailWidth, item->itemLabel.c_str(), va_list())) {
			// Tree node hidden so no need to render anything
			ImGui::PopID();
			item->openStatus = EditorFormTreeview_Closed;
			continue;
		}
		ImGui::PopID();
		item->openStatus = EditorFormTreeview_Opened;

		// Responsive form that tries to use big widths to draw up to 4 fields (+ 4 labels) per line
		for (uint8_t category : m_categories)
		{
			const int headerFlags = ImGuiTreeNodeFlags_Framed | (category & 1 ? 0 : ImGuiTreeNodeFlags_DefaultOpen);
			if (category != 0 && !ImGui::CollapsingHeader(_(m_categoryStringIdentifiers[category].c_str()), headerFlags)) {
				// Only show titles for category > 0, and if tree is not open: no need to render anything
				continue;
			}

			// Render each field name / field input in columns
			if (ImGui::BeginTable(m_windowTitle.c_str(), columnCount))
			{
				std::vector<EditorInput*>& inputs = item->categoryMaps[category];
				RenderInputs(listIndex, inputs, category, columnCount);
				ImGui::EndTable();
			}

		}

		ImGui::TreePop();
	}
}
