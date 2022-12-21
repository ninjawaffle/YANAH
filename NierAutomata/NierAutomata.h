#pragma once

#include "offsets.h"

#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <fstream>

class NierAutomata
{
public:
	// Bases
	uintptr_t module;
	uintptr_t pl0000;

	// Offset struct
	Offsets offsets;

	// Values
	uint32_t funds;
	uint32_t exp;

	uint32_t health;
	uint32_t max_health;

	NierAutomata();

	void CreateConsole();
	void Initialise();
	void Update();

	void WriteMemory(BYTE* dst, BYTE* src, unsigned int size);
	template <typename T> void WriteMemory(uintptr_t address, T value);
	void DumpItems();
	void GiveAllItems();

	void SetHealth(uint32_t value);
	bool AddItem(uint32_t itemID, uint32_t quantity);

	void ChangePlayer();
};
