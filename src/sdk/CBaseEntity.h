#pragma once
#include "BaseStruct.h"
#include "OffPtr.h"
#include "QAngle.h"
#include "../Netvars.h"
#include "Definitions.h" // so u can use flags
#include "../utils/Logger.h"

#define CBASE_ENTITY_OFFSETS(HANDLER)           \
    HANDLER(Vector, 0x130, velocity)            \
    HANDLER(Vector, 0x13c, origin)              \
    HANDLER(Vector, 0x1c0, glowCol)             \
    HANDLER(float, 0x2c0, glowTimes1)           \
    HANDLER(float, 0x2c4, glowTimes2)           \
    HANDLER(float, 0x2c8, glowTimes3)           \
    HANDLER(float, 0x2cc, glowTimes4)           \
    HANDLER(float, 0x2d0, glowTimes5)           \
    HANDLER(float, 0x2d4, glowTimes6)           \
    HANDLER(float, 0x2ec, glowDistance)         \
    HANDLER(int, 0x300, iGlowEnable)            \
    HANDLER(bool, 0x380, bGlowEnable)           \
    HANDLER(int, 0x3F0, teamNum)                \
            HANDLER(Vector, 0x414, localAngles)         \
    HANDLER(uintptr_t, 0xed8, boneMatrix)       \
    HANDLER(uintptr_t, 0x1704, activeWeapon)    \
    HANDLER(QAngle, 0x2114 , aimPunch)           \
    HANDLER(QAngle, 0x21A8, swayAngles)         \
    HANDLER(QAngle, 0x21B8, viewAngles)         \
    /*HANDLER(int, 0x2368, lifeState)*/         \
    HANDLER(int, 0x3E0, health)                 \
    HANDLER(Vector, 0x1BC4, eyePos)             \
    /*HANDLER(Vector, 0x4264, eyePos)             \*/

#define CONSTRUCTOR_HANDLER(type, offset, name) , name(baseClass)
#define DEFINE_HANDLER(type, offset, name) OffPtr<type, offset> name;
#define WRITE_BACK_HANDLER(type, offset, name) name.WriteBack(writeList);

class CBaseEntity
{
  private:
    char rBuf[0x2200];
    ProcessBaseClass baseClass;
  public:

    CBaseEntity(uintptr_t addr = 0)
        : baseClass(rBuf, addr) CBASE_ENTITY_OFFSETS(CONSTRUCTOR_HANDLER)
    {
    }

    const ProcessBaseClass& GetBaseClass()
    {
        return baseClass;
    }

    void Update(uintptr_t newAddress = 0)
    {
        if (newAddress)
            baseClass.address = newAddress;
        process->Read(baseClass.address, rBuf, sizeof(rBuf));
    }

    void WriteBack(WriteList& writeList)
    {
        CBASE_ENTITY_OFFSETS(WRITE_BACK_HANDLER);
    }

    inline bool operator==(const CBaseEntity &o)
    {
        return baseClass.address == o.baseClass.address;
    }

    inline bool operator==(uintptr_t addr)
    {
        return baseClass.address == addr;
    }

    inline operator bool() const
    {
        return baseClass.address;
    }

    inline int GetBleedoutState() {
        static uint32_t offset = Netvars::netvars["CPlayer"]["m_bleedoutState"];
        if( !offset ) {
            Logger::Log("Can't find Netvar [\"CPlayer\"][\"m_bleedoutState\"]!\n");
            return -1;
        }
        return process->Read<int>( baseClass.address + offset );
    }

    inline int GetLifestate() {
        static uint32_t offset = Netvars::netvars["CPlayer"]["m_lifeState"];
        if( !offset ) {
            Logger::Log("Can't find Netvar [\"CPlayer\"][\"m_lifeState\"]!\n");
            return -1;
        }
        return process->Read<int>( baseClass.address + offset );
    }

    inline int GetFlags() {
        static uint32_t offset = Netvars::netvars["CPlayer"]["m_fFlags"];
        if( !offset ) {
            Logger::Log("Can't find Netvar [\"CPlayer\"][\"m_fFlags\"]!\n");
            return -1;
        }
        return process->Read<int>( baseClass.address + offset );
    }

    inline int GetTeamNum() {
        static uint32_t offset = Netvars::netvars["CBaseEntity"]["m_iTeamNum"];
        if( !offset ){
            Logger::Log("Can't find Netvar [\"CBaseEntity\"][\"m_iTeamNum\"]!\n");
            return -1;
        }

        return process->Read<int>( baseClass.address + offset );
    }

    CBASE_ENTITY_OFFSETS(DEFINE_HANDLER)
};
