#include "main.h"

ENTITY G_Entities[128] = { 0 };
int8_t G_DirPicker[3][3] = { { -1, 3, 1 } , { 0, 4, 5 } , { 2, 7, 6 } };

void inline PPI_OverWorld(void)
{
    if (G_GameInput.EscapeKeyIsDown && !G_GameInput.EscapeKeyWasDown)
    {
        G_PreviousGameState = G_CurrentGameState;
        G_CurrentGameState = GAMESTATE_TITLESCREEN;
        PlayGameSound(&G_SFXEnterTitleScreen);
    }

    if (G_GameInput.DebugKeyIsDown && !G_GameInput.DebugKeyWasDown)
    {
        G_PerformanceData.DisplayDebugInfo = !G_PerformanceData.DisplayDebugInfo;
        G_Player.ShowInventory = !G_Player.ShowInventory;
    }

    if (G_GameInput.SpeedKeyIsDown && G_Player.Stamina > 0)
    {
        G_Player.CurrentSpeed = G_Player.RunningSpeed;
    }
    else if (G_GameInput.SpeedKeyWasDown)
    {
        G_Player.CurrentSpeed = 2;
    }

    if (G_GameInput.LeftKeyIsDown && !G_GameInput.RightKeyIsDown && !G_GameInput.UpKeyIsDown && !G_GameInput.DownKeyIsDown)
    {
        G_Player.Dir = 0;
        if (G_Player.WorldPosX > (float)(G_Player.Sprite[0].BitmapInfo.bmiHeader.biWidth / 2) && !IsLookingAtBarriers(G_Player.Dir, G_Player.WorldPosX, G_Player.WorldPosY, G_Player.Sprite[0]))
        {
            G_Player.WorldPosX -= G_Player.CurrentSpeed;
            G_Player.WalkCounter[G_Player.Dir]++;
            if (G_Player.WalkCounter[G_Player.Dir] % (30 / G_Player.CurrentSpeed) == 0)
            {
                PlayGameSound(&G_SFXSteps[(int)(3.f * (float)rand() / (float)RAND_MAX)]);
                G_Player.Walking[0] = !G_Player.Walking[0];
            }
        }
    }
    if (!G_GameInput.LeftKeyIsDown && G_GameInput.LeftKeyWasDown)
    {
        G_Player.Walking[0] = FALSE;
    }

    if (!G_GameInput.LeftKeyIsDown && !G_GameInput.RightKeyIsDown && !G_GameInput.UpKeyIsDown && G_GameInput.DownKeyIsDown)
    {
        G_Player.Dir = 1;
        if (G_Player.WorldPosY < (float)(G_WorldMap.Height * 32 - G_Player.Sprite->BitmapInfo.bmiHeader.biHeight / 2) && !IsLookingAtBarriers(G_Player.Dir, G_Player.WorldPosX, G_Player.WorldPosY, G_Player.Sprite[0]))
        {
            G_Player.WorldPosY += G_Player.CurrentSpeed;
            G_Player.WalkCounter[G_Player.Dir]++;
            if (G_Player.WalkCounter[G_Player.Dir] % (30 / G_Player.CurrentSpeed) == 0)
            {
                PlayGameSound(&G_SFXSteps[(int)(3.f * (float)rand() / (float)RAND_MAX)]);
                G_Player.Walking[1] = !G_Player.Walking[1];
            }
        }
    }
    if (!G_GameInput.DownKeyIsDown && G_GameInput.DownKeyWasDown)
    {
        G_Player.Walking[1] = FALSE;
    }

    if (!G_GameInput.LeftKeyIsDown && G_GameInput.RightKeyIsDown && !G_GameInput.UpKeyIsDown && !G_GameInput.DownKeyIsDown)
    {
        G_Player.Dir = 2;
        if (G_Player.WorldPosX < (float)(G_WorldMap.Width * 32 - G_Player.Sprite[0].BitmapInfo.bmiHeader.biWidth / 2) && !IsLookingAtBarriers(G_Player.Dir, G_Player.WorldPosX, G_Player.WorldPosY, G_Player.Sprite[0]))
        {
            G_Player.WorldPosX += G_Player.CurrentSpeed;
            G_Player.WalkCounter[G_Player.Dir]++;
            if (G_Player.WalkCounter[G_Player.Dir] % (30 / G_Player.CurrentSpeed) == 0)
            {
                PlayGameSound(&G_SFXSteps[(int)(3.f * (float)rand() / (float)RAND_MAX)]);
                G_Player.Walking[G_Player.Dir] = !G_Player.Walking[G_Player.Dir];
            }
        }
    }
    if (!G_GameInput.RightKeyIsDown && G_GameInput.RightKeyWasDown)
    {
        G_Player.Walking[2] = FALSE;
    }

    if (!G_GameInput.LeftKeyIsDown && !G_GameInput.RightKeyIsDown && G_GameInput.UpKeyIsDown && !G_GameInput.DownKeyIsDown)
    {
        G_Player.Dir = 3;
        if (G_Player.WorldPosY > (float)(G_Player.Sprite[0].BitmapInfo.bmiHeader.biHeight / 2) && !IsLookingAtBarriers(G_Player.Dir, G_Player.WorldPosX, G_Player.WorldPosY, G_Player.Sprite[0]))
        {
            G_Player.WorldPosY -= G_Player.CurrentSpeed;
            G_Player.WalkCounter[G_Player.Dir]++;
            if (G_Player.WalkCounter[G_Player.Dir] % (30 / G_Player.CurrentSpeed) == 0)
            {
                PlayGameSound(&G_SFXSteps[(int)(3.f * (float)rand() / (float)RAND_MAX)]);
                G_Player.Walking[3] = !G_Player.Walking[3];
            }
        }
    }
    if (!G_GameInput.UpKeyIsDown && G_GameInput.UpKeyWasDown)
    {
        G_Player.Walking[3] = FALSE;
    }

    if (G_GameInput.LeftKeyIsDown && !G_GameInput.RightKeyIsDown && G_GameInput.UpKeyIsDown && !G_GameInput.DownKeyIsDown)
    {
        G_Player.Dir = 4;
        if (G_Player.WorldPosY > (float)(G_Player.Sprite[0].BitmapInfo.bmiHeader.biHeight / 2) && G_Player.WorldPosX > (float)(G_Player.Sprite[0].BitmapInfo.bmiHeader.biWidth / 2) && !IsLookingAtBarriers(G_Player.Dir, G_Player.WorldPosX, G_Player.WorldPosY, G_Player.Sprite[0]))
        {
            G_Player.WorldPosY -= G_Player.CurrentSpeed;
            G_Player.WorldPosX -= G_Player.CurrentSpeed;
            G_Player.WalkCounter[G_Player.Dir]++;
            if (G_Player.WalkCounter[G_Player.Dir] % (30 / G_Player.CurrentSpeed) == 0)
            {
                PlayGameSound(&G_SFXSteps[(int)(3.f * (float)rand() / (float)RAND_MAX)]);
                G_Player.Walking[G_Player.Dir] = !G_Player.Walking[G_Player.Dir];
            }
        }
    }

    if (G_GameInput.LeftKeyIsDown && !G_GameInput.RightKeyIsDown && !G_GameInput.UpKeyIsDown && G_GameInput.DownKeyIsDown)
    {
        G_Player.Dir = 5;
        if (G_Player.WorldPosY < (float)(G_WorldMap.Height * 32 - G_Player.Sprite->BitmapInfo.bmiHeader.biHeight / 2) && G_Player.WorldPosX >(float)(G_Player.Sprite[0].BitmapInfo.bmiHeader.biWidth / 2) && !IsLookingAtBarriers(G_Player.Dir, G_Player.WorldPosX, G_Player.WorldPosY, G_Player.Sprite[0]))
        {
            G_Player.WorldPosY += G_Player.CurrentSpeed;
            G_Player.WorldPosX -= G_Player.CurrentSpeed;
            G_Player.WalkCounter[G_Player.Dir]++;
            if (G_Player.WalkCounter[G_Player.Dir] % (30 / G_Player.CurrentSpeed) == 0)
            {
                PlayGameSound(&G_SFXSteps[(int)(3.f * (float)rand() / (float)RAND_MAX)]);
                G_Player.Walking[G_Player.Dir] = !G_Player.Walking[G_Player.Dir];
            }
        }
    }

    if (!G_GameInput.LeftKeyIsDown && G_GameInput.RightKeyIsDown && !G_GameInput.UpKeyIsDown && G_GameInput.DownKeyIsDown)
    {
        G_Player.Dir = 6;
        if (G_Player.WorldPosY < (float)(G_WorldMap.Height * 32 - G_Player.Sprite->BitmapInfo.bmiHeader.biHeight / 2) && G_Player.WorldPosX < (float)(G_WorldMap.Width * 32 - G_Player.Sprite[0].BitmapInfo.bmiHeader.biWidth / 2) && !IsLookingAtBarriers(G_Player.Dir, G_Player.WorldPosX, G_Player.WorldPosY, G_Player.Sprite[0]))
        {
            G_Player.WorldPosY += G_Player.CurrentSpeed;
            G_Player.WorldPosX += G_Player.CurrentSpeed;
            G_Player.WalkCounter[G_Player.Dir]++;
            if (G_Player.WalkCounter[G_Player.Dir] % (30 / G_Player.CurrentSpeed) == 0)
            {
                PlayGameSound(&G_SFXSteps[(int)(3.f * (float)rand() / (float)RAND_MAX)]);
                G_Player.Walking[G_Player.Dir] = !G_Player.Walking[G_Player.Dir];
            }
        }
    }

    if (!G_GameInput.LeftKeyIsDown && G_GameInput.RightKeyIsDown && G_GameInput.UpKeyIsDown && !G_GameInput.DownKeyIsDown)
    {
        G_Player.Dir = 7;
        if (G_Player.WorldPosX < (float)(G_WorldMap.Width * 32 - G_Player.Sprite[0].BitmapInfo.bmiHeader.biWidth / 2) && G_Player.WorldPosY >(float)(G_Player.Sprite[0].BitmapInfo.bmiHeader.biHeight / 2) && !IsLookingAtBarriers(G_Player.Dir, G_Player.WorldPosX, G_Player.WorldPosY, G_Player.Sprite[0]))
        {
            G_Player.WorldPosY -= G_Player.CurrentSpeed;
            G_Player.WorldPosX += G_Player.CurrentSpeed;
            G_Player.WalkCounter[G_Player.Dir]++;
            if (G_Player.WalkCounter[G_Player.Dir] % (30 / G_Player.CurrentSpeed) == 0)
            {
                PlayGameSound(&G_SFXSteps[(int)(3.f * (float)rand() / (float)RAND_MAX)]);
                G_Player.Walking[G_Player.Dir] = !G_Player.Walking[G_Player.Dir];
            }
        }
    }


    if (G_GameInput.AttackKeyIsDown && !G_GameInput.AttackKeyWasDown)
    {
        SpawnEntity(ETYPE_FIREBALL, G_Player.WorldPosX, G_Player.WorldPosY, G_Player.Dir, TRUE);
    }
    
    ProcessPlayer();
    ProcessEntities();

}

