#include "main.h"
#include "menus.h"

int G_FontCharacterPixelOffset[] = {
    //  .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..
        65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,
    //      !  "  #  $  %  &  '  (  )  *  +  ,  -  .  /  0  1  2  3  4  5  6  7  8  9  :  ;  <  =  >  ?
        65,64,87,68,65,69,65,86,72,73,71,81,63,83,62,78,52,53,54,55,56,57,58,59,60,61,66,67,90,82,89,65,
    //  @  A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y  Z  [  \  ]  ^  _
        65,0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,38,13,14,15,16,17,18,19,20,21,22,23,24,74,79,75,70,83,
    //  `  a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z  {  |  }  ~  ..
        85,25,26,27,28,29,30,31,32,33,34,35,36,37,39,40,41,42,43,44,45,46,47,48,49,50,51,76,79,77,80,65,
    //  .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..
        65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,
    //  .. .. .. .. .. .. .. .. .. .. .. «  .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. »  .. .. .. ..
        65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,
    //  .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..
        65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,
    //  .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. F2 .. .. .. .. .. .. .. .. .. .. .. .. ..
        65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,88,65,65,65,65,65,65,65,65,65,65,65,65,65
};

HWND           G_GameWindow;
BOOL           G_GameIsRunning;
BOOL           G_WindowHasFocus;
GAMEBITMAP     G_Error;
REGISTRYPARAMS G_RegistryParameters;
IXAudio2*               G_XAudio;
IXAudio2MasteringVoice* G_XAudioMasteringVoice;
IXAudio2SourceVoice*    G_XAudioSFXSourceVoice[NUMBER_SFX_SRC];
IXAudio2SourceVoice*    G_XAudioBGMSourceVoice;

