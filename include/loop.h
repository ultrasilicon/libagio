#ifndef AGIO_LOOP_H
#define AGIO_LOOP_H

#include "agio.h"

A_NS_BEGIN

enum Mode {
  Async = 0,
  Sync = 1
};

class Loop
    : public AgioObject<uv_loop_t, Loop, GCRule::Manual>
{
public:
  static Loop* defaultLoop();
  static Loop default_loop;

  Loop();
  Loop(uv_loop_t* l);
  ~Loop();

  int run(const uv_run_mode& mode = UV_RUN_DEFAULT);
  void close();
  int tryClose();

};




A_NS_END
#endif // AGIO_LOOP_H
