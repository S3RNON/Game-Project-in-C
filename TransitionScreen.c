#include "main.h"
#pragma warning(disable: 26451)

void inline DrawTransitionScreen(void)
{
    static uint64_t LocalFrameCounter;
    static uint64_t LastFrameSeen;
    static BOOL TransOutFinished[3] = { FALSE, FALSE, FALSE };
    static int32_t StartingScreenPixel = 0;
    uint8_t DecreasingNumber = (uint8_t)floor(255 / (60 * G_TransitionSeconds));
    uint8_t StoppingNumber = 255 % DecreasingNumber;
    if (G_PerformanceData.TotalFramesRendered > (LastFrameSeen + 1))
    {
        LocalFrameCounter = 0;
        TransOutFinished[0] = FALSE;
        TransOutFinished[1] = FALSE;
        TransOutFinished[2] = FALSE;
    }

    PIXEL32 BackgroundPixel = { 0 };
    for (int XPixel = 0; XPixel < G_BackBuffer.BitmapInfo.bmiHeader.biWidth; XPixel++)
    {
        for (int YPixel = 0; YPixel < G_BackBuffer.BitmapInfo.bmiHeader.biHeight; YPixel++)
        {
            switch (G_TransitionType)
            {
                case TRANSITION_FADE:
                {
                    memcpy_s(&BackgroundPixel, sizeof(PIXEL32), (PIXEL32*)G_BackBuffer.Memory + G_GameWidth * (G_GameHeight - YPixel - 1) + XPixel, sizeof(PIXEL32));
                    if (BackgroundPixel.Red > StoppingNumber)
                    {
                        BackgroundPixel.Red -= DecreasingNumber;
                    }
                    else TransOutFinished[0] = TRUE;
                    if (BackgroundPixel.Green > StoppingNumber)
                    {
                        BackgroundPixel.Green -= DecreasingNumber;
                    }
                    else TransOutFinished[1] = TRUE;
                    if (BackgroundPixel.Blue > StoppingNumber)
                    {
                        BackgroundPixel.Blue -= DecreasingNumber;
                    }
                    else TransOutFinished[2] = TRUE;
                    memcpy_s((PIXEL32*)G_BackBuffer.Memory + G_GameWidth * (G_GameHeight - YPixel - 1) + XPixel, sizeof(PIXEL32), &BackgroundPixel, sizeof(PIXEL32));
                    break;
                }
            }
        }
    }


    LocalFrameCounter++;
    LastFrameSeen = G_PerformanceData.TotalFramesRendered;

    if (TransOutFinished[0] == TRUE && TransOutFinished[1] == TRUE && TransOutFinished[2] == TRUE && LocalFrameCounter == (uint64_t)floor(255 / DecreasingNumber))
    {
        G_CurrentGameState = G_DesiredGameState;
    }
}