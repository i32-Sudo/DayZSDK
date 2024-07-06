#include "../Driver/API/Driver.h"

#include "../Lib/json.hpp"
#include <D3DX11tex.h>

#include <cmath>
#include <psapi.h>
#include <tchar.h>
#include <tchar.h>
#include <thread>

#include "../core/offsets.h"
#include "../core/sdk.hpp"

#include "../Utils/pimraryXor.h"
#include "drawUtils.hpp"

using json = nlohmann::json;
bool StyleLoaded = false;
bool ShowMenu = true;
bool Cpopulate = false;
int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);

namespace config {

    namespace Aimbot {
        bool MasterSwitch = false;
        bool SilentAim = false;

        bool HitZombies = false;
        bool HitPlayers = false;
        bool HitAnimals = false;

        bool AimbotKey = false;
        static int selectedKey = 0;
        const char* keys[] = { "MB1", "MB2", "X", "Z", "C", "V", "Left Control", "Left Alt", "Left Shift", "Caps Lock" };
        int virtualKeyCodes[] = { VK_LBUTTON, VK_RBUTTON, 'X', 'Z', 'C', 'V', VK_LCONTROL, VK_LMENU, VK_LSHIFT, VK_CAPITAL };

        float Fov = 30.f;
        bool ShowFov = false;
        float FovCircleColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

        float MaxDistance = 100.f;

    }

    namespace Visuals {

        bool Global = false;

        namespace Items {
            bool MasterSwitch = false;

            bool ShowWeapons = false;
            bool ShowClothing = false;
            bool ShowMisc = false;
            float MaxDistance = 200.f;

            float WeaponColor[4] = { 1.0f, 0.9f, 0.9f, 1.0f };
            float ClothingColor[4] = { 0.7f, 0.8f, 0.9f, 1.0f };
            float MiscColor[4] = { 1.0f, 1.0f, 0.8f, 1.0f };

        }

        namespace Player {
            bool MasterSwitch = false;

            bool ShowName = false;
            float MaxDistance = 200.f;
            float NameColor[4] = { 0.5f, 0.0f, 0.5f, 1.0f };

            bool ShowItemHolding = false;
            float ItemColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        }

        namespace Zombie {
            bool MasterSwitch = false;

            bool ShowName = false;
            float MaxDistance = 200.f;
            float NameColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
        }

        namespace Animal {
            bool MasterSwitch = false;

            bool ShowName = false;
            float MaxDistance = 200.f;
            float NameColor[4] = { 1.0f, 0.5f, 1.0f, 1.0f };
        }

    }
}

