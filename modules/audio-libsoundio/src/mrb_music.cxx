#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include "moon/audio/libsoundio/mrb/music.hxx"
#include "moon/audio/libsoundio/music.hxx"
#include "moon/api.h"
#include "moon/intern.h"

MOON_C_API void
mmrb_music_init(mrb_state *mrb, struct RClass *mod)
{
}