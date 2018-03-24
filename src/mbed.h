#ifndef _MBED_H_
#define _MBED_H_

/* gpio.c */
void mrb_mruby_mbed_gpio_init(mrb_state*);

/* tick.c */
void mrb_mruby_mbed_tick_init(mrb_state*);

/* i2c.c */
void mrb_mruby_mbed_i2c_init(mrb_state*);

#endif /* _MBED_H_ */