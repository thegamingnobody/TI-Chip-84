
#include "QuirkManager.h"

void QuirkManager::Init()
{
    m_Quirks.shiftQuirk = false;
    m_Quirks.loadStoreQuirkIncrement = false;
    m_Quirks.loadStoreQuirkUnchanged = false;
    m_Quirks.wrapQuirk = false;
    m_Quirks.jumpQuirk = false;
    m_Quirks.vblankQuirk = true;
    m_Quirks.vFResetQuirk = true;
}