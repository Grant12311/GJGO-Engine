#ifndef GJGO_LOG_H
#define GJGO_LOG_H

#include <iostream>

#ifndef GJGO_BUILD_CONFIG_DIST
    #define GJGO_LOG_INFO(x...) GJGO::Log::logInfo(__FILE__, __PRETTY_FUNCTION__, __LINE__, x)
    #define GJGO_LOG_WARN(x...) GJGO::Log::logWarn(__FILE__, __PRETTY_FUNCTION__, __LINE__, x)
    #define GJGO_LOG_ERROR(x...) GJGO::Log::logError(__FILE__, __PRETTY_FUNCTION__, __LINE__, x)

    #define GJGO_SET_LOG_LEVEL_INFO() GJGO::Log::level = GJGO::Log::Level::INFO
    #define GJGO_SET_LOG_LEVEL_WARN() GJGO::Log::level = GJGO::Log::Level::WARN
    #define GJGO_SET_LOG_LEVEL_ERROR() GJGO::Log::level = GJGO::Log::Level::ERROR
    #define GJGO_SET_LOG_LEVEL_NONE() GJGO::Log::level = GJGO::Log::Level::NONE

    #define GJGO_LOG_SET_PRINT_FILE(x) GJGO::Log::printFile = x
    #define GJGO_LOG_SET_PRINT_FUNCTION(x) GJGO::Log::printFunction = x
    #define GJGO_LOG_SET_PRINT_LINE(x) GJGO::Log::printLine = x
#else
    #define GJGO_LOG_INFO(x...)
    #define GJGO_LOG_WARN(x...)
    #define GJGO_LOG_ERROR(x...)

    #define GJGO_SET_LOG_LEVEL_INFO()
    #define GJGO_SET_LOG_LEVEL_WARN()
    #define GJGO_SET_LOG_LEVEL_ERROR()
    #define GJGO_SET_LOG_LEVEL_NONE()

    #define GJGO_LOG_SET_PRINT_FILE(x)
    #define GJGO_LOG_SET_PRINT_FUNCTION(x)
    #define GJGO_LOG_SET_PRINT_LINE(x)
#endif // GJGO_BUILD_TARGET_DIST

#ifndef GJGO_BUILD_CONFIG_DIST
    namespace GJGO
    {
        namespace Log
        {
            enum class Level : unsigned char
            {
                INFO = 0,
                WARN,
                ERROR,
                NONE
            };

            inline Level level = Level::INFO;
            inline bool printFile = false;
            inline bool printFunction = false;
            inline bool printLine = false;

            template<typename... Args>
            static void logInfo(const char* const a_file, const char* const a_function, const int a_line, Args... args)
            {
                if (level <= Level::INFO)
                {
                    if (printFile)
                        std::cout << "{" << a_file << "}";
                    if (printFunction)
                        std::cout << "[" << a_function << "]";
                    if (printLine)
                        std::cout << "(" << a_line << ")";

                    if (printFile || printFunction || printLine)
                        std::cout << ": ";

                    ((std::cout << args), ...);
                    std::cout << std::endl;
                }
            }

            template<typename... Args>
            static void logWarn(const char* const a_file, const char* const a_function, const int a_line, Args... args)
            {
                if (level <= Level::WARN)
                {
                    if (printFile)
                        std::cout << "{" << a_file << "}";
                    if (printFunction)
                        std::cout << "[" << a_function << "]";
                    if (printLine)
                        std::cout << "(" << a_line << ")";

                    if (printFile || printFunction || printLine)
                        std::cout << ": ";

                    ((std::cout << args), ...);
                    std::cout << std::endl;
                }
            }

            template<typename... Args>
            static void logError(const char* const a_file, const char* const a_function, const int a_line, Args... args)
            {
                if (level <= Level::ERROR)
                {
                    if (printFile)
                        std::cout << "{" << a_file << "}";
                    if (printFunction)
                        std::cout << "[" << a_function << "]";
                    if (printLine)
                        std::cout << "(" << a_line << ")";

                    if (printFile || printFunction || printLine)
                        std::cout << ": ";

                    ((std::cout << args), ...);
                    std::cout << std::endl;
                }
            }
        }
    }
#endif // GJGO_BUILD_CONFIG_DIST

#endif
