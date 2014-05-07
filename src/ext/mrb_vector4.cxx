/*
 * Moon Vector4, a wrapper around glm::vec4
 */
#include "mrb.hxx"
#include <memory>
#include <glm/glm.hpp>
#include "mrb_shared_types.hxx"

namespace Moon {

  static struct RClass *vector4_class = NULL;

  #define mrb_set_vector4_value_xyz(mrb, target, x, y, z, w)   \
    {                                                          \
      moon_vec4 *vector4 = new moon_vec4(new glm::vec4(x, y, z, w)); \
      DATA_TYPE(target) = &vector4_data_type;                  \
      DATA_PTR(target) = vector4;                              \
    }

  #define vec4_math_head(_func_)                                            \
    moon_vec4* src_vec4;                                                    \
    Data_Get_Struct(mrb, self, &vector4_data_type, src_vec4);               \
    glm::vec4 oth_vec4 = moon_vector4_extract_mrb_args(mrb);                \
    _func_

  #define vec4_math_op(_op_)                                 \
    glm::vec4 oth_vec4 = moon_vector4_extract_mrb_args(mrb);   \
    mrb_value dest_vec = mrb_obj_dup(mrb, self);           \
    **((moon_vec4*)DATA_PTR(dest_vec)) _op_ ## = oth_vec4; \
    return dest_vec;

  static void moon_mrb_vector4_deallocate(mrb_state *mrb, void *p) {
    delete((moon_vec4*)p);
  };

  const struct mrb_data_type vector4_data_type = {
    "Vector4", moon_mrb_vector4_deallocate,
  };

  /*
   * Black Magic
   */
  glm::vec4
  moon_vector4_extract_args(mrb_state *mrb, int argc, mrb_value *vals) {
    glm::vec4 result;

    switch (argc) {
      case 1:
        mrb_value val;
        val = vals[0];
        if ((mrb_type(val) == MRB_TT_FIXNUM) || (mrb_type(val) == MRB_TT_FLOAT)) {
          double i = mrb_to_flo(mrb, val);
          result.x = i;
          result.y = i;
          result.z = i;
          result.w = i;
        } else if (mrb_type(val) == MRB_TT_ARRAY) {
          int _ary_len = mrb_ary_len(mrb, val);
          if (_ary_len != 4) {
            mrb_raisef(mrb, E_ARGUMENT_ERROR,
                       "wrong array size %d (expected 4)", _ary_len);
          } else {
            result.x = mrb_to_flo(mrb, RARRAY_PTR(val)[0]);
            result.y = mrb_to_flo(mrb, RARRAY_PTR(val)[1]);
            result.z = mrb_to_flo(mrb, RARRAY_PTR(val)[2]);
            result.w = mrb_to_flo(mrb, RARRAY_PTR(val)[3]);
          }
        } else if (mrb_type(val) == MRB_TT_DATA) {
          moon_vec4* vec4;
          Data_Get_Struct(mrb, val, &vector4_data_type, vec4);
          result = **vec4;
        } else {
          mrb_raisef(mrb, E_TYPE_ERROR,
                     "wrong type %S (expected Numeric, Array or Vector4)",
                     mrb_obj_classname(mrb, val));
        }
        break;
      case 2:
      case 3:
        int index;
        index = 0;
        for (int i=0; i < argc; ++i) {
          mrb_value val = vals[i];

          if (mrb_type(val) == MRB_TT_DATA) {
            if (DATA_TYPE(val) == &vector2_data_type) {
              moon_vec2* vec2;
              Data_Get_Struct(mrb, val, &vector2_data_type, vec2);

              result[index++] = (**vec2)[0]; if (index >= 4) break;
              result[index++] = (**vec2)[1];
            } else if (DATA_TYPE(val) == &vector3_data_type) {
              moon_vec3* vec3;
              Data_Get_Struct(mrb, val, &vector3_data_type, vec3);

              result[index++] = (**vec3)[0]; if (index >= 4) break;
              result[index++] = (**vec3)[1]; if (index >= 4) break;
              result[index++] = (**vec3)[2];
            } else {
              mrb_raisef(mrb, E_TYPE_ERROR,
                         "wrong type %S (expected Vector2 or Vector3)",
                         mrb_obj_classname(mrb, val));
            }
          } else if ((mrb_type(val) == MRB_TT_FIXNUM) || (mrb_type(val) == MRB_TT_FLOAT)) {
            result[index++] = mrb_to_flo(mrb, val);
          }
          if (index >= 4) break;
        };
        break;
      case 4:
        result.x = mrb_to_flo(mrb, vals[0]);
        result.y = mrb_to_flo(mrb, vals[1]);
        result.z = mrb_to_flo(mrb, vals[2]);
        result.w = mrb_to_flo(mrb, vals[3]);
        break;
      default:
        mrb_raisef(mrb, E_ARGUMENT_ERROR,
                   //"wrong number of arguments (%d for 1 or 4)", argc);
                   "wrong number of arguments (%d for 1, 2, 3, or 4)", argc);
    }

    return result;
  }

