#ifndef _CONSOLEOUTPUT_H_
#define _CONSOLEOUTPUT_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __FUNCTION__
#define TRACESTR "[%s:%s:%d]: "
#define TRACEARGS __FILE__ , __FUNCTION__ , __LINE__
#elif defined __FUNC__
#define TRACESTR "[%s:%s:%d]: "
#define TRACEARGS __FILE__ , __FUNC__ , __LINE__
#elif defined(__func__)
#define TRACESTR "[%s:%s:%d]: "
#define TRACEARGS __FILE__ , __func__ , __LINE__
#else
#define TRACESTR "[%s:%d]: "
#define TRACEARGS __FILE__ , __LINE__
#endif

#define errorf(fmt, args...) printf(fmt , ##args)
//#define errorf(fmt, args...)
#define errortf(fmt, args...) errorf(TRACESTR fmt , TRACEARGS , ##args)
#define debugf(fmt, args...) printf(fmt , ##args)
//#define debugf(fmt, args...)
#define infotf(fmt, args...) debugf(TRACESTR fmt , TRACEARGS , ##args)
#define infof(fmt, args...) printf(fmt , ##args)
//#define infof(fmt, args...)
#define infotf(fmt, args...) infof(TRACESTR fmt , TRACEARGS , ##args)

#ifdef __cplusplus
}
#endif

#endif