BOOL inline IsLookingAtBarriers(_In_ int8_t Dir, _In_ float x, _In_ float y, _In_ GAMEBITMAP Bitmap)
{
    BOOL ReturnValue = FALSE;
    int32_t Buffer = 0;

    switch (Dir)
    {
        case 0: Buffer = G_WorldMap.Tiles[CalcWorldMapIndex(x - (float)Bitmap.BitmapInfo.bmiHeader.biWidth / 2, y)]; break;
        case 1: Buffer = G_WorldMap.Tiles[CalcWorldMapIndex(x, y + (float)Bitmap.BitmapInfo.bmiHeader.biHeight / 2)]; break;
        case 2: Buffer = G_WorldMap.Tiles[CalcWorldMapIndex(x + (float)Bitmap.BitmapInfo.bmiHeader.biWidth / 2, y)]; break;
        case 3: Buffer = G_WorldMap.Tiles[CalcWorldMapIndex(x, y - (float)Bitmap.BitmapInfo.bmiHeader.biHeight / 2)]; break;
        case 4: Buffer = G_WorldMap.Tiles[CalcWorldMapIndex(x - (float)Bitmap.BitmapInfo.bmiHeader.biWidth / 2, y - (float)Bitmap.BitmapInfo.bmiHeader.biHeight / 2)]; break;
        case 5: Buffer = G_WorldMap.Tiles[CalcWorldMapIndex(x - (float)Bitmap.BitmapInfo.bmiHeader.biWidth / 2, y + (float)Bitmap.BitmapInfo.bmiHeader.biHeight / 2)]; break;
        case 6: Buffer = G_WorldMap.Tiles[CalcWorldMapIndex(x + (float)Bitmap.BitmapInfo.bmiHeader.biWidth / 2, y + (float)Bitmap.BitmapInfo.bmiHeader.biHeight / 2)]; break;
        case 7: Buffer = G_WorldMap.Tiles[CalcWorldMapIndex(x + (float)Bitmap.BitmapInfo.bmiHeader.biWidth / 2, y - (float)Bitmap.BitmapInfo.bmiHeader.biHeight / 2)]; break;
    }

    switch (Buffer)
    {
        case 1: ReturnValue = TRUE; break;
        default: break;
    }

    return ReturnValue;
}




