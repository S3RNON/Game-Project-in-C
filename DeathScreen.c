#include "main.h"

void inline PPI_DeathScreen(void)
{
    //PBYTE Keyboard[256] = { 0 };
    //if (GetKeyboardState(Keyboard) == 0) return;
    //if (Keyboard != 0)
    //{
    //    G_PreviousGameState = G_CurrentGameState;
    //    G_CurrentGameState = GAMESTATE_TITLESCREEN;
    //}
}

void inline DrawDeathScreen(void)
{
    __stosd(G_BackBuffer.Memory, 0x00000000, G_CanvasMemorySize / sizeof(PIXEL32));
    BlitStringToBuffer(&G_6x7Font, (uint32_t)((G_GameWidth - strlen("GAME OVER") * 6) / 2), 200, "GAME OVER", 255, 0, 0, 255);
    BlitStringToBuffer(&G_6x7Font, (uint32_t)((G_GameWidth - strlen("Press any button to enter the title screen.") * 6) / 2), 400, "Press any button to enter the title screen.", 255, 0, 0, 255);
}