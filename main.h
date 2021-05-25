#pragma once
#pragma warning(push, 3)
#include <xaudio2.h>
#pragma comment(lib, "XAudio2.lib")
#include <xinput.h>
#pragma comment(lib, "XInput.lib")
#include <psapi.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <stdint.h>
#pragma comment(lib, "Winmm.lib")
#include <immintrin.h>
#include <emmintrin.h>
#include <time.h>
#pragma warning(pop)
#ifdef _DEBUG 
#define ASSERT(Expression, Message) if (!(Expression)) { *(int*)0 = 0; }
#else
#define ASSERT(Expression, Message) ((void)0)
#endif
#define GAME_NAME			"Fantasy Game"
#define LOG_FILE            GAME_NAME ".log"
#define RES_WIDTH	        384
#define RES_HEIGHT	        240
#define RES_BPP				32
#define RES_WIDTH_X_HEIGHT  (RES_WIDTH * RES_HEIGHT * (RES_BPP / 8))
#define NUMBER_SFX_SRC      8
#define CANVAS_MEMORY_SIZE	(RES_WIDTH * RES_HEIGHT * (RES_BPP / 8))
#define CALC_AVG_FPS		120
#define TARGET_MCS_FRAME    16667ULL
#define FONT_SHEET_CHARACTERS_PER_ROW 94

#define LOG_LEVEL_NULL      0
#define LOG_LEVEL_ERROR     1
#define LOG_LEVEL_WARNING   2
#define LOG_LEVEL_INFO      3
#define LOG_LEVEL_DEBUG     4

#define BLIT_NORMAL              0
#define BLIT_USE_COLOUR          1
#define BLIT_COLOUR_IGNORE_ALPHA 2

#pragma warning(disable: 5045) // Disable warning about Spectre/Meltdown CPU vulnerability
#pragma warning(disable: 4820) // Disable waring about structure padding
#pragma warning(disable: 4710) // Disable warning about functions not being inclined

typedef LONG(NTAPI* _NtQueryTimerResolution) (OUT PULONG MinimumResolution, OUT PULONG MaximumResolution, OUT PULONG CurrentResolution);

_NtQueryTimerResolution NtQueryTimerResolution;

typedef struct GAMEBITMAP
{
	BITMAPINFO BitmapInfo;
	void*	   Memory;

} GAMEBITMAP;

typedef struct GAMESOUND
{
	WAVEFORMATEX   WaveFormat;
	XAUDIO2_BUFFER Buffer;

} GAMESOUND;

typedef struct PIXEL32
{
	uint8_t Blue;
	uint8_t Green;
	uint8_t Red;
	uint8_t Alpha;

} PIXEL32;

typedef struct GAMEPERF_DATA
{
	uint64_t      TotalFramesRendered;
	float         RawFPSAverage;
	float         CookedFPSAverage;
	uint64_t      PerfFrequency;
	MONITORINFO   MonitorInfo;
	unsigned long MonitorWidth;
	unsigned long MonitorHeight;
	int32_t       WindowWidth;
	int32_t       WindowHeight;
	BOOL          DisplayDebugInfo;
	ULONG		  MinimumTimerResolution;
	ULONG	      MaximumTimerResolution;
	ULONG         CurrentTimerResolution;
	DWORD         HandleCount;
	PROCESS_MEMORY_COUNTERS_EX MemInfo;
	SYSTEM_INFO   SystemInfo;
	int64_t       CurrentSystemTime;
	int64_t       PreviousSystemTime;
	double        CPUPercent;

} GAMEPERF_DATA;

typedef struct PLAYER
{
	char       Name[12];
	GAMEBITMAP Sprite[8];
	int32_t    WalkCounter[8];
	BOOL       Walking[8];
	int8_t     CurrentSpeed;
	int8_t     RunningSpeed;
	int8_t     Dir;
	float  	   WorldPosX;
	float      WorldPosY;
	int16_t    ScreenPosX;
	int16_t    ScreenPosY;
	float      HP;
	float      MP;
	float      Stamina;
	float      HPRegen;
	float      MPRegen;
	float      StaminaRegen;
	int32_t    STR;
	int8_t     ShowInventory;
	uint8_t    Hurt;

} PLAYER;

typedef struct REGISTRYPARAMS
{
	DWORD LogLVL;
	DWORD SFXVolume;
	DWORD BGMVolume;
	DWORD WindowWidth;
	DWORD WindowHeight;

} REGISTRYPARAMS;

