#pragma once

inline float radius = 60.0f;
struct _globals
{
	uint64_t modBase;
	uint64_t World;
	uint64_t RemoveFog;
	uint64_t Network;
	uint64_t Entitylist;
	uint64_t Entity;
	uint32_t Size;

	float width = GetSystemMetrics(SM_CXSCREEN);
	float height = GetSystemMetrics(SM_CYSCREEN);
};


typedef struct _player_t
{
	std::uint64_t EntityPtr;
	std::uint64_t TableEntry;
	int NetworkID;
} player_t;

typedef struct _item_t
{
	std::uint64_t ItemPtr;
	std::uint64_t ItemTable;
} item_t;

extern _globals globals;