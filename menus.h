#pragma once

typedef struct MENUITEM
{
	char* Name;
	int16_t X;
	int16_t Y;
	BOOL Enabled;
	void(*Action)(void);

} MENUITEM;

typedef struct MENU
{
	char* Name;
	uint8_t	   SelectedItem;
	uint8_t    ItemCount;
	MENUITEM** Items;

} MENU;

void MenuItem_TitleScreen_Resume(void);
void MenuItem_TitleScreen_StartNew(void);
void MenuItem_TitleScreen_Options(void);
void MenuItem_TitleScreen_Exit(void);
void MenuItem_ExitYesNoScreen_Yes(void);
void MenuItem_ExitYesNoScreen_No(void);
void MenuItem_OptionsScreen_SFXVolume(void);
void MenuItem_OptionsScreen_BGMVolume(void);
void MenuItem_OptionsScreen_Resolution(void);
void MenuItem_OptionsScreen_Back(void);

//         Title Screen

MENUITEM  GMI_TitleScreen_ResumeGame;
MENUITEM  GMI_TitleScreen_StartNewGame;
MENUITEM  GMI_TitleScreen_Options;
MENUITEM  GMI_TitleScreen_Exit;
MENUITEM* GMI_TitleScreenItems[4];
MENU      GM_TitleScreen;

////////////

//         Exit Confirm Screen

MENUITEM  GMI_ExitConfirmScreen_Yes;
MENUITEM  GMI_ExitConfirmScreen_No;
MENUITEM* GMI_ExitConfirmScreenItems[2];
MENU      GM_ExitYesNoScreen;

////////////

//         Options Sceen

MENUITEM  GMI_OptionsScreen_SFXVolume;
MENUITEM  GMI_OptionsScreen_BGMVolume;
MENUITEM  GMI_OptionsScreen_Resolution;
MENUITEM  GMI_OptionsScreen_Back;
MENUITEM* GMI_OptionsScreenItems[4];
MENU      GM_OptionsScreen;

////////////
