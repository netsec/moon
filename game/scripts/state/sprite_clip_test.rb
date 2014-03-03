class State::SpriteClipTest < State

  include Moon
  include Input

  def init
    super
    @sprite = Sprite.new("resources/media_buttons_96x96.png")
    @sprite.clip_rect = Rect.new(0, 0, 96, 96)
    w = @sprite.texture.width
    h = @sprite.texture.height
    @cols = @sprite.texture.width / 96
    @total = ((@sprite.texture.height / 96) * @cols).to_i
  end

  def render
    super
    @sprite.render
  end

  def update
    if Mouse.triggered?(Mouse::Buttons::BUTTON_1)
      i = rand(@total)
      @sprite.clip_rect = Rect.new((i % 4) * 96, (i / 4) * 96, 96, 96)
      puts i
    end
    super
  end

end