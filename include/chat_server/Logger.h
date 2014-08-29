#ifndef CHAT_SERVER_LOGGER_H
#define CHAT_SERVER_LOGGER_H

#if defined(CHAT_SERVER_LOGGER)
#ifdef _MSC_VER
#include <WinSock2.h>
#endif
#ifndef _ELPP_THREAD_SAFE
#define _ELPP_THREAD_SAFE
#endif
#include <easylogging++.h>
#define CS_LOG_DEBUG(args) LOG(DEBUG) << args
#define CS_LOG_INFO(args)  LOG(INFO) << args
#define CS_LOG_WARN(args)  LOG(WARNING) << args
#define CS_LOG_ERROR(args) LOG(ERROR) << args
#define CS_LOG_FATAL(args) LOG(FATAL) << args
#else
#define CS_LOG_DEBUG(args)
#define CS_LOG_INFO(args)
#define CS_LOG_WARN(args)
#define CS_LOG_ERROR(args)
#define CS_LOG_FATAL(args)
#endif

#endif /* CHAT_SERVER_LOGGER_H */
