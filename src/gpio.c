#include "mruby.h"
#include "mbed.h"
#include "mbedapi.h"

#ifdef NO_MBED
#include <stdio.h>

void mbedDigitalWrite(int pin, int val)
{
  printf("mbedDigitalWrite: pin=%d, val=%d\n", pin, val);
}

int mbedDigitalRead(int pin)
{
  printf("mbedDigitalRead: pin=%d\n", pin);
  return 0;
}
#endif

static void
mbed_define_pins(mrb_state *mrb, struct RClass *gpio)
{
  int pin;
  char pin_name[8];

  /* D0 - D15 */
  for (pin=0; pin<MBED_DIGITAL_PINS; pin++) {
    sprintf(pin_name, "D%d", pin);
    mrb_define_const(mrb, gpio, pin_name, mrb_fixnum_value(MBED_D0 + pin));
  }

  /* A0 - A5 */
  for (pin=0; pin<MBED_ANALOG_PINS; pin++) {
    sprintf(pin_name, "A%d", pin);
    mrb_define_const(mrb, gpio, pin_name, mrb_fixnum_value(MBED_A0 + pin));
  }

  /* LED1 - LED4 */
  for (pin=0; pin<MBED_LED_PINS; pin++) {
    sprintf(pin_name, "LED%d", pin + 1);
    mrb_define_const(mrb, gpio, pin_name, mrb_fixnum_value(MBED_LED1 + pin));
  }

  /* BUTTON1 */
  mrb_define_const(mrb, gpio, "BUTTON1", mrb_fixnum_value(MBED_BUTTON1));
}

static mrb_value
mbed_digital_write(mrb_state *mrb, mrb_value self)
{
  mrb_int pin, val;

  mrb_get_args(mrb, "ii", &pin, &val);
  mbedDigitalWrite(pin, val);
  return mrb_fixnum_value(val);
}

static mrb_value
mbed_digital_read(mrb_state *mrb, mrb_value self)
{
  mrb_int pin;
  int val;

  mrb_get_args(mrb, "i", &pin);
  val = mbedDigitalRead(pin);
  return mrb_fixnum_value(val);
}

void
mrb_mruby_mbed_gpio_init(mrb_state *mrb)
{
  struct RClass *gpio;
  gpio = mrb_define_class(mrb, "GPIO", mrb->object_class);

  mbed_define_pins(mrb, gpio);

  mrb_define_class_method(mrb, mrb->kernel_module, "digitalWrite", mbed_digital_write, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, mrb->kernel_module, "digitalRead", mbed_digital_read, MRB_ARGS_REQ(1));
}
