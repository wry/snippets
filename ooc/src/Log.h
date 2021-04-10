#ifndef LOG_H
#define LOG_H

typedef enum
{
    LogLevelTrace,
    LogLevelDebug,
} LogLevel;

typedef struct Log
{
    struct Log *(*init)(struct Log *self);
    void (*deinit)(struct Log *self);

    struct Log *(*t)(struct Log *self, const char *fl, int ln, const char *fn);
    struct Log *(*d)(struct Log *self, const char *fl, int ln, const char *fn, const char *fmt, ...);
} Log;

Log *Log_alloc(Log **self);
void Log_dealloc(Log **self);

void Log_static_log(LogLevel ll, const char *fl, int ln, const char *fn, const char *fmt, ...);

#define S_LOG_T() Log_static_log(LogLevelTrace, __FILE__, __LINE__, __func__, "\n")
#define S_LOG_D(F,...) Log_static_log(LogLevelDebug, __FILE__, __LINE__, __func__, F"\n", ##__VA_ARGS__)

#define LOG_T(L) L->t(L, __FILE__, __LINE__, __func__)
#define LOG_D(L,F,...) L->d(L, __FILE__, __LINE__, __func__, F"\n", ##__VA_ARGS__)

#endif /* ifndef LOG_H */
