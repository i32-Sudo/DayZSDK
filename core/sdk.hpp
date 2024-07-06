
#include "../Driver/API/Driver.h"
#include "Value.h"
#include <string>
#include "offsets.h"
#include "../utils/vector.h"
using namespace std;

namespace Game
{

	// void SetTerrainGrid(float value) {
	// 	DriverAPI.Write<float>(globals.World + 0xB80, value);
	// }


	template <typename T = std::uint64_t>
	bool valid_ptr(T ptr) {
		return (ptr && ptr > (T)0xFFFFFF && ptr < (T)0x7FFFFFFFFFFF);
	}


	std::string GetEntityTypeName(uint64_t Entity)
	{
		return read_arma_string(Driver->RPM<uint64_t>(Driver->RPM<uint64_t>(Entity + CLASS_OFF_ENTITY::OFF_ENTITY_TYPE) + 0xA8)).c_str();
	}

	uint64_t GetNetworkId(uint64_t Entity) {
		return Driver->RPM<uint64_t>(Entity + CLASS_OFF_PLAYER_IDENTITY::OFF_NETWORK_ID);
	}


	// string GetQuality(uint64_t Entity) {
	// 
	// 	auto quality = Driver->RPM<uint64_t>(Entity + off_itemquality);
	// 
	// 	if (quality == 1) return "Quality (Worn)";
	// 	if (quality == 2) return "Quality(Damaged)";
	// 	if (quality == 3) return "Quality (Badly damaged)";
	// 	if (quality == 4) return "Quality (Ruined)";
	// 
	// 	else return "Quality (Pristine)";
	// }

	std::string GetEntityName(uint64_t Entity)
	{
		return read_arma_string(Driver->RPM<uint64_t>(Driver->RPM<uint64_t>(Entity + CLASS_OFF_ENTITY::OFF_ENTITY_TYPE) + 0x70)).c_str();
	}

	uint64_t GetInventory(uint64_t Entity) {
		return Driver->RPM<uint64_t>(Entity + CLASS_OFF_ENTITY::OFF_INVENTORY);
	}






	string GetItemInHands(uint64_t Entity) {
		return read_arma_string(Driver->RPM<uint64_t>(
			Driver->RPM<uint64_t>(Driver->RPM<uint64_t>(Game::GetInventory(Entity) + 0x1B0) + CLASS_OFF_ENTITY::OFF_ENTITY_TYPE) + 0x68)).c_str();
	}

	std::string GetEntityItemName(uint64_t Entity)
	{
		return read_arma_string(Driver->RPM<uint64_t>(Driver->RPM<uint64_t>(Entity + CLASS_OFF_ENTITY::OFF_ENTITY_TYPE) + 0x4E0)).c_str();
	}


	uint64_t NearEntityTable()
	{
		

		return Driver->RPM<uint64_t>(globals.World + CLASS_OFF_TABLES::OFF_NEAR_ENTITY_TABLE);
	}

	uint32_t NearEntityTableSize()
	{
		return Driver->RPM<uint32_t>(globals.World + CLASS_OFF_TABLES::OFF_NEAR_ENTITY_TABLE + 0x08);
	}

	uint64_t GetEntity(uint64_t PlayerList, uint64_t SelectedPlayer)
	{
		return Driver->RPM<uint64_t>(PlayerList + SelectedPlayer * 0x8);
	}

	uint64_t FarEntityTable()
	{
		

		return Driver->RPM<uint64_t>(globals.World + CLASS_OFF_TABLES::OFF_FAR_ENTITY_TABLE);
	}

	uint32_t FarEntityTableSize()
	{
		return Driver->RPM<uint32_t>(globals.World + CLASS_OFF_TABLES::OFF_FAR_ENTITY_TABLE + 0x08);
	}

	// uint64_t GetCameraOn()
	// {
	// 	return Driver->RPM<uint64_t>(globals.World + 0x28D0);
	// }

	uint64_t GetLocalPlayer()
	{
		

		return Driver->RPM<uint64_t>(Driver->RPM<uint64_t>(globals.World + CLASS_OFF_BASE::OFF_LOCAL_PLAYER) + 0x8) - 0xA8;
	}

	uint64_t GetLocalPlayerVisualState()
	{
		

		return Driver->RPM<uint64_t>(GetLocalPlayer() + CLASS_OFF_ENTITY::OFF_VISUAL_STATE);
	}

	uint64_t GetPlayerVisualState()
	{
		

		return Driver->RPM<uint64_t>(GetLocalPlayer() + CLASS_OFF_ENTITY::OFF_VISUAL_STATE);
	}


