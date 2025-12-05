#pragma once

#include "Singleton.h"  

struct Quirks  
{  
    bool shiftQuirk;  
    bool loadStoreQuirkIncrement;  
    bool loadStoreQuirkUnchanged;  
    bool wrapQuirk;  
    bool jumpQuirk;  
    bool vblankQuirk;  
    bool vFResetQuirk;
};  

class QuirkManager : public Singleton<QuirkManager>
{  
public:  
    void Init();

    Quirks& GetQuirks() { return m_Quirks; }
    void SetQuirks(const Quirks& quirks) { m_Quirks = quirks; }

    bool GetShiftQuirk() const { return m_Quirks.shiftQuirk; }
    void SetShiftQuirk(bool value) { m_Quirks.shiftQuirk = value; }

    bool GetLoadStoreQuirkIncrement() const { return m_Quirks.loadStoreQuirkIncrement; }
    void SetLoadStoreQuirkIncrement(bool value) { m_Quirks.loadStoreQuirkIncrement = value; }

    bool GetLoadStoreQuirkUnchanged() const { return m_Quirks.loadStoreQuirkUnchanged; }
    void SetLoadStoreQuirkUnchanged(bool value) { m_Quirks.loadStoreQuirkUnchanged = value; }
    
    bool GetWrapQuirk() const { return m_Quirks.wrapQuirk; }
    void SetWrapQuirk(bool value) { m_Quirks.wrapQuirk = value; }
    
    bool GetJumpQuirk() const { return m_Quirks.jumpQuirk; }
    void SetJumpQuirk(bool value) { m_Quirks.jumpQuirk = value; }
    
    bool GetVblankQuirk() const { return m_Quirks.vblankQuirk; }
    void SetVblankQuirk(bool value) { m_Quirks.vblankQuirk = value; }

    bool GetVfResetQuirk() const { return m_Quirks.vFResetQuirk; }
    void SetVfResetQuirk(bool value) { m_Quirks.vFResetQuirk = value; }
private:
    Quirks m_Quirks{};
    
};  