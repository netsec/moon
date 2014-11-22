#include "moon/mrb/rect.hxx"
#include "moon/rect.hxx"
#include "moon/graphics.hxx"

using Moon::Rect;

static struct RClass *rect_class;

static void
rect_free(mrb_state *mrb, void *p)
{
  moon_rect *rect = (moon_rect*)p;
  if (rect) {
    delete(rect);
  }
}

struct mrb_data_type rect_data_type = { "Rect", rect_free };

static mrb_value
rect_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_int x, y, w, h;
  moon_rect *rect;
  mrb_get_args(mrb, "iiii", &x, &y, &w, &h);

  rect = (moon_rect*)DATA_PTR(self);
  if (rect) {
    rect_free(mrb, (void*)rect);
  }

  rect = new moon_rect(new Rect(x, y, w, h));

  mrb_data_init(self, rect, &rect_data_type);

  return self;
}

static mrb_value
rect_initialize_copy(mrb_state *mrb, mrb_value self)
{
  mrb_value other;
  moon_rect *src_rect;

  mrb_get_args(mrb, "o", &other);

  src_rect = (moon_rect*)mrb_data_get_ptr(mrb, other, &rect_data_type);

  auto rect = new moon_rect(new Rect((*src_rect)->x, (*src_rect)->y, (*src_rect)->w, (*src_rect)->h));
  DATA_TYPE(self) = &rect_data_type;
  DATA_PTR(self) = rect;

  return self;
}

static mrb_value
rect_x_setter(mrb_state *mrb, mrb_value self)
{
  mrb_int x;
  mrb_get_args(mrb, "i", &x);

  moon_rect* rect;
  rect = (moon_rect*)mrb_data_get_ptr(mrb, self, &rect_data_type);

  (*rect)->x = x;

  return mrb_nil_value();
}

static mrb_value
rect_x_getter(mrb_state *mrb, mrb_value self)
{
  moon_rect* rect;
  rect = (moon_rect*)mrb_data_get_ptr(mrb, self, &rect_data_type);

  //return mrb_fixnum_value((int)(*rect)->x);
  return mrb_float_value(mrb, (*rect)->x);
}

static mrb_value
rect_y_setter(mrb_state *mrb, mrb_value self)
{
  mrb_int y;
  mrb_get_args(mrb, "i", &y);

  moon_rect* rect;
  rect = (moon_rect*)mrb_data_get_ptr(mrb, self, &rect_data_type);

  (*rect)->y = y;

  return mrb_nil_value();
}

static mrb_value
rect_y_getter(mrb_state *mrb, mrb_value self)
{
  moon_rect* rect;
  rect = (moon_rect*)mrb_data_get_ptr(mrb, self, &rect_data_type);

  return mrb_float_value(mrb, (*rect)->y);
}

static mrb_value
rect_width_setter(mrb_state *mrb, mrb_value self)
{
  mrb_int width;
  mrb_get_args(mrb, "i", &width);

  moon_rect* rect;
  rect = (moon_rect*)mrb_data_get_ptr(mrb, self, &rect_data_type);

  (*rect)->w = width;

  return mrb_nil_value();
}

static mrb_value
rect_width_getter(mrb_state *mrb, mrb_value self)
{
  moon_rect* rect;
  rect = (moon_rect*)mrb_data_get_ptr(mrb, self, &rect_data_type);

  return mrb_float_value(mrb, (*rect)->w);
}

static mrb_value
rect_height_setter(mrb_state *mrb, mrb_value self)
{
  mrb_int height;
  mrb_get_args(mrb, "i", &height);

  moon_rect* rect;
  rect = (moon_rect*)mrb_data_get_ptr(mrb, self, &rect_data_type);

  (*rect)->h = height;

  return mrb_nil_value();
}

static mrb_value
rect_height_getter(mrb_state *mrb, mrb_value self)
{
  moon_rect* rect;
  rect = (moon_rect*)mrb_data_get_ptr(mrb, self, &rect_data_type);

  return mrb_float_value(mrb, (*rect)->h);
}

struct RClass*
mmrb_rect_init(mrb_state *mrb)
{
  rect_class = mrb_define_class_under(mrb, mmrb_Moon, "Rect", mrb->object_class);
  MRB_SET_INSTANCE_TT(rect_class, MRB_TT_DATA);

  mrb_define_method(mrb, rect_class, "initialize",      rect_initialize,      MRB_ARGS_REQ(4));
  mrb_define_method(mrb, rect_class, "initialize_copy", rect_initialize_copy, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, rect_class, "x=",              rect_x_setter,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, rect_class, "x",               rect_x_getter,        MRB_ARGS_NONE());
  mrb_define_method(mrb, rect_class, "y=",              rect_y_setter,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, rect_class, "y",               rect_y_getter,        MRB_ARGS_NONE());
  mrb_define_method(mrb, rect_class, "width=",          rect_width_setter,    MRB_ARGS_REQ(1));
  mrb_define_method(mrb, rect_class, "width",           rect_width_getter,    MRB_ARGS_NONE());
  mrb_define_method(mrb, rect_class, "height=",         rect_height_setter,   MRB_ARGS_REQ(1));
  mrb_define_method(mrb, rect_class, "height",          rect_height_getter,   MRB_ARGS_NONE());

  return rect_class;
}

mrb_value
mmrb_rect_wrap(mrb_state *mrb, Moon::Rect *ptr)
{
  moon_rect *rect_ptr = new moon_rect(ptr);
  mrb_value rect = mrb_obj_value(Data_Wrap_Struct(mrb, rect_class, &rect_data_type, rect_ptr));
  return rect;
}

mrb_value
mmrb_rect_create(mrb_state *mrb, GLint x, GLint y, GLint w, GLint h)
{
  return mmrb_rect_wrap(mrb, new Moon::Rect(x, y, w, h));
}