  static glm::vec4
  moon_vector4_extract_mrb_args(mrb_state *mrb) {
    mrb_value *vals;
    int len;

    mrb_get_args(mrb, "*", &vals, &len);
    return moon_vector4_extract_args(mrb, len, vals);
  }

  static mrb_value
  moon_mrb_vector4_initialize(mrb_state *mrb, mrb_value self) {
    mrb_float x = 0.0;
    mrb_float y = 0.0;
    mrb_float z = 0.0;
    mrb_float w = 0.0;
    mrb_get_args(mrb, "|ffff", &x, &y, &z, &w);

    mrb_set_vector4_value_xyz(mrb, self, x, y, z, w)

    return mrb_nil_value();
  };

  static mrb_value
  moon_mrb_vector4_initialize_copy(mrb_state *mrb, mrb_value self) {
    mrb_value other;

    mrb_get_args(mrb, "o", &other);

    moon_vec4* src_vector4;
    Data_Get_Struct(mrb, other, &vector4_data_type, src_vector4);

    moon_vec4 *vector4 = new moon_vec4(new glm::vec4(**src_vector4));
    DATA_TYPE(self) = &vector4_data_type;
    DATA_PTR(self) = vector4;

    return mrb_nil_value();
  }

  static mrb_value
  moon_mrb_vector4_coerce(mrb_state *mrb, mrb_value self) {
    mrb_value other;
    mrb_get_args(mrb, "o", &other);
    mrb_value argv[2] = { self, other };
    return mrb_ary_new_from_values(mrb, 2, argv);
  }

  static mrb_value
  moon_mrb_vector4_x_getter(mrb_state *mrb, mrb_value self) {
    moon_vec4* vector4;
    Data_Get_Struct(mrb, self, &vector4_data_type, vector4);

    return mrb_float_value(mrb, moon_vec4_p(vector4)->x);
  }

  static mrb_value
  moon_mrb_vector4_y_getter(mrb_state *mrb, mrb_value self) {
    moon_vec4* vector4;
    Data_Get_Struct(mrb, self, &vector4_data_type, vector4);

    return mrb_float_value(mrb, moon_vec4_p(vector4)->y);
  }

  static mrb_value
  moon_mrb_vector4_z_getter(mrb_state *mrb, mrb_value self) {
    moon_vec4* vector4;
    Data_Get_Struct(mrb, self, &vector4_data_type, vector4);

    return mrb_float_value(mrb, moon_vec4_p(vector4)->z);
  }

  static mrb_value
  moon_mrb_vector4_w_getter(mrb_state *mrb, mrb_value self) {
    moon_vec4* vector4;
    Data_Get_Struct(mrb, self, &vector4_data_type, vector4);

    return mrb_float_value(mrb, moon_vec4_p(vector4)->w);
  }

  static mrb_value
  moon_mrb_vector4_x_setter(mrb_state *mrb, mrb_value self) {
    mrb_float x;
    mrb_get_args(mrb, "f", &x);

    moon_vec4* vector4;
    Data_Get_Struct(mrb, self, &vector4_data_type, vector4);

    moon_vec4_p(vector4)->x = x;

    return mrb_nil_value();
  }

