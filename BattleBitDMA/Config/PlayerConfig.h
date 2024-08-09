#pragma once
#include "pch.h"

class PlayerConfig {
private:
    std::string ConfigName;

public:
    // Constructor
    PlayerConfig(const std::string& name) : ConfigName(name) {}

    // Screen settings
    int screenX = GetSystemMetrics(SM_CXSCREEN);
    int screenY = GetSystemMetrics(SM_CYSCREEN);
    Vector2 screenCenter = Vector2(screenX / 2, screenY / 2);

    // ESP settings
    bool Health = false;
    bool Distance = true;
    int MaxDistance = 100;
    bool PrimaryWeapon = false;
    bool Circle = true;
    int CircleSize = 2;
    bool EspCircle = false;
    bool EspCircleShow = false;
    int EspCircleSize = 80;

    // Aimbot settings
    bool Aimbot = true;
    int Aimkey = 1;
    int AimbotXSmooth = 1;
    int AimbotYSmooth = 1;
    int FOV = 52;
    bool ShowAimbotFov = true;

    // Visual settings
    bool Transparent = false;
    bool Lines = false;
    int LineOrigin = 2;
    int lineStartX = 0, lineStartY = 0;
    int FontSize = 8;

    D2D1::ColorF TextColour = Colour(255, 255, 255);
    D2D1::ColorF CircleColour = Colour(255, 0, 0);

    // Kmbox Net settings
    std::string Ip = "192.168.2.188";
    WORD Port = 10240;
    std::string Uuid = "793A5C53";

    // JSON serialization for color
    void ToJsonColour(json* j, const std::string& name, D2D1::ColorF* colour) {
        (*j)[ConfigName][name][LIT("r")] = colour->r;
        (*j)[ConfigName][name][LIT("g")] = colour->g;
        (*j)[ConfigName][name][LIT("b")] = colour->b;
        (*j)[ConfigName][name][LIT("a")] = colour->a;
    }

    void FromJsonColour(const json& j, const std::string& name, D2D1::ColorF* colour) {
        if (j[ConfigName].contains(name)) {
            colour->r = j[ConfigName][name][LIT("r")];
            colour->g = j[ConfigName][name][LIT("g")];
            colour->b = j[ConfigName][name][LIT("b")];
            colour->a = j[ConfigName][name][LIT("a")];
        }
    }

    // JSON serialization
    json ToJson() {
        json j;
        j[ConfigName][LIT("Health")] = Health;
        j[ConfigName][LIT("Distance")] = Distance;
        j[ConfigName][LIT("MaxDistance")] = MaxDistance;
        j[ConfigName][LIT("PrimaryWeapon")] = PrimaryWeapon;
        j[ConfigName][LIT("Circle")] = Circle;
        j[ConfigName][LIT("CircleSize")] = CircleSize;
        j[ConfigName][LIT("EspCircle")] = EspCircle;
        j[ConfigName][LIT("EspCircleShow")] = EspCircleShow;
        j[ConfigName][LIT("EspCircleSize")] = EspCircleSize;
        j[ConfigName][LIT("Transparent")] = Transparent;
        j[ConfigName][LIT("Lines")] = Lines;
        j[ConfigName][LIT("LineOrigin")] = LineOrigin;
        j[ConfigName][LIT("FontSize")] = FontSize;
        j[ConfigName][LIT("Aimbot")] = Aimbot;
        j[ConfigName][LIT("Aimkey")] = Aimkey;
        j[ConfigName][LIT("AimbotXSmooth")] = AimbotXSmooth;
        j[ConfigName][LIT("AimbotYSmooth")] = AimbotYSmooth;
        j[ConfigName][LIT("FOV")] = FOV;
        j[ConfigName][LIT("ShowAimbotFov")] = ShowAimbotFov;
        j[ConfigName][LIT("Ip")] = Ip;
        j[ConfigName][LIT("Port")] = Port;
        j[ConfigName][LIT("Uuid")] = Uuid;
        ToJsonColour(&j, LIT("TextColour"), &TextColour);
        ToJsonColour(&j, LIT("CircleColour"), &CircleColour);

        return j;
    }

    void FromJson(const json& j) {
        if (!j.contains(ConfigName)) return;

        Health = j[ConfigName].value(LIT("Health"), Health);
        Distance = j[ConfigName].value(LIT("Distance"), Distance);
        MaxDistance = j[ConfigName].value(LIT("MaxDistance"), MaxDistance);
        PrimaryWeapon = j[ConfigName].value(LIT("PrimaryWeapon"), PrimaryWeapon);
        Circle = j[ConfigName].value(LIT("Circle"), Circle);
        CircleSize = j[ConfigName].value(LIT("CircleSize"), CircleSize);
        EspCircle = j[ConfigName].value(LIT("EspCircle"), EspCircle);
        EspCircleShow = j[ConfigName].value(LIT("EspCircleShow"), EspCircleShow);
        EspCircleSize = j[ConfigName].value(LIT("EspCircleSize"), EspCircleSize);
        Transparent = j[ConfigName].value(LIT("Transparent"), Transparent);
        Lines = j[ConfigName].value(LIT("Lines"), Lines);
        LineOrigin = j[ConfigName].value(LIT("LineOrigin"), LineOrigin);
        FontSize = j[ConfigName].value(LIT("FontSize"), FontSize);
        Aimbot = j[ConfigName].value(LIT("Aimbot"), Aimbot);
        Aimkey = j[ConfigName].value(LIT("Aimkey"), Aimkey);
        AimbotXSmooth = j[ConfigName].value(LIT("AimbotXSmooth"), AimbotXSmooth);
        AimbotYSmooth = j[ConfigName].value(LIT("AimbotYSmooth"), AimbotYSmooth);
        FOV = j[ConfigName].value(LIT("FOV"), FOV);
        ShowAimbotFov = j[ConfigName].value(LIT("ShowAimbotFov"), ShowAimbotFov);
        Ip = j[ConfigName].value(LIT("Ip"), Ip);
        Port = j[ConfigName].value(LIT("Port"), Port);
        Uuid = j[ConfigName].value(LIT("Uuid"), Uuid);

        FromJsonColour(j, LIT("TextColour"), &TextColour);
        FromJsonColour(j, LIT("CircleColour"), &CircleColour);
    }
};
