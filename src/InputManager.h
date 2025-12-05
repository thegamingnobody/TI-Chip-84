#pragma once

#include "Singleton.h"
#include "TypeDefinitions.h"
#include <TINYSTL/vector.h>
#include <TINYSTL/string.h>

struct KeyDef {
    uint8_t Row;
    uint8_t Mask;

    KeyDef(uint8_t row, uint8_t mask) 
        : Row(row)
        , Mask(mask)
    {}

    KeyDef(int row, int mask)
        : Row(static_cast<uint8_t>(row))
        , Mask(static_cast<uint8_t>(mask))
    {}
};

class InputManager final : public Singleton<InputManager>
{
public:
    void Init();
    bool ProcessInput();

    void Destroy();
    
    bool IsKeyPressed(int key) const;
    bool IsKeyReleased(int key) const;

    bool IsKeyPressedThisFrame() const { return m_KeyPressedThisFrame; }
    bool isKeyReleasedThisFrame() const { return m_KeyReleasedThisFrame; }

    void RenderImGui(tinystl::string windowName);

    int GetNumberOfKeys() const { return m_Keymap.size(); }
private:
    void SetKey(int key, bool newState);

    tinystl::vector<bool> m_KeysStateCurrent;
    tinystl::vector<bool> m_KeysStateLastFrame;
    tinystl::vector<KeyDef> m_Keymap;



    bool m_KeyPressedThisFrame{};
    bool m_KeyReleasedThisFrame{};
};