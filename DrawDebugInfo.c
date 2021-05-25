#include "main.h"

void inline DrawDebugInfo(void)
{
    char DebugTextBuffer[128] = { 0 };
    sprintf_s(DebugTextBuffer, sizeof(DebugTextBuffer), "FPS Raw:        %.01f", G_PerformanceData.RawFPSAverage);
    BlitStringToBuffer(&G_6x7Font, 2, 1, DebugTextBuffer, 255, 255, 255, 255);
    sprintf_s(DebugTextBuffer, sizeof(DebugTextBuffer), "FPS Cooked:     %.01f", G_PerformanceData.CookedFPSAverage);
    BlitStringToBuffer(&G_6x7Font, 2, 9, DebugTextBuffer, 255, 255, 255, 255);
    sprintf_s(DebugTextBuffer, sizeof(DebugTextBuffer), "Min. Timer Res: %.02f ms", G_PerformanceData.MinimumTimerResolution / 10000.0f);
    BlitStringToBuffer(&G_6x7Font, 2, 17, DebugTextBuffer, 255, 255, 255, 255);
    sprintf_s(DebugTextBuffer, sizeof(DebugTextBuffer), "Max. Timer Res: %.02f  ms", G_PerformanceData.MaximumTimerResolution / 10000.0f);
    BlitStringToBuffer(&G_6x7Font, 2, 25, DebugTextBuffer, 255, 255, 255, 255);
    sprintf_s(DebugTextBuffer, sizeof(DebugTextBuffer), "Cur. Timer Res: %.02f ms", G_PerformanceData.CurrentTimerResolution / 10000.0f);
    BlitStringToBuffer(&G_6x7Font, 2, 33, DebugTextBuffer, 255, 255, 255, 255);
    sprintf_s(DebugTextBuffer, sizeof(DebugTextBuffer), "Handles:        %lu", G_PerformanceData.HandleCount);
    BlitStringToBuffer(&G_6x7Font, 2, 41, DebugTextBuffer, 255, 255, 255, 255);
    sprintf_s(DebugTextBuffer, sizeof(DebugTextBuffer), "Memory:         %llu KB", G_PerformanceData.MemInfo.PrivateUsage / 1024);
    BlitStringToBuffer(&G_6x7Font, 2, 49, DebugTextBuffer, 255, 255, 255, 255);
    sprintf_s(DebugTextBuffer, sizeof(DebugTextBuffer), "CPU:            %.05f%%", G_PerformanceData.CPUPercent);
    BlitStringToBuffer(&G_6x7Font, 2, 57, DebugTextBuffer, 255, 255, 255, 255);
    sprintf_s(DebugTextBuffer, sizeof(DebugTextBuffer), "%lu x %lu px", G_PerformanceData.MonitorWidth, G_PerformanceData.MonitorHeight);
    BlitStringToBuffer(&G_6x7Font, 2, 65, DebugTextBuffer, 255, 255, 255, 255);
    sprintf_s(DebugTextBuffer, sizeof(DebugTextBuffer), "%f x %f", G_Player.WorldPosX, G_Player.WorldPosY);
    BlitStringToBuffer(&G_6x7Font, 2, 73, DebugTextBuffer, 255, 255, 255, 255);
}