  static mrb_value
  moon_mrb_vector4_y_setter(mrb_state *mrb, mrb_value self) {
    mrb_float y;
    mrb_get_args(mrb, "f", &y);

    moon_vec4* vector4;
    Data_Get_Struct(mrb, self, &vector4_data_type, vector4);

    moon_vec4_p(vector4)->y = y;

    return mrb_nil_value();
  }

  static mrb_value
  moon_mrb_vector4_z_setter(mrb_state *mrb, mrb_value self) {
    mrb_float z;
    mrb_get_args(mrb, "f", &z);

    moon_vec4* vector4;
    Data_Get_Struct(mrb, self, &vector4_data_type, vector4);

    moon_vec4_p(vector4)->z = z;

    return mrb_nil_value();
  }

  static mrb_value
  moon_mrb_vector4_w_setter(mrb_state *mrb, mrb_value self) {
    mrb_float w;
    mrb_get_args(mrb, "f", &w);

    moon_vec4* vector4;
    Data_Get_Struct(mrb, self, &vector4_data_type, vector4);

    moon_vec4_p(vector4)->w = w;

    return mrb_nil_value();
  }

  static mrb_value
  moon_mrb_vector4_negate(mrb_state *mrb, mrb_value self) {
    moon_vec4* dvec4;
    moon_vec4* svec4;

    //mrb_value dest_vec = mrb_obj_new(mrb, vector4_class, 0, {});
    mrb_value dest_vec = mrb_obj_dup(mrb, self);

    Data_Get_Struct(mrb, dest_vec, &vector4_data_type, dvec4);
    Data_Get_Struct(mrb, self, &vector4_data_type, svec4);

    **dvec4 = -(**svec4);

    return dest_vec;
  }

  static mrb_value
  moon_mrb_vector4_identity(mrb_state *mrb, mrb_value self) {
    return mrb_obj_dup(mrb, self);
  }

  static mrb_value
  moon_mrb_vector4_add(mrb_state *mrb, mrb_value self) {
    vec4_math_op(+)
  };

  static mrb_value
  moon_mrb_vector4_sub(mrb_state *mrb, mrb_value self) {
    vec4_math_op(-)
  };

  static mrb_value
  moon_mrb_vector4_mul(mrb_state *mrb, mrb_value self) {
    vec4_math_op(*)
  };

  static mrb_value
  moon_mrb_vector4_div(mrb_state *mrb, mrb_value self) {
    vec4_math_op(/)
  };

  //static mrb_value
  //moon_mrb_vector4_mod(mrb_state *mrb, mrb_value self) {
  //  vec4_math_op(%)
  //};

  static mrb_value
  moon_mrb_vector4_dot(mrb_state *mrb, mrb_value self) {
    vec4_math_head({
      return mrb_float_value(mrb, glm::dot(**src_vec4, oth_vec4));
    })
  };

  static mrb_value
  moon_mrb_vector4_set(mrb_state *mrb, mrb_value self) {
    moon_vec4* mvec4;
    Data_Get_Struct(mrb, self, &vector4_data_type, mvec4);

    **mvec4 = moon_vector4_extract_mrb_args(mrb);

    return self;
  };

  static mrb_value
  moon_mrb_vector4_to_a(mrb_state *mrb, mrb_value self) {
    moon_vec4* mvec4;
    Data_Get_Struct(mrb, self, &vector4_data_type, mvec4);
    mrb_value argv[4] = { mrb_float_value(mrb, moon_vec4_p(mvec4)->x),
                          mrb_float_value(mrb, moon_vec4_p(mvec4)->y),
                          mrb_float_value(mrb, moon_vec4_p(mvec4)->z),
                          mrb_float_value(mrb, moon_vec4_p(mvec4)->w) };
    return mrb_ary_new_from_values(mrb, 4, argv);
  };

  static mrb_value
  moon_mrb_vector4_s_extract(mrb_state *mrb, mrb_value self) {
    glm::vec4 v4a = moon_vector4_extract_mrb_args(mrb);

    mrb_value argv[4] = { mrb_float_value(mrb, v4a.x),
                          mrb_float_value(mrb, v4a.y),
                          mrb_float_value(mrb, v4a.z),
                          mrb_float_value(mrb, v4a.w) };
    return mrb_ary_new_from_values(mrb, 4, argv);
  }