typedef struct GAMEINPUT
{
	int16_t EnterKeyIsDown;
	int16_t EscapeKeyIsDown;
	int16_t DebugKeyIsDown;
	int16_t LeftKeyIsDown;
	int16_t DownKeyIsDown;
	int16_t RightKeyIsDown;
	int16_t UpKeyIsDown;
	int16_t SpeedKeyIsDown;
	int16_t AttackKeyIsDown;
	int16_t InventoryKeyIsDown;

    int16_t EnterKeyWasDown;
	int16_t EscapeKeyWasDown;
    int16_t DebugKeyWasDown;
    int16_t LeftKeyWasDown;
    int16_t DownKeyWasDown;
    int16_t RightKeyWasDown;
    int16_t UpKeyWasDown;
	int16_t SpeedKeyWasDown;
	int16_t AttackKeyWasDown;
	int16_t InventoryKeyWasDown;

} GAMEINPUT;

typedef enum ENTITYTYPE
{
	ETYPE_NONE = 0,
	ETYPE_FIREBALL = 1,
	ETYPE_EVILCHICKEN = 2,
	ETYPE_QUESTION = 3

} ENTITYTYPE;

typedef struct ENTITY
{
	float x;
	float y;
	float HP;
	float MaxHP;
	float PreviousHP;
	float MP;
	float Speed;
	int8_t  dir;
	int8_t  Hurt;
	int8_t  HurtCounter;
	BOOL    ShotByPlayer;
	BOOL    Projectile;
	BOOL    StartHitCounter;
	BOOL    Detected;
	uint16_t DetectCounter;
	uint16_t HitCounter;
	ENTITYTYPE Entitytype;
	GAMEBITMAP Sprite;

} ENTITY;



typedef enum GAMESTATE
{
	GAMESTATE_NONE = 0,
	GAMESTATE_OPENINGSPLASHSCREEN = 1,
	GAMESTATE_TITLESCREEN = 2,
	GAMESTATE_OVERWORLD = 3,
	GAMESTATE_OPTIONSSCREEN = 4,
	GAMESTATE_EXITYESNOSCREEN = 5,
	GAMESTATE_GAMEPADUNPLUGGED = 6,
	GAMESTATE_TRANSITION = 7,
	GAMESTATE_DEATH = 8,
	GAMESTATE_TEXTBOX = 9

} GAMESTATE;

typedef enum TRANSITION
{
	TRANSITION_FADE = 0

} TRANSITION;

typedef struct WORLDMAP
{
	int16_t Tiles[102400];
	uint32_t Size;
	uint32_t Width;
	uint32_t Height;

} WORLDMAP;

typedef struct SCRIPTLINE
{
	char*     Line;
	char**	  Options;
	uint16_t* Procedures;

} SCRIPTLINE;

typedef struct SCRIPT
{
	SCRIPTLINE*   Scripts;
	uint16_t      ScriptPointer;

} SCRIPT;

XINPUT_STATE   G_XinputState;
int8_t         G_XinputID;
GAMEINPUT      G_GameInput;
GAMESTATE      G_CurrentGameState;
GAMESTATE      G_PreviousGameState;
GAMESTATE      G_DesiredGameState;
GAMEPERF_DATA  G_PerformanceData;
GAMEBITMAP     G_BackBuffer;
GAMEBITMAP     G_6x7Font;
GAMEBITMAP     G_Vogel;
GAMEBITMAP     G_FireBallbmp;
GAMEBITMAP     G_WorldTileSet;
GAMEBITMAP     G_Question;
float		   G_SFXVolume;
float		   G_BGMVolume;
int			   G_Chooser;
int		       G_GameHeight;
int            G_GameWidth;
int            G_CanvasMemorySize;
unsigned long  G_ResWidth;
unsigned long  G_ResHeight;
GAMESOUND      G_SFXSplashIntroDrop;
GAMESOUND      G_SFXMenuNavigate;
GAMESOUND      G_SFXMenuEnter;
GAMESOUND      G_SFXEnterTitleScreen;
GAMESOUND      G_SFXFireBallShoot;
GAMESOUND      G_SFXPlayerHurt;
GAMESOUND      G_SFXSteps[3];
uint8_t        G_SFXSourceSelector;
uint16_t       G_TransitionSeconds;
TRANSITION     G_TransitionType;
PLAYER         G_Player;
WORLDMAP       G_WorldMap;
BOOL           G_GameplayIsInProgress;

LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM WParam, _In_ LPARAM LParam);
DWORD CreateMyWindow(void);
BOOL GameIsAlreadyRunning(void);
void ProcessPlayerInput(void);
DWORD InitializePlayer(_Inout_ PLAYER* VPlayer);
DWORD Load32BppBitmapFromFile(_In_ char* FileName, _Inout_  GAMEBITMAP* GameBitMap);
DWORD LoadWaveFromFile(_In_ char* FileName, _Inout_ GAMESOUND* GameSound);
DWORD LoadRegistryParameters(void);
DWORD inline SaveRegistryParameters(void);
void LogMessageA(_In_ DWORD LogLVL, _In_ char* TextString, _In_ ...);
void Blit32BppBitmapToBuffer(_In_ GAMEBITMAP* GameBitmap, _In_ int32_t x, _In_ int32_t y, _In_ int8_t UseColours, _In_ uint8_t Red, _In_ uint8_t Green, _In_ uint8_t Blue, _In_ uint8_t Alpha);
void BlitStringToBuffer(_In_ GAMEBITMAP* Text, _In_ uint32_t x, _In_ uint32_t y, _In_ char* TextString, _In_ uint8_t R, _In_ uint8_t G, _In_ uint8_t B, _In_ uint8_t A);
void BlitTileToBuffer(_In_ int32_t x, _In_ int32_t y, _In_ GAMEBITMAP* TileSet, _In_ int32_t Tile);
void RenderFrameGraphics(void);
void FindFirstConnectedGamepad(void);
void inline InitializeMenus(void);
HRESULT InitializeSoundEngine(void);
DWORD inline LoadWorldMap(LPCSTR FileName);
extern void inline PPI_OpeningSplashScreen(void);
extern void inline PPI_DeathScreen(void);
extern void inline PPI_GamepadUnplugged(void);
extern void inline PPI_ExitYesNoScreen(void);
extern void inline PPI_OverWorld(void);
extern void inline PPI_OptionsScreen(void);
extern void inline PPI_TitleScreen(void);
extern void inline PPI_TextBoxScreen(void);
extern void inline DrawDebugInfo(void);
extern void inline DrawTransitionScreen(void);
extern void inline DrawOpeningSplashScreen(void);
extern void inline DrawDeathScreen(void);
extern void inline DrawGamepadUnpluggedScreen(void);
extern void inline DrawExitYesNoScreen(void);
extern void inline DrawOverWorld(void);
extern void inline DrawOptionsScreen(void);
extern void inline DrawTitleScreen(void);
extern void inline DrawHudOnScreen(void);
extern void inline DrawTextBoxScreen(void);
extern void inline DrawTextBox(_In_ char* TextString);
extern BOOL inline IsLookingAtBarriers(_In_ int8_t Dir, _In_ float x, _In_ float y, _In_ GAMEBITMAP Bitmap);
extern void inline SpawnEnemies(_In_ uint8_t Amount, _In_ float X, _In_ float Y, _In_ float Radius);
extern void inline SpawnEntity(_In_ ENTITYTYPE EntityType, _In_ float X, _In_ float Y, _In_ int8_t dir, _In_ BOOL ShotByPlayer);
extern void inline ProcessEntities(void);
extern void inline DrawEntities(void);
extern BOOL inline HitRectDect(_In_ float x1, _In_ float y1, _In_ float halfwidth1, _In_ float halfheight1, _In_ float x2, _In_ float y2, _In_ float halfwidth2, _In_ float halfheight2);
extern void inline ProcessPlayer(void);
extern void inline DrawPlayerSprite(_In_ PLAYER VPlayer);
extern int32_t CalcScreenX(_Inout_ int32_t x);
extern int32_t CalcScreenY(_Inout_ int32_t y);
extern void inline DrawRect(_In_ int32_t x1, _In_ int32_t y1, _In_ int32_t x2, _In_ int32_t y2, _In_ uint8_t Red, _In_ uint8_t Green, _In_ uint8_t Blue, _In_ uint8_t Alpha);
extern int32_t inline CalcWorldMapIndex(float x, float y);
extern void inline TransitionScreen(_In_ GAMESTATE DesiredGameState, _In_ TRANSITION TransitionType, _In_ uint16_t TransitionsSeconds);
extern void inline PlayGameSound(_In_ GAMESOUND* GameSound);
extern int8_t inline CalcDirToPlayer(_In_ PLAYER Player, _In_ ENTITY Entity);
extern void inline LoadScriptIntoBuffer(_In_ SCRIPT Script, _In_ char* Name, _In_ uint16_t Chunk);
extern WORD inline ReturnRead(_In_ HANDLE FileHandle, _In_ LONG Pointer);

