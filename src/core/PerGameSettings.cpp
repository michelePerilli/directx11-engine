#include "../Engine.h"

PerGameSettings* PerGameSettings::inst;

PerGameSettings::PerGameSettings() {

    inst = this;

    inst->m_GameName = L"undefined";
    inst->m_ShortName = L"undefined";
    inst->m_BootTime = Time::GetDateTimeString(TRUE);
}

PerGameSettings::~PerGameSettings() = default;