  static mrb_value
  moon_mrb_vector4_s_cast(mrb_state *mrb, mrb_value klass) {
    mrb_value dest_vec = mrb_obj_new(mrb, vector4_class, 0, {});

    moon_vec4* _vec4;
    Data_Get_Struct(mrb, dest_vec, &vector4_data_type, _vec4);

    **_vec4 = moon_vector4_extract_mrb_args(mrb);

    return dest_vec;
  }

  struct RClass*
  moon_mrb_vector4_init(mrb_state *mrb) {
    vector4_class = mrb_define_class_under(mrb, moon_module, "Vector4", mrb->object_class);
    MRB_SET_INSTANCE_TT(vector4_class, MRB_TT_DATA);

    mrb_define_method(mrb, vector4_class, "initialize",      moon_mrb_vector4_initialize,      MRB_ARGS_OPT(4));
    mrb_define_method(mrb, vector4_class, "initialize_copy", moon_mrb_vector4_initialize_copy, MRB_ARGS_REQ(1));

    mrb_define_method(mrb, vector4_class, "coerce",          moon_mrb_vector4_coerce,          MRB_ARGS_REQ(1));

    mrb_define_method(mrb, vector4_class, "x",               moon_mrb_vector4_x_getter,        MRB_ARGS_NONE());
    mrb_define_method(mrb, vector4_class, "y",               moon_mrb_vector4_y_getter,        MRB_ARGS_NONE());
    mrb_define_method(mrb, vector4_class, "z",               moon_mrb_vector4_z_getter,        MRB_ARGS_NONE());
    mrb_define_method(mrb, vector4_class, "w",               moon_mrb_vector4_w_getter,        MRB_ARGS_NONE());
    mrb_define_method(mrb, vector4_class, "x=",              moon_mrb_vector4_x_setter,        MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector4_class, "y=",              moon_mrb_vector4_y_setter,        MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector4_class, "z=",              moon_mrb_vector4_z_setter,        MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector4_class, "w=",              moon_mrb_vector4_w_setter,        MRB_ARGS_REQ(1));

    mrb_define_method(mrb, vector4_class, "-@",              moon_mrb_vector4_negate,          MRB_ARGS_NONE());
    mrb_define_method(mrb, vector4_class, "+@",              moon_mrb_vector4_identity,        MRB_ARGS_NONE());

    mrb_define_method(mrb, vector4_class, "+",               moon_mrb_vector4_add,             MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector4_class, "-",               moon_mrb_vector4_sub,             MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector4_class, "*",               moon_mrb_vector4_mul,             MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector4_class, "/",               moon_mrb_vector4_div,             MRB_ARGS_REQ(1));
    //mrb_define_method(mrb, vector4_class, "%",               moon_mrb_vector4_mod,             MRB_ARGS_REQ(1));

    mrb_define_method(mrb, vector4_class, "dot",             moon_mrb_vector4_dot,             MRB_ARGS_REQ(1));
    //mrb_define_method(mrb, vector4_class, "cross",           moon_mrb_vector4_cross,           MRB_ARGS_REQ(1));

    mrb_define_method(mrb, vector4_class, "set",             moon_mrb_vector4_set,             MRB_ARGS_ANY());

    mrb_define_method(mrb, vector4_class, "to_a",            moon_mrb_vector4_to_a,            MRB_ARGS_NONE());

    mrb_define_class_method(mrb, vector4_class, "[]",        moon_mrb_vector4_s_cast,          MRB_ARGS_ANY());
    mrb_define_class_method(mrb, vector4_class, "extract",   moon_mrb_vector4_s_extract,       MRB_ARGS_REQ(1));

    mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "r"), mrb_intern_cstr(mrb, "x"));
    mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "g"), mrb_intern_cstr(mrb, "y"));
    mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "b"), mrb_intern_cstr(mrb, "z"));
    mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "a"), mrb_intern_cstr(mrb, "w"));

    mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "r="), mrb_intern_cstr(mrb, "x="));
    mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "g="), mrb_intern_cstr(mrb, "y="));
    mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "b="), mrb_intern_cstr(mrb, "z="));
    mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "a="), mrb_intern_cstr(mrb, "w="));

    return vector4_class;
  };

}