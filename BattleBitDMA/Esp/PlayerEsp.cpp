#include "Pch.h"
#include "PlayerEsp.h"
#include "globals.h"
#include "CheatFunction.h"
#include "Drawing.h"
#include "ConfigInstance.h"

void SortPlayerListByDistance() {
    if (CurrentLocalPlayer == nullptr) {
        return;
    }

    std::sort(BasePlayer->PlayerList.begin(), BasePlayer->PlayerList.end(), [](const std::shared_ptr<PlayerNetwork>& a, const std::shared_ptr<PlayerNetwork>& b) {
        auto aState = a->GetPlayerNettworkState();
        auto bState = b->GetPlayerNettworkState();
        auto localPlayerPosition = CurrentLocalPlayer->GetPlayerNettworkState()->GetPosition();

        if (aState->GetHealth() <= 0 || aState->GetFriendly() || Vector3::Distance(aState->GetPosition(), localPlayerPosition) > ConfigInstance.Player.MaxDistance || a == CurrentLocalPlayer) {
            return false;
        }

        if (bState->GetHealth() <= 0 || bState->GetFriendly() || Vector3::Distance(bState->GetPosition(), localPlayerPosition) > ConfigInstance.Player.MaxDistance || b == CurrentLocalPlayer) {
            return true;
        }

        Vector2 screenPosA = Camera->WorldToScreen(aState->GetPosition());
        Vector2 screenPosB = Camera->WorldToScreen(bState->GetPosition());
        float distanceA = Vector2::Distance(screenPosA, ConfigInstance.Player.screenCenter);
        float distanceB = Vector2::Distance(screenPosB, ConfigInstance.Player.screenCenter);

        return distanceA < distanceB;
        });
}

std::shared_ptr<CheatFunction> UpdatePlayers = std::make_shared<CheatFunction>(10, [] {
    if (CurrentLocalPlayer == nullptr) {
        return;
    }

    auto handle = TargetProcess.CreateScatterHandle();

    for (auto& player : BasePlayer->PlayerList) {
        if (player->GetPlayerNettworkState()->GetHealth() > 0) {
            player->GetPlayerNettworkState()->UpdatePosition(handle);
        }
    }

    TargetProcess.ExecuteReadScatter(handle);
    TargetProcess.CloseScatterHandle(handle);

    SortPlayerListByDistance();
    });

void DrawPlayers() {
    auto& playerConfig = ConfigInstance.Player;

    if (playerConfig.EspCircleShow) {
        OutlineCircle(playerConfig.screenCenter.x, playerConfig.screenCenter.y, playerConfig.EspCircleSize, 1, playerConfig.TextColour);
    }

    if (playerConfig.ShowAimbotFov) {
        OutlineCircle(playerConfig.screenCenter.x, playerConfig.screenCenter.y, playerConfig.FOV, 1, playerConfig.TextColour);
    }

    if (CurrentLocalPlayer == nullptr) {
        return;
    }

    for (const auto& player : BasePlayer->PlayerList) {
        auto playerState = player->GetPlayerNettworkState();
        if (player == CurrentLocalPlayer || playerState->GetHealth() <= 0 || playerState->GetFriendly()) {
            continue;
        }

        Vector3 position = playerState->GetPosition();
        int distance = Vector3::Distance(position, CurrentLocalPlayer->GetPlayerNettworkState()->GetPosition());

        if (distance > playerConfig.MaxDistance) {
            continue;
        }

        Vector2 screenpos = Camera->WorldToScreen(position);
        if (screenpos == Vector2::Zero()) {
            continue;
        }

        if (playerConfig.EspCircle) {
            int distanceFromCenter = Vector2::Distance(screenpos, playerConfig.screenCenter);
            if (distanceFromCenter > playerConfig.EspCircleSize) {
                continue;
            }
        }

        std::wstring distancestr = playerConfig.Distance ? LIT(L"[") + std::to_wstring(distance) + LIT(L"m]") : LIT(L"");
        std::wstring healthstr = playerConfig.Health ? LIT(L"[") + std::to_wstring(static_cast<int>(playerState->GetHealth())) + LIT(L"❤]") : LIT(L"");
        std::wstring primary = playerConfig.PrimaryWeapon ? playerState->GetPrimaryWeaponName() : LIT(L"");

        DrawText(screenpos.x, screenpos.y + (playerConfig.FontSize + 2), distancestr, LIT("Verdana"), playerConfig.FontSize, playerConfig.TextColour, FontAlignment::Centre);
        DrawText(screenpos.x, screenpos.y + (playerConfig.FontSize * 2), healthstr, LIT("Verdana"), playerConfig.FontSize, playerConfig.TextColour, FontAlignment::Centre);
        DrawText(screenpos.x, screenpos.y + (playerConfig.FontSize * 3), primary, LIT("Verdana"), playerConfig.FontSize, playerConfig.TextColour, FontAlignment::Centre);

        if (playerConfig.Circle) {
            FilledCircle(screenpos.x, screenpos.y, playerConfig.CircleSize, playerConfig.CircleColour);
        }

        if (playerConfig.Lines) {
            if (playerConfig.LineOrigin == 0) {
                playerConfig.lineStartX = 960;
                playerConfig.lineStartY = 0;
            }
            else if (playerConfig.LineOrigin == 1) {
                playerConfig.lineStartX = 960;
                playerConfig.lineStartY = 540;
            }
            else {
                playerConfig.lineStartX = 960;
                playerConfig.lineStartY = 1080;
            }

            FilledLine(playerConfig.lineStartX, playerConfig.lineStartY, screenpos.x, screenpos.y, 1, playerConfig.TextColour);
        }

    }
}
