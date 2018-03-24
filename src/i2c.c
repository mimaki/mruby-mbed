#include "mruby.h"
#include "mruby/array.h"
#include "mruby/_string.h"
#include "mruby/variable.h"
#include "mbed.h"
#include "mbedapi.h"
#include <string.h>

#ifdef NO_MBED
#include <stdio.h>

void mbedI2CWrite(int addr, const char *data, int length, uint8_t repeat)
{
  int i;
  printf("mbedI2CWrite: addr=0x%x(%d), length=%d, data=", addr, addr, length);
  for (i=0; i<length; i++) {
    if (i) printf(",");
    printf("0x%02x", data[i]);
  }
  printf("\n");
}

int mbedI2CRead(int addr, char *data, int length)
{
  printf("mbedI2CRead: addr=0x%x(%d), length=%d\n", addr, addr, length);
  return 0;
}

void mbedI2CStart(void)
{
  printf("mbedI2CStart\n");
}

void mbedI2CStop(void)
{
  printf("mbedI2CStop\n");
}
#endif

static mrb_value
mrb_i2c_init(mrb_state *mrb, mrb_value self)
{
  mrb_int addr;
  mrb_get_args(mrb, "i", &addr);
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@addr"), mrb_fixnum_value(addr));
  return self;
}

static mrb_value
mrb_i2c_write(mrb_state *mrb, mrb_value self)
{
  mrb_int reg;
  uint8_t *buf;
  size_t len, i;
  mrb_value addr, data, v;

  mrb_get_args(mrb, "io", &reg, &data);

  if (mrb_array_p(data)) {
    len = RARRAY_LEN(data);
    buf = mrb_malloc(mrb, len + 1);
    buf[0] = (uint8_t)reg;
    for (i=0; i<len; i++) {
      buf[i + 1] = (uint8_t)mrb_fixnum(mrb_ary_ref(mrb, data, i));
    }
  }
  else if (mrb_fixnum_p(data)) {
    len = 1;
    buf = mrb_malloc(mrb, len + 1);
    buf[0] = (uint8_t)reg;
    buf[1] = mrb_fixnum(data);
  }
  else {
    v = mrb_obj_as_string(mrb, data);
    len = RSTRING_LEN(v);
    buf = mrb_malloc(mrb, len + 1);
    buf[0] = (uint8_t)reg;
    memcpy(buf + 1, RSTRING_PTR(v), len);
  }

  addr = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@addr"));

  mbedI2CWrite(mrb_fixnum(addr), (const char*)buf, len + 1, 0);

  mrb_free(mrb, buf);

  return mrb_nil_value();
}

static mrb_value
mrb_i2c_read(mrb_state *mrb, mrb_value self)
{
  mrb_int reg, len, i;
  uint8_t *buf;
  mrb_value addr, data;
  char creg;

  mrb_get_args(mrb, "ii", &reg, &len);

  creg = (char)reg;
  buf = mrb_malloc(mrb, len);
  memset(buf, 0, len);

  addr = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@addr"));

  mbedI2CWrite(mrb_fixnum(addr), &creg, sizeof(creg), 0);

  while (mbedI2CRead(mrb_fixnum(addr), (char*)buf, len)) {
    mbedDelay(1);
  }

  data = mrb_ary_new_capa(mrb, len);
  for (i=0; i<len; i++) {
    mrb_ary_push(mrb, data, mrb_fixnum_value(buf[i]));
  }

  mrb_free(mrb, buf);

  return data;
}

static mrb_value
mrb_i2c_start(mrb_state *mrb, mrb_value self)
{
  mbedI2CStart();
  return mrb_nil_value();
}

static mrb_value
mrb_i2c_end(mrb_state *mrb, mrb_value self)
{
  mbedI2CStop();
  return mrb_nil_value();
}

void
mrb_mruby_mbed_i2c_init(mrb_state *mrb)
{
  struct RClass *i2c = mrb_define_class(mrb, "I2C", mrb->object_class);

  mrb_define_method(mrb, i2c, "initialize", mrb_i2c_init, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, i2c, "write", mrb_i2c_write, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, i2c, "read", mrb_i2c_read, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, i2c, "_start", mrb_i2c_start, MRB_ARGS_NONE());
  mrb_define_method(mrb, i2c, "_end", mrb_i2c_end, MRB_ARGS_NONE());
}
