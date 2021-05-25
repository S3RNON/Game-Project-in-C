#include "main.h"
#include "menus.h"

void inline PPI_TitleScreen(void)
{
    if (G_GameInput.EnterKeyIsDown && !G_GameInput.EnterKeyWasDown)
    {
        GM_TitleScreen.Items[GM_TitleScreen.SelectedItem]->Action();
        PlayGameSound(&G_SFXMenuEnter);
    }

    if (G_GameInput.DownKeyIsDown && !G_GameInput.DownKeyWasDown)
    {
        if (GM_TitleScreen.SelectedItem < GM_TitleScreen.ItemCount - 1)
        {
            GM_TitleScreen.SelectedItem++;
        }
        else
        {
            GM_TitleScreen.SelectedItem = (uint8_t)!G_GameplayIsInProgress;
        }
        PlayGameSound(&G_SFXMenuNavigate);
    }

    if (G_GameInput.UpKeyIsDown && !G_GameInput.UpKeyWasDown)
    {
        if (GM_TitleScreen.SelectedItem > !G_GameplayIsInProgress)
        {
            GM_TitleScreen.SelectedItem--;
        }
        else
        {
            GM_TitleScreen.SelectedItem = GM_TitleScreen.ItemCount - 1;
        }
        PlayGameSound(&G_SFXMenuNavigate);
    }
}

void inline DrawTitleScreen(void)
{
    static uint64_t LocalFrameCounter = 0;
    static uint64_t LastFrameSeen = 0;
    if (G_PerformanceData.TotalFramesRendered > (LastFrameSeen + 1))
    {
        LocalFrameCounter = 0;
        GM_TitleScreen.SelectedItem = !G_GameplayIsInProgress;
    }

    memset(G_BackBuffer.Memory, 0, G_CanvasMemorySize);
    BlitStringToBuffer(&G_6x7Font, (G_GameWidth - ((uint32_t)strlen(GAME_NAME) * 6)) / 2, 130, GAME_NAME, 255, 255, 255, 255);

    for (uint8_t i = 0; i < GM_TitleScreen.ItemCount; i++)
    {
        if (GM_TitleScreen.Items[i]->Enabled == TRUE)
        {
            BlitStringToBuffer(&G_6x7Font, GM_TitleScreen.Items[i]->X, GM_TitleScreen.Items[i]->Y, GM_TitleScreen.Items[i]->Name, 255, 255, 255, 255);
        }
    }
    BlitStringToBuffer(&G_6x7Font, GM_TitleScreen.Items[GM_TitleScreen.SelectedItem]->X - 6, GM_TitleScreen.Items[GM_TitleScreen.SelectedItem]->Y, ">", 255, 255, 255, 255);

    LocalFrameCounter++;
    LastFrameSeen = G_PerformanceData.TotalFramesRendered;
}