	Vector3 GetPlayerVisualState(uintptr_t Entity)
	{
		
		if (Entity)
		{

			uintptr_t renderVisualState = Driver->RPM<uintptr_t>(Entity + CLASS_OFF_ENTITY::OFF_VISUAL_STATE);

			if (renderVisualState)
			{
				Vector3 pos = Driver->RPM<Vector3>(renderVisualState + 0x2C);
				return pos;
			}
		}

		return Vector3(-1, -1, -1);
	}

	Vector3 GetCoordinate(uint64_t Entity)
	{
		while (true)
		{
			

			if (Entity == GetLocalPlayer())
			{
				return Vector3(Driver->RPM<Vector3>(GetLocalPlayerVisualState() + 0x2C));
			}
			else
			{
				return  Vector3(Driver->RPM<Vector3>(Driver->RPM<uint64_t>(Entity + CLASS_OFF_ENTITY::OFF_VISUAL_STATE) + 0x2C));
			}
		}
	}


	uint64_t GetCamera()
	{
		while (true)
		{
			
			return Driver->RPM<uint64_t>(globals.World + CLASS_OFF_BASE::OFF_CAMERA);
		}
	}

	Vector3 GetInvertedViewTranslation()
	{
		return Vector3(Driver->RPM<Vector3>(GetCamera() + CLASS_OFF_CAMERA::OFF_InvertViewTranslation));
	}

	Vector3 GetInvertedViewRight()
	{
		return Vector3(Driver->RPM<Vector3>(GetCamera() + CLASS_OFF_CAMERA::OFF_InvertedViewRight));
	}

	Vector3 GetInvertedViewUp()
	{
		return Vector3(Driver->RPM<Vector3>(GetCamera() + CLASS_OFF_CAMERA::OFF_InvertedViewUp));
	}

	Vector3 GetInvertedViewForward()
	{
		return Vector3(Driver->RPM<Vector3>(GetCamera() + CLASS_OFF_CAMERA::OFF_InvertedViewForward));
	}

	Vector3 GetViewportSize()
	{
		return Vector3(Driver->RPM<Vector3>(GetCamera() + CLASS_OFF_CAMERA::OFF_ViewPortSize));
	}

	Vector3 GetProjectionD1()
	{
		return Vector3(Driver->RPM<Vector3>(GetCamera() + CLASS_OFF_CAMERA::OFF_Projection1));
	}

	Vector3 GetProjectionD2()
	{
		return Vector3(Driver->RPM<Vector3>(GetCamera() + CLASS_OFF_CAMERA::OFF_Projection2));
	}

	//uint32_t GetSlowEntityTableSize()
	//{
	//	return Driver->RPM<uint32_t>(globals.World + oSlowEntityTable + 0x08);
	//}

	float GetDistanceToMe(Vector3 Entity)
	{
		

		return Entity.Distance(GetCoordinate(GetLocalPlayer()));
	}

	bool WorldToScreen(Vector3 Position, Vector3& output)
	{
		

		if (!GetCamera()) return false;

		Vector3 temp = Position - GetInvertedViewTranslation();

		float x = temp.Dot(GetInvertedViewRight());
		float y = temp.Dot(GetInvertedViewUp());
		float z = temp.Dot(GetInvertedViewForward());

		if (z < 0.1f)
			return false;

		Vector3 res(
			GetViewportSize().x * (1 + (x / GetProjectionD1().x / z)),
			GetViewportSize().y * (1 - (y / GetProjectionD2().y / z)),
			z);

		output.x = res.x;
		output.y = res.y;
		output.z = res.z;
		return true;
	}
	Vector3 GetHeadPosition()
	{
		

		return Driver->RPM<Vector3>((const DWORD64)+0xF8);

	}

	bool SetPosition(uint64_t Entity, Vector3 TargetPosition)
	{
		

		if (Entity == Game::GetLocalPlayer()) {
			Driver->WPM<Vector3>(
				Driver->RPM<uint64_t>(
					Entity + 0xF0) + 0x2C, TargetPosition);
		}
		else {
			Driver->WPM<Vector3>(
				Driver->RPM<uint64_t>(
					Entity + CLASS_OFF_ENTITY::OFF_VISUAL_STATE) + 0x2C, TargetPosition);
		}
		return true;
	}

	uint64_t BulletTable() {
		

		return Driver->RPM<uint64_t>(globals.World + CLASS_OFF_TABLES::OFF_BULLET_TABLE);
	}

	uint64_t BulletTableSize() {
		

		return sizeof(BulletTable());
	}

	bool SilentAim(uint64_t Entity)
	{
		for (uint64_t playerId = NULL; playerId < BulletTableSize(); ++playerId) {
			Vector3 WorldPosition = Game::GetCoordinate(Entity);

			int distance = Game::GetDistanceToMe(WorldPosition);
			if (distance != NULL && playerId != NULL)
				SetPosition(Game::GetEntity(BulletTable(), playerId), Vector3(WorldPosition.x, WorldPosition.y + 1.f, WorldPosition.z));
		}
		return true;
	}

}