//
// Created by adolin on 21. 12. 20.
//

#ifndef ARC_ENGINE_APPLICATION_H
#define ARC_ENGINE_APPLICATION_H

#include "core.h"

namespace arc{
     class Application {
    public:

        virtual void Init() = 0;
        virtual void Dispose() = 0;

        virtual void Update() = 0;

        virtual void ImGuiRender() = 0;



    };
    class ApplicationCaller: public Caller{
        typedef void(VoidMemberFn)(void*);
    public:
        inline void Create(Application* application){
            application_ = application;
            init_fn_ = (VoidMemberFn*)GetVTable<Application>(application)[0];
            dispose_fn_ = (VoidMemberFn*)GetVTable<Application>(application)[1];
            update_fn_ = (VoidMemberFn*)GetVTable<Application>(application)[2];
            imgui_render_fn_ = (VoidMemberFn*)GetVTable<Application>(application)[3];
        }
        ApplicationCaller() = default;
        ~ApplicationCaller() = default;
        inline void Init(){
            init_fn_(application_);
        }
        inline void Dispose(){
            dispose_fn_(application_);
        };

        inline void Update(){
            update_fn_(application_);
        };

        inline void ImGuiRender(){
            imgui_render_fn_(application_);
        };
        inline Application* application() const{return application_;}
    private:
        Application* application_;

        VoidMemberFn* init_fn_;
        VoidMemberFn* dispose_fn_;
        VoidMemberFn* update_fn_;
        VoidMemberFn* imgui_render_fn_;

    };
}



#endif //ARC_ENGINE_APPLICATION_H
