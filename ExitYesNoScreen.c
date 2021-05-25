#include "main.h"
#include "menus.h"

void inline PPI_ExitYesNoScreen(void)
{
    if (G_GameInput.EnterKeyIsDown && !G_GameInput.EnterKeyWasDown)
    {
        GM_ExitYesNoScreen.Items[GM_ExitYesNoScreen.SelectedItem]->Action();
        PlayGameSound(&G_SFXMenuEnter);
    }

    if (G_GameInput.RightKeyIsDown && !G_GameInput.RightKeyWasDown)
    {
        if (GM_ExitYesNoScreen.SelectedItem < GM_ExitYesNoScreen.ItemCount - 1)
        {
            GM_ExitYesNoScreen.SelectedItem++;
            PlayGameSound(&G_SFXMenuNavigate);
        }
        else
        {
            GM_ExitYesNoScreen.SelectedItem = 0;
            PlayGameSound(&G_SFXMenuNavigate);
        }
    }

    if (G_GameInput.LeftKeyIsDown && !G_GameInput.LeftKeyWasDown)
    {
        if (GM_ExitYesNoScreen.SelectedItem > 0)
        {
            GM_ExitYesNoScreen.SelectedItem--;
            PlayGameSound(&G_SFXMenuNavigate);
        }
        else
        {
            GM_ExitYesNoScreen.SelectedItem = GM_ExitYesNoScreen.ItemCount - 1;
            PlayGameSound(&G_SFXMenuNavigate);
        }
    }
}

void inline DrawExitYesNoScreen(void)
{
    static uint64_t LocalFrameCounter;
    static uint64_t LastFrameSeen;
    if (G_PerformanceData.TotalFramesRendered > (LastFrameSeen + 1))
    {
        LocalFrameCounter = 0;
        GM_ExitYesNoScreen.SelectedItem = 1;
    }

    __stosd(G_BackBuffer.Memory, 0x00000000, G_CanvasMemorySize / sizeof(PIXEL32));
    BlitStringToBuffer(&G_6x7Font, (G_GameWidth - ((uint32_t)strlen("Are you sure you want to exit?") * 6)) / 2, 130, "Are you sure you want to exit?", 255, 255, 255, 255);

    for (uint8_t i = 0; i < GM_ExitYesNoScreen.ItemCount; i++)
    {
        BlitStringToBuffer(&G_6x7Font, GM_ExitYesNoScreen.Items[i]->X, GM_ExitYesNoScreen.Items[i]->Y, GM_ExitYesNoScreen.Items[i]->Name, 255, 255, 255, 255);
    }
    BlitStringToBuffer(&G_6x7Font, GM_ExitYesNoScreen.Items[GM_ExitYesNoScreen.SelectedItem]->X - 6, GM_ExitYesNoScreen.Items[GM_ExitYesNoScreen.SelectedItem]->Y, ">", 255, 255, 255, 255);

    LocalFrameCounter++;
    LastFrameSeen = G_PerformanceData.TotalFramesRendered;
}