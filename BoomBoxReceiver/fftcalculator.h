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
  /*!
   * \brief calculate
   * \param data - zdekodowane i znormalizowane dane z jednj ramki
   * \return std::tuple<float*, float*> krotka z obliczoną fft dla lewego i prawgo kanału audio
   */
  std::tuple<float*, float*> calculate(int16_t *data);
  /*!
   * \brief getSamplesPerChannel
   * \return int - liczba próbek dla kanału
   */
  int getSamplesPerChannel();
 private:
  RDFTContext* rdftContext;
  int samples;
  int samplesPerChannel;

};

#endif  // FFTCALCULATOR_H