void LoadConfig()
{
    std::string roaming = std::getenv(xorstr_("TMP"));
    std::string filename = roaming + xorstr_("\\ckf.json");

    std::ifstream file(filename);
    if (!file.is_open())
    {
        return;
    }

    json j;
    file >> j;

    // Aimbot settings
    config::Aimbot::MasterSwitch = j[xorstr_("Aimbot")][xorstr_("MasterSwitch")];
    config::Aimbot::SilentAim = j[xorstr_("Aimbot")][xorstr_("SilentAim")];
    config::Aimbot::HitZombies = j[xorstr_("Aimbot")][xorstr_("HitZombies")];
    config::Aimbot::HitPlayers = j[xorstr_("Aimbot")][xorstr_("HitPlayers")];
    config::Aimbot::HitAnimals = j[xorstr_("Aimbot")][xorstr_("HitAnimals")];
    config::Aimbot::AimbotKey = j[xorstr_("Aimbot")][xorstr_("AimbotKey")];
    config::Aimbot::selectedKey = j[xorstr_("Aimbot")][xorstr_("selectedKey")];
    config::Aimbot::Fov = j[xorstr_("Aimbot")][xorstr_("Fov")];
    config::Aimbot::ShowFov = j[xorstr_("Aimbot")][xorstr_("ShowFov")];
    for (int i = 0; i < 4; ++i)
        config::Aimbot::FovCircleColor[i] = j[xorstr_("Aimbot")][xorstr_("FovCircleColor")][i];
    config::Aimbot::MaxDistance = j[xorstr_("Aimbot")][xorstr_("MaxDistance")];

    config::Visuals::Global = j[xorstr_("Visuals")][xorstr_("MasterSwitch")];

    // Visuals settings - Items
    config::Visuals::Items::MasterSwitch = j[xorstr_("Visuals")][xorstr_("Items")][xorstr_("MasterSwitch")];
    config::Visuals::Items::ShowWeapons = j[xorstr_("Visuals")][xorstr_("Items")][xorstr_("ShowWeapons")];
    config::Visuals::Items::ShowClothing = j[xorstr_("Visuals")][xorstr_("Items")][xorstr_("ShowClothing")];
    config::Visuals::Items::ShowMisc = j[xorstr_("Visuals")][xorstr_("Items")][xorstr_("ShowMisc")];
    config::Visuals::Items::MaxDistance = j[xorstr_("Visuals")][xorstr_("Items")][xorstr_("MaxDistance")];
    for (int i = 0; i < 4; ++i)
        config::Visuals::Items::WeaponColor[i] = j[xorstr_("Visuals")][xorstr_("Items")][xorstr_("WeaponColor")][i];
    for (int i = 0; i < 4; ++i)
        config::Visuals::Items::ClothingColor[i] = j[xorstr_("Visuals")][xorstr_("Items")][xorstr_("ClothingColor")][i];
    for (int i = 0; i < 4; ++i)
        config::Visuals::Items::MiscColor[i] = j[xorstr_("Visuals")][xorstr_("Items")][xorstr_("MiscColor")][i];

    // Visuals settings - Player
    config::Visuals::Player::MasterSwitch = j[xorstr_("Visuals")][xorstr_("Player")][xorstr_("MasterSwitch")];
    config::Visuals::Player::ShowName = j[xorstr_("Visuals")][xorstr_("Player")][xorstr_("ShowName")];
    config::Visuals::Player::MaxDistance = j[xorstr_("Visuals")][xorstr_("Player")][xorstr_("MaxDistance")];
    for (int i = 0; i < 4; ++i)
        config::Visuals::Player::NameColor[i] = j[xorstr_("Visuals")][xorstr_("Player")][xorstr_("NameColor")][i];

    // Visuals settings - Zombie
    config::Visuals::Zombie::MasterSwitch = j[xorstr_("Visuals")][xorstr_("Zombie")][xorstr_("MasterSwitch")];
    config::Visuals::Zombie::ShowName = j[xorstr_("Visuals")][xorstr_("Zombie")][xorstr_("ShowName")];
    config::Visuals::Zombie::MaxDistance = j[xorstr_("Visuals")][xorstr_("Zombie")][xorstr_("MaxDistance")];
    for (int i = 0; i < 4; ++i)
        config::Visuals::Zombie::NameColor[i] = j[xorstr_("Visuals")][xorstr_("Zombie")][xorstr_("NameColor")][i];

    // Visuals settings - Animal
    config::Visuals::Animal::MasterSwitch = j[xorstr_("Visuals")][xorstr_("Animal")][xorstr_("MasterSwitch")];
    config::Visuals::Animal::ShowName = j[xorstr_("Visuals")][xorstr_("Animal")][xorstr_("ShowName")];
    config::Visuals::Animal::MaxDistance = j[xorstr_("Visuals")][xorstr_("Animal")][xorstr_("MaxDistance")];
    for (int i = 0; i < 4; ++i)
        config::Visuals::Animal::NameColor[i] = j[xorstr_("Visuals")][xorstr_("Animal")][xorstr_("NameColor")][i];

    file.close();
}

