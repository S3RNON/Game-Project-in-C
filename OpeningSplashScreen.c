#include "main.h"

void inline PPI_OpeningSplashScreen(void)
{
    if (G_GameInput.EscapeKeyIsDown && !G_GameInput.EscapeKeyWasDown)
    {
        G_PreviousGameState = G_CurrentGameState;
        G_CurrentGameState = GAMESTATE_TITLESCREEN;
    }
}

void inline DrawOpeningSplashScreen(void)
{
    static uint64_t LocalFrameCounter;
    static uint64_t LastFrameSeen;
    static uint8_t Alpha1 = 0;
    static uint8_t Alpha2 = 0;
    static uint8_t Alpha3 = 255;

    if (G_PerformanceData.TotalFramesRendered > (LastFrameSeen + 1))
    {
        LocalFrameCounter = 0;
    }

    __stosd(G_BackBuffer.Memory, 0x00000000, G_CanvasMemorySize / sizeof(PIXEL32));
    if (LocalFrameCounter > 60 && LocalFrameCounter <= 200)
    {
        BlitStringToBuffer(&G_6x7Font, (G_GameWidth - ((uint32_t)strlen("lucas north") * 6)) / 2, (G_GameHeight / 2) - 10, "lucas north", 255, 255, 255, Alpha1);
        if (Alpha1 < 254)
        {
            Alpha1 += 2;
        }
    }
    if (LocalFrameCounter > 200 && LocalFrameCounter < 320)
    {
        BlitStringToBuffer(&G_6x7Font, (G_GameWidth - ((uint32_t)strlen("lucas north") * 6)) / 2, (G_GameHeight / 2) - 10, "lucas north", 255, 255, 255, Alpha1);
        if (Alpha1 > 0)
        {
            Alpha1 -= 2;
        }
    }
    if (LocalFrameCounter > 110 && LocalFrameCounter <= 200)
    {
        BlitStringToBuffer(&G_6x7Font, (G_GameWidth - ((uint32_t)strlen("presents") * 6)) / 2, (G_GameHeight / 2) + 5, "presents", 255, 255, 255, Alpha2);
        if (Alpha2 < 252)
        {
            Alpha2 += 4;
        }
    }
    if (LocalFrameCounter > 200 && LocalFrameCounter < 320)
    {
        BlitStringToBuffer(&G_6x7Font, (G_GameWidth - ((uint32_t)strlen("presents") * 6)) / 2, (G_GameHeight / 2) + 5, "presents", 255, 255, 255, Alpha2);
        if (Alpha2 > 0)
        {
            Alpha2 -= 2;
        }
    }
    if (LocalFrameCounter > 360 && LocalFrameCounter < 583)
    {
        BlitStringToBuffer(&G_6x7Font, (G_GameWidth - ((uint32_t)strlen("Fantasy Game") * 6)) / 2, (G_GameHeight / 2) - 50, "Fantasy Game", 255, 255, 255, Alpha3);
    }
    if (LocalFrameCounter == 360)
    {
        PlayGameSound(&G_SFXSplashIntroDrop);
    }
    if (LocalFrameCounter > 520)
    {
        if (Alpha3 > 3)
        {
            Alpha3 -= 4;
        }
    }

    if (LocalFrameCounter > 600)
    {
        G_PreviousGameState = G_CurrentGameState;
        G_CurrentGameState = GAMESTATE_TITLESCREEN;
    }


    LocalFrameCounter++;
    LastFrameSeen = G_PerformanceData.TotalFramesRendered;
}