#include "../PerGameSettings.h"
#include "../../component/Time.h"

PerGameSettings* PerGameSettings::inst;

PerGameSettings::PerGameSettings() {

    inst = this;

    inst->m_GameName = "undefined";
    inst->m_ShortName = "undefined";
    inst->m_MainIcon = LoadIcon(HInstance(), MAKEINTRESOURCE(1));
    std::stringstream ss;
    ss << Time::GetDateTimeString(TRUE).c_str();
    inst->m_BootTime = ss.str();

    inst->m_SplashURL = "assets/img.bmp";
}

PerGameSettings::~PerGameSettings() = default;