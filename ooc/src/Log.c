#include "Log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

static Log *Log_init(Log *self);
static void Log_deinit(Log *self);

static Log *Log_t(struct Log *self, const char *fl, int ln, const char *fn);
static Log *Log_d(struct Log *self, const char *fl, int ln, const char *fn, const char *fmt, ...);

static const char * Log_static_l2s(LogLevel ll);

Log *Log_alloc(Log **self)
{
    *self = calloc(1, sizeof(Log));

    (*self)->init = Log_init;
    (*self)->deinit = Log_deinit;

    (*self)->t = Log_t;
    (*self)->d = Log_d;

    return *self;
}

void Log_dealloc(Log **self)
{
    Log_deinit(*self);
    free(*self);
    *self = NULL;
}

Log *Log_init(Log *self)
{
    return self;
}

void Log_deinit(Log *self)
{
}

Log *Log_t(struct Log *self, const char *fl, int ln, const char *fn)
{
    printf("[%s] %s:%d (%s)\n", Log_static_l2s(LogLevelTrace), fl, ln, fn);
    return self;
}

Log *Log_d(struct Log *self, const char *fl, int ln, const char *fn, const char *fmt, ...)
{
    printf("[%s] %s:%d (%s) ", Log_static_l2s(LogLevelDebug), fl, ln, fn);
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    return self;
}

const char * Log_static_l2s(LogLevel ll)
{
    const char *llstr = NULL;
    switch (ll)
    {
        case LogLevelTrace:
            llstr = "T";
            break;
        case LogLevelDebug:
            llstr = "D";
            break;
        default:
            llstr = "U";
            break;
    }
    return llstr;
}

void Log_static_log(LogLevel ll, const char *fl, int ln, const char *fn, const char *fmt, ...)
{
    printf("[%s] %s:%d (%s) ", Log_static_l2s(ll), fl, ln, fn);
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}