void SaveConfig()
{
    json j;

    std::string roaming = std::getenv(xorstr_("TMP"));
    std::string filename = roaming + xorstr_("\\ckf.json");

    // Aimbot settings
    j[xorstr_("Aimbot")][xorstr_("MasterSwitch")] = config::Aimbot::MasterSwitch;
    j[xorstr_("Aimbot")][xorstr_("SilentAim")] = config::Aimbot::SilentAim;
    j[xorstr_("Aimbot")][xorstr_("HitZombies")] = config::Aimbot::HitZombies;
    j[xorstr_("Aimbot")][xorstr_("HitPlayers")] = config::Aimbot::HitPlayers;
    j[xorstr_("Aimbot")][xorstr_("HitAnimals")] = config::Aimbot::HitAnimals;
    j[xorstr_("Aimbot")][xorstr_("AimbotKey")] = config::Aimbot::AimbotKey;
    j[xorstr_("Aimbot")][xorstr_("selectedKey")] = config::Aimbot::selectedKey;
    j[xorstr_("Aimbot")][xorstr_("Fov")] = config::Aimbot::Fov;
    j[xorstr_("Aimbot")][xorstr_("ShowFov")] = config::Aimbot::ShowFov;
    j[xorstr_("Aimbot")][xorstr_("FovCircleColor")] = config::Aimbot::FovCircleColor;
    j[xorstr_("Aimbot")][xorstr_("MaxDistance")] = config::Aimbot::MaxDistance;

    // Visual Settings
    j[xorstr_("Visuals")][xorstr_("MasterSwitch")] = config::Visuals::Global;

    // Visuals settings - Items
    j[xorstr_("Visuals")][xorstr_("Items")][xorstr_("MasterSwitch")] = config::Visuals::Items::MasterSwitch;
    j[xorstr_("Visuals")][xorstr_("Items")][xorstr_("ShowWeapons")] = config::Visuals::Items::ShowWeapons;
    j[xorstr_("Visuals")][xorstr_("Items")][xorstr_("ShowClothing")] = config::Visuals::Items::ShowClothing;
    j[xorstr_("Visuals")][xorstr_("Items")][xorstr_("ShowMisc")] = config::Visuals::Items::ShowMisc;
    j[xorstr_("Visuals")][xorstr_("Items")][xorstr_("MaxDistance")] = config::Visuals::Items::MaxDistance;
    j[xorstr_("Visuals")][xorstr_("Items")][xorstr_("WeaponColor")] = config::Visuals::Items::WeaponColor;
    j[xorstr_("Visuals")][xorstr_("Items")][xorstr_("ClothingColor")] = config::Visuals::Items::ClothingColor;
    j[xorstr_("Visuals")][xorstr_("Items")][xorstr_("MiscColor")] = config::Visuals::Items::MiscColor;

    // Visuals settings - Player
    j[xorstr_("Visuals")][xorstr_("Player")][xorstr_("MasterSwitch")] = config::Visuals::Player::MasterSwitch;
    j[xorstr_("Visuals")][xorstr_("Player")][xorstr_("ShowName")] = config::Visuals::Player::ShowName;
    j[xorstr_("Visuals")][xorstr_("Player")][xorstr_("MaxDistance")] = config::Visuals::Player::MaxDistance;
    j[xorstr_("Visuals")][xorstr_("Player")][xorstr_("NameColor")] = config::Visuals::Player::NameColor;

    // Visuals settings - Zombie
    j[xorstr_("Visuals")][xorstr_("Zombie")][xorstr_("MasterSwitch")] = config::Visuals::Zombie::MasterSwitch;
    j[xorstr_("Visuals")][xorstr_("Zombie")][xorstr_("ShowName")] = config::Visuals::Zombie::ShowName;
    j[xorstr_("Visuals")][xorstr_("Zombie")][xorstr_("MaxDistance")] = config::Visuals::Zombie::MaxDistance;
    j[xorstr_("Visuals")][xorstr_("Zombie")][xorstr_("NameColor")] = config::Visuals::Zombie::NameColor;
    j[xorstr_("Visuals")][xorstr_("Animal")][xorstr_("MasterSwitch")] = config::Visuals::Animal::MasterSwitch;
    j[xorstr_("Visuals")][xorstr_("Animal")][xorstr_("ShowName")] = config::Visuals::Animal::ShowName;
    j[xorstr_("Visuals")][xorstr_("Animal")][xorstr_("MaxDistance")] = config::Visuals::Animal::MaxDistance;
    j[xorstr_("Visuals")][xorstr_("Animal")][xorstr_("NameColor")] = config::Visuals::Animal::NameColor;

    // Write JSON to file
    std::ofstream file(filename);
    if (!file.is_open())
    {
        return;
    }

    file << std::setw(4) << j << std::endl;
    file.close();
}











