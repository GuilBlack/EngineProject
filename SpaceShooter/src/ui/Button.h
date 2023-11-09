#pragma once
#include "DXHScripting.h"

typedef void(*Callback)();

class Button : public DXH::Script
{
public:
    void Update(const DXH::Timer& gt) override;

    void SetStartCallback(Callback func) { m_StartCallback = func; }
    void SetExitCallback(Callback func) { m_ExitCallback = func; }
    bool IsInStart(DXH::Vector2 tmp);
    bool IsInExit(DXH::Vector2 tmp);
private:
    Callback m_StartCallback = nullptr;
    Callback m_ExitCallback = nullptr;

};