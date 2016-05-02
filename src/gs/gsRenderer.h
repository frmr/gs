#ifndef GS_RENDERER_H
#define GS_RENDERER_H

#include <cstdint>

#include <SDL.h>
#include "../gl3w/gl3w.h"

#include "gsCamera.h"
#include "gsFullscreenQuad.h"
#include "gsGlobe.h"
#include "gsUserInterface.h"

namespace gs
{
    class Renderer
    {
    private:
        int                         width;
        int                         height;
        float                       aspectRatio;

        bool                        fullscreen;
        bool                        vsync;

        int64_t                     time;

        //gs::Globe                   globe; //last globe state

        SDL_GLContext               context;

        const GLuint                globeFbo;
        const GLuint                globeTex;
        const GLuint                sceneFbo;
        const GLuint                sceneTex;

        const gs::FullscreenQuad    quad;

    private:
        void InitOpenGl() const;
        void RenderScene() const;
        void RenderUi() const;

    public:
        bool SetResolution( const int width, const int height );
        void Render( const gs::Camera& worldCamera, const gs::Camera& interfaceCamera, const gs::UserInterface& ui ) const;
        //void Update( const gs::Globe& globe );

    public:
        Renderer( const SDL_GLContext &context, const bool fullscreen, const bool vsync );
    };
}

#endif
