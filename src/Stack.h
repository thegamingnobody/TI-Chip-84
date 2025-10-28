#pragma once

class Stack
{
public:
    void push(uint16_t addr) 
    {
        if (sp < MAX_DEPTH) 
        {
            stack[sp++] = addr;
        } else 
        {
            // Handle overflow (optional)
        }
    }

    uint16_t pop() 
    {
        if (sp > 0) 
        {
            return stack[--sp];
        } else 
        {
            // Handle underflow (optional)
            return 0;
        }
    }

    uint16_t top() const 
    {
        if (sp != 0) 
        {
            return stack[sp - 1];
        }
        return 0;
    }

    void reset() { sp = 0; }

    bool empty() const { return sp == 0; }
    bool full() const { return sp == MAX_DEPTH; }

private:
    static constexpr int MAX_DEPTH = 16;
    uint16_t stack[MAX_DEPTH];
    uint8_t sp = 0; // stack pointer

};