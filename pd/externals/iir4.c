#include "iir.h"
#include "m_pd.h"
#include <string.h>

#define DSP_ARG_COUNT 4
#define IIR_ORDER 4
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

static t_class *iir4_tilde_class;

typedef struct iir4_tilde {
  t_object self;

  float w[IIR_ORDER];
  float coefficients[(IIR_ORDER + 1) * 2];

  t_float b0;
  t_float b1;
  t_float b2;
  t_float b3;
  t_float b4;

  // a0 is normalised to 1
  t_float a1;
  t_float a2;
  t_float a3;
  t_float a4;

  t_inlet *in;

  t_inlet *in_b0;
  t_inlet *in_b1;
  t_inlet *in_b2;
  t_inlet *in_b3;
  t_inlet *in_b4;

  t_inlet *in_a1;
  t_inlet *in_a2;
  t_inlet *in_a3;
  t_inlet *in_a4;

  t_outlet *out;
} t_iir4_tilde;

t_int *iir4_tilde_perform(t_int *w) {
  t_iir4_tilde *self = (t_iir4_tilde *)(w[1]);
  t_sample *in = (t_sample *)(w[2]);
  t_sample *out = (t_sample *)(w[3]);
  int buffer_size = (int)(w[4]);

  self->coefficients[0] = self->b0;
  self->coefficients[1] = self->b1;
  self->coefficients[2] = self->b2;
  self->coefficients[3] = self->b3;
  self->coefficients[4] = self->b4;
  self->coefficients[5] = 1;
  self->coefficients[6] = self->a1;
  self->coefficients[7] = self->a2;
  self->coefficients[8] = self->a3;
  self->coefficients[9] = self->a4;

  iir_process(in, out, buffer_size, self->coefficients, self->w,
              ARRAY_SIZE(self->coefficients));

  return w + DSP_ARG_COUNT + 1;
}

void iir4_tilde_dsp(t_iir4_tilde *self, t_signal **sp) {
  dsp_add(iir4_tilde_perform, DSP_ARG_COUNT, self, sp[0]->s_vec, sp[1]->s_vec,
          sp[0]->s_n);
}

void *iir4_tilde_new(t_symbol *s, int argc, t_atom *argv) {
  t_iir4_tilde *self = (t_iir4_tilde *)pd_new(iir4_tilde_class);

  self->b0 = 0;
  self->b1 = 0;
  self->b2 = 0;
  self->b3 = 0;
  self->b4 = 0;

  self->a1 = 0;
  self->a2 = 0;
  self->a3 = 0;
  self->a4 = 0;

  memset(self->w, 0, sizeof self->w);
  memset(self->coefficients, 0, sizeof self->w);

  self->in = signalinlet_new(&self->self, 0.f);

  self->in_b0 = floatinlet_new(&self->self, &self->b0);
  self->in_b1 = floatinlet_new(&self->self, &self->b1);
  self->in_b2 = floatinlet_new(&self->self, &self->b2);
  self->in_b3 = floatinlet_new(&self->self, &self->b3);
  self->in_b4 = floatinlet_new(&self->self, &self->b4);

  self->in_a1 = floatinlet_new(&self->self, &self->a1);
  self->in_a2 = floatinlet_new(&self->self, &self->a2);
  self->in_a3 = floatinlet_new(&self->self, &self->a3);
  self->in_a4 = floatinlet_new(&self->self, &self->a4);

  self->out = outlet_new(&self->self, &s_signal);

  return self;
}

void iir4_tilde_free(t_iir4_tilde *self) {
  inlet_free(self->in);

  inlet_free(self->in_b0);
  inlet_free(self->in_b1);
  inlet_free(self->in_b2);
  inlet_free(self->in_b3);
  inlet_free(self->in_b4);

  inlet_free(self->in_a1);
  inlet_free(self->in_a2);
  inlet_free(self->in_a3);
  inlet_free(self->in_a4);

  outlet_free(self->out);
}

void iir4_tilde_setup(void) {
  iir4_tilde_class = class_new(gensym("iir4~"), (t_newmethod)iir4_tilde_new,
                               (t_method)iir4_tilde_free, sizeof(t_iir4_tilde),
                               CLASS_DEFAULT, A_GIMME, 0);

  class_addmethod(iir4_tilde_class, (t_method)iir4_tilde_dsp, gensym("dsp"),
                  A_CANT, 0);
}
