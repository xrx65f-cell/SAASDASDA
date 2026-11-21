#include <Windows.h>
#include <cstdint>
#include "script.h"
#include "natives.h"

// GTA VI Legacy Core - native ScriptHookV ASI starter
// Story Mode only. Built as a native x64 ASI plugin.

static bool g_menuOpen = false;
static bool g_hudEnabled = true;
static int g_selected = 0;
static bool g_prevF10 = false;
static bool g_prevUp = false;
static bool g_prevDown = false;
static bool g_prevEnter = false;
static bool g_prevEsc = false;

static bool KeyPressed(int vk, bool& wasDown)
{
    const bool down = (GetAsyncKeyState(vk) & 0x8000) != 0;
    const bool pressed = down && !wasDown;
    wasDown = down;
    return pressed;
}

static void DrawTextLine(const char* text, float x, float y, float scale,
                         int r, int g, int b, int a = 255)
{
    HUD::SET_TEXT_FONT(4);
    HUD::SET_TEXT_SCALE(0.0f, scale);
    HUD::SET_TEXT_COLOUR(r, g, b, a);
    HUD::SET_TEXT_OUTLINE();
    HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
    HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
    HUD::END_TEXT_COMMAND_DISPLAY_TEXT(x, y);
}

static void DrawRect(float x, float y, float w, float h,
                     int r, int g, int b, int a)
{
    GRAPHICS::DRAW_RECT(x, y, w, h, r, g, b, a);
}

static void DrawHud()
{
    const Ped ped = PLAYER::PLAYER_PED_ID();
    if (!ENTITY::DOES_ENTITY_EXIST(ped)) return;

    const int health = ENTITY::GET_ENTITY_HEALTH(ped);
    const int maxHealth = ENTITY::GET_ENTITY_MAX_HEALTH(ped);
    const int armor = PED::GET_PED_ARMOUR(ped);
    const int wanted = PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());

    DrawRect(0.112f, 0.916f, 0.185f, 0.115f, 12, 9, 22, 185);
    DrawRect(0.112f, 0.866f, 0.185f, 0.006f, 105, 65, 190, 235);
    DrawTextLine("GTA VI  |  LEGACY CORE", 0.035f, 0.858f, 0.30f, 210, 190, 255);

    char line[96];
    sprintf_s(line, "HEALTH  %d / %d", health, maxHealth);
    DrawTextLine(line, 0.035f, 0.887f, 0.27f, 255, 255, 255);
    sprintf_s(line, "ARMOR   %d", armor);
    DrawTextLine(line, 0.035f, 0.915f, 0.27f, 195, 170, 255);
    sprintf_s(line, "WANTED  %d", wanted);
    DrawTextLine(line, 0.035f, 0.943f, 0.27f, 255, 255, 255);
}

static void ActivateOption()
{
    const Ped ped = PLAYER::PLAYER_PED_ID();

    switch (g_selected)
    {
    case 0: // Heal
        ENTITY::SET_ENTITY_HEALTH(ped, ENTITY::GET_ENTITY_MAX_HEALTH(ped), 0);
        break;
    case 1: // Full armor
        PED::SET_PED_ARMOUR(ped, 100);
        break;
    case 2: // Clear wanted level
        PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
        break;
    case 3: // Repair vehicle
        if (PED::IS_PED_IN_ANY_VEHICLE(ped, false))
        {
            const Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(ped, false);
            VEHICLE::SET_VEHICLE_FIXED(vehicle);
            VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(vehicle);
            VEHICLE::SET_VEHICLE_DIRT_LEVEL(vehicle, 0.0f);
        }
        break;
    case 4: // Toggle HUD
        g_hudEnabled = !g_hudEnabled;
        break;
    case 5:
        g_menuOpen = false;
        break;
    }
}

static void DrawMenu()
{
    static const char* options[] = {
        "Heal player",
        "Give full armor",
        "Clear wanted level",
        "Repair current vehicle",
        "Toggle HUD",
        "Close menu"
    };

    DrawRect(0.50f, 0.50f, 0.32f, 0.44f, 10, 8, 18, 238);
    DrawRect(0.50f, 0.305f, 0.32f, 0.047f, 78, 42, 135, 250);
    DrawTextLine("GTA VI LEGACY CORE", 0.393f, 0.292f, 0.40f, 255, 255, 255);

    for (int i = 0; i < 6; ++i)
    {
        const float y = 0.365f + (i * 0.052f);
        if (i == g_selected)
            DrawRect(0.50f, y + 0.012f, 0.285f, 0.041f, 92, 55, 150, 225);

        DrawTextLine(options[i], 0.385f, y, 0.32f,
                     i == g_selected ? 255 : 220,
                     i == g_selected ? 255 : 220,
                     i == g_selected ? 255 : 235);
    }

    DrawTextLine("F10 MENU  |  UP/DOWN SELECT  |  ENTER CONFIRM  |  ESC CLOSE",
                 0.285f, 0.735f, 0.20f, 205, 190, 225);
}

static void ScriptMain()
{
    while (true)
    {
        if (KeyPressed(VK_F10, g_prevF10))
            g_menuOpen = !g_menuOpen;

        if (g_menuOpen)
        {
            if (KeyPressed(VK_UP, g_prevUp))
                g_selected = (g_selected + 5) % 6;
            if (KeyPressed(VK_DOWN, g_prevDown))
                g_selected = (g_selected + 1) % 6;
            if (KeyPressed(VK_RETURN, g_prevEnter))
                ActivateOption();
            if (KeyPressed(VK_ESCAPE, g_prevEsc))
                g_menuOpen = false;
        }

        if (g_hudEnabled)
            DrawHud();

        if (g_menuOpen)
            DrawMenu();

        WAIT(0);
    }
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
        scriptRegister(module, ScriptMain);
    else if (reason == DLL_PROCESS_DETACH)
        scriptUnregister(module);

    return TRUE;
}