bool WorldToScreen(Vector3 vWorld, Vector3& vScreen,
    Vector3 Translation,
    Vector3 Right,
    Vector3 Up,
    Vector3 Forward,
    Vector3 ViewPort,
    Vector3 Projection1,
    Vector3 Projection2)
{


    Vector3 vTemp = vWorld - Translation;

    vTemp = {
        vTemp.Dot(Right),
        vTemp.Dot(Up),
        vTemp.Dot(Forward)
    };

    if (vTemp.z <= 0.19f)
        return false;

    vScreen = {
        ViewPort.x * (1.0F + vTemp.x / Projection1.x / vTemp.z),
        ViewPort.y * (1.0F - vTemp.y / Projection2.y / vTemp.z),
        vTemp.z
    };

    return true;
}

uintptr_t base_address{};
uint64_t TargetPlayer;
std::vector<player_t> entities = {};
std::vector<item_t> items = {};
_globals globals;
uint64_t TargetEntity;

bool UpdateItemsThreadRunning = false;
bool UpdateEntitiesThreadRunning = false;

void UpdateItems()
{

    UpdateItemsThreadRunning = true;
    std::vector<item_t> tmp{};
    tmp.clear();
    if (config::Visuals::Items::MasterSwitch)
    {
        uint64_t ItemTableSize = Driver->RPM<uint32_t>(globals.World + CLASS_OFF_TABLES::OFF_ITEM_TABLE + 0x08);
        for (int i = 0; i < ItemTableSize; i++)
        {

            uint64_t ItemTable = Driver->RPM<uint64_t>(globals.World + CLASS_OFF_TABLES::OFF_ITEM_TABLE);
            if (!ItemTable) continue;

            int Check = Driver->RPM<int>(ItemTable + (i * 0x18));
            if (Check != 1) continue;

            uint64_t ItemEntity = Driver->RPM<uint64_t>(ItemTable + ((i * 0x18) + 0x8));

            item_t Item{};
            Item.ItemTable = ItemTable;
            Item.ItemPtr = ItemEntity;
            tmp.push_back(Item);
        }
        items.clear();
        items = tmp;
    }
}

