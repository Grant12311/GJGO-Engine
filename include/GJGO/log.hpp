#ifndef GJGO_LOG_H
#define GJGO_LOG_H

#include <iostream>
#include <memory>

#ifndef GJGO_BUILD_TARGET_DIST
    #define GJGO_LOG_INFO(x) GJGO::Log::logInfo(x)
    #define GJGO_LOG_WARN(x) GJGO::Log::logWarn(x)
    #define GJGO_LOG_ERROR(x) GJGO::Log::logError(x)

    #define GJGO_SET_LOG_LEVEL_INFO() GJGO::Log::level = GJGO::Log::Level::INFO
    #define GJGO_SET_LOG_LEVEL_WARN() GJGO::Log::level = GJGO::Log::Level::WARN
    #define GJGO_SET_LOG_LEVEL_ERROR() GJGO::Log::level = GJGO::Log::Level::ERROR
    #define GJGO_SET_LOG_LEVEL_NONE() GJGO::Log::level = GJGO::Log::Level::NONE
#else
    #define GJGO_LOG_INFO(x)
    #define GJGO_LOG_WARN(x)
    #define GJGO_LOG_ERROR(x)

    #define GJGO_SET_LOG_LEVEL_INFO()
    #define GJGO_SET_LOG_LEVEL_WARN()
    #define GJGO_SET_LOG_LEVEL_ERROR()
    #define GJGO_SET_LOG_LEVEL_NONE()
#endif // GJGO_BUILD_TARGET_DIST

#ifndef GJGO_BUILD_TARGET_DIST
    namespace GJGO
    {
        namespace Log
        {
            namespace Level
            {
                const int8_t INFO  = 0;
                const int8_t WARN  = 1;
                const int8_t ERROR = 2;
                const int8_t NONE  = 3;
            }

            inline int8_t level = Level::INFO;

            template<typename... Args>
            static void logInfo(Args... args)
            {
                if (level <= Level::INFO)
                {
                    ((std::cout << args), ...);
                    std::cout << std::endl;
                }
            }

            template<typename... Args>
            static void logWarn(Args... args)
            {
                if (level <= Level::WARN)
                {
                    ((std::cout << args), ...);
                    std::cout << std::endl;
                }
            }

            template<typename... Args>
            static void logError(Args... args)
            {
                if (level <= Level::ERROR)
                {
                    ((std::cout << args), ...);
                    std::cout << std::endl;
                }
            }
        }
    }
#endif // GJGO_BUILD_TARGET_DIST

#endif
