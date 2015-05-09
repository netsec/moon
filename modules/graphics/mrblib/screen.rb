module Moon
  class Screen
    attr_reader :window
    attr_reader :clear_color

    def initialize(window)
      @window = window
      init_clear_color
      init_shader
    end

    private def init_clear_color
      @clear_color = Vector4.new 0, 0, 0, 0
      self.clear_color = @clear_color
    end

    private def init_shader
      resize w, h
    end

    def w
      @window.window_size[0]
    end

    def h
      @window.window_size[1]
    end

    # @return [Moon::Rect]
    def rect
      Rect.new 0, 0, w, h
    end

    def clear_color=(color)
      @clear_color = Vector4[color]
      GL2.glClearColor @clear_color.r, @clear_color.g, @clear_color.b, @clear_color.a
    end

    def resize(w, h)
      @window.window_size = [w, h]
      Shader.projection_matrix = Transform.ortho 0.0, w, h, 0.0, -1.0, 1.0
    end
  end
end