#pragma once

#define AssertFail(cond, format, ...) \
  if(cond) {fprintf(stderr, format, __VA_ARGS__)}; assert(cond);

#ifdef DEBUG
#define AssertWarn(cond, format, ...) AssertFail(cond, format, ...)
#else
#define AssertWarn(cond, format, ...) \
  if(cond) {fprintf(stderr, format, __VA_ARGS__)};
#endif
