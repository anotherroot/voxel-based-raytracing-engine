//
//
// Created by adolin on 21. 12. 20.
//

#include "log.h"
namespace arc{
    namespace impl {


        std::shared_ptr<spdlog::logger> Log::s_core_logger_;
        std::shared_ptr<spdlog::logger> Log::s_client_logger_;

        void Log::Init() {
            spdlog::set_pattern("%^[%T] %n: %v%$");
#ifdef ARC_ANDROID
            //TODO: log for android
            std::string tag = "spdlog-android";
            s_core_logger_ = spdlog::android_logger_mt("android",tag);

            s_client_logger_ = s_core_logger_;
#else
            s_core_logger_ = spdlog::stdout_color_mt("ARC");
            s_core_logger_->set_level(spdlog::level::trace);


            s_client_logger_ = spdlog::stdout_color_mt("APP");
            s_client_logger_->set_level(spdlog::level::trace);
#endif

        }
        /* void Log::Info( */
    }
}
