#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include <mruby/string.h>
#include "moon/mrb/rect.hxx"
#include "moon/mrb/vector2.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"
#include "moon/mrb/vertex_buffer.hxx"
#include "moon/mrb/helpers.hxx"
#include "moon/vertex_buffer.hxx"
#include "moon/api.h"

static void
vbo_free(mrb_state *mrb, void *p)
{
  Moon::VertexBuffer *vbo = (Moon::VertexBuffer*)p;
  if (vbo) {
    delete(vbo);
  }
}

MOON_C_API const struct mrb_data_type vbo_data_type = { "Moon::VertexBuffer", vbo_free };

static mrb_value
vbo_initialize(mrb_state *mrb, mrb_value self)
{
  GLenum usage;
  Moon::VertexBuffer *vbo;
  mrb_get_args(mrb, "i", &usage);

  vbo_free(mrb, DATA_PTR(self));
  DATA_PTR(self) = NULL;

  vbo = new Moon::VertexBuffer(usage);
  mrb_data_init(self, vbo, &vbo_data_type);
  return self;
}

static mrb_value
vbo_clear(mrb_state *mrb, mrb_value self)
{
  mmrb_vertex_buffer_ptr(mrb, self)->Clear();
  return self;
}

static mrb_value
vbo_render(mrb_state *mrb, mrb_value self)
{
  GLenum mode;
  GLuint offset;
  offset = 0;
  mrb_get_args(mrb, "i|i", &mode, &offset);

  mmrb_vertex_buffer_ptr(mrb, self)->Render(mode, offset);
  return self;
}

static mrb_value
vbo_push_back(mrb_state *mrb, mrb_value self)
{
  Moon::Vector2 *pos;
  Moon::Vector2 *tex_coord;
  Moon::Vector4 *color;
  mrb_get_args(mrb, "ddd",
    &pos, &vbo_data_type,
    &tex_coord, &vbo_data_type,
    &color, &vbo_data_type
  );
  mmrb_vertex_buffer_ptr(mrb, self)->PushBack(Moon::Vertex(*pos, *tex_coord, *color));
  return self;
}

static mrb_value
vbo_push_indices(mrb_state *mrb, mrb_value self)
{
  mrb_int length;
  mrb_value *values;
  mrb_get_args(mrb, "a", &values, &length);
  // hell no, am I gonna malloc an array for this just to push it at once.
  // its a bit ugly to be resizing it each time though...
  for (int i = 0; i < length; ++i) {
    GLuint index = mrb_int(mrb, values[i]);
    mmrb_vertex_buffer_ptr(mrb, self)->PushBackIndices(&index, 1);
  }
  return self;
}

static mrb_value
vbo_vertex_count(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mmrb_vertex_buffer_ptr(mrb, self)->GetVertexCount());
}

static mrb_value
vbo_index_count(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mmrb_vertex_buffer_ptr(mrb, self)->GetIndexCount());
}

static mrb_value
vbo_add_quad(mrb_state *mrb, mrb_value self)
{
  Moon::IntRect quad_rect;
  Moon::FloatRect quad_texture_rect;
  Moon::Vector4 color;
  Moon::VertexBuffer *vbo = NULL;
  Moon::Vertex vertices[4];
  GLuint indices[] = { 0, 1, 3, 2, 3, 1 };

  mrb_value rect_obj;
  mrb_value tex_rect_obj;
  mrb_value color_obj;
  mrb_int i = 0;

  mrb_get_args(mrb, "ooo", &rect_obj, &tex_rect_obj, &color_obj);
  quad_rect = mmrb_to_rect(mrb, rect_obj);
  quad_texture_rect = mmrb_to_float_rect(mrb, tex_rect_obj);
  color = mmrb_to_vector4(mrb, tex_rect_obj);
  vbo = mmrb_vertex_buffer_ptr(mrb, self);

  printf("quad: x=%i y=%i w=%i h=%i\n", quad_rect.x, quad_rect.y, quad_rect.w, quad_rect.h);
  printf("tex_quad: x=%f y=%f w=%f h=%f\n", quad_texture_rect.x, quad_texture_rect.y, quad_texture_rect.w, quad_texture_rect.h);

  vertices[i].pos = Moon::Vector2(quad_rect.x, quad_rect.y);
  vertices[i].tex_coord = Moon::Vector2(quad_texture_rect.x, quad_texture_rect.y);
  vertices[i].color = color;

  i++;
  vertices[i].pos = Moon::Vector2(quad_rect.x + quad_rect.w, quad_rect.y);
  vertices[i].tex_coord = Moon::Vector2(quad_texture_rect.x + quad_texture_rect.w, quad_texture_rect.y);
  vertices[i].color = color;

  i++;
  vertices[i].pos = Moon::Vector2(quad_rect.x + quad_rect.w, quad_rect.y + quad_rect.h);
  vertices[i].tex_coord = Moon::Vector2(quad_texture_rect.x + quad_texture_rect.w, quad_texture_rect.y + quad_texture_rect.h);
  vertices[i].color = color;

  i++;
  vertices[i].pos = Moon::Vector2(quad_rect.x, quad_rect.y + quad_rect.h);
  vertices[i].tex_coord = Moon::Vector2(quad_texture_rect.x, quad_texture_rect.y + quad_texture_rect.h);
  vertices[i].color = color;

  vbo->PushBack(vertices, 4, indices, 6);
  return self;
}

MOON_C_API void
mmrb_vbo_init(mrb_state *mrb, struct RClass* mod)
{
  struct RClass *vbo_class = mrb_define_class_under(mrb, mod, "VertexBuffer", mrb->object_class);
  MRB_SET_INSTANCE_TT(vbo_class, MRB_TT_DATA);

  mrb_define_method(mrb, vbo_class, "initialize",   vbo_initialize,   MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vbo_class, "clear",        vbo_clear,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vbo_class, "render",       vbo_render,       MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  mrb_define_method(mrb, vbo_class, "push_back",    vbo_push_back,    MRB_ARGS_REQ(3));
  mrb_define_method(mrb, vbo_class, "push_indices", vbo_push_indices, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vbo_class, "vertex_count", vbo_vertex_count, MRB_ARGS_NONE());
  mrb_define_method(mrb, vbo_class, "index_count",  vbo_index_count,  MRB_ARGS_NONE());
  mrb_define_method(mrb, vbo_class, "add_quad",     vbo_add_quad,     MRB_ARGS_REQ(3));
  // vbo.render(mode, {offset: [default: 0]})
  // vbo.flush // vbo.upload

  // vbo.push(vertices, indices)
  // vbo.push_vertices(v)
  // vbo.push_indices(i)
}
