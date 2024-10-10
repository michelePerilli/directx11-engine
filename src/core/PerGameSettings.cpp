#include "../Engine.h"

PerGameSettings* PerGameSettings::inst;

PerGameSettings::PerGameSettings() {

    inst = this;

    inst->m_GameName = L"undefined";
    inst->m_ShortName = L"undefined";

    std::wstringstream ss;
    ss << Time::GetDateTimeString(TRUE).c_str();
    inst->m_BootTime = ss.str();
}

PerGameSettings::~PerGameSettings() = default;