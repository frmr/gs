#include "gsInputState.h"

void gs::InputState::ResetAll()
{
	up = false;
	down = false;
	left = false;
	right = false;

    exit = false;
    pause = false;

	leftMouse = false;
	middleMouse = false;
	rightMouse = false;

	mouseWheelUp = false;
	mouseWheelDown = false;
}

void gs::InputState::ResetPresses()
{
    exit = false;
    pause = false;
    mouseWheelUp = false;
    mouseWheelDown = false;
}

//void InputState::Update( const EngineConfig &engineCfg )
void gs::InputState::Update()
{
    ResetPresses();
	SDL_Event event;

	while ( SDL_PollEvent( &event ) )
    {
        if ( event.type == SDL_KEYUP )
        {
            if ( event.key.keysym.sym == SDLK_w )           { up = false; }
            else if ( event.key.keysym.sym == SDLK_s )      { down = false; }
            else if ( event.key.keysym.sym == SDLK_a )      { left = false; }
            else if ( event.key.keysym.sym == SDLK_d )      { right = false; }
            else if ( event.key.keysym.sym == SDLK_ESCAPE ) { pause = false; }
        }
        else if ( event.type == SDL_KEYDOWN )
        {
            if ( event.key.keysym.sym == SDLK_w )           { up = true; }
            else if ( event.key.keysym.sym == SDLK_s )      { down = true; }
            else if ( event.key.keysym.sym == SDLK_a )      { left = true; }
            else if ( event.key.keysym.sym == SDLK_d )      { right = true; }
            else if ( event.key.keysym.sym == SDLK_ESCAPE ) { pause = true; }
        }
        else if ( event.type == SDL_MOUSEBUTTONUP )
        {
            if ( event.button.button == SDL_BUTTON_LEFT )           { leftMouse = false; }
            else if ( event.button.button == SDL_BUTTON_MIDDLE )    { middleMouse = false; }
            else if ( event.button.button == SDL_BUTTON_RIGHT )     { rightMouse = false; }
        }
        else if ( event.type == SDL_MOUSEBUTTONDOWN )
        {
            if ( event.button.button == SDL_BUTTON_LEFT )           { leftMouse = true; }
            else if ( event.button.button == SDL_BUTTON_MIDDLE )    { middleMouse = true; }
            else if ( event.button.button == SDL_BUTTON_RIGHT )     { rightMouse = true; }
        }
        else if ( event.type == SDL_MOUSEWHEEL )
        {
            if ( event.wheel.y > 0 )    { mouseWheelUp = true; }
            else                        { mouseWheelDown = true; }
        }
        else if ( event.type == SDL_QUIT )
        {
            exit = true;
        }
    }
}

bool gs::InputState::GetUp() const      { return up; }
bool gs::InputState::GetDown() const	{ return down; }
bool gs::InputState::GetLeft() const	{ return left; }
bool gs::InputState::GetRight() const	{ return right; }

bool gs::InputState::GetExit() const    { return exit; }
bool gs::InputState::GetPause() const   { return pause; }

bool gs::InputState::GetLeftMouse() const   { return leftMouse; }
bool gs::InputState::GetMiddleMouse() const { return middleMouse; }
bool gs::InputState::GetRightMouse() const  { return rightMouse; }

bool gs::InputState::GetMouseWheelUp() const    { return mouseWheelUp; }
bool gs::InputState::GetMouseWheelDown() const  { return mouseWheelDown; }

gs::InputState::InputState()
{
	ResetAll();
}
