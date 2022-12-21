#include "NierAutomata.h"

// Initialise fields
NierAutomata::NierAutomata()
{
    module = 0;
    pl0000 = 0;

    offsets = {};

    funds = 0;
    exp = 0;

    health = 0;
    max_health = 0;
}

// Define functions
void NierAutomata::CreateConsole()
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONIN$", "r", stdin);
    freopen_s(&f, "CONOUT$", "w", stderr);
    freopen_s(&f, "CONOUT$", "w", stdout);
}

void NierAutomata::Initialise()
{
    // Resolve module base
    module = (uintptr_t)GetModuleHandle(L"NieRAutomata.exe");
}

void NierAutomata::Update()
{
    // Only update if module has been resolved
    if (!module) return;
    funds = *(uint32_t*)(module + offsets.funds);
    exp = *(uint32_t*)(module + offsets.exp);
    pl0000 = *(uintptr_t*)(module + offsets.pl0000);
    
    // Only update if pl0000 has been resolved
    if (!pl0000) return;
    health = *(uint32_t*)(pl0000 + offsets.health);
    max_health = *(uint32_t*)(pl0000 + offsets.max_health);
}

void NierAutomata::WriteMemory(BYTE* dst, BYTE* src, unsigned int size)
{
    DWORD oldProtection;
    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtection);
    memcpy(dst, src, size);
    VirtualProtect(dst, size, oldProtection, &oldProtection);
}

template <typename T> void NierAutomata::WriteMemory(uintptr_t address, T value)
{
    void* dst = (void*)address;

    DWORD oldProtection;
    VirtualProtect((LPVOID)address, sizeof(value), PAGE_EXECUTE_READWRITE, &oldProtection);
    memcpy(dst, &value, sizeof(value));
    VirtualProtect((LPVOID)address, sizeof(value), oldProtection, &oldProtection);
}

void NierAutomata::DumpItems()
{
    std::ofstream dump("item_dump.txt");
    std::cout << "[>] Starting item dump to \"item_dump.txt\"" << std::endl;

    struct item_info
    {
        uint32_t itemID;
        uint32_t unk04;
        uint32_t max_held;
        uint32_t unk12;
        uint8_t* name;
    };

    uint32_t* start_address = (uint32_t*)(module + 0x133B6E0);

    item_info item_array[266]{};

    for (int i = 0; i < 266; i++)
    {
        uint32_t* start = (start_address + (i * 104));
        item_info item{};

        item.itemID = *(uint32_t*)(start + 0);
        item.unk04 = *(uint32_t*)(start + 1);
        item.max_held = *(uint32_t*)(start + 2);
        item.unk12 = *(uint32_t*)(start + 3);
        item.name = (uint8_t*)(start + 4);

        item_array[i] = item;

        dump << "[" << i << "] " << item.name << std::endl;
        dump << "Item ID: " << item.itemID << std::endl;
        dump << "Max Held: " << item.max_held << std::endl;
        dump << "" << std::endl;

        std::cout << "[>] Dumped " << item.name << " (" << i << ")" << std::endl;
    }

    std::cout << "[>] Dump finished. Closing file" << std::endl;
    dump.close();
}

void NierAutomata::GiveAllItems()
{
    uint32_t* start_address = (uint32_t*)(module + 0x133B6E0);

    for (int i = 0; i < 266; i++)
    {
        uint32_t* start = (start_address + (i * 104));

        uint32_t itemID = *(uint32_t*)(start);
        uint32_t max_held = *(uint32_t*)(start + 2);

        AddItem(itemID, max_held);
    }

    std::cout << "Finished" << std::endl;
}

void NierAutomata::SetHealth(uint32_t value)
{
    //WriteMemory((BYTE*)(pl0000 + offsets.health), (BYTE*)&value, sizeof(value));
    WriteMemory(pl0000 + offsets.health, value);
}

bool NierAutomata::AddItem(uint32_t itemID, uint32_t quantity)
{
    // Define start and end points
    uint32_t* region1_end_address = (uint32_t*)(module + offsets.body_inventory_start);

    uint32_t* region2_start_address = (uint32_t*)(module + offsets.items_start2);
    uint32_t* region2_end_address = (uint32_t*)(module + offsets.weapons_start);

    uint32_t* current_address = (uint32_t*)(module + offsets.items_start);

    while (current_address != region1_end_address)
    {
        // Pull the memory value
        uint32_t item_id = *(uint32_t*)(current_address);

        if (item_id == itemID)
        {
            *(uint32_t*)(current_address + 2) = quantity;
            return true;
        }

        // If item slot is blank, and in valid memory region
        if (item_id == 0xFFFFFFFF)
        {
            // Add item into memory
            *(uint32_t*)(current_address + 0) = itemID;
            *(uint32_t*)(current_address + 1) = 0x70000;
            *(uint32_t*)(current_address + 2) = quantity;
            std::cout << "Gave: " << itemID << "to item slot " << current_address <<std::endl;
            return true;
        }

        current_address += 3;
    }

    // If region 1 is full, move onto next

    // Skip ahead to region 2
    current_address = region2_start_address;

    while (current_address != region2_end_address)
    {
        // Pull the memory value
        uint32_t item_id = *(uint32_t*)(current_address);

        if (item_id == itemID)
        {
            *(uint32_t*)(current_address + 2) = quantity;
            return true;
        }

        // If item slot is blank, and in valid memory region
        if (item_id == 0xFFFFFFFF)
        {
            // Add item into memory
            *(uint32_t*)(current_address + 0) = itemID;
            *(uint32_t*)(current_address + 1) = 0x70000;
            *(uint32_t*)(current_address + 2) = quantity;
            std::cout << "Gave: " << itemID << "to item slot " << current_address << std::endl;
            return true;
        }

        current_address += 3;
    }

    return false;
}

void NierAutomata::ChangePlayer()
{
    Update();

    typedef void(__fastcall* changePlayer)(__int64 player);
    changePlayer pl0000changePlayer = (changePlayer)(module + offsets.pl0000_changePlayer);

    pl0000changePlayer((__int64)pl0000);
}