int _stdcall WinMain(_In_ HINSTANCE Instance, _In_opt_ HINSTANCE PrevInstance, _In_ PSTR CmdLine, _In_ INT CmdShow)
{
    UNREFERENCED_PARAMETER(Instance);
    UNREFERENCED_PARAMETER(PrevInstance);
    UNREFERENCED_PARAMETER(CmdLine);
    UNREFERENCED_PARAMETER(CmdShow);
    srand((unsigned int)time(0));

    MSG Message = { 0 };
    int64_t  FrameStart = 0;
    int64_t  FrameEnd = 0;
    int64_t  ElapsedMS = 0;
    int64_t  FPS_msAccumulatorRaw = 0;
    int64_t  FPS_msAccumulatorCooked = 0;
    HMODULE  NtDllModuleHandle = NULL;
    FILETIME ProcessCreationTime = { 0 };
    FILETIME ProcessExitTime = { 0 };
    int64_t  CurrentUserCPUTime = 0;
    int64_t  CurrentKernelCPUTime = 0;
    int64_t  PreviousUserCPUTime = 0;
    int64_t  PreviousKernelCPUTime = 0;
    HANDLE   ProcessHandle = GetCurrentProcess();

    G_GameplayIsInProgress = TRUE;
    G_CurrentGameState = GAMESTATE_OPENINGSPLASHSCREEN;
    G_XinputID = -1;

    if (LoadRegistryParameters() != ERROR_SUCCESS)
    {
        goto Exit;
    }

    LogMessageA(LOG_LEVEL_ERROR, "[%s] Error.", __FUNCTION__);
    LogMessageA(LOG_LEVEL_WARNING, "[%s] Warning.", __FUNCTION__);
    LogMessageA(LOG_LEVEL_INFO, "[%s] Information.", __FUNCTION__);
    LogMessageA(LOG_LEVEL_DEBUG, "[%s] Debug.", __FUNCTION__);

    if ((NtDllModuleHandle = GetModuleHandleA("ntdll.dll")) == NULL)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Couldn't load ntdll.dll! Error 0x%08lx!", __FUNCTION__, GetLastError());
        MessageBoxA(NULL, "Couldn't load ntdll.dll!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }

    if ((NtQueryTimerResolution = (_NtQueryTimerResolution)GetProcAddress(NtDllModuleHandle, "NtQueryTimerResolution")) == NULL)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Couldn't find the NtQueryTimerResolution function in ntdll.dll! GetProcAddress failed! Error 0x%08lx!", __FUNCTION__, GetLastError());
        MessageBoxA(NULL, "Couldn't find the NtQueryTimerResolution function in ntdll.dll!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;

    }
    NtQueryTimerResolution(&G_PerformanceData.MinimumTimerResolution, &G_PerformanceData.MaximumTimerResolution, &G_PerformanceData.CurrentTimerResolution);
    GetSystemInfo(&G_PerformanceData.SystemInfo);
    GetSystemTimeAsFileTime((FILETIME*)&G_PerformanceData.PreviousSystemTime);
    if (GameIsAlreadyRunning() == TRUE)
    {
        LogMessageA(LOG_LEVEL_WARNING, "[%s] Another instance of this programm is already running!", __FUNCTION__);
        MessageBoxA(NULL, "Another instance of this programm is already running!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    if (timeBeginPeriod(1) == TIMERR_NOCANDO)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to set global timer resolution!", __FUNCTION__);
        MessageBoxA(NULL, "Failed to set global timer resolution!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    if (SetPriorityClass(ProcessHandle, HIGH_PRIORITY_CLASS) == 0)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to set process priority! Error 0x%08lx!", __FUNCTION__, GetLastError());
        MessageBoxA(NULL, "Failed to set process priority!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }

    if (SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST) == 0)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to set thread priority! Error 0x%08lx!", __FUNCTION__, GetLastError());
        MessageBoxA(NULL, "Failed to set thread priority!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }

    if (CreateMyWindow() != ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] CreateMyWindow failed!", __FUNCTION__);
        goto Exit;
    }
    int SwitchBuffer = 0;
    SwitchBuffer = (int)(((float)G_PerformanceData.MonitorWidth / (float)G_PerformanceData.MonitorHeight) * (float)1000);
    switch (SwitchBuffer)
    {
        case (1777):    // 16/9   (*1000)
        {
            G_GameWidth = 1024;
            G_GameHeight = 576;
            G_Chooser = 2;
            break;
        }
        case (2370):    // 21/9   (*1000)
        {
            G_GameWidth = 1344;
            G_GameHeight = 576;
            G_Chooser = 0;
            break;
        }
        case (1600):    // 16/10  (*1000)
        {
            G_GameWidth = 1024;
            G_GameHeight = 640;
            G_Chooser = 1;
        }
        default:       // I set the default to be 16/9;
        {
            G_GameWidth = 1024;
            G_GameHeight = 576;
            break;
        }
    }
    G_CanvasMemorySize = G_GameWidth * G_GameHeight * RES_BPP / 8;

    InitializeMenus();

    if (Load32BppBitmapFromFile(".\\Assets\\Fonts\\6x7font.bmp", &G_6x7Font) != ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to load 6x7font.bmp!", __FUNCTION__);
        MessageBoxA(NULL, "Failed to load 6x7font.bmp!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    if (Load32BppBitmapFromFile(".\\Assets\\MonsterBitmaps\\vogel.bmp", &G_Vogel) != ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to load vogel.bmp!", __FUNCTION__);
        MessageBoxA(NULL, "Failed to load vogel.bmp!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    if (Load32BppBitmapFromFile(".\\Assets\\ProjectileBitmaps\\fireball.bmp", &G_FireBallbmp) != ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to load fireball.bmp!", __FUNCTION__);
        MessageBoxA(NULL, "Failed to load fireball.bmp!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    if (Load32BppBitmapFromFile(".\\Assets\\ExtraBitmaps\\question.bmp", &G_Question) != ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to load question.bmp!", __FUNCTION__);
        MessageBoxA(NULL, "Failed to load question.bmp!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    if (Load32BppBitmapFromFile(".\\Assets\\World\\WorldBitmaps\\worldtileset.bmp", &G_WorldTileSet) != ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to load worldtileset.bmp!", __FUNCTION__);
        MessageBoxA(NULL, "Failed to load worldtileset.bmp!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    if (InitializeSoundEngine() != S_OK)
    {
        MessageBoxA(NULL, "Failed to initialize sound engine!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    if (LoadWaveFromFile(".\\Assets\\SFX\\Menu_Select.wav", &G_SFXMenuNavigate) != ERROR_SUCCESS)
    {
        MessageBoxA(NULL, "Failed to load .wav from file!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }
    if (LoadWaveFromFile(".\\Assets\\SFX\\Menu_Enter.wav", &G_SFXMenuEnter) != ERROR_SUCCESS)
    {
        MessageBoxA(NULL, "Failed to load .wav from file!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }
    if (LoadWaveFromFile(".\\Assets\\SFX\\EnterTitleScreen.wav", &G_SFXEnterTitleScreen) != ERROR_SUCCESS)
    {
        MessageBoxA(NULL, "Failed to load .wav from file!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }
    if (LoadWaveFromFile(".\\Assets\\SFX\\splashintrodrop.wav", &G_SFXSplashIntroDrop) != ERROR_SUCCESS)
    {
        MessageBoxA(NULL, "Failed to load .wav from file!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }
    if (LoadWaveFromFile(".\\Assets\\SFX\\FireBall.wav", &G_SFXFireBallShoot) != ERROR_SUCCESS)
    {
        MessageBoxA(NULL, "Failed to load .wav from file!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }
    if (LoadWaveFromFile(".\\Assets\\SFX\\playerhurt.wav", &G_SFXPlayerHurt) != ERROR_SUCCESS)
    {
        MessageBoxA(NULL, "Failed to load .wav from file!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    if (LoadWaveFromFile(".\\Assets\\SFX\\step1.wav", &G_SFXSteps[0]) != ERROR_SUCCESS)
    {
        MessageBoxA(NULL, "Failed to load .wav from file!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }
    if (LoadWaveFromFile(".\\Assets\\SFX\\step2.wav", &G_SFXSteps[1]) != ERROR_SUCCESS)
    {
        MessageBoxA(NULL, "Failed to load .wav from file!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }
    if (LoadWaveFromFile(".\\Assets\\SFX\\step3.wav", &G_SFXSteps[2]) != ERROR_SUCCESS)
    {
        MessageBoxA(NULL, "Failed to load .wav from file!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    if ((QueryPerformanceFrequency((LARGE_INTEGER*)&G_PerformanceData.PerfFrequency)) == 0)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to set timer frequency! Error 0x%08lx!", __FUNCTION__, GetLastError());
        MessageBoxA(NULL, "Failed to set timer frequency!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    
    G_PerformanceData.DisplayDebugInfo = FALSE;
    G_BackBuffer.BitmapInfo.bmiHeader.biSize        = sizeof(G_BackBuffer.BitmapInfo.bmiHeader);
    G_BackBuffer.BitmapInfo.bmiHeader.biWidth       = G_GameWidth;
    G_BackBuffer.BitmapInfo.bmiHeader.biHeight      = G_GameHeight;
    G_BackBuffer.BitmapInfo.bmiHeader.biBitCount    = RES_BPP;
    G_BackBuffer.BitmapInfo.bmiHeader.biCompression = BI_RGB;
    G_BackBuffer.BitmapInfo.bmiHeader.biPlanes      = 1;
    if ((G_BackBuffer.Memory = VirtualAlloc(NULL, G_CanvasMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)) == NULL)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed To Allocate Memory For Canvas! Error 0x%08lx!", __FUNCTION__, GetLastError());
        MessageBoxA(NULL, "Failed To Allocate Memory For Canvas!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }

    if (InitializePlayer(&G_Player) != ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to initialize MainPlayer!", __FUNCTION__);
        MessageBoxA(NULL, "Failed to initialize MainPlayer!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }

    SpawnEnemies(10, 400, 400, 100);
    SpawnEntity(ETYPE_QUESTION, 200, 70, 0, FALSE);

    G_GameIsRunning = TRUE;
    while (G_GameIsRunning)
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&FrameStart);
        while (PeekMessageA(&Message, G_GameWindow, 0, 0, PM_REMOVE))
        {
            DispatchMessageA(&Message);
        }
        ProcessPlayerInput();
        RenderFrameGraphics();
        QueryPerformanceCounter((LARGE_INTEGER*)&FrameEnd);
        ElapsedMS  = FrameEnd - FrameStart;
        ElapsedMS *= 1000000;
        ElapsedMS /= G_PerformanceData.PerfFrequency;

        FPS_msAccumulatorRaw += ElapsedMS;
        G_PerformanceData.TotalFramesRendered++;


        while (ElapsedMS < TARGET_MCS_FRAME)
        {
            ElapsedMS = FrameEnd - FrameStart;
            ElapsedMS *= 1000000;
            ElapsedMS /= G_PerformanceData.PerfFrequency;
            QueryPerformanceCounter((LARGE_INTEGER*)&FrameEnd);
            if (ElapsedMS < (TARGET_MCS_FRAME * 0.75f))
            {
                Sleep(1);
            }
        }

        FPS_msAccumulatorCooked += ElapsedMS;

        if (G_PerformanceData.TotalFramesRendered % CALC_AVG_FPS == 0)
        {
            GetSystemTimeAsFileTime((FILETIME*)&G_PerformanceData.CurrentSystemTime);

            FindFirstConnectedGamepad();

            GetProcessTimes(ProcessHandle,
                &ProcessCreationTime,
                &ProcessExitTime,
                (FILETIME*)&CurrentKernelCPUTime,
                (FILETIME*)&CurrentUserCPUTime);
            G_PerformanceData.CPUPercent = (double)(CurrentKernelCPUTime - PreviousKernelCPUTime + CurrentUserCPUTime - PreviousUserCPUTime);
            G_PerformanceData.CPUPercent /= (G_PerformanceData.CurrentSystemTime - G_PerformanceData.PreviousSystemTime);
            G_PerformanceData.CPUPercent /= G_PerformanceData.SystemInfo.dwNumberOfProcessors;
            G_PerformanceData.CPUPercent *= 100;
            
            GetProcessHandleCount(ProcessHandle, &G_PerformanceData.HandleCount);
            K32GetProcessMemoryInfo(ProcessHandle, (PROCESS_MEMORY_COUNTERS*)&G_PerformanceData.MemInfo, sizeof(G_PerformanceData.MemInfo));

            G_PerformanceData.RawFPSAverage    = 1.0f / (((float)FPS_msAccumulatorRaw / CALC_AVG_FPS) * 0.000001f);
            G_PerformanceData.CookedFPSAverage = 1.0f / (((float)FPS_msAccumulatorCooked / CALC_AVG_FPS) * 0.000001f);
            FPS_msAccumulatorRaw    = 0;
            FPS_msAccumulatorCooked = 0;
            PreviousKernelCPUTime = CurrentKernelCPUTime;
            PreviousUserCPUTime   = CurrentUserCPUTime;
            G_PerformanceData.PreviousSystemTime    = G_PerformanceData.CurrentSystemTime;
        }
    }

Exit:
    return 0;
}

LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM WParam, _In_ LPARAM LParam)
{
    LRESULT Result = 0;

    switch (Message)
    {
        case WM_CLOSE:
        {
            G_GameIsRunning = FALSE;
            PostQuitMessage(0);
            break;
        }
        case WM_ACTIVATE:
        {
            if (WParam == 0)
            {
                G_WindowHasFocus = FALSE;
            }
            else
            {
                ShowCursor(FALSE);
                G_WindowHasFocus = TRUE;
            }
            break;
        }
        default:
            Result = DefWindowProcA(WindowHandle, Message, WParam, LParam);
    }
    return Result;
}

DWORD CreateMyWindow(void) {
    DWORD Result = ERROR_SUCCESS;

    WNDCLASSEXA WindowClass = { 0 };

    WindowClass.cbSize          = sizeof(WNDCLASSEXA);
    WindowClass.style           = 0;
    WindowClass.lpfnWndProc     = MainWindowProc;
    WindowClass.cbClsExtra      = 0;
    WindowClass.cbWndExtra      = 0;
    WindowClass.hInstance       = GetModuleHandleA(NULL);
    WindowClass.hIcon           = LoadIconA(NULL, IDI_APPLICATION);
    WindowClass.hIconSm         = LoadIconA(NULL, IDI_APPLICATION);
    WindowClass.hCursor         = LoadCursorA(NULL, IDC_ARROW);
    WindowClass.hbrBackground   = CreateSolidBrush(RGB(255, 0, 255));
    WindowClass.lpszMenuName    = NULL;
    WindowClass.lpszClassName   = GAME_NAME "_WINDOWCLASS";

    if (!RegisterClassExA(&WindowClass))
    {
        Result = GetLastError();
        LogMessageA(LOG_LEVEL_ERROR, "[%s] RegisterClassA() failed! Error 0x%08lx!", __FUNCTION__, Result);
        MessageBoxA(NULL, "Failed to register window!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    G_GameWindow = CreateWindowExA(0, WindowClass.lpszClassName, GAME_NAME, WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, GetModuleHandleA(NULL), NULL);
    if (G_GameWindow == NULL)
    {
        Result = GetLastError();
        LogMessageA(LOG_LEVEL_ERROR, "[%s] CreateWindowExA() failed! Error 0x%08lx!", __FUNCTION__, Result);
        MessageBoxA(NULL, "Failed to create window!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    G_PerformanceData.MonitorInfo.cbSize = sizeof(MONITORINFO);
    if (GetMonitorInfoA(MonitorFromWindow(G_GameWindow, MONITOR_DEFAULTTOPRIMARY), &G_PerformanceData.MonitorInfo) == 0) {
        Result = ERROR_MONITOR_NO_DESCRIPTOR;
        LogMessageA(LOG_LEVEL_ERROR, "[%s] GetMonitorInfoA() failed! Error 0x%08lx!", __FUNCTION__, Result);
        MessageBoxA(NULL, "Failed to obtain monitor size!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    G_PerformanceData.MonitorWidth  = G_PerformanceData.MonitorInfo.rcMonitor.right  - G_PerformanceData.MonitorInfo.rcMonitor.left;
    G_PerformanceData.MonitorHeight = G_PerformanceData.MonitorInfo.rcMonitor.bottom - G_PerformanceData.MonitorInfo.rcMonitor.top;
    G_ResWidth = G_PerformanceData.MonitorWidth;
    G_ResHeight = G_PerformanceData.MonitorHeight;

    if (SetWindowLongPtrA(G_GameWindow, GWL_STYLE, WS_VISIBLE) == 0)
    {
        Result = GetLastError();
        LogMessageA(LOG_LEVEL_ERROR, "[%s] SetWindowLongPtrA() failed! Error 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
    }
    if (SetWindowPos(
        G_GameWindow,
        HWND_TOP,
        G_PerformanceData.MonitorInfo.rcMonitor.left,
        G_PerformanceData.MonitorInfo.rcMonitor.top,
        G_PerformanceData.MonitorWidth,
        G_PerformanceData.MonitorHeight,
        SWP_NOOWNERZORDER |SWP_FRAMECHANGED) == 0)
    {
        Result = GetLastError();
        LogMessageA(LOG_LEVEL_ERROR, "[%s] SetWindowPos() failed! Error 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
    }

Exit:
    return Result;
}

BOOL inline GameIsAlreadyRunning(void)
{
    HANDLE Mutex = NULL;
    Mutex = CreateMutexA(NULL, FALSE, GAME_NAME "_GameMutex");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

DWORD inline LoadWorldMap(LPCSTR FileName)
{
    DWORD Error = ERROR_SUCCESS;
    HANDLE FileHandle = INVALID_HANDLE_VALUE;
    WORD ReadByte = 0;
    WORD TotalByte = 0;
    DWORD NumberOfBytesRead = 0;
    uint16_t Pointer = 0;
    BOOL EnterIsFound = FALSE;
    BOOL XIsFound = FALSE;

    for (uint16_t i = 0; i < G_WorldMap.Size; i++) G_WorldMap.Tiles[i] = -1;

    if ((FileHandle = CreateFileA(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
    {
        Error = GetLastError();
        goto Exit;
    }

    while (!XIsFound)
    {
        if (SetFilePointer(FileHandle, Pointer, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        {
            Error = GetLastError();
            goto Exit;
        }

        if (ReadFile(FileHandle, &ReadByte, 1, &NumberOfBytesRead, NULL) == 0)
        {
            Error = GetLastError();
            goto Exit;
        }

        if (ReadByte == 0x78) // "x"
        {
            XIsFound = TRUE;
        }

        Pointer++;
    }

    if (XIsFound)
    {
        for (int16_t i = Pointer - 2; i >= 0; i--)
        {
            if (SetFilePointer(FileHandle, i, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
            {
                Error = GetLastError();
                goto Exit;
            }

            if (ReadFile(FileHandle, &ReadByte, 1, &NumberOfBytesRead, NULL) == 0)
            {
                Error = GetLastError();
                goto Exit;
            }

            TotalByte += (ReadByte - 48) * (WORD)pow(10, Pointer - 2 - i);
        }
    }
    G_WorldMap.Width = TotalByte;
    TotalByte = 0;
    Pointer = 0;
    while (!EnterIsFound)
    {
        if (SetFilePointer(FileHandle, Pointer, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        {
            Error = GetLastError();
            goto Exit;
        }

        if (ReadFile(FileHandle, &ReadByte, 1, &NumberOfBytesRead, NULL) == 0)
        {
            Error = GetLastError();
            goto Exit;
        }
        
        if (ReadByte == 0x0A) // ".."
        {
            EnterIsFound = TRUE;
        }

        Pointer++;
    }

    if (EnterIsFound)
    {
        for (int16_t i = Pointer - 3; i > 0; i--)
        {
            if (SetFilePointer(FileHandle, i, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
            {
                Error = GetLastError();
                goto Exit;
            }

            if (ReadFile(FileHandle, &ReadByte, 1, &NumberOfBytesRead, NULL) == 0)
            {
                Error = GetLastError();
                goto Exit;
            }
            if (ReadByte == 0x78) break;
            else TotalByte += (ReadByte - 48) * (WORD)pow(10, Pointer - 3 - i);
        }

        uint32_t Index = 0;
        int16_t BSum = 0;
        G_WorldMap.Height = TotalByte;
        G_WorldMap.Size = G_WorldMap.Height * G_WorldMap.Width;

        for (uint32_t i = 0; Index < G_WorldMap.Size && i < 102400; i++)
        {
            if (SetFilePointer(FileHandle, i + Pointer, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
            {
                Error = GetLastError();
                goto Exit;
            }

            if (ReadFile(FileHandle, &ReadByte, 1, &NumberOfBytesRead, NULL) == 0)
            {
                Error = GetLastError();
                goto Exit;
            }

            switch (ReadByte)
            {
                case 0x5d:
                {
                    BSum = 0;
                    for (uint16_t j = 1; j < 10; j++)
                    {
                        if (SetFilePointer(FileHandle, i + Pointer - j, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
                        {
                            Error = GetLastError();
                            goto Exit;
                        }

                        if (ReadFile(FileHandle, &ReadByte, 1, &NumberOfBytesRead, NULL) == 0)
                        {
                            Error = GetLastError();
                            goto Exit;
                        }

                        if (ReadByte == 0x5b) break;
                        else
                        {
                            BSum += (int16_t)pow(10, j - 1) * (ReadByte - 0x30);                            
                        }
                    }
                    G_WorldMap.Tiles[Index] = BSum;
                    if (Index < G_WorldMap.Size) Index++;
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }

Exit:

    if (FileHandle && (FileHandle != INVALID_HANDLE_VALUE))
    {
        CloseHandle(FileHandle);
    }

    if (Error == ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_INFO, "[%s] Loading successful: %s", __FUNCTION__, ".\\Assets\\worldmap.txt");
    }
    else
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Loading failed: %s!", __FUNCTION__, ".\\Assets\\worldmap.txt");
    }
    return Error;
}

void inline ProcessPlayerInput(void)
{
    if (G_WindowHasFocus == FALSE)
    {
        return;
    }

    G_GameInput.EnterKeyIsDown = GetAsyncKeyState(VK_RETURN);
    G_GameInput.EscapeKeyIsDown = GetAsyncKeyState(VK_ESCAPE);
    G_GameInput.DebugKeyIsDown = GetAsyncKeyState(VK_F1);
    G_GameInput.LeftKeyIsDown = GetAsyncKeyState('A');
    G_GameInput.DownKeyIsDown = GetAsyncKeyState('S');
    G_GameInput.RightKeyIsDown = GetAsyncKeyState('D');
    G_GameInput.UpKeyIsDown = GetAsyncKeyState('W');
    G_GameInput.SpeedKeyIsDown = GetAsyncKeyState(VK_SHIFT);
    G_GameInput.AttackKeyIsDown = GetAsyncKeyState('F');
    G_GameInput.InventoryKeyIsDown = GetAsyncKeyState('I');

    if (G_XinputID >= 0)
    {
        if (XInputGetState(G_XinputID, &G_XinputState) == ERROR_SUCCESS)
        {
            if (G_XinputState.Gamepad.sThumbLX > 16384)
            {
                G_GameInput.RightKeyIsDown = TRUE;
            }
            if (G_XinputState.Gamepad.sThumbLX < -16384)
            {
                G_GameInput.LeftKeyIsDown = TRUE;
            }
            if (G_XinputState.Gamepad.sThumbLY > 16384)
            {
                G_GameInput.UpKeyIsDown = TRUE;
            }
            if (G_XinputState.Gamepad.sThumbLY < -16384)
            {
                G_GameInput.DownKeyIsDown = TRUE;
            }
            G_GameInput.EscapeKeyIsDown    |= G_XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_START;
            G_GameInput.DebugKeyIsDown     |= G_XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
            G_GameInput.EnterKeyIsDown     |= G_XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_A;
            G_GameInput.SpeedKeyIsDown     |= G_XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
            G_GameInput.AttackKeyIsDown    |= G_XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_X;
            G_GameInput.InventoryKeyIsDown |= G_XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
        }
        else
        {
            G_XinputID = -1;
            G_PreviousGameState = G_CurrentGameState;
            G_CurrentGameState  = GAMESTATE_GAMEPADUNPLUGGED;
        }
    }

    switch (G_CurrentGameState)
    {
        case GAMESTATE_NONE:
        {
            break;
        }
        case GAMESTATE_OPENINGSPLASHSCREEN:
        {
            PPI_OpeningSplashScreen();
            break;
        }
        case GAMESTATE_GAMEPADUNPLUGGED:
        {
            PPI_GamepadUnplugged();
            break;
        }
        case GAMESTATE_TITLESCREEN:
        {
            PPI_TitleScreen();
            break;
        }
        case GAMESTATE_OVERWORLD:
        {
            PPI_OverWorld();
            break;
        }
        case GAMESTATE_OPTIONSSCREEN:
        {
            PPI_OptionsScreen();
            break;
        }
        case GAMESTATE_EXITYESNOSCREEN:
        {
            PPI_ExitYesNoScreen();
            break;
        }
        case GAMESTATE_TRANSITION:
        {
            break;
        }
        case GAMESTATE_DEATH:
        {
            PPI_DeathScreen();
            break;
        }
        case GAMESTATE_TEXTBOX:
        {
            PPI_TextBoxScreen();
            break;
        }
        default:
        {
            ASSERT(FALSE, "Unknown game state!");
            break;
        }
    }

    G_GameInput.EnterKeyWasDown     = G_GameInput.EnterKeyIsDown;
    G_GameInput.EscapeKeyWasDown    = G_GameInput.EscapeKeyIsDown;
    G_GameInput.DebugKeyWasDown     = G_GameInput.DebugKeyIsDown;
    G_GameInput.LeftKeyWasDown      = G_GameInput.LeftKeyIsDown;
    G_GameInput.DownKeyWasDown      = G_GameInput.DownKeyIsDown;
    G_GameInput.RightKeyWasDown     = G_GameInput.RightKeyIsDown;
    G_GameInput.UpKeyWasDown        = G_GameInput.UpKeyIsDown;
    G_GameInput.SpeedKeyWasDown     = G_GameInput.SpeedKeyIsDown;
    G_GameInput.AttackKeyWasDown    = G_GameInput.AttackKeyIsDown;
    G_GameInput.InventoryKeyWasDown = G_GameInput.InventoryKeyIsDown;
}

void inline TransitionScreen(_In_ GAMESTATE DesiredGameState, _In_ TRANSITION TransitionType, _In_ uint16_t TransitionsSeconds)
{
    if (TransitionsSeconds < 5 && TransitionsSeconds > 0)
    {
        G_TransitionType = TransitionType;
        G_PreviousGameState = G_CurrentGameState;
        G_TransitionSeconds = TransitionsSeconds;
        G_DesiredGameState = DesiredGameState;
        G_CurrentGameState = GAMESTATE_TRANSITION;
    }
}

void RenderFrameGraphics(void)
{
    switch (G_CurrentGameState)
    {
        case GAMESTATE_NONE :
        {
            break;
        }
        case GAMESTATE_GAMEPADUNPLUGGED:
        {
            DrawGamepadUnpluggedScreen();
            break;
        }
        case GAMESTATE_OPENINGSPLASHSCREEN:
        {
            DrawOpeningSplashScreen();
            break;
        }
        case GAMESTATE_TITLESCREEN:
        {
            DrawTitleScreen();
            break;
        }
        case GAMESTATE_OVERWORLD:
        {
            DrawOverWorld();
            break;
        }
        case GAMESTATE_OPTIONSSCREEN:
        {
            DrawOptionsScreen();
            break;
        }
        case GAMESTATE_EXITYESNOSCREEN:
        {
            DrawExitYesNoScreen();
            break;
        }
        case GAMESTATE_TRANSITION:
        {
            DrawTransitionScreen();
            break;
        }
        case GAMESTATE_DEATH:
        {
            DrawDeathScreen();
            break;
        }
        case GAMESTATE_TEXTBOX:
        {
            DrawTextBoxScreen();
            break;
        }
        default:
        {
            ASSERT(FALSE, "Gamestate not implemented");
            break;
        }
    }

    if (G_PerformanceData.DisplayDebugInfo == TRUE)
    {
        DrawDebugInfo();
    }

    HDC DeviceContext = GetDC(G_GameWindow);
    StretchDIBits(DeviceContext, 0, 0, G_PerformanceData.MonitorWidth, G_PerformanceData.MonitorHeight, 0, 0, G_GameWidth, G_GameHeight, G_BackBuffer.Memory, &G_BackBuffer.BitmapInfo, DIB_RGB_COLORS, SRCCOPY);
    ReleaseDC(G_GameWindow, DeviceContext);
}

DWORD Load32BppBitmapFromFile(_In_ char* FileName, _Inout_ GAMEBITMAP* GameBitmap)
{
    DWORD Error = ERROR_SUCCESS;
    HANDLE FileHandle = INVALID_HANDLE_VALUE;
    WORD BitmapHeader = 0;
    DWORD PixelDataOffset = 0;
    DWORD NumberOfBytesRead = 2;

    if ((FileHandle = CreateFileA(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
    {
        Error = GetLastError();
        goto Exit;
    }

    if (ReadFile(FileHandle, &BitmapHeader, 2, &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        goto Exit;
    }

    if (BitmapHeader != 0x4d42) // "BM" backwards
    {
        Error = ERROR_FILE_INVALID;
        goto Exit;
    }

    if (SetFilePointer(FileHandle, 0xA, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        Error = GetLastError();
        goto Exit;
    }

    if (ReadFile(FileHandle, &PixelDataOffset, sizeof(DWORD), &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        goto Exit;
    }

    if (SetFilePointer(FileHandle, 0xE, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        Error = GetLastError();
        goto Exit;
    }

    if (ReadFile(FileHandle, &GameBitmap->BitmapInfo.bmiHeader, sizeof(BITMAPINFOHEADER), &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        goto Exit;
    }

    if ((GameBitmap->Memory = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, GameBitmap->BitmapInfo.bmiHeader.biSizeImage)) == NULL)
    {
        Error = ERROR_NOT_ENOUGH_MEMORY;
        goto Exit;
    }

    if (SetFilePointer(FileHandle, PixelDataOffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        Error = GetLastError();
        goto Exit;
    }

    if (ReadFile(FileHandle, GameBitmap->Memory, GameBitmap->BitmapInfo.bmiHeader.biSizeImage, &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        goto Exit;
    }

Exit:

    if (FileHandle && (FileHandle != INVALID_HANDLE_VALUE))
    {
        CloseHandle(FileHandle);
    }

    if (Error == ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_INFO, "[%s] Loading successful: %s", __FUNCTION__, FileName);
    }
    else 
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Loading failed: %s!", __FUNCTION__, FileName);
    }
    return(Error);
}

DWORD InitializePlayer(_Inout_ PLAYER* VPlayer)
{
    DWORD Error = ERROR_SUCCESS;

    VPlayer->Hurt = FALSE;
    if ((Error = Load32BppBitmapFromFile(".\\Assets\\PlayerBitmaps\\Zelda_Down.bmp", &VPlayer->Sprite[0])) != ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to load Zelda_Down.bmp!", __FUNCTION__);
        MessageBoxA(NULL, "Failed to load Zelda_Down.bmp!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile(".\\Assets\\PlayerBitmaps\\Zelda_Down_2.bmp", &VPlayer->Sprite[1])) != ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to load Zelda_Down_2.bmp!", __FUNCTION__);
        MessageBoxA(NULL, "Failed to load Zelda_Down_2.bmp!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile(".\\Assets\\PlayerBitmaps\\Zelda_Left.bmp", &VPlayer->Sprite[2])) != ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to load Zelda_Left.bmp!", __FUNCTION__);
        MessageBoxA(NULL, "Failed to load Zelda_Left.bmp!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile(".\\Assets\\PlayerBitmaps\\Zelda_Left_2.bmp", &VPlayer->Sprite[3])) != ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to load Zelda_Left_2.bmp", __FUNCTION__);
        MessageBoxA(NULL, "Failed to load Zelda_Left_2.bmp!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile(".\\Assets\\PlayerBitmaps\\Zelda_Right.bmp", &VPlayer->Sprite[4])) != ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to load Zelda_Right.bmp!", __FUNCTION__);
        MessageBoxA(NULL, "Failed to load .bmp!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile(".\\Assets\\PlayerBitmaps\\Zelda_Right_2.bmp", &VPlayer->Sprite[5])) != ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to load Zelda_Right_2.bmp!", __FUNCTION__);
        MessageBoxA(NULL, "Failed to load .bmp!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile(".\\Assets\\PlayerBitmaps\\Zelda_Up.bmp", &VPlayer->Sprite[6])) != ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to load Zelda_Up.bmp!", __FUNCTION__);
        MessageBoxA(NULL, "Failed to load .bmp!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile(".\\Assets\\PlayerBitmaps\\Zelda_Up_2.bmp", &VPlayer->Sprite[7])) != ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to load Zelda_Up_2.bmp!", __FUNCTION__);
        MessageBoxA(NULL, "Failed to load .bmp!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    VPlayer->Dir = 1;
    for (int i = 0; i < 4; i++)
    {
        VPlayer->Walking[i] = FALSE;
    }
    VPlayer->WorldPosX = (float)VPlayer->Sprite->BitmapInfo.bmiHeader.biWidth / 2;
    VPlayer->WorldPosY = (float)VPlayer->Sprite->BitmapInfo.bmiHeader.biHeight / 2;
    VPlayer->ScreenPosX = (int16_t)((G_GameWidth) / 2);
    VPlayer->ScreenPosY = (int16_t)((G_GameHeight) / 2);
    VPlayer->CurrentSpeed = 2;
    VPlayer->RunningSpeed = 4;
    VPlayer->ShowInventory = FALSE;
    VPlayer->Stamina = 100;
    VPlayer->HP = 100;
    VPlayer->MP = 100;
    VPlayer->HPRegen = 0.01f;
    VPlayer->MPRegen = 0.01f;
    VPlayer->StaminaRegen = 0.01f;

Exit:
    return Error;
}

void Blit32BppBitmapToBuffer(_In_ GAMEBITMAP* GameBitmap, _In_ int32_t x, _In_ int32_t y, _In_ int8_t UseColours, _In_ uint8_t Red, _In_ uint8_t Green, _In_ uint8_t Blue, _In_ uint8_t Alpha)
{
    int32_t StartingScreenPixel = (G_GameWidth * G_GameHeight) - G_GameWidth - (G_GameWidth * y) + x;
    int32_t StartingBitmapPixel = (GameBitmap->BitmapInfo.bmiHeader.biWidth * GameBitmap->BitmapInfo.bmiHeader.biHeight) - GameBitmap->BitmapInfo.bmiHeader.biWidth;
    int32_t MemoryOffset = 0;
    int32_t BitmapOffset = 0;
    PIXEL32 BitmapPixel = { 0 };
    PIXEL32 BackgroundPixel = { 0 };

    for (int16_t YPixel = 0; YPixel < GameBitmap->BitmapInfo.bmiHeader.biHeight; YPixel++)
    {
        for (int16_t XPixel = 0; XPixel < GameBitmap->BitmapInfo.bmiHeader.biWidth; XPixel++)
        {
            if (x + XPixel < G_GameWidth && x + XPixel >= 0 && y + YPixel < G_GameHeight && y + YPixel >= 0)
            {   
                MemoryOffset = StartingScreenPixel + XPixel - (G_GameWidth * YPixel);
                memcpy_s(&BackgroundPixel, sizeof(PIXEL32), (PIXEL32*)G_BackBuffer.Memory + MemoryOffset, sizeof(PIXEL32));
                BitmapOffset = StartingBitmapPixel + XPixel - (GameBitmap->BitmapInfo.bmiHeader.biWidth * YPixel);
                memcpy_s(&BitmapPixel, sizeof(PIXEL32), (PIXEL32*)GameBitmap->Memory + BitmapOffset, sizeof(PIXEL32));

                if (BackgroundPixel.Red != BitmapPixel.Red || BackgroundPixel.Blue != BitmapPixel.Blue || BackgroundPixel.Green != BitmapPixel.Green || BackgroundPixel.Alpha != BitmapPixel.Alpha)
                {
                    BitmapPixel.Red = (uint8_t)((float)BitmapPixel.Alpha / 255 * (float)BitmapPixel.Red + (1 - ((float)BitmapPixel.Alpha / 255)) * (float)BackgroundPixel.Red);
                    BitmapPixel.Green = (uint8_t)((float)BitmapPixel.Alpha / 255 * (float)BitmapPixel.Green + (1 - ((float)BitmapPixel.Alpha / 255)) * (float)BackgroundPixel.Green);
                    BitmapPixel.Blue = (uint8_t)((float)BitmapPixel.Alpha / 255 * (float)BitmapPixel.Blue + (1 - ((float)BitmapPixel.Alpha / 255)) * (float)BackgroundPixel.Blue);

                    switch (UseColours)
                    {
                        case 1:
                        {
                            BitmapPixel.Red = Red;
                            BitmapPixel.Green = Green;
                            BitmapPixel.Blue = Blue;
                            BitmapPixel.Alpha = Alpha;
                            break;
                        }
                        case 2:
                        {
                            if (BitmapPixel.Alpha > 0)
                            {
                                BitmapPixel.Red = Red;
                                BitmapPixel.Green = Green;
                                BitmapPixel.Blue = Blue;
                                BitmapPixel.Alpha = Alpha;
                            }
                            break;
                        }
                        default: break;

                    }

                    memcpy_s((PIXEL32*)G_BackBuffer.Memory + MemoryOffset, sizeof(PIXEL32), &BitmapPixel, sizeof(PIXEL32));
                }
            }
        }
    }
}

void LogMessageA(_In_ DWORD LogLVL, _In_ char* TextString, _In_ ...)
{
    size_t MessageLength = strlen(TextString);
    SYSTEMTIME Time = { 0 };
    HANDLE LogFileHandle = INVALID_HANDLE_VALUE;
    DWORD EndOfFile = 0;
    DWORD NumberOfBytesWritten = 0;
    char DateTimeString[96] = { 0 };
    char SeverityString[9] = { 0 };
    char FormattedString[4096] = { 0 };
    int Error = 0;
    if (G_RegistryParameters.LogLVL < LogLVL)
    {
        return;
    }

    if (MessageLength < 1 || MessageLength > 4096)
    {
        ASSERT(FALSE, "Message length was either too short or too long!");
        return;
    }

    switch (LogLVL)
    {
        case 0:
        {
            return;
        }
        case 1: // INFO
        {
            strcpy_s(SeverityString, sizeof(SeverityString), "[INFO]");
            break;
        }
        case 2: // WARN
        {
            strcpy_s(SeverityString, sizeof(SeverityString), "[WARN]");
            break;
        }
        case 3: // ERROR
        {
            strcpy_s(SeverityString, sizeof(SeverityString), "[ERROR]");
            break;
        }
        case 4: // DEBUG
        {
            strcpy_s(SeverityString, sizeof(SeverityString), "[DEBUG]");
            break;
        }
        default:
        {
            ASSERT(FALSE, "LogLVL was an unusable value!");
            break;
        }
    }
    
    GetLocalTime(&Time);
    va_list ArgPointer = NULL;
    va_start(ArgPointer, TextString);
    _vsnprintf_s(FormattedString, sizeof(FormattedString), sizeof(FormattedString), TextString, ArgPointer);
    va_end(ArgPointer);
    Error = _snprintf_s(DateTimeString, sizeof(DateTimeString), _TRUNCATE, "\r\n[%02u/%02u/%u %02u:%02u:%02u.%03u]", Time.wDay, Time.wMonth, Time.wYear, Time.wHour, Time.wMinute, Time.wSecond, Time.wMilliseconds);
    if ((LogFileHandle = CreateFileA(LOG_FILE, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
    {
        ASSERT(FALSE, "Failed to create or update the log file!");
        return;
    }
    
    EndOfFile = SetFilePointer(LogFileHandle, 0, NULL, FILE_END);
    WriteFile(LogFileHandle, DateTimeString, (DWORD)strlen(DateTimeString), &NumberOfBytesWritten, NULL);
    WriteFile(LogFileHandle, SeverityString, (DWORD)strlen(SeverityString), &NumberOfBytesWritten, NULL);
    WriteFile(LogFileHandle, FormattedString, (DWORD)strlen(FormattedString), &NumberOfBytesWritten, NULL);
    if (LogFileHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(LogFileHandle);
    }
}

void inline InitializeMenus(void)
{
    // Title Screen

    GMI_TitleScreen_ResumeGame = (MENUITEM){ "Resume", -1, -2, TRUE, MenuItem_TitleScreen_Resume };
    GMI_TitleScreen_StartNewGame = (MENUITEM){ "New Game", -1, -2, TRUE, MenuItem_TitleScreen_StartNew };
    GMI_TitleScreen_Options = (MENUITEM){ "Options", -1, -2, TRUE, MenuItem_TitleScreen_Options };
    GMI_TitleScreen_Exit = (MENUITEM){ "Exit", -1, -2, TRUE, MenuItem_TitleScreen_Exit };
    GMI_TitleScreenItems[0] = &GMI_TitleScreen_ResumeGame;
    GMI_TitleScreenItems[1] = &GMI_TitleScreen_StartNewGame;
    GMI_TitleScreenItems[2] = &GMI_TitleScreen_Options;
    GMI_TitleScreenItems[3] = &GMI_TitleScreen_Exit;
    GM_TitleScreen = (MENU){ "Title Screen Menu", 1,  _countof(GMI_TitleScreenItems), GMI_TitleScreenItems };

    // Exit Confirm Screen

    GMI_ExitConfirmScreen_Yes = (MENUITEM){ "Yes", -2, -1, TRUE, MenuItem_ExitYesNoScreen_Yes };
    GMI_ExitConfirmScreen_No = (MENUITEM){ "No", -2, -1, TRUE, MenuItem_ExitYesNoScreen_No };
    GMI_ExitConfirmScreenItems[0] = &GMI_ExitConfirmScreen_Yes;
    GMI_ExitConfirmScreenItems[1] = &GMI_ExitConfirmScreen_No;
    GM_ExitYesNoScreen = (MENU){ "Exit Confrim Screen", 1, _countof(GMI_ExitConfirmScreenItems), GMI_ExitConfirmScreenItems };

    // Options Sceen

    GMI_OptionsScreen_SFXVolume = (MENUITEM){ "SFX Volume:", -1, -2, TRUE, MenuItem_OptionsScreen_SFXVolume };
    GMI_OptionsScreen_BGMVolume = (MENUITEM){ "BGM Volume:", -1, -2, TRUE, MenuItem_OptionsScreen_BGMVolume };
    GMI_OptionsScreen_Resolution = (MENUITEM){ "Resolution:", -1, -2, TRUE, MenuItem_OptionsScreen_Resolution };
    GMI_OptionsScreen_Back = (MENUITEM){ "Back", -1, -2, TRUE, MenuItem_OptionsScreen_Back };
    GMI_OptionsScreenItems[0] = &GMI_OptionsScreen_SFXVolume;
    GMI_OptionsScreenItems[1] = &GMI_OptionsScreen_BGMVolume;
    GMI_OptionsScreenItems[2] = &GMI_OptionsScreen_Resolution;
    GMI_OptionsScreenItems[3] = &GMI_OptionsScreen_Back;
    GM_OptionsScreen = (MENU){ "Options Screen", 2, _countof(GMI_OptionsScreenItems), GMI_OptionsScreenItems };

    // Title Screen
    uint16_t xCount = 0;
    uint16_t yCount = 0;
    for (uint8_t i = 0; i < GM_TitleScreen.ItemCount; i++)
    {
        switch (GM_TitleScreen.Items[i]->X)
        {
            case -1:
            {
                GM_TitleScreen.Items[i]->X = ((uint16_t)G_GameWidth - ((uint16_t)strlen(GM_TitleScreen.Items[i]->Name) * 6)) / 2;
                break;
            }
            case -2:
            {
                GM_TitleScreen.Items[i]->X = (uint16_t)(160 + 30 * xCount);
                xCount++;
                break;
            }
        }
        switch (GM_TitleScreen.Items[i]->Y)
        {
            case -1:
            {
                GM_TitleScreen.Items[i]->Y = (uint16_t)((G_GameHeight - 3) / 2);
                break;
            }
            case -2:
            {
                GM_TitleScreen.Items[i]->Y = (uint16_t)(160 + 30 * yCount);
                yCount++;
                break;
            }
            default:
            {
                break;
            }
        }
    }
    ////

    // Exit Confirm Screen
    xCount = 0;
    yCount = 0;
    for (uint8_t i = 0; i < GM_ExitYesNoScreen.ItemCount; i++)
    {
        switch (GM_ExitYesNoScreen.Items[i]->X)
        {
            case -1:
            {
                GM_ExitYesNoScreen.Items[i]->X = ((uint16_t)G_GameWidth - ((uint16_t)strlen(GM_ExitYesNoScreen.Items[i]->Name) * 6)) / 2;
                break;
            }
            case -2:
            {
                GM_ExitYesNoScreen.Items[i]->X = ((uint16_t)G_GameWidth - ((uint16_t)strlen("Yes_No")*6))/2 + (44 * xCount) - 10;
                xCount++;
                break;
            }
        }
        switch (GM_ExitYesNoScreen.Items[i]->Y)
        {
            case -1:
            {
                GM_ExitYesNoScreen.Items[i]->Y = (uint16_t)((G_GameHeight - 3) / 2);
                break;
            }
            case -2:
            {
                GM_ExitYesNoScreen.Items[i]->Y = (uint16_t)(160 + 30 * yCount);
                yCount++;
                break;
            }
            default:
            {
                break;
            }
        }
    }
    ////
    
    // Options Screen
    xCount = 0;
    yCount = 0;
    for (uint8_t i = 0; i < GM_OptionsScreen.ItemCount - 1; i++)
    {
        switch (GM_OptionsScreen.Items[i]->X)
        {
            case -1:
            {
                GM_OptionsScreen.Items[i]->X = ((uint16_t)G_GameWidth - ((uint16_t)(strlen(GM_OptionsScreen.Items[i]->Name) + 8) * 6)) / 2;
                break;
            }
            case -2:
            {
                GM_OptionsScreen.Items[i]->X = ((uint16_t)G_GameWidth - (uint16_t)(strlen("Yes_No") * 6)) / 2 + (44 * xCount) - 10;
                xCount++;
                break;
            }
        }
        switch (GM_OptionsScreen.Items[i]->Y)
        {
            case -1:
            {
                GM_OptionsScreen.Items[i]->Y = (uint16_t)((G_GameHeight - 3) / 2);
                break;
            }
            case -2:
            {
                GM_OptionsScreen.Items[i]->Y = (uint16_t)(160 + 30 * yCount);
                yCount++;
                break;
            }
            default:
            {
                break;
            }
        }
    }
    GM_OptionsScreen.Items[GM_OptionsScreen.ItemCount - 1]->X = ((uint16_t)G_GameWidth - ((uint16_t)(strlen(GM_OptionsScreen.Items[GM_OptionsScreen.ItemCount - 1]->Name)) * 6)) / 2;
    GM_OptionsScreen.Items[GM_OptionsScreen.ItemCount - 1]->Y = GM_OptionsScreen.Items[GM_OptionsScreen.ItemCount - 2]->Y + 30;
    ////
}

void inline FindFirstConnectedGamepad(void)
{
    G_XinputID = -1;

    for (int8_t GamepadIndex = 0; GamepadIndex < XUSER_MAX_COUNT && G_XinputID == -1; GamepadIndex++)
    {
        XINPUT_STATE State = { 0 };

        if (XInputGetState(GamepadIndex, &State) == ERROR_SUCCESS)
        {
            G_XinputID = GamepadIndex;
        }
    }
}

void BlitTileToBuffer(_In_ int32_t x, _In_ int32_t y, _In_ GAMEBITMAP* TileSet, _In_ int32_t Tile)
{
    int32_t StartingScreenPixel = (G_GameWidth * G_GameHeight) - G_GameWidth - (G_GameWidth * y) + x;
    int32_t StartingBitmapPixel = (TileSet->BitmapInfo.bmiHeader.biWidth * TileSet->BitmapInfo.bmiHeader.biHeight) - TileSet->BitmapInfo.bmiHeader.biWidth;
    int32_t MemoryOffset = 0;
    int32_t BitmapOffset = 0;
    int32_t XTileOffset = 0;
    int32_t YTileOffset = 0;
    PIXEL32 BitmapPixel = { 0 };
    PIXEL32 BackgroundPixel = { 0 };

    XTileOffset = Tile % (TileSet->BitmapInfo.bmiHeader.biWidth / 32);
    YTileOffset = (int32_t)floor((double)(Tile / (TileSet->BitmapInfo.bmiHeader.biWidth / 32)));

    
    for (int16_t YPixel = 0; YPixel < 32; YPixel++)
    {
        for (int16_t XPixel = 0; XPixel < 32; XPixel++)
        {
            if (x + XPixel < G_GameWidth && x + XPixel >= 0 && y + YPixel < G_GameHeight && y + YPixel >= 0)
            {
                MemoryOffset = StartingScreenPixel + XPixel - (G_GameWidth * YPixel);
                BitmapOffset = StartingBitmapPixel + XPixel + XTileOffset * 32 - (TileSet->BitmapInfo.bmiHeader.biWidth * (YPixel + YTileOffset * 32));
                memcpy_s(&BackgroundPixel, sizeof(PIXEL32), (PIXEL32*)G_BackBuffer.Memory + MemoryOffset, sizeof(PIXEL32));
                memcpy_s(&BitmapPixel, sizeof(PIXEL32), (PIXEL32*)TileSet->Memory + BitmapOffset, sizeof(PIXEL32));

                if (BackgroundPixel.Red != BitmapPixel.Red || BackgroundPixel.Blue != BitmapPixel.Blue || BackgroundPixel.Green != BitmapPixel.Green || BackgroundPixel.Alpha != BitmapPixel.Alpha)
                {
                    BitmapPixel.Red = (uint8_t)((float)BitmapPixel.Alpha / 255 * (float)BitmapPixel.Red + (1 - ((float)BitmapPixel.Alpha / 255)) * (float)BackgroundPixel.Red);
                    BitmapPixel.Green = (uint8_t)((float)BitmapPixel.Alpha / 255 * (float)BitmapPixel.Green + (1 - ((float)BitmapPixel.Alpha / 255)) * (float)BackgroundPixel.Green);
                    BitmapPixel.Blue = (uint8_t)((float)BitmapPixel.Alpha / 255 * (float)BitmapPixel.Blue + (1 - ((float)BitmapPixel.Alpha / 255)) * (float)BackgroundPixel.Blue);

                    memcpy_s((PIXEL32*)G_BackBuffer.Memory + MemoryOffset, sizeof(PIXEL32), &BitmapPixel, sizeof(PIXEL32));
                }
            }
        }
    }
}

void BlitStringToBuffer(_In_ GAMEBITMAP* Text, _In_ uint32_t x, _In_ uint32_t y, _In_ char* TextString, _In_ uint8_t R, _In_ uint8_t G, _In_ uint8_t B, _In_ uint8_t A)
{
    uint16_t CharWidth = (uint16_t)Text->BitmapInfo.bmiHeader.biWidth   / FONT_SHEET_CHARACTERS_PER_ROW;
    uint16_t CharHeight = (uint16_t)Text->BitmapInfo.bmiHeader.biHeight / 1;
    uint16_t BytesPerChar = (CharWidth * CharHeight * Text->BitmapInfo.bmiHeader.biBitCount / 8);
    uint16_t StringLength = (uint16_t)strlen(TextString);
    GAMEBITMAP StringBitmap = { 0 };
    PIXEL32 EmptyPixel = { 0 };

    StringBitmap.BitmapInfo.bmiHeader.biBitCount    = Text->BitmapInfo.bmiHeader.biBitCount;
    StringBitmap.BitmapInfo.bmiHeader.biHeight      = CharHeight;
    StringBitmap.BitmapInfo.bmiHeader.biWidth       = CharWidth * StringLength;
    StringBitmap.BitmapInfo.bmiHeader.biPlanes      = 1;
    StringBitmap.BitmapInfo.bmiHeader.biCompression = BI_RGB;
    StringBitmap.Memory = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ((size_t)BytesPerChar * (size_t)StringLength));

    for (int Character = 0; Character < StringLength; Character++)
    {
        int StartingFontSheetPixel = 0;
        int FontSheetOffset = 0;
        int StringBitmapOffset = 0;
        BOOL CharSkip = FALSE;
        BOOL Space = FALSE;
        PIXEL32 FontSheetPixel = { 0 };

        if  (TextString[Character] == ' ')
        {
            Space = TRUE;
            StartingFontSheetPixel = 0;
        }
        StartingFontSheetPixel = (Text->BitmapInfo.bmiHeader.biWidth * Text->BitmapInfo.bmiHeader.biHeight) - Text->BitmapInfo.bmiHeader.biWidth + (CharWidth * G_FontCharacterPixelOffset[(uint8_t)TextString[Character]]);

        for (int YPixel = 0; YPixel < CharHeight; YPixel++)
        {
            for (int XPixel = 0; XPixel < CharWidth; XPixel++)
            {
                FontSheetOffset = StartingFontSheetPixel + XPixel - (Text->BitmapInfo.bmiHeader.biWidth * YPixel);
                StringBitmapOffset = (Character * CharWidth) + (StringBitmap.BitmapInfo.bmiHeader.biWidth * StringBitmap.BitmapInfo.bmiHeader.biHeight) - \
                    StringBitmap.BitmapInfo.bmiHeader.biWidth + XPixel - (StringBitmap.BitmapInfo.bmiHeader.biWidth * YPixel);
                memcpy_s(&FontSheetPixel, sizeof(PIXEL32), (PIXEL32*)Text->Memory + FontSheetOffset, sizeof(PIXEL32));

                FontSheetPixel.Red   = R;
                FontSheetPixel.Green = G;
                FontSheetPixel.Blue  = B;
                if (FontSheetPixel.Alpha == 255)
                {
                    FontSheetPixel.Alpha = A;
                }

                if (Space == TRUE)
                {
                    memcpy_s((PIXEL32*)StringBitmap.Memory + StringBitmapOffset, sizeof(PIXEL32), &EmptyPixel, sizeof(PIXEL32));
                }
                else
                {
                    memcpy_s((PIXEL32*)StringBitmap.Memory + StringBitmapOffset, sizeof(PIXEL32), &FontSheetPixel, sizeof(PIXEL32));
                }
            }
        }
        if (CharSkip == TRUE)
        {
            Character++;
        }
    }

    Blit32BppBitmapToBuffer(&StringBitmap, x, y, BLIT_NORMAL, 0, 0, 0, 0);

    if (StringBitmap.Memory)
    {
        HeapFree(GetProcessHeap(), 0, StringBitmap.Memory);
    }
}

void MenuItem_TitleScreen_Resume(void)
{
    if (LoadWorldMap(".\\Assets\\World\\WorldData\\world.txt") != ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to load worldmap!", __FUNCTION__);
        MessageBoxA(NULL, "Failed to load worldmap!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return;
    }
    G_PreviousGameState = G_CurrentGameState;
    G_CurrentGameState = GAMESTATE_OVERWORLD;
}

void MenuItem_TitleScreen_StartNew(void)
{
    G_PreviousGameState = G_CurrentGameState;
    G_CurrentGameState = GAMESTATE_OVERWORLD;
}

void MenuItem_TitleScreen_Options(void)
{
    G_PreviousGameState = G_CurrentGameState;
    G_CurrentGameState = GAMESTATE_OPTIONSSCREEN;
}

void MenuItem_TitleScreen_Exit(void)
{
    G_PreviousGameState = G_CurrentGameState;
    G_CurrentGameState = GAMESTATE_EXITYESNOSCREEN;
}

void MenuItem_ExitYesNoScreen_Yes(void)
{
    PostMessageA(G_GameWindow, WM_CLOSE, 0, 0);
}

void MenuItem_ExitYesNoScreen_No(void)
{
    G_PreviousGameState = G_CurrentGameState;
    G_CurrentGameState = GAMESTATE_TITLESCREEN;
}

void MenuItem_OptionsScreen_SFXVolume(void)
{
    G_SFXVolume += 0.1f;
    if (G_SFXVolume > 1.1f)
    {
        G_SFXVolume = 0;
    }
    for (uint8_t i = 0; i < NUMBER_SFX_SRC; i++)
    {
        G_XAudioSFXSourceVoice[i]->lpVtbl->SetVolume(G_XAudioSFXSourceVoice[i], G_SFXVolume, XAUDIO2_COMMIT_NOW);
    }
}

void MenuItem_OptionsScreen_BGMVolume(void)
{
    G_BGMVolume += 0.1f;
    if (G_BGMVolume > 1.1f)
    {
        G_BGMVolume = 0;
    }
    G_XAudioBGMSourceVoice->lpVtbl->SetVolume(G_XAudioBGMSourceVoice, G_BGMVolume, XAUDIO2_COMMIT_NOW);
}

void MenuItem_OptionsScreen_Resolution(void)
{
    switch (G_Chooser)
    {
        case (0):    // 16/10
        {
            G_GameWidth = 1024;
            G_GameHeight = 640;
            G_ResWidth = 16;
            G_ResHeight = 10;
            break;
        }
        case (1):    // 16/9 
        {
            G_GameWidth = 1024;
            G_GameHeight = 576;
            G_ResWidth = 16;
            G_ResHeight = 9;
            break;
        }
        case (2):    // 21/9
        {
            G_GameWidth = 1344;
            G_GameHeight = 576;
            G_ResWidth = 21;
            G_ResHeight = 9;
            break;
        }
        default:
        {
            break;
        }
    }

    G_CanvasMemorySize = G_GameWidth * G_GameHeight * RES_BPP / 8;
    G_BackBuffer.BitmapInfo.bmiHeader.biWidth = G_GameWidth;
    G_BackBuffer.BitmapInfo.bmiHeader.biHeight = G_GameHeight;
   
    if ((G_BackBuffer.Memory = VirtualAlloc(NULL, G_CanvasMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)) == NULL)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed To Allocate Memory For Canvas! Error 0x%08lx!", __FUNCTION__, GetLastError());
        MessageBoxA(NULL, "Failed To Allocate Memory For Canvas!", "Error!", MB_ICONERROR | MB_OK);
    }

    G_Player.ScreenPosX = (int16_t)((G_GameWidth  - G_Player.Sprite->BitmapInfo.bmiHeader.biWidth)  / 2);
    G_Player.ScreenPosY = (int16_t)((G_GameHeight - G_Player.Sprite->BitmapInfo.bmiHeader.biHeight) / 2);

    InitializeMenus();

    if (G_Chooser < 2) G_Chooser++;
    else G_Chooser = 0;
}

void MenuItem_OptionsScreen_Back(void)
{
    G_CurrentGameState = G_PreviousGameState;
    G_PreviousGameState = GAMESTATE_OPTIONSSCREEN;

    if (SaveRegistryParameters() != ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[&s] SaveRegistryParameters failed!", __FUNCTION__);
    }
}

HRESULT InitializeSoundEngine(void)
{
    WAVEFORMATEX SFXWaveFormat = { 0 };
    WAVEFORMATEX BGMWaveFormat = { 0 };

    HRESULT Result = S_OK;
    Result = CoInitializeEx(NULL, COINIT_MULTITHREADED);

    if (Result != S_OK)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] CoInitializeEx failed with 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
    }

    Result = XAudio2Create(&G_XAudio, 0, XAUDIO2_ANY_PROCESSOR);
    if (FAILED(Result))
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] XAudio2Create failed with 0x%08lx!", __FUNCTION__, Result);
    }

    Result = G_XAudio->lpVtbl->CreateMasteringVoice(G_XAudio, &G_XAudioMasteringVoice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL, 0);
    if (FAILED(Result))
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] CreateMasteringVoice failed with 0x%08lx!", __FUNCTION__, Result);
    }

    SFXWaveFormat.wFormatTag = WAVE_FORMAT_PCM;
    SFXWaveFormat.nChannels  = 1;
    SFXWaveFormat.nSamplesPerSec  = 44100;
    SFXWaveFormat.nAvgBytesPerSec = SFXWaveFormat.nSamplesPerSec * SFXWaveFormat.nChannels * 2;
    SFXWaveFormat.nBlockAlign = SFXWaveFormat.nChannels * 2;
    SFXWaveFormat.wBitsPerSample = 16;
    SFXWaveFormat.cbSize = 0x6164;
    for (uint8_t i = 0; i < NUMBER_SFX_SRC; i++)
    {
        Result = G_XAudio->lpVtbl->CreateSourceVoice(G_XAudio, &G_XAudioSFXSourceVoice[i], &SFXWaveFormat, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
        if (Result != S_OK)
        {
            LogMessageA(LOG_LEVEL_ERROR, "[%s] CreateSourceVoice failed with 0x%08lx!", __FUNCTION__, Result);
            goto Exit;
        }
        G_XAudioSFXSourceVoice[i]->lpVtbl->SetVolume(G_XAudioSFXSourceVoice[i], G_SFXVolume, XAUDIO2_COMMIT_NOW);
    }

    BGMWaveFormat.wFormatTag = WAVE_FORMAT_PCM;
    BGMWaveFormat.nChannels = 2;
    BGMWaveFormat.nSamplesPerSec = 44100;
    BGMWaveFormat.nAvgBytesPerSec = BGMWaveFormat.nSamplesPerSec * BGMWaveFormat.nChannels * 2;
    BGMWaveFormat.nBlockAlign = BGMWaveFormat.nChannels * 2;
    BGMWaveFormat.wBitsPerSample = 16;
    BGMWaveFormat.cbSize = 0;
    Result = G_XAudio->lpVtbl->CreateSourceVoice(G_XAudio, &G_XAudioBGMSourceVoice, &BGMWaveFormat, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
    if (Result != S_OK)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] CreateSourceVoice failed with 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
    }
    G_XAudioBGMSourceVoice->lpVtbl->SetVolume(G_XAudioBGMSourceVoice, G_BGMVolume, XAUDIO2_COMMIT_NOW);

Exit:
    return Result;
}

DWORD LoadWaveFromFile(_In_ char* FileName, _Inout_ GAMESOUND* GameSound)
{
    DWORD Error = ERROR_SUCCESS;
    DWORD RIFF = 0;
    DWORD NumberOfBytesRead = 0;
    uint16_t DataChunkOffset = 0;
    DWORD DataChunkSearcher = 0;
    BOOL DataChunkFound = FALSE;
    DWORD DataChunkSize = 0;;
    HANDLE FileHandle = INVALID_HANDLE_VALUE;
    void* AudioData = NULL;

    if ((FileHandle = CreateFileA(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
    {
        Error = GetLastError();
        LogMessageA(LOG_LEVEL_ERROR, "[%s] CreateFileA failed with 0x%08lx", __FUNCTION__, Error);
        goto Exit;
    }

    if (ReadFile(FileHandle, &RIFF, sizeof(DWORD), &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        LogMessageA(LOG_LEVEL_ERROR, "[%s] ReadFile failed with 0x%08lx", __FUNCTION__, Error);
        goto Exit;
    }

    if (RIFF != 0x46464952) // "RIFF" backwards
    {
        Error = ERROR_FILE_INVALID;
        LogMessageA(LOG_LEVEL_ERROR, "[%s] First 4 bytes are not 'RIFF'!", __FUNCTION__);
        goto Exit;
    }

    if (SetFilePointer(FileHandle, 20, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        Error = GetLastError();
        LogMessageA(LOG_LEVEL_ERROR, "[%s] SetFilePointer failed with 0x%08lx", __FUNCTION__, Error);
        goto Exit;
    }

    if (ReadFile(FileHandle, &GameSound->WaveFormat, sizeof(WAVEFORMATEX), &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        LogMessageA(LOG_LEVEL_ERROR, "[%s] ReadFile failed with 0x%08lx", __FUNCTION__, Error);
        goto Exit;
    }

    if ((GameSound->WaveFormat.nBlockAlign != GameSound->WaveFormat.nChannels * (GameSound->WaveFormat.wBitsPerSample / 8)) || (GameSound->WaveFormat.wFormatTag != WAVE_FORMAT_PCM) || (GameSound->WaveFormat.wBitsPerSample != 16))
    {
        Error = ERROR_DATATYPE_MISMATCH;
        LogMessageA(LOG_LEVEL_ERROR, "[%s] This wave file did not meet the format reguirements! Only PCM format, 44.1kHz, 16 bits per sample .wac files are supported. Error: 0x%08lx", __FUNCTION__, Error);
        goto Exit;
    }

    while (DataChunkFound == FALSE)
    {
        if (SetFilePointer(FileHandle, DataChunkOffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        {
            Error = GetLastError();
            LogMessageA(LOG_LEVEL_ERROR, "[%s] SetFilePointer failed with 0x%08lx", __FUNCTION__, Error);
            goto Exit;
        }
        if (ReadFile(FileHandle, &DataChunkSearcher, sizeof(DWORD), &NumberOfBytesRead, NULL) == 0)
        {
            Error = GetLastError();
            LogMessageA(LOG_LEVEL_ERROR, "[%s] ReadFile failed with 0x%08lx", __FUNCTION__, Error);
            goto Exit;
        }
        if (DataChunkSearcher == 0x61746164) // "data" backwards
        {
            DataChunkFound = TRUE;

            break;
        }
        else 
        {
            DataChunkOffset += 4;
        }

        if (DataChunkOffset > 256)
        {
            Error = ERROR_DATATYPE_MISMATCH;
            LogMessageA(LOG_LEVEL_ERROR, "[%s] Data chunk not found within first 256 bytes of this file! Error: 0x%08lx", __FUNCTION__, Error);
            goto Exit;
        }
    }

    if (SetFilePointer(FileHandle, DataChunkOffset + 4, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        Error = GetLastError();
        LogMessageA(LOG_LEVEL_ERROR, "[%s] SetFilePointer failed with 0x%08lx", __FUNCTION__, Error);
        goto Exit;
    }

    if (ReadFile(FileHandle, &DataChunkSize, sizeof(DWORD), &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        LogMessageA(LOG_LEVEL_ERROR, "[%s] ReadFile failed with 0x%08lx", __FUNCTION__, Error);
        goto Exit;
    }

    AudioData = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, DataChunkSize);
    if (AudioData == NULL)
    {
        Error = ERROR_NOT_ENOUGH_MEMORY;
        LogMessageA(LOG_LEVEL_ERROR, "[%s] HeapAlloc failed with 0x%08lx", __FUNCTION__, Error);    
        goto Exit;
    }

    GameSound->Buffer.Flags = XAUDIO2_END_OF_STREAM;
    GameSound->Buffer.AudioBytes = DataChunkSize;

    if (SetFilePointer(FileHandle, DataChunkOffset + 8, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        Error = GetLastError();
        LogMessageA(LOG_LEVEL_ERROR, "[%s] SetFilePointer failed with 0x%08lx", __FUNCTION__, Error);
        goto Exit;
    }

    if (ReadFile(FileHandle, AudioData, DataChunkSize, &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        LogMessageA(LOG_LEVEL_ERROR, "[%s] ReadFile failed with 0x%08lx", __FUNCTION__, Error);
        goto Exit;
    }

    GameSound->Buffer.pAudioData = AudioData;

Exit:

    if (Error == ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_INFO, "[%s] Successfully loaded %s.", __FUNCTION__, FileName);
    }
    else
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to load %s! Error: 0x%08lx", __FUNCTION__, FileName, Error);
    }

    if (FileHandle && (FileHandle != INVALID_HANDLE_VALUE))
    {
        CloseHandle(FileHandle);
    }

    return Error;
}

void inline PlayGameSound(_In_ GAMESOUND* GameSound)
{
    G_XAudioSFXSourceVoice[G_SFXSourceSelector]->lpVtbl->SubmitSourceBuffer(G_XAudioSFXSourceVoice[G_SFXSourceSelector], &GameSound->Buffer, NULL);
    G_XAudioSFXSourceVoice[G_SFXSourceSelector]->lpVtbl->Start(G_XAudioSFXSourceVoice[G_SFXSourceSelector], 0, XAUDIO2_COMMIT_NOW);
    G_SFXSourceSelector++;
    if (G_SFXSourceSelector > (NUMBER_SFX_SRC - 1))
    {
        G_SFXSourceSelector = 0;
    }
}

DWORD inline LoadRegistryParameters(void)
{
    DWORD Result = ERROR_SUCCESS;
    HKEY  RegKey = NULL;
    DWORD RegDisposition = 0;
    DWORD RegBytesRead = sizeof(DWORD);
    Result = RegCreateKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\" GAME_NAME, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &RegKey, &RegDisposition);
    if (Result != ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] RegCreateKeyFailed failed with error code 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
    }

    if (RegDisposition == REG_CREATED_NEW_KEY)
    {
        LogMessageA(LOG_LEVEL_INFO, "[%s] Registry key did not exist; created new key HKCU\\SOFTWARE\\%s.", __FUNCTION__, GAME_NAME);
    }
    else
    {
        LogMessageA(LOG_LEVEL_INFO, "[%s] Opened existing registry key HKCU\\SOFTWARE\\%s.", __FUNCTION__, GAME_NAME);
    }

    Result = RegGetValueA(RegKey, NULL, "LogLVL", RRF_RT_DWORD, NULL, (BYTE*)&G_RegistryParameters.LogLVL, &RegBytesRead);
    if (Result != ERROR_SUCCESS)
    {
        if (Result == ERROR_FILE_NOT_FOUND)
        {
            Result = ERROR_SUCCESS;
            LogMessageA(LOG_LEVEL_INFO, "[%s] Registry value 'LogLVL' not found. Using default of 0. (LOG_LEVEL_NONE)", __FUNCTION__);
            G_RegistryParameters.LogLVL = 0;
        }
        else
        {
            LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to read the 'LogLVL' registry value! Error 0x%08lx!", __FUNCTION__, Result);
            goto Exit;
        }
    }
    LogMessageA(LOG_LEVEL_INFO, "[%s] LogLVL is %d.", __FUNCTION__, G_RegistryParameters.WindowWidth);

    Result = RegGetValueA(RegKey, NULL, "WindowWidth", RRF_RT_DWORD, NULL, (BYTE*)&G_RegistryParameters.WindowWidth, &RegBytesRead);
    if (Result != ERROR_SUCCESS)
    {
        if (Result == ERROR_FILE_NOT_FOUND)
        {
            Result = ERROR_SUCCESS;
            LogMessageA(LOG_LEVEL_INFO, "[%s] Registry value 'WindowWidth' not found. Using default of 0.", __FUNCTION__);
            G_RegistryParameters.WindowWidth = 0;
        }
        else
        {
            LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to read the 'WindowWidth' registry value! Error 0x%08lx!", __FUNCTION__, Result);
            goto Exit;
        }
    }
    LogMessageA(LOG_LEVEL_INFO, "[%s] WindowWidth is %d.", __FUNCTION__, G_RegistryParameters.WindowWidth);

    Result = RegGetValueA(RegKey, NULL, "WindowHeight", RRF_RT_DWORD, NULL, (BYTE*)&G_RegistryParameters.WindowHeight, &RegBytesRead);
    if (Result != ERROR_SUCCESS)
    {
        if (Result == ERROR_FILE_NOT_FOUND)
        {
            Result = ERROR_SUCCESS;
            LogMessageA(LOG_LEVEL_INFO, "[%s] Registry value 'WindowHeight' not found. Using default of 0.", __FUNCTION__);
            G_RegistryParameters.WindowHeight = 0;
        }
        else
        {
            LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to read the 'WindowHeight' registry value! Error 0x%08lx!", __FUNCTION__, Result);
            goto Exit;
        }
    }
    LogMessageA(LOG_LEVEL_INFO, "[%s] WindowHeight is %d.", __FUNCTION__, G_RegistryParameters.WindowHeight);

    Result = RegGetValueA(RegKey, NULL, "SFXVolume", RRF_RT_DWORD, NULL, (BYTE*)&G_RegistryParameters.SFXVolume, &RegBytesRead);
    if (Result != ERROR_SUCCESS)
    {
        if (Result == ERROR_FILE_NOT_FOUND)
        {
            Result = ERROR_SUCCESS;
            LogMessageA(LOG_LEVEL_INFO, "[%s] Registry value 'SFXVolume' not found. Using default of 50.", __FUNCTION__);
            G_RegistryParameters.SFXVolume = 50;
        }
        else
        {
            LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to read the 'SFXVolume' registry value! Error 0x%08lx!", __FUNCTION__, Result);
            goto Exit;
        }
    }
    LogMessageA(LOG_LEVEL_INFO, "[%s] SFXVolume is %.1f.", __FUNCTION__, (float)G_RegistryParameters.SFXVolume / 100);

    Result = RegGetValueA(RegKey, NULL, "BGMVolume", RRF_RT_DWORD, NULL, (BYTE*)&G_RegistryParameters.BGMVolume, &RegBytesRead);
    if (Result != ERROR_SUCCESS)
    {
        if (Result == ERROR_FILE_NOT_FOUND)
        {
            Result = ERROR_SUCCESS;
            LogMessageA(LOG_LEVEL_INFO, "[%s] Registry value 'BGMVolume' not found. Using default of 50.", __FUNCTION__);
            G_RegistryParameters.BGMVolume = 50;
        }
        else
        {
            LogMessageA(LOG_LEVEL_ERROR, "[%s] Failed to read the 'BGMVolume' registry value! Error 0x%08lx!", __FUNCTION__, Result);
            goto Exit;
        }
    }
    LogMessageA(LOG_LEVEL_INFO, "[%s] BGMVolume is %.1f.", __FUNCTION__, (float)G_RegistryParameters.BGMVolume / 100);

    G_SFXVolume = ((float)G_RegistryParameters.SFXVolume / 100.f);
    G_BGMVolume = ((float)G_RegistryParameters.BGMVolume / 100.f);

Exit:
    if (RegKey)
    {
        RegCloseKey(RegKey);
    }

    return Result;
}

DWORD inline SaveRegistryParameters(void)
{
    DWORD Result = ERROR_SUCCESS;
    HKEY  RegKey = NULL;
    DWORD RegDisposition = 0;

    DWORD SFXVolume = (DWORD)(G_SFXVolume * 100.0f);
    DWORD BGMVolume = (DWORD)(G_BGMVolume * 100.0f);

    Result = RegCreateKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\" GAME_NAME, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &RegKey, &RegDisposition);
    if (Result != ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] RegCreateKeyFailed failed with error code 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
    }
    
    Result = RegSetValueExA(RegKey, "SFXVolume", 0, REG_DWORD, (const BYTE*)&SFXVolume, sizeof(DWORD));
    if (Result != ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Saving SFXVolume failed with error code 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
    }

    Result = RegSetValueExA(RegKey, "BGMVolume", 0, REG_DWORD, (const BYTE*)&BGMVolume, sizeof(DWORD));
    if (Result != ERROR_SUCCESS)
    {
        LogMessageA(LOG_LEVEL_ERROR, "[%s] Saving BGMVolume failed with error code 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
    }

Exit:
    if (RegKey)
    {
        RegCloseKey(RegKey);
    }

    return Result;
}