void UpdateList()
{
    UpdateEntitiesThreadRunning = true;
    std::vector<player_t> tmp{};
    tmp.clear();


    INT32 NearTableSize = Driver->RPM<INT32>(globals.World + CLASS_OFF_TABLES::OFF_NEAR_ENTITY_TABLE + 0x08);
    INT32 FarTableSize = Driver->RPM<INT32>(globals.World + CLASS_OFF_TABLES::OFF_FAR_ENTITY_TABLE + 0x08);

    for (int i = 0; i < NearTableSize; i++)
    {

        uint64_t EntityTable = Driver->RPM<uint64_t>(globals.World + CLASS_OFF_TABLES::OFF_NEAR_ENTITY_TABLE);
        if (!EntityTable) continue;

        uint64_t Entity = Driver->RPM<uint64_t>(EntityTable + (i * 0x8));
        if (!Entity) continue;

        uintptr_t networkId = Game::GetNetworkId(Entity);
        if (networkId == 0) continue;

        player_t Player{};
        Player.NetworkID = networkId;
        Player.TableEntry = EntityTable;
        Player.EntityPtr = Entity;

        if (Player.NetworkID == Game::GetNetworkId(Game::GetLocalPlayer())) continue;
        tmp.push_back(Player);
    }

    for (int i = 0; i < FarTableSize; i++)
    {

        if (config::Visuals::Player::MaxDistance >= 100.f || config::Visuals::Zombie::MaxDistance >= 100.f) {
            uint64_t EntityTable = Driver->RPM<uint64_t>(globals.World + CLASS_OFF_TABLES::OFF_FAR_ENTITY_TABLE);
            if (!EntityTable) continue;

            uint64_t Entity = Driver->RPM<uint64_t>(EntityTable + (i * 0x8));
            if (!Entity) continue;

            uintptr_t networkId = Game::GetNetworkId(Entity);
            uintptr_t worldptr = Driver->RPM<uintptr_t>(base_address + globals.World);;
            if (networkId == 0) continue;
            player_t Player{};
            Player.NetworkID = networkId;

            Player.TableEntry = EntityTable;
            Player.EntityPtr = Entity;

            if (Player.NetworkID == Game::GetNetworkId(Game::GetLocalPlayer())) continue;
            tmp.push_back(Player);
        }
    }
    entities.clear();
    entities = tmp;

    if (TargetPlayer != NULL && config::Aimbot::SilentAim && config::Aimbot::MasterSwitch)
    {
        if (config::Aimbot::AimbotKey == false) {
            Game::SilentAim(TargetPlayer);
        }
        else if (config::Aimbot::AimbotKey == true) {
            if (GetAsyncKeyState(config::Aimbot::virtualKeyCodes[config::Aimbot::selectedKey])) {
                Game::SilentAim(TargetPlayer);
            }
        }
    }

}

void Items(ImDrawList* drawList)
{

    for (int i = 0; i < items.size(); i++)
    {
        auto ItemEntities = items[i];

        Vector3 worldPosition = Game::GetCoordinate(ItemEntities.ItemPtr);
        Vector3 screenPosition;

        Game::WorldToScreen(worldPosition, screenPosition);

        if (screenPosition.z < 1.0f) continue;
        int distance = Game::GetDistanceToMe(worldPosition);

        if (screenPosition.z >= 1.0f)
        {
            std::string TypeName = Game::GetEntityTypeName(ItemEntities.ItemPtr);
            std::string Name = Game::GetEntityName(ItemEntities.ItemPtr);


            if (TypeName == xorstr_("Weapon") && distance <= config::Visuals::Items::MaxDistance && config::Visuals::Items::ShowWeapons && config::Visuals::Items::MasterSwitch)
            {
                std::string pDrawInfo = (Name + " [" + std::to_string(distance) + xorstr_("m]"));
                overlay::DrawText2D(drawList, ImVec2(screenPosition.x, screenPosition.y), 12, Float4ToImColor(config::Visuals::Items::WeaponColor), pDrawInfo.c_str());
            }

            if (TypeName == xorstr_("clothing") && distance <= config::Visuals::Items::MaxDistance && config::Visuals::Items::ShowClothing && config::Visuals::Items::MasterSwitch)
            {
                std::string pDrawInfo = (Name + " [" + std::to_string(distance) + xorstr_("m]"));
                overlay::DrawText2D(drawList, ImVec2(screenPosition.x, screenPosition.y), 12, Float4ToImColor(config::Visuals::Items::ClothingColor), pDrawInfo.c_str());
            }

            if (TypeName == xorstr_("inventoryItem") && distance <= config::Visuals::Items::MaxDistance && config::Visuals::Items::ShowMisc && config::Visuals::Items::MasterSwitch)
            {
                std::string pDrawInfo = (Name + " [" + std::to_string(distance) + xorstr_("m]"));
                overlay::DrawText2D(drawList, ImVec2(screenPosition.x, screenPosition.y), 12, Float4ToImColor(config::Visuals::Items::MiscColor), pDrawInfo.c_str());
            }

        }
    }
}

