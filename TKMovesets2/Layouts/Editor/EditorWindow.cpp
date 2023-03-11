#include <ImGui.h>

#include <format>

#include "EditorWindow.hpp"
#include "Localization.hpp"
#include "imgui_extras.hpp"
#include "helpers.hpp"

// -- Private methods -- //

bool EditorWindow::MovesetStillLoaded()
{
	// todo: check if moveset
	gameAddr movesetAddress = importerHelper.importer->GetMovesetAddress(importerHelper.currentPlayerId);
	return movesetAddress == loadedMoveset;
}

void EditorWindow::Save()
{
	// todo
	m_savedLastChange = true;
}

void EditorWindow::RenderToolBar()
{
	// todo: ImGuiWindowFlags_MenuBar ?
	ImGui::Text("TOOLS");
}

void EditorWindow::RenderStatusBar()
{
	if (ImGuiExtra::RenderButtonEnabled(_("edition.save"), !m_savedLastChange)) {
		Save();
	}

	ImGui::SameLine();
	
	ImGui::TextUnformatted(_("edition.last_saved"));
	ImGui::SameLine();
	ImGui::TextUnformatted(Helpers::currentDateTime(m_loadedCharacter.lastSavedDate).c_str());
	ImGui::SameLine();

	// Vertical separator
	ImGui::TextUnformatted("|");
	ImGui::SameLine();

	// Game list
	int8_t currentGameId = importerHelper.currentGameId;
	ImGui::PushItemWidth(100.0f);
	ImGui::PushID(&importerHelper); // Have to push an ID here because extraction.select_game would cause a conflict
	uint8_t gameListCount = Games::GetGamesCount();

	if (ImGui::BeginCombo("##", (currentGameId == -1) ? _("select_game") : Games::GetGameInfo(currentGameId)->name))
	{
		for (uint8_t i = 0; i < gameListCount; ++i)
		{
			GameInfo* game = Games::GetGameInfo(i);
			if (game->importer != nullptr) {
				if (ImGui::Selectable(game->name, currentGameId == i, 0, ImVec2(100.0f, 0))) {
					importerHelper.SetTargetProcess(game->processName, i);
					loadedMoveset = 0;
					m_liveEdition = false;
					m_importNeeded = true;
				}
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopID();
	ImGui::SameLine();


	// Process error
	bool isAttached = importerHelper.process->IsAttached();
	if (currentGameId != -1 && !isAttached)
	{
		// Short process error message
		ImGuiExtra::RenderTextbox(_("edition.process_err"), TEXTBOX_BORDER_ERROR, TEXTBOX_BORDER_ERROR, 2.5f);
		ImGui::SameLine();
	}

	// Vertical separator
	ImGui::TextUnformatted("|");
	ImGui::SameLine();

	// Player list
	{
		ImGui::SameLine();
		char buf[3] = { '1' + importerHelper.currentPlayerId, 'p', '\0' };
		ImGui::PushItemWidth(100.0f);

		uint8_t playerCount = min(2, importerHelper.GetCharacterCount());
		if (ImGui::BeginCombo("##", _(buf)))
		{
			size_t currentPlayerId = importerHelper.currentPlayerId;
			for (int8_t i = 0; i < playerCount; ++i)
			{
				buf[0] = '1' + i;
				if (ImGui::Selectable(_(buf), currentPlayerId == i, 0, ImVec2(100.0f, 0))) {
					importerHelper.currentPlayerId = i;
				}
			}
			ImGui::EndCombo();
		}
	}
	
	// Import button
	ImGui::SameLine();
	bool canImport = isAttached && m_importNeeded && !importerHelper.IsBusy() && importerHelper.CanStart();
	if (ImGuiExtra::RenderButtonEnabled(_("moveset.import"), canImport)) {
		importerHelper.lastLoadedMoveset = 0;
		importerHelper.QueueCharacterImportation(m_moveset, m_movesetSize);
		loadedMoveset = 0; // We will get the loaded moveset later since the import is in another thread
		m_liveEdition = false;
		m_importNeeded = false;
	}

	// Live edition. Might not be implemented for every game.
	if (m_liveEditable)
	{
		if (loadedMoveset == 0) {
			ImGui::BeginDisabled();
		}
		ImGui::SameLine();
		ImGui::Checkbox(_("edition.live_edition"), &m_liveEdition);
		ImGui::SameLine();
		ImGuiExtra::HelpMarker(_("edition.live_edition_explanation"));
		if (loadedMoveset == 0) {
			ImGui::EndDisabled();
		}
	}

}

void EditorWindow::RenderMovesetData(ImGuiID dockId)
{
	ImGui::SetNextWindowDockID(dockId, ImGuiCond_Once);
	ImGui::Begin("Move");
	ImGui::Text("o");
	ImGui::End();

	ImGui::SetNextWindowDockID(dockId, ImGuiCond_Once);
	ImGui::Begin("Move2");
	ImGui::Text("o2");
	ImGui::End();

	ImGui::SetNextWindowDockID(dockId, ImGuiCond_Once);
	ImGui::Begin("Move3");
	ImGui::Text("o3");
	ImGui::End();
}

void EditorWindow::RenderMovelist()
{
	uint8_t moveFilter = 0;
	// Filter
	if (ImGui::BeginTabBar("MovelistTabs"))
	{
		if (ImGui::BeginTabItem(_("edition.moves_all"))) {
			moveFilter = 0xFF;
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(_("edition.moves_attacks"))) {
			moveFilter = 0xFF;
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(_("edition.moves_generic"))) {
			moveFilter = 0xFF;
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(_("edition.moves_throws"))) {
			moveFilter = 0xFF;
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	// Movelist. Leave some 50 units of space for move player
	ImVec2 Size = ImVec2(0, ImGui::GetContentRegionAvail().y - 60);
	ImGui::BeginTable("MovelistTable", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY
	| ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit, Size);
	ImGui::TableSetupColumn("ID");
	ImGui::TableSetupColumn(_("edition.move_name"));

	for (unsigned int i = 0; i < 2000; ++i)
	{
		ImGui::TableNextColumn();
		ImGui::TextUnformatted(std::format("{}", i).c_str());

		ImGui::TableNextColumn();
		ImGui::TextUnformatted("nom");
		ImGui::TableNextRow();
	}

	ImGui::EndTable();

	// Move player
	uint16_t moveId = 32769;
	ImGui::BeginTable("MovelistPlay", 2);
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	if (ImGuiExtra::RenderButtonEnabled(_("edition.play_move_1p"), loadedMoveset != 0))
	{
		gameAddr playerAddress = importerHelper.importer->GetCharacterAddress(0);
		importerHelper.importer->SetCurrentMove(playerAddress, loadedMoveset, moveId);
	}
	ImGui::TableNextColumn();
	if (ImGuiExtra::RenderButtonEnabled(_("edition.play_move_2p"), loadedMoveset != 0))
	{
		gameAddr playerAddress = importerHelper.importer->GetCharacterAddress(1);
		importerHelper.importer->SetCurrentMove(playerAddress, loadedMoveset, moveId);
	}
	ImGui::EndTable();
}

// -- Public methods -- //

EditorWindow::~EditorWindow()
{
	importerHelper.StopThreadAndCleanup();
	free(m_moveset);
}

EditorWindow::EditorWindow(movesetInfo* movesetInfo)
{
	std::ifstream file(movesetInfo->filename.c_str(), std::ios::binary);

	if (file.fail()) {
		throw EditorWindow_MovesetLoadFail();
	}

	file.seekg(0, std::ios::end);
	m_movesetSize = file.tellg();

	m_moveset = (byte*)malloc(m_movesetSize);
	if (m_moveset == nullptr) {
		throw EditorWindow_MovesetLoadFail();
	}

	file.seekg(0, std::ios::beg);
	file.read((char*)m_moveset, m_movesetSize);

	file.close();

	m_liveEditable = Games::IsGameLiveEditable(movesetInfo->gameId);

	m_loadedCharacter.filename = movesetInfo->filename;
	m_loadedCharacter.name = movesetInfo->name;
	m_loadedCharacter.lastSavedDate = movesetInfo->date;
	m_loadedCharacter.gameId = movesetInfo->gameId;
	filename = movesetInfo->filename;
}

void EditorWindow::Render(int dockid)
{
	// Check for important changes here
	if (loadedMoveset != 0) {
		if (!MovesetStillLoaded())
		{
			m_liveEdition = false;
			m_importNeeded = true;
			loadedMoveset = 0;
		}
	}
	else {
		// If the moveset was successfully imported, this will be filled with a nonzero value
		loadedMoveset = importerHelper.lastLoadedMoveset;
	}

	// Layout start
	ImGui::SetNextWindowSize(ImVec2(1280, 720), ImGuiCond_Once);
	ImGui::SetNextWindowSizeConstraints(ImVec2(200, 200), ImVec2(9999, 9999));

	if (setFocus) {
		ImGui::SetNextWindowFocus();
		setFocus = false;
	}

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
	// ImGuiWindowFlags_MenuBar
	if (!m_savedLastChange) {
		windowFlags |= ImGuiWindowFlags_UnsavedDocument;
	}

	if (ImGui::Begin(m_loadedCharacter.name.c_str(), &popen, windowFlags))
	{
		RenderToolBar();

		const ImVec2& Size = ImGui::GetContentRegionAvail();

		if (ImGui::BeginTable("MovesetMainTable", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders
		| ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_SizingFixedSame))
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			RenderMovelist();


			ImGui::TableNextColumn();

			ImGuiID dockId = ImGui::DockSpace(dockid + 2);
			RenderMovesetData(dockId);

			ImGui::EndTable();
		}

		RenderStatusBar();
	}
	ImGui::End();
}
