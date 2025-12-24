
#include "GameLoader.h"

#include <tice.h>
#include <fileioc.h>
#include <graphx.h>

GameLoader::GameLoader()
{
    
}

void GameLoader::ListChip8Games()
{
    void* vat_ptr = NULL;
    char* var_name;

    while ((var_name = ti_DetectVar(&vat_ptr, NULL, OS_TYPE_APPVAR))) 
    {
        if (IsChip8Game(var_name)) 
        {
            if (AppvarNames.size() >= 8) break;
            
            AppvarNames.emplace_back(var_name);
        }    
    }

    OutputScreen();
}


bool GameLoader::IsChip8Game(const char* appVarName)
{
    uint8_t handle;

    handle = ti_OpenVar(appVarName, "r", OS_TYPE_APPVAR);
    if (!handle) 
    {
        return false;
    }

    int const numberOfSignatureCharacters{6};

    // the converter writes "Chip84" into the file, so check for this to see if the file is a runnable file or not
    uint8_t signature[numberOfSignatureCharacters];
    size_t read = ti_Read(signature, 1, sizeof(signature), handle);
    ti_Close(handle);

    const uint8_t expected[numberOfSignatureCharacters] = { 'C','h','i','p','8','4' };

    for (int i = 0; i < numberOfSignatureCharacters; i++)
    {
        if (signature[i] != expected[i]) return false;
    }

    return true;
}

void GameLoader::ChangeCursorIndex(int offset)
{
    int newCursorIndex{CursorIndex + offset};

    if (newCursorIndex < 0)
    {
        newCursorIndex = 0;
    }else if (newCursorIndex >= static_cast<int>(AppvarNames.size()))
    {
        newCursorIndex = static_cast<int>(AppvarNames.size()) - 1;
    }

    CursorIndex = newCursorIndex;

    OutputScreen();
}

tinystl::string GameLoader::GetSelectedGameName()
{
    if (CursorIndex >= static_cast<int>(AppvarNames.size())) return "";
    
    return AppvarNames[CursorIndex];
}

void GameLoader::OutputScreen()
{
    gfx_FillScreen(0xFF);

    const char* pointer = ">";
    gfx_PrintStringXY(pointer, 0, CursorIndex*10);

    for (int i{0}; i < static_cast<int>(AppvarNames.size()); i++)
    {
        gfx_PrintStringXY(AppvarNames[i].c_str(), 10, i*10);
    }   
}