#include "main.h"

void inline PPI_GamepadUnplugged(void)
{
    if (G_XinputID >= 0 || (G_GameInput.EscapeKeyIsDown && !G_GameInput.EscapeKeyWasDown))
    {
        if (G_PreviousGameState != 0)
        {
            G_CurrentGameState = G_PreviousGameState;
            G_PreviousGameState = GAMESTATE_GAMEPADUNPLUGGED;
        }
        else
        {
            G_PreviousGameState = G_CurrentGameState;
            G_CurrentGameState = GAMESTATE_TITLESCREEN;
        }
    }
}

void inline DrawGamepadUnpluggedScreen(void)
{
    __stosd(G_BackBuffer.Memory, 0x00000000, G_CanvasMemorySize / sizeof(PIXEL32));
    BlitStringToBuffer(&G_6x7Font, (uint32_t)((G_GameWidth - strlen("Gamepad Disconnected!") * 6) / 2), 100, "Gamepad Disconnected!", 255, 255, 255, 255);
    BlitStringToBuffer(&G_6x7Font, (uint32_t)((G_GameWidth - strlen("Reconnect it, or press escape to continue using the keyboard.") * 6) / 2), 130, "Reconnect it, or press escape to continue using the keyboard.", 255, 255, 255, 255);
}