void hackthread(ImDrawList* drawList) {
    uint64_t base = (uint64_t)Driver->FindBaseAddress();
    if (base == NULL) exit(0);

    globals.modBase = base;
    globals.World = Driver->RPM<uint64_t>(base + CLASS_OFF_BASE::OFF_WORLD);
    base_address = base;

    overlay::DrawText2D(drawList, ImVec2(0, 0), 16.0f, ImColor(255, 255, 255, 255), std::to_string(ImGui::GetIO().Framerate).c_str());

    radius = config::Aimbot::Fov;

    if (config::Aimbot::ShowFov && config::Aimbot::MasterSwitch)
        overlay::DrawCircle2D(drawList, ImVec2(globals.width / 2, globals.height / 2), radius, Float4ToImColor(config::Aimbot::FovCircleColor));

    if (config::Visuals::Global) {
        UpdateList();
        if (config::Visuals::Items::MasterSwitch) {
            UpdateItems();
            Items(drawList);
        }
    }

    for (int i = 0; i < entities.size(); i++)
    {


        auto Entities = entities[i];
        Vector3 player_world_position = Game::GetCoordinate(Entities.EntityPtr);
        Vector3 player_screen_position;
        if (Game::WorldToScreen(player_world_position, player_screen_position)) {
            int distance = Game::GetDistanceToMe(player_world_position);

            if (player_screen_position.z < 1.0f) continue;
            if (TargetPlayer != NULL)
            {
                Vector3 targetplayer_world_position = Game::GetCoordinate(TargetPlayer);
                Vector3 targetplayer_screen_position;
                if (Game::WorldToScreen(targetplayer_world_position, targetplayer_screen_position))
                {

                }
            }

            if (player_screen_position.z >= 1.0f)
            {
                std::string TypeName = Game::GetEntityTypeName(Entities.EntityPtr);
                std::string Name = Game::GetEntityName(Entities.EntityPtr);

                TargetPlayer == NULL;

                if (TypeName == xorstr_("dayzplayer") && distance <= config::Visuals::Player::MaxDistance && config::Visuals::Player::MasterSwitch)
                {

                    if (config::Visuals::Player::ShowName)
                    {
                        std::string pDrawInfo = (xorstr_("Player [") + std::to_string(distance) + xorstr_("m]"));
                        overlay::DrawText2D(drawList, ImVec2(player_screen_position.x - 10, player_screen_position.y), 12, Float4ToImColor(config::Visuals::Player::NameColor), pDrawInfo.c_str());
                    }

                    if (config::Visuals::Player::ShowItemHolding) {
                        std::string pDrawInfo = (Game::GetEntityItemName(Entities.EntityPtr));
                        float PreviousTextHeight = ImGui::GetTextLineHeight();
                        if (config::Visuals::Player::ShowName)
                            overlay::DrawText2D(drawList, ImVec2(player_screen_position.x - 10, player_screen_position.y + PreviousTextHeight + 2), 12, Float4ToImColor(config::Visuals::Player::ItemColor), pDrawInfo.c_str());
                        else
                            overlay::DrawText2D(drawList, ImVec2(player_screen_position.x - 10, player_screen_position.y), 12, Float4ToImColor(config::Visuals::Player::ItemColor), pDrawInfo.c_str());

                    }

                    float centerX = globals.width / 2.0f;
                    float centerY = globals.height / 2.0f;
                    if (config::Aimbot::HitPlayers && player_screen_position.x - centerX < radius && centerX - player_screen_position.x < radius && centerY - player_screen_position.y < radius && player_screen_position.y - centerY < radius)
                    {
                        if (Entities.EntityPtr != NULL && Entities.EntityPtr != Game::GetLocalPlayer())
                            TargetPlayer = Entities.EntityPtr;
                    }
                }


                if (TypeName == xorstr_("dayzinfected") && distance <= config::Visuals::Zombie::MaxDistance && config::Visuals::Zombie::MasterSwitch)
                {
                    if (config::Visuals::Zombie::ShowName)
                    {
                        std::string pDrawInfo = (xorstr_("Infected [") + std::to_string(distance) + xorstr_("m]"));
                        overlay::DrawText2D(drawList, ImVec2(player_screen_position.x - 10, player_screen_position.y), 12, Float4ToImColor(config::Visuals::Zombie::NameColor), pDrawInfo.c_str());
                    }

                    float centerX = globals.width / 2.0f;
                    float centerY = globals.height / 2.0f;
                    if (config::Aimbot::HitZombies && player_screen_position.x - centerX < radius && centerX - player_screen_position.x < radius && centerY - player_screen_position.y < radius && player_screen_position.y - centerY < radius)
                    {
                        if (Entities.EntityPtr != NULL)
                            TargetPlayer = Entities.EntityPtr;
                    }

                    /*	DrawFilledRect(player_screen_position.x, player_screen_position.y, 8, 8, &Col.glassblack);*/
                    /*	DrawPlayerBar(player_screen_position.x, player_screen_position.y + 25, &Col.glassblack, &Col.red, (xorstr_("Zombie [") + std::to_string(distance) + xorstr_("m]")).c_str());*/
                }

                if (TypeName == xorstr_("dayzanimal") && distance <= config::Visuals::Animal::MaxDistance && config::Visuals::Animal::MasterSwitch)
                {
                    if (config::Visuals::Animal::ShowName)
                    {
                        std::string pDrawInfo = (xorstr_("Animal [") + std::to_string(distance) + xorstr_("m]"));
                        overlay::DrawText2D(drawList, ImVec2(player_screen_position.x - 10, player_screen_position.y), 12, Float4ToImColor(config::Visuals::Animal::NameColor), pDrawInfo.c_str());
                    }

                    float centerX = globals.width / 2.0f;
                    float centerY = globals.height / 2.0f;
                    if (config::Aimbot::HitAnimals && player_screen_position.x - centerX < radius && centerX - player_screen_position.x < radius && centerY - player_screen_position.y < radius && player_screen_position.y - centerY < radius)
                    {
                        if (Entities.EntityPtr != NULL)
                            TargetPlayer = Entities.EntityPtr;
                    }

                    /*	DrawFilledRect(player_screen_position.x, player_screen_position.y, 8, 8, &Col.glassblack);*/
                    /*	DrawPlayerBar(player_screen_position.x, player_screen_position.y + 25, &Col.glassblack, &Col.red, (xorstr_("Zombie [") + std::to_string(distance) + xorstr_("m]")).c_str());*/
                }
            }
        }
    }
}



