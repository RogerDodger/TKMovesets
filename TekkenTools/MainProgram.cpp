#include "MainProgram.h"
#include "NavigationMenu.h"

MainProgram::MainProgram(GLFWwindow* window, const char* glsl_version)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();

	io.IniFilename = NULL; //I don't want to save settings (for now). Perhaps save in appdata later.
}

void MainProgram::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void MainProgram::Update()
{
	// Interface should be done here
	ImGui::SetNextWindowSize(ImVec2(1280, 720));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("MainWindow", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav);

	ImGui::Columns(2);
	ImGui::SetColumnOffset(1, 230); 
	navMenu.Render();

	ImGui::NextColumn();
	switch (navMenu.getMenuId())
	{
	case NAV__MENU_EXTRACT:
		break;
	case NAV__MENU_IMPORT:
		break;
	case NAV__MENU_EDITION:
		break;
	case NAV__MENU_CAMERA:
		break;
	}

	ImGui::End(); 
}

void MainProgram::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MainProgram::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}