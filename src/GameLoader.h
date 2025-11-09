#pragma once

#include "Singleton.h"
#include "TypeDefinitions.h"
#include <TINYSTL/vector.h>
#include <TINYSTL/string.h>

class GameLoader : public Singleton<GameLoader>
{
public:
    void ListChip8Games();
  	bool IsChip8Game(const char* appVarName);

    void ChangeCursorIndex(int offset);

    void LoadSelectedGame();

    tinystl::string GetSelectedGameName();

    void OutputScreen();
private:
    int CursorIndex{0};

    tinystl::vector<tinystl::string> AppvarNames;
};