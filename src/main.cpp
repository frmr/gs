#include <iostream>

#include <SDL.h>
#include <gl/gl.h>

using std::cerr;
using std::endl;

namespace gs
{
    bool InitWindow( SDL_Window* window, const int screenWidth, const int screenHeight )
    {
        window = SDL_CreateWindow( "gs", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

        if ( window == NULL )
        {
            cerr << "gs::InitWindow() in src/main.cpp: Failed to initialise SDL window." << endl;
            return false;
        }

        if ( SDL_GL_CreateContext( window ) == NULL )
        {
            cerr << "gs::InitWindow() in src/main.cpp: Failed to create OpenGL context." << endl;
            return false;
        }

        return true;
    }

    bool InitOpenGl()
    {
        glClearColor( 1.0f, 0.0f, 0.0f, 1.0 );
        return true;
    }

    void Draw( SDL_Window* window )
    {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        SDL_GL_SwapWindow( window );
    }
}



int main(int argc, char* argv[] )
{
    SDL_Init( SDL_INIT_VIDEO );

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    int screenWidth = 1024;
    int screenHeight = 768;

    SDL_Window* window = NULL;
    gs::InitWindow( window, screenWidth, screenHeight );
    gs::InitOpenGl();

    while (1)
    {
        gs::Draw( window );
    }

    SDL_DestroyWindow( window );
    SDL_Quit();

    return 0;
}
