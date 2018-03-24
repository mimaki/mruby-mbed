class LED < GPIO
  def initialize(pin)
    @pin = pin
    @last = 0
  end

  def on
    out(1)
  end

  def off
    out(0)
  end

  def toggle
    out(@last ^ 1)
  end

  def out(v)
    @last = v
    digitalWrite(@pin, v)
  end
end