float currentAlpha = 0.0f;
const float targetAlpha = 140.0f;
const float fadeSpeed = 6.0f;
void DrawOverlay() {
    ImGui::Begin(xorstr_("renderer"), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs );
    ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetWindowSize(ImVec2(screenWidth, screenHeight));
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    /* Overlay */
    hackthread(drawList);

    if (currentAlpha < targetAlpha) {
        currentAlpha += fadeSpeed;
        if (currentAlpha > targetAlpha) {
            currentAlpha = targetAlpha;
        }
    }

    if (ShowMenu) {
        overlay::DrawFilledBox2D(drawList, ImVec2(0, 0), ImVec2(screenWidth, screenHeight), ImColor(255, 0, 0, 255), ImColor(0, 0, 0, static_cast<int>(currentAlpha)));
        overlay::DrawText2D(drawList, ImVec2(0, 30), 13, ImColor(255, 0, 0, 255), xorstr_("PRESS END KEY BEFORE CLOSING GAME TO UNLOAD! RISK OF BSOD!"));
    }
    else {
        currentAlpha = 0.0f;
    }

    ImGui::End();
}

ID3D11ShaderResourceView* Image = nullptr;

void RunStyle() {
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.0f, 0.0263f, 0.0357f, 1.00f);
    style->Colors[ImGuiCol_WindowBg] = ImColor(20, 20, 20);
    style->Colors[ImGuiCol_ChildBg] = ImColor(15, 15, 15);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.080f, 0.080f, 0.080f, 0.940f);
    style->Colors[ImGuiCol_Border] = ImColor(57, 57, 57);
    style->Colors[ImGuiCol_BorderShadow] = ImColor(1, 1, 1);
    style->Colors[ImGuiCol_FrameBg] = ImColor(40, 40, 40);
    style->Colors[ImGuiCol_FrameBgHovered] = ImColor(40, 40, 40);
    style->Colors[ImGuiCol_FrameBgActive] = ImColor(40, 40, 40);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(0.102f, 0.090f, 0.122f, 1.000f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.102f, 0.090f, 0.122f, 1.000f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.102f, 0.090f, 0.122f, 1.000f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.0f, 0.263f, 0.357f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    //style->Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    style->Colors[ImGuiCol_SliderGrab] = ImColor(255, 182, 193);
    style->Colors[ImGuiCol_SliderGrabActive] = ImColor(255, 182, 193);
    style->Colors[ImGuiCol_Button] = ImColor(20, 20, 20);
    style->Colors[ImGuiCol_ButtonHovered] = ImColor(20, 20, 20);
    style->Colors[ImGuiCol_ButtonActive] = ImColor(20, 20, 20);
    style->Colors[ImGuiCol_Header] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    //style->Colors[ImGuiCol_Column] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    //style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
//	style->Colors[ImGuiCol] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    //style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
    style->Colors[ImGuiCol_Separator] = ImColor(255,182,193);
    style->Colors[ImGuiCol_CheckMark] = ImColor(255, 182, 193);
    style->Colors[ImGuiCol_Tab] = ImVec4(0.29f, 0.09f, 0.40f, 0.86f);
    style->Colors[ImGuiCol_TabHovered] = ImVec4(0.21f, 0.09f, 0.27f, 0.80f);
    style->Colors[ImGuiCol_TabActive] = ImVec4(0.49f, 0.00f, 0.54f, 1.00f);
    style->TabRounding = 0.0f;
    style->WindowRounding = 5.0f;
    style->FrameRounding = 4.0f;

    style->AntiAliasedLines = true;
    style->AntiAliasedFill = true;

    StyleLoaded = !StyleLoaded;
}


