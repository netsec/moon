#include "moon.hxx"
#include "sprite.hxx"

namespace Moon {
  static void moon_mrb_sprite_deallocate(mrb_state *mrb, void *p) {
    delete((Sprite*)p); //(Sprite*)
  };

  static const struct mrb_data_type sprite_data_type = {
    "sprite", moon_mrb_sprite_deallocate,
  };

  static mrb_value moon_mrb_sprite_new(mrb_state *mrb, mrb_value klass) {
    mrb_value filename;
    mrb_get_args(mrb, "S", &filename);

    Sprite *sprite = new Sprite(StringValueCStr(filename));
    return mrb_obj_value(Data_Wrap_Struct(mrb, mrb_class_ptr(klass), &sprite_data_type, sprite));
  };

  static mrb_value moon_mrb_sprite_render(mrb_state *mrb, mrb_value self) {
    Sprite *sprite;
    Data_Get_Struct(mrb, self, &sprite_data_type, sprite);
    sprite->render();
    return mrb_nil_value();
  };
  
  void moon_mrb_sprite_init(mrb_state *mrb) {
    struct RClass *sprite_class;
    sprite_class = mrb_define_class(mrb, "Sprite", mrb->object_class);
    MRB_SET_INSTANCE_TT(sprite_class, MRB_TT_DATA);
    
    mrb_define_class_method(mrb, sprite_class, "new", moon_mrb_sprite_new, MRB_ARGS_REQ(1));
    //mrb_define_method(mrb, sprite_class, "initialize", moon_mrb_sprite_initialize, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, sprite_class, "render", moon_mrb_sprite_render, MRB_ARGS_NONE());
  };
};