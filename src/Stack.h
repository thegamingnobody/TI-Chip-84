#pragma once
#include <TINYSTL/vector.h>

class Stack
{
public:
    Stack()
    {
        stack.resize(MAX_DEPTH);
    }

    void push(uint16_t addr) 
    {
        if (sp < MAX_DEPTH) 
        {
            stack[sp++] = addr;
        } 
        else 
        {
            // Handle overflow
        }
    }

    uint16_t pop() 
    {
        if (sp > 0) 
        {
            return stack[--sp];
        } 
        else 
        {
            // Handle underflow
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
    tinystl::vector<uint16_t> stack;
    uint8_t sp = 0;

};