int tabb = 1;
void DrawMenu() {
    /* Overlay Shit */
    if (!StyleLoaded)
        RunStyle();
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        ShowMenu = !ShowMenu;
    }

    HWND window = FindWindowW(NULL, xorstr_(L"U"));
    LONG_PTR exStyle = GetWindowLongPtr(window, GWL_EXSTYLE);
    if (ShowMenu)
    {
        ImGuiIO& io = ImGui::GetIO();

        exStyle &= ~WS_EX_TRANSPARENT;

        bool show_demo_window = false;
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        /* BG BEHIND */
        // Begin the ImGui window
        ImGuiStyle* style = &ImGui::GetStyle();

        /* Menu */
        ImGui::Begin(xorstr_("i64NtNvda"), nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
        style = &ImGui::GetStyle();
        style->Colors[ImGuiCol_WindowBg] = ImColor(20, 20, 20);
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
        // Removed Menu
        
        ImGui::End();
    }
    else {
        exStyle |= WS_EX_TRANSPARENT;
    }
    SetWindowLongPtr(window, GWL_EXSTYLE, exStyle);
    SetWindowPos(window, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    ImGui::End();
}


bool IsProcessForegroundWindow(DWORD processID) {
    HWND foregroundWindow = GetForegroundWindow();
    if (foregroundWindow == NULL) {
        return false;
    }

    DWORD foregroundProcessID;
    GetWindowThreadProcessId(foregroundWindow, &foregroundProcessID);

    return processID == foregroundProcessID;
}

void drawLoop() {
    if (GetAsyncKeyState(VK_END))
        exit(1);

    if (IsProcessForegroundWindow(Driver->ProcessID) || IsProcessForegroundWindow(_getpid())) {
        DrawOverlay();
        DrawMenu();
    }

}
