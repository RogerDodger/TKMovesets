#pragma once

#include "GameImport.hpp"

class Submenu_Import
{
private:
	// Stores the last error code
	ImportationErrcode_ m_err;

public:
	Submenu_Import();
	void Render(GameImport& importerHelper);
};