void inline SpawnEnemies(_In_ uint8_t Amount, _In_ float X, _In_ float Y, _In_ float Radius)
{
    float Xr;
    float Yr;
    for (uint8_t i = 0; i < Amount; i++)
    {
        Xr = (((float)rand() / RAND_MAX) * Radius * 2 - Radius);
        Xr = (float)round((double)Xr);
        Yr = (((float)rand() / RAND_MAX) * Radius * 2 - Radius);
        Yr = (float)round((double)Yr);
        SpawnEntity(ETYPE_EVILCHICKEN, X + Xr, Y + Yr, 0, FALSE);
    }
}

void inline SpawnEntity(_In_ ENTITYTYPE EntityType, _In_ float X, _In_ float Y, _In_ int8_t dir, _In_ BOOL ShotByPlayer)
{
    ENTITY BufferEntity = { 0 };
    BufferEntity.Entitytype = EntityType;
    BufferEntity.x = X;
    BufferEntity.y = Y;
    BufferEntity.dir = dir;

    switch (BufferEntity.Entitytype)
    {
        case ETYPE_FIREBALL:
        {
            if (ShotByPlayer)
            {
                if (G_Player.MP > 40)
                {
                    G_Player.MP -= 40;
                    BufferEntity.HP = 100;
                    BufferEntity.Speed = 6;
                    BufferEntity.ShotByPlayer = TRUE;
                    BufferEntity.Sprite = G_FireBallbmp;
                    BufferEntity.Projectile = TRUE;
                    PlayGameSound(&G_SFXFireBallShoot);
                }
            }
            else
            {
                BufferEntity.HP = 100;
                BufferEntity.Speed = 6;
                BufferEntity.Sprite = G_FireBallbmp;
                PlayGameSound(&G_SFXFireBallShoot);
            }
            break;
        }
        case ETYPE_EVILCHICKEN:
        {
            BufferEntity.HP = 300;
            BufferEntity.MaxHP = 300;
            BufferEntity.MP = 0;
            BufferEntity.Speed = 1;
            BufferEntity.Hurt = 0;
            BufferEntity.HurtCounter = 0;
            BufferEntity.Detected = FALSE;
            BufferEntity.Sprite = G_Vogel;
            break;
        }
        case ETYPE_QUESTION:
        {
            BufferEntity.HP = 1000;
            BufferEntity.Hurt = 0;
            BufferEntity.HurtCounter = 0;
            BufferEntity.Sprite = G_Question;
        }
        case ETYPE_NONE:
        {
            break;
        }
    }

    BufferEntity.PreviousHP = BufferEntity.HP;

    static int16_t IndexBuffer = 0;
    G_Entities[IndexBuffer] = BufferEntity;
    if (IndexBuffer < 128) IndexBuffer++;
    else
    {
        for (int16_t i = 0; i < 128; i++)
        {
            if (G_Entities[i].Entitytype == ETYPE_NONE) { IndexBuffer = (int16_t)i; return; }
        }
    }
}

