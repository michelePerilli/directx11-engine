
#include "Simulation.h"
#include "Logger.h"
#include "Time.h"
#include "../core/PerGameSettings.h"

Engine::Simulation::Simulation() = default;

Engine::Simulation::~Simulation() = default;

VOID Engine::Simulation::PreInitialize() {
    Logger::separator();
    Logger::info("Application Starting...");
    Logger::info("Game Name: %s", PerGameSettings::GameName().c_str());
    Logger::info("Boot Time: %s", Time::GetDateTimeString().c_str());
    Logger::separator();
}