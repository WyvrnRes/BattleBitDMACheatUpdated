#include "Pch.h"
#include "aimbot.h"
#include "PlayerEsp.h"
#include "globals.h"
#include "CheatFunction.h"
#include "ConfigInstance.h"
#include "ConfigUtilities.h"
#include "Manager.h"
#include "InputManager.h"

bool AimKeyDown = false;

std::shared_ptr<CheatFunction> UpdateAimKey = std::make_shared<CheatFunction>(50, [] {
    AimKeyDown = Keyboard::IsKeyDown(ConfigInstance.Player.Aimkey);
    });

std::chrono::system_clock::time_point KmboxStart;

void Aimbot() {
    auto CurrentHealth = CurrentLocalPlayer->GetPlayerNettworkState()->GetHealth();
    auto AimbotTarget = BasePlayer->PlayerList.empty() ? nullptr : BasePlayer->PlayerList.front();

    if (CurrentHealth == 0) {
        AimbotTarget = nullptr;
    }

    UpdateAimKey->Execute();

    if (!ConfigInstance.Player.Aimbot || !AimKeyDown || AimbotTarget == nullptr) {
        AimbotTarget = nullptr;
        return;
    }

    auto TargetPosition = AimbotTarget->GetPlayerNettworkState()->GetPosition();
    if (TargetPosition == Vector3::Zero()) {
        AimbotTarget = nullptr;
        return;
    }

    Vector2 closestScreenPos = Camera->WorldToScreen(TargetPosition);
    if (Vector2::Distance(closestScreenPos, ConfigInstance.Player.screenCenter) > Configs.Player.FOV) {
        return;
    }

    if (closestScreenPos == Vector2::Zero()) {
        AimbotTarget = nullptr;
        return;
    }

    Vector2 diff = { 0, 0 };
    if (ConfigInstance.Player.AimbotXSmooth >= 1 || ConfigInstance.Player.AimbotYSmooth >= 1) {
        float x = closestScreenPos.x - ConfigInstance.Player.screenCenter.x;
        float y = closestScreenPos.y - ConfigInstance.Player.screenCenter.y;
        int smoothx = Configs.Player.AimbotXSmooth;
        int smoothy = Configs.Player.AimbotYSmooth;

        srand(__rdtsc());
        float randomnumber = static_cast<float>(rand() % 100001) / 100000;
        diff.x = static_cast<int>(std::round(x / ((smoothx > 1 ? smoothx / 2 : smoothx) +
            static_cast<float>(rand() % (smoothx > 1 ? smoothx / 2 : smoothx)) +
            static_cast<float>(rand() % 5) + randomnumber)));
        diff.y = static_cast<int>(std::round(y / ((smoothy > 1 ? smoothy / 2 : smoothy) +
            static_cast<float>(rand() % (smoothy > 1 ? smoothy / 2 : smoothy)) +
            static_cast<float>(rand() % 5) + randomnumber)));
    }

    float x = diff.x;
    float y = diff.y;

    if (KmboxStart + std::chrono::milliseconds(15) < std::chrono::system_clock::now()) {
        Kmbox.Mouse.Move(x, y);
        KmboxStart = std::chrono::system_clock::now();
    }
}
