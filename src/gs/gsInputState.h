#ifndef GS_INPUT_STATE_H
#define GS_INPUT_STATE_H

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

        bool    exit;
        bool    pause;

        bool 	leftMouse;
        bool 	middleMouse;
        bool 	rightMouse;

        bool    mouseWheelUp;
        bool    mouseWheelDown;

    public:
        void	ResetAll();
        //void	Update( const EngineConfig &engineCfg );
        void	Update();

        bool 	GetUp() const;
        bool 	GetDown() const;
        bool 	GetLeft() const;
        bool 	GetRight() const;

        bool 	GetExit();
        bool    GetPause();

        bool 	GetLeftMouse() const;
        bool 	GetMiddleMouse() const;
        bool 	GetRightMouse() const;


        bool    GetMouseWheelUp();
        bool    GetMouseWheelDown();

    public:
        InputState(); //binding config
    };
}



#endif //GS_INPUT_STATE_H
