#include "main.h"

SCRIPT G_ScriptBuffer;

void inline PPI_TextBoxScreen(void)
{
	static BOOL Once = TRUE;
	if (Once)
	{
		LoadScriptIntoBuffer(G_ScriptBuffer, "Question", 20);
		Once == FALSE;
	}

	if (G_GameInput.EscapeKeyIsDown && !G_GameInput.EscapeKeyWasDown)
	{
		G_CurrentGameState = G_PreviousGameState;
		G_PreviousGameState = GAMESTATE_TEXTBOX;
	}

	ProcessEntities();
	ProcessPlayer();
}

void inline DrawTextBoxScreen(void)
{
	DrawOverWorld();
	DrawTextBox("Hello, World!");
}

void inline DrawTextBox(_In_ char* TextString)
{
	DrawRect(10, G_GameHeight - 10 - 150, G_GameWidth - 10, G_GameHeight - 10, 200, 30, 150, 133);
	BlitStringToBuffer(&G_6x7Font, (G_GameWidth - strlen(TextString))/ 2, G_GameHeight - 10 - 75, TextString, 255, 255, 255, 255);
}

void inline LoadScriptIntoBuffer(_In_ SCRIPT Script, _In_ char* Name, _In_ uint16_t Chunk)
{
	DWORD Error = ERROR_SUCCESS;
	HANDLE FileHandle = INVALID_HANDLE_VALUE;
	DWORD NumberOfBytesRead = 0;

	char* Buffer = ".\\Assets\\Script\\";
	strcat(Buffer, Name);
	strcat(Buffer, ".txt");

	if ((FileHandle = CreateFileA(Buffer, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
	{
		Error = GetLastError();
		goto Exit;
	}

	BOOL End = FALSE;
	BOOL BracketIsFound = FALSE;
	uint16_t CurrentLine = 0;
	for (LONG i = 0; End == FALSE; i++)
	{
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
	return;
}

WORD inline ReturnRead(_In_ HANDLE FileHandle, _In_ LONG Pointer)
{
	WORD ReadByte = 0;
	DWORD NumberOfBytesRead = 0;
	SetFilePointer(FileHandle, Pointer, NULL, FILE_BEGIN);
	if (ReadFile(FileHandle, &ReadByte, 1, &NumberOfBytesRead, NULL) == 0) return 0;
	return ReadByte;
}