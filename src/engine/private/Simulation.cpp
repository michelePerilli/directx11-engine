#include "../../Engine.h"
#include "../Simulation.h"

Simulation::Simulation() = default;

Simulation::~Simulation() = default;

VOID Simulation::PreInitialize() {
    Logger::separator();
    Logger::info("Application Starting...");
    Logger::info("Game Name: %s", PerGameSettings::GameName().c_str());
    Logger::info("Boot Time: %s", Time::GetDateTimeString().c_str());
    Logger::info("Engine Mode: %s", EngineModeToString().c_str());
    Logger::separator();

}