void inline ProcessEntities(void)
{
    for (int16_t i = 0; i < 128; i++)
    {
        if (G_Entities[i].HP <= 0) G_Entities[i].Entitytype = ETYPE_NONE;
        switch (G_Entities[i].Entitytype)
        {
            case ETYPE_FIREBALL:
            {   
                if (IsLookingAtBarriers(G_Entities[i].dir, G_Entities[i].x, G_Entities[i].y, G_Entities[i].Sprite) || G_Entities[i].x <= (float)(G_Entities[i].Sprite.BitmapInfo.bmiHeader.biWidth / 2) || G_Entities[i].y <= (float)(G_Entities[i].Sprite.BitmapInfo.bmiHeader.biHeight / 2) || G_Entities[i].x >= G_GameWidth - (float)(G_Entities[i].Sprite.BitmapInfo.bmiHeader.biWidth / 2) /*|| G_Entities[i].y >= G_GameHeight - (float)(G_Entities[i].Sprite.BitmapInfo.bmiHeader.biHeight / 2)*/) G_Entities[i].HP = 0;
                if (!G_Entities[i].ShotByPlayer && HitRectDect(G_Player.WorldPosX,
                    G_Player.WorldPosX,
                    (float)G_Player.Sprite[0].BitmapInfo.bmiHeader.biWidth / 2,
                    (float)G_Player.Sprite[0].BitmapInfo.bmiHeader.biHeight / 2,
                    G_Entities[i].x,
                    G_Entities[i].y,
                    (float)G_Entities[i].Sprite.BitmapInfo.bmiHeader.biWidth / 2,
                    (float)G_Entities[i].Sprite.BitmapInfo.bmiHeader.biHeight / 2))
                {
                    if (G_Player.HP > 40) G_Player.HP -= 40;
                    else G_Player.HP = 0;
                    G_Entities[i].HP = 0;
                }
                for (uint16_t k = 0; k < 128; k++)
                {
                    if (i == k) continue;
                    if (HitRectDect(G_Entities[i].x, G_Entities[i].y, (float)G_Entities[i].Sprite.BitmapInfo.bmiHeader.biWidth / 2, (float)G_Entities[i].Sprite.BitmapInfo.bmiHeader.biHeight / 2,
                        G_Entities[k].x, G_Entities[k].y, (float)G_Entities[k].Sprite.BitmapInfo.bmiHeader.biWidth / 2, (float)G_Entities[k].Sprite.BitmapInfo.bmiHeader.biHeight / 2) && !G_Entities[k].Projectile)
                    {
                        if (G_Entities[k].HP > 40) G_Entities[k].HP -= 40;
                        else G_Entities[k].HP = 0;
                        G_Entities[i].HP = 0;
                    }
                }
                G_Entities[i].HP -= 5;
                float sqrt2 = (float)sqrt(2);
                switch (G_Entities[i].dir)
                {
                    case 0: G_Entities[i].x -= G_Entities[i].Speed; break;
                    case 1: G_Entities[i].y += G_Entities[i].Speed; break;
                    case 2: G_Entities[i].x += G_Entities[i].Speed; break;
                    case 3: G_Entities[i].y -= G_Entities[i].Speed; break;
                    case 4: G_Entities[i].x -= G_Entities[i].Speed / sqrt2; G_Entities[i].y -= G_Entities[i].Speed / sqrt2; break;
                    case 5: G_Entities[i].x -= G_Entities[i].Speed / sqrt2; G_Entities[i].y += G_Entities[i].Speed / sqrt2; break;
                    case 6: G_Entities[i].x += G_Entities[i].Speed / sqrt2; G_Entities[i].y += G_Entities[i].Speed / sqrt2; break;
                    case 7: G_Entities[i].x += G_Entities[i].Speed / sqrt2; G_Entities[i].y -= G_Entities[i].Speed / sqrt2; break;
                }

                break;
            }
            case ETYPE_EVILCHICKEN:
            {
                G_Entities[i].dir = CalcDirToPlayer(G_Player, G_Entities[i]);


                if (HitRectDect(G_Player.WorldPosX,
                    G_Player.WorldPosY,
                    (float)G_Player.Sprite[0].BitmapInfo.bmiHeader.biWidth / 2,
                    (float)G_Player.Sprite[0].BitmapInfo.bmiHeader.biHeight / 2,
                    G_Entities[i].x,
                    G_Entities[i].y,
                    (float)G_Entities[i].Sprite.BitmapInfo.bmiHeader.biWidth / 2,
                    (float)G_Entities[i].Sprite.BitmapInfo.bmiHeader.biHeight / 2) && G_Entities[i].HitCounter == 0)
                {
                    if (G_Player.HP > 40) G_Player.HP -= 40;
                    else G_Player.HP = 0;
                    G_Entities[i].StartHitCounter = TRUE;
                }
                if (G_Entities[i].StartHitCounter) G_Entities[i].HitCounter++;
                if (G_Entities[i].HitCounter > 100) { G_Entities[i].HitCounter = 0; G_Entities[i].StartHitCounter = FALSE; }

                if (G_Player.WorldPosX > G_Entities[i].x - 100 && G_Player.WorldPosX < G_Entities[i].x + 100 && G_Player.WorldPosY > G_Entities[i].y - 100 && G_Player.WorldPosY < G_Entities[i].y + 100 && !IsLookingAtBarriers(G_Entities[i].dir, G_Entities[i].x, G_Entities[i].y, G_Entities[i].Sprite))
                {
                    G_Entities[i].Detected = TRUE;
                    G_Entities[i].DetectCounter = 0;
                }
                else
                {
                    if (G_Entities[i].DetectCounter < 300) G_Entities[i].DetectCounter++;
                    else G_Entities[i].Detected = FALSE;
                }

                if (G_Entities[i].Detected == TRUE)
                {
                    for (int8_t j = 0; j < 8; j++)
                    {
                        if (IsLookingAtBarriers(G_Entities[i].dir, G_Entities[i].x, G_Entities[i].y, G_Entities[i].Sprite))
                        {
                            if (G_Entities[i].dir < 7) G_Entities[i].dir++;
                            else G_Entities[i].dir = 0;
                        }
                        else break;
                    }
                    switch (G_Entities[i].dir)
                    {
                        case 0: G_Entities[i].x -= G_Entities[i].Speed; break;
                        case 1: G_Entities[i].y += G_Entities[i].Speed; break;
                        case 2: G_Entities[i].x += G_Entities[i].Speed; break;
                        case 3: G_Entities[i].y -= G_Entities[i].Speed; break;
                        case 4: G_Entities[i].x -= G_Entities[i].Speed; G_Entities[i].y -= G_Entities[i].Speed; break;
                        case 5: G_Entities[i].x -= G_Entities[i].Speed; G_Entities[i].y += G_Entities[i].Speed; break;
                        case 6: G_Entities[i].x += G_Entities[i].Speed; G_Entities[i].y += G_Entities[i].Speed; break;
                        case 7: G_Entities[i].x += G_Entities[i].Speed; G_Entities[i].y -= G_Entities[i].Speed; break;
                        default: break;
                    }
                }

                if (G_Entities[i].PreviousHP > G_Entities[i].HP && G_Entities[i].HurtCounter == 0) { G_Entities[i].Hurt = 2; G_Entities[i].Detected = TRUE; G_Entities[i].DetectCounter = 0; }
                if (G_Entities[i].Hurt == 2) G_Entities[i].HurtCounter++;
                if (G_Entities[i].HurtCounter > 3) { G_Entities[i].HurtCounter = 0; G_Entities[i].Hurt = 0; }
                G_Entities[i].PreviousHP = G_Entities[i].HP;

                break;
            }
            case ETYPE_QUESTION:
            {
                if (G_Entities[i].PreviousHP > G_Entities[i].HP) { G_PreviousGameState = G_CurrentGameState; G_CurrentGameState = GAMESTATE_TEXTBOX; }
                G_Entities[i].PreviousHP = G_Entities[i].HP;
                break;
            }
            case ETYPE_NONE:
            {

                break;
            }
        }
    }
}

