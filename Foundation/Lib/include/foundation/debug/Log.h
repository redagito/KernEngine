#pragma once

#include "foundation/debug/CLogger.h"

/**
* \brief Simple logging macros.
*/
#define KERN_INFO(x)                                                           \
  CLogger::CTempLogger(ELogLevel::Info, __FILE__, __LINE__, __FUNCTION__)      \
          .getStream()                                                         \
      << x
#define KERN_DEBUG(x)                                                          \
  CLogger::CTempLogger(ELogLevel::Debug, __FILE__, __LINE__, __FUNCTION__)     \
          .getStream()                                                         \
      << x
#define KERN_WARNING(x)                                                        \
  CLogger::CTempLogger(ELogLevel::Warning, __FILE__, __LINE__, __FUNCTION__)   \
          .getStream()                                                         \
      << x
#define KERN_ERROR(x)                                                          \
  CLogger::CTempLogger(ELogLevel::Error, __FILE__, __LINE__, __FUNCTION__)     \
          .getStream()                                                         \
      << x

#define KERN_STRIPPED_FILE_NAME                                                \
  (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define LOG_MESSAGE(fmt, ...)                                                  \
  CLogger::log(ELogLevel::Info, KERN_STRIPPED_FILE_NAME, __LINE__,             \
               __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...)                                                    \
  CLogger::log(ELogLevel::Debug, KERN_STRIPPED_FILE_NAME, __LINE__,            \
               __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)                                                     \
  CLogger::log(ELogLevel::Info, KERN_STRIPPED_FILE_NAME, __LINE__,             \
               __FUNCTION__, fmt, ##__VA_ARGS__)

#define LOG_WARNING(fmt, ...)                                                  \
  CLogger::log(ELogLevel::Warning, KERN_STRIPPED_FILE_NAME, __LINE__,          \
               __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)                                                    \
  CLogger::log(ELogLevel::Error, KERN_STRIPPED_FILE_NAME, __LINE__,            \
               __FUNCTION__, fmt, ##__VA_ARGS__)
