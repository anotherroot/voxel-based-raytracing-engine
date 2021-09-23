//
// Created by adolin on 21. 12. 20.
//

#ifndef ARC_ENGINE_LOG_H
#define ARC_ENGINE_LOG_H

#ifdef ARC_ANDROID
#include <spdlog/spdlog.h>
#include "spdlog/sinks/android_sink.h"
#else
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks-inl.h>
#include <spdlog/fmt/ostr.h>
#endif


namespace arc{
    namespace impl{
        class Log {
        public:
            static void Init();

            inline static std::shared_ptr<spdlog::logger>& core_logger(){
                return s_core_logger_;
            }
            inline static std::shared_ptr<spdlog::logger>& client_logger(){
                return s_client_logger_;
            }
            /* static void Info( */


        private:
            static std::shared_ptr<spdlog::logger> s_core_logger_;
            static std::shared_ptr<spdlog::logger> s_client_logger_;
        };
    }
}
//
#define ARC_DEBUG //TODO: put this into a config file od somekind
#ifdef ARC_DEBUG
#define LOG_INIT()      arc::impl::Log::Init()


#define logct(...)     arc::impl::Log::core_logger()->trace(__VA_ARGS__)
#define logci(...)      arc::impl::Log::core_logger()->info(__VA_ARGS__)
#define logcw(...)      arc::impl::Log::core_logger()->warn(__VA_ARGS__)
#define logce(...)     arc::impl::Log::core_logger()->error(__VA_ARGS__)
#define logcf(...)     arc::impl::Log::core_logger()->fatal(__VA_ARGS__)

#define logt(...)     arc::impl::Log::client_logger()->trace(__VA_ARGS__)
#define logi(...)      arc::impl::Log::client_logger()->info(__VA_ARGS__)
#define logw(...)      arc::impl::Log::client_logger()->warn(__VA_ARGS__)
#define loge(...)     arc::impl::Log::client_logger()->error(__VA_ARGS__)
/* #define logf(...)     arc::impl::Log::client_logger()->fatal(__VA_ARGS__) */

#else

#define LOG_INIT()

#define logct(...)
#define logci(...)
#define logcw(...)
#define logce(...)
#define logcf(...)

#define logt(...)
#define logi(...)
#define logw(...)
#define loge(...)
#define logf(...)

#endif

#endif //ARC_ENGINE_LOG_H
