//
// Created by adolin on 22. 12. 20.
//

#ifndef ARC_ENGINE_OPENGL_CONTEXT_H
#define ARC_ENGINE_OPENGL_CONTEXT_H

namespace arc{
    class GraphicsContext{
    public:
        void Setup(void* window_handle);
        void Dispose();
        ~GraphicsContext(){Dispose();}
        operator bool() const {return window_handle_;}

        void Init();

        void SwapBuffers();


    private:
        void* window_handle_{nullptr};
    };
}


#endif //ARC_ENGINE_OPENGL_CONTEXT_H
