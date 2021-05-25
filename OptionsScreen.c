#include "main.h"
#include "menus.h"

void inline PPI_OptionsScreen(void)
{
    if (G_GameInput.EnterKeyIsDown && !G_GameInput.EnterKeyWasDown)
    {
        GM_OptionsScreen.Items[GM_OptionsScreen.SelectedItem]->Action();
        PlayGameSound(&G_SFXMenuEnter);
    }

    if (G_GameInput.DownKeyIsDown && !G_GameInput.DownKeyWasDown)
    {
        if (GM_OptionsScreen.SelectedItem < GM_OptionsScreen.ItemCount - 1)
        {
            GM_OptionsScreen.SelectedItem++;
        }
        else
        {
            GM_OptionsScreen.SelectedItem = 0;
        }
        PlayGameSound(&G_SFXMenuNavigate);
    }

    if (G_GameInput.UpKeyIsDown && !G_GameInput.UpKeyWasDown)
    {
        if (GM_OptionsScreen.SelectedItem > 0)
        {
            GM_OptionsScreen.SelectedItem--;
        }
        else
        {
            GM_OptionsScreen.SelectedItem = GM_OptionsScreen.ItemCount - 1;
        }
        PlayGameSound(&G_SFXMenuNavigate);
    }
}

void inline DrawOptionsScreen(void)
{
    static uint64_t LocalFrameCounter;
    static uint64_t LastFrameSeen;
    char TextBuffer[128] = { 0 };

    if (G_PerformanceData.TotalFramesRendered > (LastFrameSeen + 1))
    {
        LocalFrameCounter = 0;
        GM_OptionsScreen.SelectedItem = 0;
    }
    __stosd(G_BackBuffer.Memory, 0x00000000, G_CanvasMemorySize / sizeof(PIXEL32));

    sprintf_s(TextBuffer, sizeof(TextBuffer), "SFX Volume: %.0lf%%", (double)G_SFXVolume * 100.0f);
    BlitStringToBuffer(&G_6x7Font, GM_OptionsScreen.Items[0]->X, GM_OptionsScreen.Items[0]->Y, TextBuffer, 255, 255, 255, 255);
    sprintf_s(TextBuffer, sizeof(TextBuffer), "BGM Volume: %.0lf%%", (double)G_BGMVolume * 100.0f);
    BlitStringToBuffer(&G_6x7Font, GM_OptionsScreen.Items[1]->X, GM_OptionsScreen.Items[1]->Y, TextBuffer, 255, 255, 255, 255);
    sprintf_s(TextBuffer, sizeof(TextBuffer), "Resolution: %lu / %lu", G_ResWidth, G_ResHeight);
    BlitStringToBuffer(&G_6x7Font, GM_OptionsScreen.Items[2]->X, GM_OptionsScreen.Items[2]->Y, TextBuffer, 255, 255, 255, 255);
    BlitStringToBuffer(&G_6x7Font, GM_OptionsScreen.Items[GM_OptionsScreen.ItemCount - 1]->X, GM_OptionsScreen.Items[GM_OptionsScreen.ItemCount - 1]->Y, GM_OptionsScreen.Items[GM_OptionsScreen.ItemCount - 1]->Name, 255, 255, 255, 255);
    BlitStringToBuffer(&G_6x7Font, GM_OptionsScreen.Items[GM_OptionsScreen.SelectedItem]->X - 6, GM_OptionsScreen.Items[GM_OptionsScreen.SelectedItem]->Y, ">", 255, 255, 255, 255);

    LocalFrameCounter++;
    LastFrameSeen = G_PerformanceData.TotalFramesRendered;
}