int8_t inline CalcDirToPlayer(_In_ PLAYER Player, _In_ ENTITY Entity)
{
    float DiffX = Entity.x - Player.WorldPosX;
    float DiffY = Entity.y - Player.WorldPosY;
    int8_t DiffXState = -1;
    int8_t DiffYState = -1;
    
    if (DiffX >= 0)
    {
        if (DiffX == 0) DiffXState = 0;
        else DiffXState = 1;
    }
    else DiffXState = 2;

    if (DiffY >= 0)
    {
        if (DiffY == 0) DiffYState = 0;
        else DiffYState = 1;
    }
    else DiffYState = 2;

    return G_DirPicker[DiffXState][DiffYState];
}

void inline DrawEntities(void)
{
    for (int16_t i = 0; i < 128; i++)
    {
        if (G_Entities[i].Entitytype != ETYPE_NONE) Blit32BppBitmapToBuffer(&G_Entities[i].Sprite, CalcScreenX((int32_t)G_Entities[i].x - G_Entities[i].Sprite.BitmapInfo.bmiHeader.biWidth / 2), CalcScreenY((int32_t)G_Entities[i].y - G_Entities[i].Sprite.BitmapInfo.bmiHeader.biHeight / 2), G_Entities[i].Hurt, 255, 255, 255, 255);
    }
}

