#pragma once

#include <Windows.h>

struct Offsets
{
#pragma region Module Offsets
	// All offsets here are relative to NieRAutomata.exe
	uintptr_t pl0000 = 0x1020948;
	uintptr_t funds = 0x148C4C0;
	uintptr_t exp = 0x1494670;

	// Inventory
	uintptr_t items_start = 0x148C4C4;
	uintptr_t body_inventory_start = 0x148D0C4;
	uintptr_t items_start2 = 0x148D364;
	uintptr_t weapons_start = 0x148DCC4;
	uintptr_t pod_programs_start = 0x148E304;
	uintptr_t plugin_chips_start = 0x148E410;
	uintptr_t inbox_start = 0x1491C80;

	uintptr_t save_name = 0x1494820;

	// Functions
	uintptr_t pl0000_changePlayer = 0x476220;

	// Play Records
	uintptr_t enemies_defeated = 0x14944C4;

#pragma endregion

#pragma region Player Offsets

	// All offsets here are relative to pl0000
	uintptr_t x = 0x50;
	uintptr_t y = 0x54;
	uintptr_t z = 0x58;
	uintptr_t health = 0x10668;
	uintptr_t max_health = 0x85C;
	uintptr_t level = 0x14BC;

#pragma endregion
};
