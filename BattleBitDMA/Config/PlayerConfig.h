#pragma once
#include "pch.h"
class PlayerConfig
{
	std::string ConfigName;

public:
	PlayerConfig(const std::string& name)
	{
		ConfigName = name;
	}

    bool Health = true;
    bool Distance = true;
    int MaxDistance = 300;
    bool PrimaryWeapon = false;
    bool Circle = true;
    bool Transparent = false;

    D2D1::ColorF TextColour = Colour(255, 255, 255);
    D2D1::ColorF CircleColour = Colour(255, 0, 0);
    int FontSize = 8;
    int CircleSize = 2;
    std::string Ip = "192.168.2.188";
    WORD Port = 10240;
    std::string Uuid = "793A5C53";
    void ToJsonColour(json* j, const std::string& name, D2D1::ColorF* colour)
    {
        (*j)[ConfigName][name][LIT("r")] = colour->r;
        (*j)[ConfigName][name][LIT("g")] = colour->g;
        (*j)[ConfigName][name][LIT("b")] = colour->b;
        (*j)[ConfigName][name][LIT("a")] = colour->a;

    }
    void FromJsonColour(json j, const std::string& name, D2D1::ColorF* colour)
    {
        if (j[ConfigName].contains(name))
        {
            colour->r = j[ConfigName][name][LIT("r")];
            colour->g = j[ConfigName][name][LIT("g")];
            colour->b = j[ConfigName][name][LIT("b")];
            colour->a = j[ConfigName][name][LIT("a")];
        }
    }

    json ToJson()
    {
        json j;
        j[ConfigName][LIT("Health")] = Health;
        j[ConfigName][LIT("Distance")] = Distance;
        j[ConfigName][LIT("MaxDistance")] = MaxDistance;
        j[ConfigName][LIT("FontSize")] = FontSize;
        j[ConfigName][LIT("CircleSize")] = CircleSize;
        j[ConfigName][LIT("PrimaryWeapon")] = PrimaryWeapon;
        j[ConfigName][LIT("Circle")] = Circle;
        j[ConfigName][LIT("Ip")] = Ip;
        j[ConfigName][LIT("Port")] = Port;
        j[ConfigName][LIT("Uuid")] = Uuid;
        j[ConfigName][LIT("Transparent")] = Transparent;
        ToJsonColour(&j, LIT("TextColour"), &TextColour);

        return j;
    }
    void FromJson(const json& j)
    {
        if (!j.contains(ConfigName))
            return;
        if (j[ConfigName].contains(LIT("Health")))
            Health = j[ConfigName][LIT("Health")];
        if (j[ConfigName].contains(LIT("Distance")))
            Distance = j[ConfigName][LIT("Distance")];
        if (j[ConfigName].contains(LIT("PrimaryWeapon")))
            PrimaryWeapon = j[ConfigName][LIT("PrimaryWeapon")];
        if (j[ConfigName].contains(LIT("Circle")))
            Circle = j[ConfigName][LIT("Circle")];
        if (j[ConfigName].contains(LIT("FontSize")))
            FontSize = j[ConfigName][LIT("FontSize")];
        if (j[ConfigName].contains(LIT("CircleSize")))
            CircleSize = j[ConfigName][LIT("CircleSize")];
        if (j[ConfigName].contains(LIT("MaxDistance")))
            MaxDistance = j[ConfigName][LIT("MaxDistance")];
        if (j[ConfigName].contains(LIT("Ip")))
            MaxDistance = j[ConfigName][LIT("Ip")];
        if (j[ConfigName].contains(LIT("Port")))
            MaxDistance = j[ConfigName][LIT("Port")];
        if (j[ConfigName].contains(LIT("Uuid")))
            MaxDistance = j[ConfigName][LIT("Uuid")];
        if (j[ConfigName].contains(LIT("Transparent")))
            MaxDistance = j[ConfigName][LIT("Transparent")];
        FromJsonColour(j, LIT("TextColour"), &TextColour);
    }
};

