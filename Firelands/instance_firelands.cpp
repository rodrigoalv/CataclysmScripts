/*
* Copyright (C) 2005 - 2011 MaNGOS <http://www.getmangos.org/>
*
* Copyright (C) 2008 - 2011 TrinityCore <http://www.trinitycore.org/>
*
* Copyright (C) 2011 TrilliumEMU <http://www.arkania.net/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

// by Naios

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "CreatureAI.h"
#include "firelands.h"

#define MAX_ENCOUNTER  7

class instance_firelands : public InstanceMapScript
{
public:
	instance_firelands() : InstanceMapScript("instance_firelands", 720) { }

	struct instance_firelands_InstanceMapScript : public InstanceScript
	{
		instance_firelands_InstanceMapScript(InstanceMap* map) : InstanceScript(map) { }

		uint32 Encounter[MAX_ENCOUNTER];

		// Creatures
		// Bosses
		uint64 uiBethilac;
		uint64 uiRhyolith;
		uint64 uiAlysrazar;
		uint64 uiShannox;
		uint64 uiBaloroc;
		uint64 uiMajordomus;
		uint64 uiRagnaros;

		// Npcs
		uint64 uiRiplimb;
		uint64 uiRageface;
		uint64 uiShannoxSpear;

		// Some Data
		uint32 TeamInInstance;
		uint32 encounterSharingPhase;

		// GameObjects

		// Miscellaneous

		void Initialize()
		{
			SetBossNumber(MAX_ENCOUNTER);

			uiBethilac                 = 0;
			uiRhyolith		           = 0;
			uiAlysrazar		           = 0;
			uiShannox				   = 0;
			uiBaloroc                  = 0;
			uiMajordomus               = 0;
			uiRagnaros                 = 0;

			uiRiplimb				   = 0;
			uiRageface				   = 0;
			uiShannoxSpear			   = 0;

			for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
				Encounter[i] = NOT_STARTED;
		}

		void OnPlayerEnter(Player* player)
		{
			if (!TeamInInstance)
				TeamInInstance = player->GetTeam();
		}

		bool IsEncounterInProgress() const
		{
			for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
			{
				if (Encounter[i] == IN_PROGRESS)
					return true;
			}

			return false;
		}

		void OnCreatureCreate(Creature* creature)
		{
			if (!TeamInInstance)
			{
				Map::PlayerList const &players = instance->GetPlayers();
				if (!players.isEmpty())
					if (Player* player = players.begin()->getSource())
						TeamInInstance = player->GetTeam();
			}

			switch (creature->GetEntry())
			{
				// Bosses
			case NPC_BETHTILAC:
				uiBethilac = creature->GetGUID();
				break;
			case NPC_RHYOLITH:
				uiRhyolith = creature->GetGUID();
				break;
			case NPC_ALYSRAZAR:
				uiAlysrazar = creature->GetGUID();
				break;
			case NPC_SHANNOX:
				uiShannox = creature->GetGUID();
				break;
			case NPC_BALOROC:
				uiBaloroc = creature->GetGUID();
				break;
			case NPC_MAJORDOMUS:
				uiMajordomus = creature->GetGUID();
				break;
			case NPC_RAGNAROS_CATA:
				uiRagnaros = creature->GetGUID();
				break;

				// Npcs
			case NPC_RAGEFACE:
				uiRageface = creature->GetGUID();
				break;
			case NPC_RIPLIMB:
				uiRiplimb = creature->GetGUID();
				break;
			case NPC_SHANNOX_SPEAR:
				uiShannoxSpear = creature->GetGUID();
				break;
			}

		}

		void OnGameObjectCreate(GameObject* gameObject)
		{
		}

		void OnGameObjectRemove(GameObject* gameObject)
		{
		}

		void OnCreatureDeath(Creature* creature)
		{
			switch (creature->GetEntry())
			{
			case NPC_BETHTILAC:
				//SetBossState();
				break;
			case NPC_RHYOLITH:
			case NPC_ALYSRAZAR:
			case NPC_SHANNOX:
			case NPC_BALOROC:
			case NPC_MAJORDOMUS:
			case NPC_RAGNAROS_CATA:
			default:
				break;
			}
		}

		bool SetBossState(uint32 type, EncounterState state)
		{
			if (!InstanceScript::SetBossState(type, state))
				return false;

			/*switch (type)
			{
			case BOSS_BETHTILAC:
			break;
			case BOSS_RHYOLITH:
			break;
			case BOSS_ALYSRAZAR:
			break;
			case BOSS_SHANNOX:
			break;
			case BOSS_BALOROC:
			break;
			case BOSS_MAJORDOMUS:
			break;
			case BOSS_RAGNAROS:
			break;

			}*/

			return true;
		}

		void SetData(uint32 type, uint32 data)
		{
			switch (type)
			{
			case DATA_CURRENT_ENCOUNTER_PHASE:
				encounterSharingPhase = data;
				break;

			default:
				break;
			}

		}

		void SetData64(uint32 /*type*/, uint64 /*data*/)
		{

		}

		uint64 GetData64(uint32 data)
		{
			switch (data)
			{
				// Bosses
			case NPC_BETHTILAC:
				return uiBethilac;
			case NPC_RHYOLITH:
				return uiRhyolith;
			case NPC_ALYSRAZAR:
				return uiAlysrazar;
			case NPC_SHANNOX:
				return uiShannox;
			case NPC_BALOROC:
				return uiBaloroc;
			case NPC_MAJORDOMUS:
				return uiMajordomus;
			case NPC_RAGNAROS_CATA:
				return uiRagnaros;

				// Npcs
			case NPC_RAGEFACE:
				return uiRageface;
			case NPC_RIPLIMB:
				return uiRiplimb;
			case NPC_SHANNOX_SPEAR:
				return uiShannoxSpear;
			}

			return NULL;
		}

		uint32 GetData(uint32 type)
		{
			switch (type)
			{
			case DATA_CURRENT_ENCOUNTER_PHASE:
				return encounterSharingPhase;
			}

			return 0;
		}

		std::string GetSaveData()
		{
			std::ostringstream saveStream;
			saveStream << "F L ";
			for(int i = 0; i < MAX_ENCOUNTER; ++i)
				saveStream << Encounter[i] << " ";

			return saveStream.str();
		}

		void Load(const char * data)
		{
			std::istringstream loadStream(data);
			char dataHead1, dataHead2;
			loadStream >> dataHead1 >> dataHead2;
			std::string newdata = loadStream.str();

			uint32 buff;
			if(dataHead1 == 'F' && dataHead2 == 'L')
			{
				for(int i = 0; i < MAX_ENCOUNTER; ++i)
				{
					loadStream >> buff;
					Encounter[i]= buff;
				}
			}

			for(int i = 0; i < MAX_ENCOUNTER; ++i)
				if(Encounter[i] != DONE)
					Encounter[i] = NOT_STARTED;
		}
	};

	InstanceScript* GetInstanceScript(InstanceMap* map) const
	{
		return new instance_firelands_InstanceMapScript(map);
	}
};

void AddSC_instance_firelands()
{
	new instance_firelands();
}
