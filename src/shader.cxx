#include "shader.hxx"

#include <fstream>

namespace Moon {
  glm::mat4 Shader::projection_matrix = glm::mat4(1.0f);

  /**
   * Store all the file's contents in memory, useful to pass shaders
   * source code to OpenGL
   */
  std::string Shader::file_read(const char *filePath) {
    std::ifstream fileStream;
    fileStream.open(filePath, std::ios_base::in);

    if(fileStream) {     
      std::string buffer(std::istreambuf_iterator<char>(fileStream), (std::istreambuf_iterator<char>()));
      return buffer;
    } else {
      fprintf(stderr, "Could not read shader file %s. File does not exist.", filePath);
      return "";
    };
  };

  /**
   * Display compilation errors from the OpenGL shader compiler
   */
  void Shader::print_log(GLuint object)
  {
    GLint infoLogLength = 0;
    if (glIsShader(object))
      glGetShaderiv(object, GL_INFO_LOG_LENGTH, &infoLogLength);
    else if (glIsProgram(object))
      glGetProgramiv(object, GL_INFO_LOG_LENGTH, &infoLogLength);
    else {
      fprintf(stderr, "printlog: Not a shader or a program\n");
      return;
    }

    GLchar *infoLog = new GLchar[infoLogLength];

    if (glIsShader(object))
      glGetShaderInfoLog(object, infoLogLength, NULL, infoLog);
    else if (glIsProgram(object))
      glGetProgramInfoLog(object, infoLogLength, NULL, infoLog);

    fprintf(stderr, "%s", infoLog);
    delete [] infoLog;
  }

  /**
   * Compile the shader from file 'filename', with error handling
   */
  GLuint Shader::create_shader(const char* filename, GLenum type)
  {
    std::string contents = file_read(filename);
    const GLchar* source = contents.c_str();

    if (source == NULL) {
      fprintf(stderr, "Error opening %s: ", filename);
      return 0;
    }
    GLuint res = glCreateShader(type);
    const GLchar* sources[] = {
      // Define GLSL version
  #ifdef GL_ES_VERSION_2_0
      "#version 100\n"
  #else
      "#version 120\n"
  #endif
      ,
      // GLES2 precision specifiers
  #ifdef GL_ES_VERSION_2_0
      // Define default float precision for fragment shaders:
      (type == GL_FRAGMENT_SHADER) ?
      "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
      "precision highp float;           \n"
      "#else                            \n"
      "precision mediump float;         \n"
      "#endif                           \n"
      : ""
      // Note: OpenGL ES automatically defines this:
      // #define GL_ES
  #else
      // Ignore GLES 2 precision specifiers:
      "#define lowp   \n"
      "#define mediump\n"
      "#define highp  \n"
  #endif
      ,
      source };
    glShaderSource(res, 3, sources, NULL);

    glCompileShader(res);
    GLint status;
    glGetShaderiv(res, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
      fprintf(stderr, "%s:", filename);
      print_log(res);
      glDeleteShader(res);
      return 0;
    }

    return res;
  }

  GLuint Shader::create_program(const char *vertexfile, const char *fragmentfile) {
    GLuint program = glCreateProgram();
    GLuint shader;

    if(vertexfile) {
      shader = create_shader(vertexfile, GL_VERTEX_SHADER);
      if(!shader)
        return 0;
      glAttachShader(program, shader);
      glDeleteShader(shader); // http://stackoverflow.com/questions/9113154/proper-way-to-delete-glsl-shader
    }

    if(fragmentfile) {
      shader = create_shader(fragmentfile, GL_FRAGMENT_SHADER);
      if(!shader)
        return 0;
      glAttachShader(program, shader);
      glDeleteShader(shader);
    }

    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
      fprintf(stderr, "glLinkProgram:");
      print_log(program);
      glDeleteProgram(program);
      return 0;
    }

    return program;
  }

  Shader::Shader(const char *vertexfile, const char *fragmentfile) {
    _program = create_program(vertexfile, fragmentfile);
  };

  Shader::~Shader() {
    glDeleteProgram(_program);
  };

  void Shader::add_attribute(const char *name) {
    GLint attribute = glGetAttribLocation(_program, name);
    if(attribute == -1)
      fprintf(stderr, "Could not bind attribute %s\n", name);
    _attributeList[name] = attribute;  
  }

  GLint Shader::get_attribute(const char *name) {
    return _attributeList[name];
  }

  void Shader::add_uniform(const char *name) {
    GLint uniform = glGetUniformLocation(_program, name);
    if(uniform == -1)
      fprintf(stderr, "Could not bind uniform %s\n", name);
    _uniformLocationList[name] = uniform;
  }

  GLint Shader::get_uniform(const char *name) {
    return _uniformLocationList[name];
  }

  GLuint Shader::get_program() {
    return _program;
  }
};