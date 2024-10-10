#include "../../Engine.h"
#include "../Simulation.h"
#include "../../engine/SplashScreen.h"

Engine::Simulation::Simulation()
{
}

Engine::Simulation::~Simulation()
{
}

VOID Engine::Simulation::PreInitialize()
{

    Logger::separator();
    // Logger::info(L"Application Starting...\n");
    // Logger::info("Game Name: %s\n", PerGameSettings::GameName());
    // Logger::info("Boot Time: %s\n", Time::GetDateTimeString().c_str());
    // Logger::info("Engine Mode: %s\n", Engine::EngineModeToString().c_str());
    Logger::separator();

    SplashScreen::Open();


}