BOOL inline HitRectDect(_In_ float x1, _In_ float y1, _In_ float halfwidth1, _In_ float halfheight1, _In_ float x2, _In_ float y2, _In_ float halfwidth2, _In_ float halfheight2)
{
    return (y1 + halfheight1 >= y2 - halfheight2 && y1 - halfheight1 <= y2 + halfheight2 && x1 + halfwidth1 >= x2 - halfwidth2 && x1 - halfwidth1 <= x2 + halfwidth2);
}

void inline ProcessPlayer(void)
{
    static float PreviousHP = 0;
    static uint8_t Counter = 0;
    static BOOL StartCounter = FALSE;

    if (G_Player.HP < PreviousHP && Counter == 0) { G_Player.Hurt = 2; StartCounter = TRUE; }
    if (StartCounter) Counter++;
    if (Counter > 3) { Counter = 0; StartCounter = FALSE; G_Player.Hurt = 0; }

    if (G_Player.CurrentSpeed == G_Player.RunningSpeed && G_Player.Stamina > 0) G_Player.Stamina--;
    else if (G_Player.Stamina < 100) G_Player.Stamina += G_Player.StaminaRegen;
    if (G_Player.MP < 100) G_Player.MP += G_Player.MPRegen;
    if (G_Player.HP < 100 && G_Player.HP > 0) G_Player.HP += G_Player.HPRegen;

    if (G_Player.HP == 0) { TransitionScreen(GAMESTATE_DEATH, TRANSITION_FADE, 2); };

    PreviousHP = G_Player.HP;
}

void inline DrawOverWorld(void)
{
    __stosd(G_BackBuffer.Memory, 0x00000000, G_CanvasMemorySize / sizeof(PIXEL32));
    //                             AARRGGBB
    for (int32_t x = (int32_t)G_Player.WorldPosX / 32 - G_GameWidth / 64 - 1; x < (int32_t)G_Player.WorldPosX / 32 + G_GameWidth / 64 + 2; x++)
    {
        for (int32_t y = (int32_t)G_Player.WorldPosY / 32 - G_GameHeight / 64 - 1; y < (int32_t)G_Player.WorldPosY / 32 + G_GameHeight / 64 + 2; y++)
        {
            if (x >= 0 && y >= 0 && x < (int32_t)G_WorldMap.Width && y < (int32_t)G_WorldMap.Height)
            {
                BlitTileToBuffer(CalcScreenX(x * 32), CalcScreenY(y * 32), &G_WorldTileSet, G_WorldMap.Tiles[CalcWorldMapIndex((float)(x * 32), (float)(y * 32))]);
            }
        }
    }

    DrawPlayerSprite(G_Player);
    DrawEntities();
    DrawHudOnScreen();
}

void inline DrawHudOnScreen(void)
{
    DrawRect(G_GameWidth - 190, 10, G_GameWidth - 190 + (180 * (int8_t)G_Player.HP / 100), 24, 255, 0, 0, 255);
    DrawRect(G_GameWidth - 190, 26, G_GameWidth - 190 + (180 * (int8_t)G_Player.MP / 100), 40, 0, 0, 255, 255);
    DrawRect(G_GameWidth - 190, 42, G_GameWidth - 190 + (180 * (int8_t)G_Player.Stamina / 100), 56, 0, 255, 0, 255);
}

