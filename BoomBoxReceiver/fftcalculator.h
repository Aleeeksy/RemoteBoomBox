#ifndef FFTCALCULATOR_H
#define FFTCALCULATOR_H

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <tuple>

#ifdef __cplusplus
extern "C" {
#endif
#include "libavcodec/avfft.h"
#include "libavutil/avutil.h"
#include "libavutil/mem.h"
#ifdef __cplusplus
};
#endif

class FftCalculator {
 public:
  FftCalculator(int samples);
  std::tuple<float*, float*> calculate(int16_t *data);
  int getSamplesPerChannel();
 private:
  RDFTContext* rdftContext;
  int samples;
  int samplesPerChannel;

};

#endif  // FFTCALCULATOR_H
