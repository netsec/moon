#ifndef MOON_ENGINE_H
#define MOON_ENGINE_H

#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include "unistd.h"
#include "dirent.h"
#include <string.h>

/* OpenGL */
#include <GL/gl.h>
#include <GLFW/glfw3.h>

/* Include the mruby header */
#include <mruby.h>
#include <mruby/array.h>
#include <mruby/proc.h>
#include <mruby/data.h>
#include <mruby/compile.h>
#include <mruby/variable.h>
#include <mruby/string.h>

#include "input.hxx"
#include "texture.hxx"
#include "sprite.hxx"
#include "spritesheet.hxx"
#include "fps.hxx"
#include "audio.hxx"
#include "music.hxx"
#include "sound.hxx"

void moon_init_mrb_core(mrb_state *mrb); // core side
void moon_init_mrb_ext(mrb_state *mrb);

namespace Moon {
  class Engine {
  public:
    Engine();
    ~Engine();
    void run();

  private:
    mrb_state *mrb;
    mrbc_context *mrb_context;
    //mrb_value game_object;

    GLFWwindow*  window; // GLFW window

    void load_mrb();
    bool load_mrb_file(const char*, const char*);
    void load_core_classes();
    void load_user_scripts();

    void setup_glfw();
    void setup_opengl();
  };
};
#endif