void inline DrawRect(_In_ int32_t x1, _In_ int32_t y1, _In_ int32_t x2, _In_ int32_t y2, _In_ uint8_t Red, _In_ uint8_t Green, _In_ uint8_t Blue, _In_ uint8_t Alpha)
{
    int32_t StartingScreenPixel = (G_GameWidth * G_GameHeight) - G_GameWidth - (G_GameWidth * y1) + x1;
    int32_t MemoryOffset = 0;
    PIXEL32 BackgroundPixel = { 0 };

    for (int XPixel = 0; XPixel < x2 - x1; XPixel++)
    {
        for (int YPixel = 0; YPixel < y2 - y1; YPixel++)
        {
            MemoryOffset = StartingScreenPixel + XPixel - (G_GameWidth * YPixel);
            if (x1 + XPixel >= 0 && x1 + XPixel < G_GameWidth && y1 + YPixel >= 0 && y1 + YPixel < G_GameHeight)
            {
                memcpy_s(&BackgroundPixel, sizeof(PIXEL32), (PIXEL32*)G_BackBuffer.Memory + MemoryOffset, sizeof(PIXEL32));
                if (BackgroundPixel.Alpha != Alpha || BackgroundPixel.Red != Red || BackgroundPixel.Green != Green || BackgroundPixel.Blue != Blue)
                {
                    BackgroundPixel.Red = (uint8_t)((float)Alpha / 255 * (float)Red + (1 - ((float)Alpha / 255)) * (float)BackgroundPixel.Red);
                    BackgroundPixel.Green = (uint8_t)((float)Alpha / 255 * (float)Green + (1 - ((float)Alpha / 255)) * (float)BackgroundPixel.Green);
                    BackgroundPixel.Blue = (uint8_t)((float)Alpha / 255 * (float)Blue + (1 - ((float)Alpha / 255)) * (float)BackgroundPixel.Blue);
                    memcpy_s((PIXEL32*)G_BackBuffer.Memory + MemoryOffset, sizeof(PIXEL32), &BackgroundPixel, sizeof(PIXEL32));
                }
            }
        }
    }
    return;
}

