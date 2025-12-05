#include "InputManager.h"
#include <ti/getcsc.h>
#include <graphx.h>
#include <keypadc.h>
#include <debug.h>

void InputManager::Init()
{
	m_KeysStateCurrent.resize(16, false);
    m_KeysStateLastFrame.resize(16, false);
	m_KeyPressedThisFrame = false;
 
    // which button values map to the keys
	// m_Keymap = { SDLK_x,  SDLK_1,  SDLK_2,  SDLK_3,   SDLK_q, SDLK_w, SDLK_e,    SDLK_a,   SDLK_s,    SDLK_d, SDLK_z, SDLK_c,  SDLK_4, SDLK_r,    SDLK_f, SDLK_v };
	// m_Keymap = {      0,       1,       2,       3,        4,      5,      6,         7,        8,         9,      a,      b,       c,      d,         e,      f };
    m_Keymap.emplace_back(KeyDef(2, sk_7));
    m_Keymap.emplace_back(KeyDef(1, sk_Math));
    m_Keymap.emplace_back(KeyDef(2, sk_Apps));
    m_Keymap.emplace_back(KeyDef(3, sk_Prgm));
    m_Keymap.emplace_back(KeyDef(1, sk_Recip));
    m_Keymap.emplace_back(KeyDef(2, sk_Sin));
    m_Keymap.emplace_back(KeyDef(3, sk_Cos));
    m_Keymap.emplace_back(KeyDef(1, sk_Square));
    m_Keymap.emplace_back(KeyDef(2, sk_Comma));
    m_Keymap.emplace_back(KeyDef(3, sk_LParen));
    m_Keymap.emplace_back(KeyDef(1, sk_Log));
    m_Keymap.emplace_back(KeyDef(3, sk_8));
    m_Keymap.emplace_back(KeyDef(4, sk_Vars));
    m_Keymap.emplace_back(KeyDef(4, sk_Tan));
    m_Keymap.emplace_back(KeyDef(4, sk_RParen));
    m_Keymap.emplace_back(KeyDef(4, sk_9));
}

void InputManager::Destroy()
{
    m_KeysStateCurrent.clear();
    m_KeysStateLastFrame.clear();
    m_Keymap.clear();
}

bool InputManager::ProcessInput()
{
	m_KeyPressedThisFrame = false;
	m_KeyReleasedThisFrame = false;

	m_KeysStateLastFrame = m_KeysStateCurrent;

    kb_Scan();  

    if ((kb_Data[6] & kb_Clear)) return false;

    for (int i = 0; i < static_cast<int>(m_Keymap.size()); i++)
    {
        bool isKeyPressed = (kb_Data[m_Keymap[i].Row] & m_Keymap[i].Mask) != 0;        
        bool wasKeyPressed = m_KeysStateLastFrame[i];
        
        if (isKeyPressed && !wasKeyPressed)
        {
            m_KeyPressedThisFrame = true;
        }
        
        if (!isKeyPressed && wasKeyPressed)
        {
            m_KeyReleasedThisFrame = true;
        }

        m_KeysStateCurrent[i] = isKeyPressed;
    }

	return true;
}

bool InputManager::IsKeyPressed(int key) const
{
	return m_KeysStateCurrent[key];
}

bool InputManager::IsKeyReleased(int key) const
{
    return m_KeysStateLastFrame[key] && !m_KeysStateCurrent[key];
}

void InputManager::SetKey(int key, bool newState)
{
    m_KeysStateCurrent[key] = newState;
	m_KeyPressedThisFrame = true;
}