#pragma once

#include <SDL.h>

//#include "gsConfig.h"

namespace gs
{
   class InputState
    {
    private:
        bool 	up;
        bool 	down;
        bool 	left;
        bool 	right;

        bool    shift;

        bool    exit;
        bool    pause;

        bool 	leftMouse;
        bool 	middleMouse;
        bool 	rightMouse;

        bool    mouseWheelUp;
        bool    mouseWheelDown;

    public:
        void	ResetAll();
        void    ResetPresses();
        //void	Update(const EngineConfig &engineCfg);
        void	Update();

        bool 	GetUp() const;
        bool 	GetDown() const;
        bool 	GetLeft() const;
        bool 	GetRight() const;

        bool    GetShift() const;

        bool 	GetExit() const;
        bool    GetPause() const;

        bool 	GetLeftMouse() const;
        bool 	GetMiddleMouse() const;
        bool 	GetRightMouse() const;


        bool    GetMouseWheelUp() const;
        bool    GetMouseWheelDown() const;

    public:
        InputState(); //binding config
    };
}