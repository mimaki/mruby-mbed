#include "mruby.h"
#include "mbed.h"
#include "mbedapi.h"

#ifdef NO_MBED
#include <stdio.h>

void mbedDelay(int ms)
{
  printf("mbedDelay(%d)\n", ms);
}

uint32_t mbedMillis(void)
{
  printf("mbedMillis()\n");
  return 0;
}
#endif

static mrb_value
mrb_mbed_delay(mrb_state *mrb, mrb_value self)
{
  mrb_int ms;

  mrb_get_args(mrb, "i", &ms);
  mbedDelay(ms);
  return mrb_nil_value();
}

static mrb_value
mrb_mbed_millis(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value((mrb_int)mbedMillis());
}

void
mrb_mruby_mbed_tick_init(mrb_state *mrb)
{
  mrb_define_method(mrb, mrb->kernel_module, "delay", mrb_mbed_delay, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, mrb->kernel_module, "millis", mrb_mbed_millis, MRB_ARGS_NONE());

  mbedMillis(); /* Initialize ticker */
}
