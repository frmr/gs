#include <iostream>

#include <SDL.h>
#include <GL/gl3w.h>

#include "gs/gsCamera.h"
#include "gs/gsGlobe.h"
#include "gs/gsInputState.h"
#include "gs/gsMatrixStack.h"

using std::cout;
using std::cerr;
using std::endl;

namespace gs
{
    bool InitSdl()
    {
        if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS ) < 0 )
        {
            cerr << "gs::InitSdl() in src/main.cpp: Failed to initialise SDL." << endl;
            return false;
        }

        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        return true;
    }

    bool InitWindow( SDL_Window** window, SDL_GLContext& context, const int screenWidth, const int screenHeight )
    {
        *window = SDL_CreateWindow( "gs", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

        if ( window == NULL )
        {
            cerr << "gs::InitWindow() in src/main.cpp: Failed to initialise SDL window." << endl;
            return false;
        }

        context = SDL_GL_CreateContext( *window );

        if ( context == NULL )
        {
            cerr << "gs::InitWindow() in src/main.cpp: Failed to create OpenGL context." << endl;
            return false;
        }

        SDL_GL_SetSwapInterval(1);

        return true;
    }

    bool InitOpenGl()
    {
        if ( gl3wInit() )
        {
            cerr << "gs::InitOpenGl() in src/main.cpp: Failed to initialise gl3w." << endl;
            return false;
        }

        if ( !gl3wIsSupported( 3, 2 ) )
        {
            cerr << "gs::InitOpenGl() in src/main.cpp: OpenGL 3.2 not supported." << endl;
            return false;
        }

        GLint maxSize;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
        cerr << "Tex size: " << maxSize << endl;

        glClearColor( 1.0f, 0.0f, 0.0f, 1.0f );
        //glViewport( 0, 0, 1024, 768 );

        return true;
    }

    void Render( SDL_Window* window, const gs::Camera& worldCamera, const gs::Camera& interfaceCamera, const gs::Globe globe )
    {
        glClear( GL_COLOR_BUFFER_BIT );

        globe.Draw( worldCamera );

        SDL_GL_SwapWindow( window );
    }
}

int main(int argc, char* argv[] )
{
    //gs::Config config( "gs.cfg" );

    bool running = true;

    if ( !gs::InitSdl() )
    {
        cerr << "main() in main.cpp: Failed to initialise SDL." << endl;
        running = false;
    }

    int screenWidth = 1024;
    int screenHeight = 768;

    SDL_Window* window = NULL;
    SDL_GLContext context = NULL;

    if ( !gs::InitWindow( &window, context, screenWidth, screenHeight ) )
    {
        cerr << "main() in main.cpp: Failed to initialise window." << endl;
        running = false;
    }

    if ( !gs::InitOpenGl() )
    {
        cerr << "main() in main.cpp: Failed to initialise OpenGL." << endl;
        running = false;
    }

    gs::Globe globe;
    gs::InputState input;

    gs::Camera worldCamera;
    worldCamera.SetOrthographic( -10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 100.0f );

    gs::Camera interfaceCamera;
    interfaceCamera.SetOrthographic( 0.0f, screenWidth, 0.0f, screenHeight, 0.0f, 10.0f );

    while ( running )
    {
        input.Update();
        running = !(input.GetExit() || input.GetPause());

        worldCamera.Update( input );
        globe.Update();

        gs::Render( window, worldCamera, interfaceCamera, globe );
    }

    SDL_GL_DeleteContext( context );
    SDL_DestroyWindow( window );
    SDL_Quit();

    return 0;
}
