#pragma once
#include <windows.h>
#include <iostream>

namespace CLASS_OFF_BASE {
	constexpr uint64_t OFF_WORLD = 0x4169C58;

	constexpr uint64_t OFF_LOCAL_PLAYER = 0x2960;
	constexpr uint64_t OFF_CAMERA = 0x1B8;

}

namespace CLASS_OFF_NETWORK {
	constexpr uint64_t OFF_NETWORK_INSTANCE = 0xF15E20;
	constexpr uint64_t OFF_NETWORK_PLAYER_ID = 0x70C;
}

namespace CLASS_OFF_NETWORK_CLIENT {
	constexpr uint64_t OFF_SCOREBOARD = 0x18;
	constexpr uint64_t OFF_IDENTITY_COUNT = 0x1C;
	constexpr uint64_t OFF_MAPNAME = 0x38;
}

namespace CLASS_OFF_NETWORK_SCOREBOARD {
	constexpr uint64_t OFF_PLAYER_IDENTITIES = 0x0;
}

namespace CLASS_OFF_PLAYER_IDENTITY {
	constexpr uint64_t OFF_NETWORK_ID = 0x30;
	constexpr uint64_t OFF_PLAYER_NAME = 0xF0;
}

namespace CLASS_OFF_TABLES {
	constexpr uint64_t OFF_ITEM_TABLE = 0x2060;
	constexpr uint64_t OFF_BULLET_TABLE = 0xE00;
	constexpr uint64_t OFF_NEAR_ENTITY_TABLE = 0x0F48;
	constexpr uint64_t OFF_FAR_ENTITY_TABLE = 0x1090;
}

namespace CLASS_OFF_ENTITY {
	constexpr uint64_t OFF_ITEM = 0x8;
	constexpr uint64_t OFF_VISUAL_STATE = 0x1D0;
	constexpr uint64_t OFF_INVENTORY = 0x680;
	constexpr uint64_t OFF_NETWORK_ID = 0x70C;
	constexpr uint64_t OFF_ENTITY_TYPE = 0x180;
	constexpr uint64_t OFF_IS_DEAD = 0xE2;

	constexpr uint64_t OFF_PLAYER_SKELETON = 0x830;
	constexpr uint64_t OFF_ZOMBIE_SKELETON = 0x6A0;
}

namespace CLASS_OFF_SKELETON {
	constexpr uint64_t OFF_ANIMCLASS_1 = 0x70;
	constexpr uint64_t OFF_ANIMCLASS_2 = 0x28;
}

namespace CLASS_OFF_CAMERA {
	constexpr uint64_t OFF_Projection1 = 0xD0;
	constexpr uint64_t OFF_Projection2 = 0xDC;
	constexpr uint64_t OFF_ViewPortSize = 0x58;
	constexpr uint64_t OFF_InvertViewTranslation = 0x2C;
	constexpr uint64_t OFF_InvertedViewRight = 0x8;
	constexpr uint64_t OFF_InvertedViewUp = 0x14;
	constexpr uint64_t OFF_InvertedViewForward = 0x20;

}

namespace CLASS_OFF_ANIM {
	constexpr uint64_t OFF_MATRIXES = 0xBF0;
}

namespace CLASS_OFF_ENTITYTYPE {
	constexpr uint64_t OFF_CONFIG_NAME = 0xA8;
	constexpr uint64_t OFF_CLEAN_NAME = 0x4E8;
	constexpr uint64_t OFF_MODEL_NAME = 0x80;
}

namespace CLASS_OFF_VISUALSTATE {
	constexpr uint64_t OFF_MATRIX = 0x8;
	constexpr uint64_t OFF_POSITION = 0x2C;
}

std::string read_string(uint64_t address, int size)
{
	std::string return_str;
	char buffer[128];
	return_str.resize(size);
	Driver->ReadProcessMemory((PVOID)address, buffer, 128);
	return_str = std::string(buffer);
	return return_str;
}

std::string read_arma_string(uint64_t address)
{
	

	int len = Driver->RPM<int>(address + 0x8);
	std::string str = read_string(address + 0x10, len);
	return str;
}

// std::string ReadArmaString(uint64_t address) {
// 	int length = ReadData<int>(address + OFF_LENGTH);
// 	std::string text = ReadString(address + OFF_TEXT, length);
// 	return text.c_str();
// }

/*
[UPDATER] AmmoType::AirFriction                -> 0x00000000000003B4
[UPDATER] AmmoType::InitSpeed                  -> 0x0000000000000364
[UPDATER] AnimClass::MatrixArray               -> 0x0000000000000BF0
[UPDATER] Camera::ViewMatrix                   -> 0x0000000000000008
[UPDATER] Camera::ViewPortMatrix               -> 0x0000000000000058
[UPDATER] Camera::ViewProjection               -> 0x00000000000000D0
[UPDATER] DayZInfected::Skeleton               -> 0x00000000000006A0
[UPDATER] DayZPlayer::Inventory                -> 0x0000000000000680
[UPDATER] DayZPlayer::NetworkID                -> 0x000000000000070C
[UPDATER] DayZPlayer::Skeleton                 -> 0x0000000000000830
[UPDATER] DayZPlayerInventory::Hands           -> 0x00000000000001B0
[UPDATER] Human::HumanType                     -> 0x0000000000000180
[UPDATER] Human::LodShape                      -> 0x0000000000000208
[UPDATER] Human::VisualState                   -> 0x00000000000001D0
[UPDATER] HumanType::CategoryName              -> 0x00000000000000A8
[UPDATER] HumanType::ObjectName                -> 0x0000000000000070
[UPDATER] InventoryItem::ItemInventory         -> 0x0000000000000680
[UPDATER] Magazine::AmmoCount                  -> 0x00000000000006DC
[UPDATER] Magazine::MagazineType               -> 0x0000000000000180
[UPDATER] Modbase::Network                     -> 0x0000000000F15E20
[UPDATER] Modbase::Tick                        -> 0x0000000000EFC230
[UPDATER] Modbase::World                       -> 0x0000000004169C58
[UPDATER] Skeleton::AnimClass1                 -> 0x0000000000000070
[UPDATER] Skeleton::AnimClass2                 -> 0x0000000000000028
[UPDATER] VisualState::InverseTransform        -> 0x00000000000000A4
[UPDATER] VisualState::Transform               -> 0x0000000000000008
[UPDATER] Weapon::MuzzleCount                  -> 0x00000000000006E4
[UPDATER] Weapon::WeaponIndex                  -> 0x00000000000006D0
[UPDATER] Weapon::WeaponInfoSize               -> 0x00000000000000E0
[UPDATER] Weapon::WeaponInfoTable              -> 0x00000000000006D8
[UPDATER] WeaponInventory::MagazineRef         -> 0x0000000000000150
[UPDATER] World::BulletList                    -> 0x0000000000000E00
[UPDATER] World::Camera                        -> 0x00000000000001B8
[UPDATER] World::FarEntList                    -> 0x0000000000001090
[UPDATER] World::LocalOffset                   -> 0xFFFFFFFFFFFFFF58  NEGATIVE VALUE BE AWARE 
[UPDATER] World::LocalPlayer                   -> 0x0000000000002960
[UPDATER] World::NearEntList                   -> 0x0000000000000F48
*/