void inline DrawPlayerSprite(_In_ PLAYER VPlayer)
{
    switch (VPlayer.Dir)
    {
        case 0:
        {
            if (VPlayer.Walking[0] == FALSE)
            {
                Blit32BppBitmapToBuffer(&VPlayer.Sprite[2], VPlayer.ScreenPosX - VPlayer.Sprite->BitmapInfo.bmiHeader.biWidth / 2, VPlayer.ScreenPosY - VPlayer.Sprite->BitmapInfo.bmiHeader.biHeight / 2, VPlayer.Hurt, 255, 255, 255, 255);
            }
            else
            {
                Blit32BppBitmapToBuffer(&VPlayer.Sprite[3], VPlayer.ScreenPosX - VPlayer.Sprite->BitmapInfo.bmiHeader.biWidth / 2, VPlayer.ScreenPosY - VPlayer.Sprite->BitmapInfo.bmiHeader.biHeight / 2, VPlayer.Hurt, 255, 255, 255, 255);
            }
            break;
        }
        case 1:
        {
            if (VPlayer.Walking[1] == FALSE)
            {
                Blit32BppBitmapToBuffer(&VPlayer.Sprite[0], VPlayer.ScreenPosX - VPlayer.Sprite->BitmapInfo.bmiHeader.biWidth / 2, VPlayer.ScreenPosY - VPlayer.Sprite->BitmapInfo.bmiHeader.biHeight / 2, VPlayer.Hurt, 255, 255, 255, 255);
            }
            else
            {
                Blit32BppBitmapToBuffer(&VPlayer.Sprite[1], VPlayer.ScreenPosX - VPlayer.Sprite->BitmapInfo.bmiHeader.biWidth / 2, VPlayer.ScreenPosY - VPlayer.Sprite->BitmapInfo.bmiHeader.biHeight / 2, VPlayer.Hurt, 255, 255, 255, 255);
            }
            break;
        }
        case 2:
        {
            if (VPlayer.Walking[2] == FALSE)
            {
                Blit32BppBitmapToBuffer(&VPlayer.Sprite[4], VPlayer.ScreenPosX - VPlayer.Sprite->BitmapInfo.bmiHeader.biWidth / 2, VPlayer.ScreenPosY - VPlayer.Sprite->BitmapInfo.bmiHeader.biHeight / 2, VPlayer.Hurt, 255, 255, 255, 255);
            }
            else
            {
                Blit32BppBitmapToBuffer(&VPlayer.Sprite[5], VPlayer.ScreenPosX - VPlayer.Sprite->BitmapInfo.bmiHeader.biWidth / 2, VPlayer.ScreenPosY - VPlayer.Sprite->BitmapInfo.bmiHeader.biHeight / 2, VPlayer.Hurt, 255, 255, 255, 255);
            }
            break;
        }
        case 3:
        {
            if (VPlayer.Walking[3] == FALSE)
            {
                Blit32BppBitmapToBuffer(&VPlayer.Sprite[6], VPlayer.ScreenPosX - VPlayer.Sprite->BitmapInfo.bmiHeader.biWidth / 2, VPlayer.ScreenPosY - VPlayer.Sprite->BitmapInfo.bmiHeader.biHeight / 2, VPlayer.Hurt, 255, 255, 255, 255);
            }
            else
            {
                Blit32BppBitmapToBuffer(&VPlayer.Sprite[7], VPlayer.ScreenPosX - VPlayer.Sprite->BitmapInfo.bmiHeader.biWidth / 2, VPlayer.ScreenPosY - VPlayer.Sprite->BitmapInfo.bmiHeader.biHeight / 2, VPlayer.Hurt, 255, 255, 255, 255);
            }
            break;
        }
        case 4:
        {
            if (VPlayer.Walking[4] == FALSE)
            {
                Blit32BppBitmapToBuffer(&VPlayer.Sprite[2], VPlayer.ScreenPosX - VPlayer.Sprite->BitmapInfo.bmiHeader.biWidth / 2, VPlayer.ScreenPosY - VPlayer.Sprite->BitmapInfo.bmiHeader.biHeight / 2, VPlayer.Hurt, 255, 255, 255, 255);
            }
            else
            {
                Blit32BppBitmapToBuffer(&VPlayer.Sprite[3], VPlayer.ScreenPosX - VPlayer.Sprite->BitmapInfo.bmiHeader.biWidth / 2, VPlayer.ScreenPosY - VPlayer.Sprite->BitmapInfo.bmiHeader.biHeight / 2, VPlayer.Hurt, 255, 255, 255, 255);
            }
            break;
        }
        case 5:
        {
            if (VPlayer.Walking[5] == FALSE)
            {
                Blit32BppBitmapToBuffer(&VPlayer.Sprite[2], VPlayer.ScreenPosX - VPlayer.Sprite->BitmapInfo.bmiHeader.biWidth / 2, VPlayer.ScreenPosY - VPlayer.Sprite->BitmapInfo.bmiHeader.biHeight / 2, VPlayer.Hurt, 255, 255, 255, 255);
            }
            else
            {
                Blit32BppBitmapToBuffer(&VPlayer.Sprite[3], VPlayer.ScreenPosX - VPlayer.Sprite->BitmapInfo.bmiHeader.biWidth / 2, VPlayer.ScreenPosY - VPlayer.Sprite->BitmapInfo.bmiHeader.biHeight / 2, VPlayer.Hurt, 255, 255, 255, 255);
            }
            break;
        }
        case 6:
        {
            if (VPlayer.Walking[6] == FALSE)
            {
                Blit32BppBitmapToBuffer(&VPlayer.Sprite[4], VPlayer.ScreenPosX - VPlayer.Sprite->BitmapInfo.bmiHeader.biWidth / 2, VPlayer.ScreenPosY - VPlayer.Sprite->BitmapInfo.bmiHeader.biHeight / 2, VPlayer.Hurt, 255, 255, 255, 255);
            }
            else
            {
                Blit32BppBitmapToBuffer(&VPlayer.Sprite[5], VPlayer.ScreenPosX - VPlayer.Sprite->BitmapInfo.bmiHeader.biWidth / 2, VPlayer.ScreenPosY - VPlayer.Sprite->BitmapInfo.bmiHeader.biHeight / 2, VPlayer.Hurt, 255, 255, 255, 255);
            }
            break;
        }
        case 7:
        {
            if (VPlayer.Walking[7] == FALSE)
            {
                Blit32BppBitmapToBuffer(&VPlayer.Sprite[4], VPlayer.ScreenPosX - VPlayer.Sprite->BitmapInfo.bmiHeader.biWidth / 2, VPlayer.ScreenPosY - VPlayer.Sprite->BitmapInfo.bmiHeader.biHeight / 2, VPlayer.Hurt, 255, 255, 255, 255);
            }
            else
            {
                Blit32BppBitmapToBuffer(&VPlayer.Sprite[5], VPlayer.ScreenPosX - VPlayer.Sprite->BitmapInfo.bmiHeader.biWidth / 2, VPlayer.ScreenPosY - VPlayer.Sprite->BitmapInfo.bmiHeader.biHeight / 2, VPlayer.Hurt, 255, 255, 255, 255);
            }
            break;
        }
    }
}

int32_t inline CalcWorldMapIndex(float x, float y) { return G_WorldMap.Width * (int32_t)(y / 32) + (int32_t)(x / 32); }
int32_t inline CalcScreenX(_Inout_ int32_t x) { if (G_ResHeight == G_PerformanceData.MonitorHeight) return x - (int32_t)G_Player.WorldPosX + G_GameWidth  / 2; else return x - (int32_t)G_Player.WorldPosX + G_GameWidth  / 2 - 7; }
int32_t inline CalcScreenY(_Inout_ int32_t y) { if (G_ResHeight == G_PerformanceData.MonitorHeight) return y - (int32_t)G_Player.WorldPosY + G_GameHeight / 2; else return y - (int32_t)G_Player.WorldPosY + G_GameHeight / 2 - 7; }