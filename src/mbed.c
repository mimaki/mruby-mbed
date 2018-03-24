#include "mruby.h"
#include "mbed.h"

void
mrb_mruby_mbed_gem_init(mrb_state *mrb)
{
  mrb_mruby_mbed_tick_init(mrb);
  mrb_mruby_mbed_gpio_init(mrb);
  mrb_mruby_mbed_i2c_init(mrb);
}

void
mrb_mruby_mbed_gem_final(mrb_state *mrb)
{
}
