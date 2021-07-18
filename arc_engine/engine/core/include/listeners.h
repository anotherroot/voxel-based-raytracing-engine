//
// Created by adolin on 22. 12. 20.
//

#ifndef ARC_ENGINE_LISTENERS_H
#define ARC_ENGINE_LISTENERS_H
#include "core.h"
namespace arc{
    class TouchListener{
        virtual bool OnTouchDown(int id, float x, float y){ return false; }

        virtual bool OnTouchUp(int id, float x, float y){ return false; }

        virtual bool OnTouchMove(int id, float dx, float dy){ return false; }

        virtual bool OnMultiGesture(float x, float y, float dist, float theta){ return false; }
    };

    class JoypadListener{
        //TODO: implement
    };

    class NativeListener{
        //TODO: implement
    };

    class MouseListener {
    public:
        virtual bool OnButtonDown(int button) { return false; }

        virtual bool OnButtonUp(int button) { return false; }

        virtual bool OnScroll(double x_offset, double y_offset) { return false;}

        virtual bool OnMove(double x_pos, double y_pos) { return false;}
    };

    class KeyboardListener {
    public:
        virtual bool OnKeyDown(int key, bool repeat) { return false; }

        virtual bool OnKeyUp(int key) { return false; }

        virtual bool OnChar(uint key_code) {return false;}
    };

    class WindowListener{
    public:
        virtual bool OnWindowResize(int width, int height){return false;}

        virtual bool OnWindowClose(){return false;}
    };
    //...
}
#endif //ARC_ENGINE_LISTENERS_H
