//
// Created by adolin on 21. 12. 20.
//

#ifndef ARC_ENGINE_CORE_H
#define ARC_ENGINE_CORE_H
#define ARC_ENABLE_ASSERTS


#ifdef ARC_ENABLE_ASSERTS
#include "log.h"
#define arc_core_assert(x, ...) { if(!(x)){ logce("Assertion Failed: {0}", __VA_ARGS__); throw std::exception();}}
#define arc_assert(x, ...) { if(!(x)){ loge("Assertion Failed: {0}", __VA_ARGS__); throw std::exception();}}
#else
#define ARC_CORE_ASSERT(x, ...)
#define ARC_ASSERT(x, ...)
#endif
namespace arc{
    class Caller{
    protected:
        template<typename T>
        static void** GetVTable(T* obj){
            return *((void***)obj);
        }
    };
    using uint = uint32_t;
}


#endif //ARC_ENGINE_CORE_H
