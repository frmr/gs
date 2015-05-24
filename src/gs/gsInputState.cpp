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
}

//void InputState::Update( const EngineConfig &engineCfg )
void gs::InputState::Update()
{
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

bool gs::InputState::GetExit()
{
    bool old = exit;
    exit = false;
    return old;
}

bool gs::InputState::GetPause()
{
    bool old = pause;
    pause = false;
    return old;
}

bool gs::InputState::GetLeftMouse() const   { return leftMouse; }
bool gs::InputState::GetMiddleMouse() const { return middleMouse; }
bool gs::InputState::GetRightMouse() const  { return rightMouse; }

gs::InputState::InputState()
{
	ResetAll();
}
