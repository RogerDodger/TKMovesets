#include "GameAddressesWrapper.hpp"

// -- Game addresses interface -- //

int64_t GameAddressesWrapper::GetValue(const char* c_addressId)
{
	try {
		return addrFile->GetValue(minorGameKey, c_addressId);
	}
	catch (GameAddressNotFound&) {
		return addrFile->GetValue(gameKey, c_addressId);
	}
}

const char* GameAddressesWrapper::GetString(const char* c_addressId)
{
	try {
		return addrFile->GetString(minorGameKey, c_addressId);
	}
	catch (GameAddressNotFound&) {
		return addrFile->GetString(gameKey, c_addressId);
	}
}

const std::vector<gameAddr>& GameAddressesWrapper::GetPtrPath(const char* c_addressId, bool& isRelative)
{
	try {
		return addrFile->GetPtrPath(minorGameKey, c_addressId, isRelative);
	}
	catch (GameAddressNotFound&) {
		return addrFile->GetPtrPath(gameKey, c_addressId, isRelative);
	}
}

uint64_t GameAddressesWrapper::ReadPtrPathInCurrProcess(const char* c_addressId, uint64_t moduleAddress)
{
	bool isRelative;
	const std::vector<gameAddr>& ptrPath = GetPtrPath(c_addressId, isRelative);

	if (ptrPath.size() == 0) {
		return GAME_ADDR_NULL;
	}

	uint64_t addr = ptrPath[0];
	if (isRelative) {
		addr += moduleAddress;
	}

	{
		size_t pathLen = ptrPath.size() - 1;
		if (pathLen > 0)
		{
			addr = *(uint64_t*)addr;
			for (size_t i = 1; i < pathLen; ++i)
			{
				if (addr == 0) {
					return 0;
				}
				addr = *(uint64_t*)(addr + ptrPath[i]);
			}
			addr += ptrPath[pathLen];
		}
	}

	return addr;
}