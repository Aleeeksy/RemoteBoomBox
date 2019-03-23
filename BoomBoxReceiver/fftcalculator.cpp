#include "fftcalculator.h"

FftCalculator::FftCalculator(int samples) {
  this->samples = samples;
  this->samplesPerChannel = samples / 2;
  this->rdftContext = av_rdft_init((int) log2(samplesPerChannel), DFT_R2C);
}

std::tuple<float*, float*> FftCalculator::calculate(int16_t* data) {
  float LeftData[samplesPerChannel * 2];
  float rightData[samplesPerChannel * 2];

  for (int i = 0, index = 0; i < samples; i += 2) {
    int16_t left = data[i];
    int16_t right = data[i + 1];

    double window_modifier =
        (0.5 * (1 - cos(2 * M_PI * index /
                        (samplesPerChannel - 1))));  // Hann (Hanning) window function
    float value = (float)(window_modifier * ((left) / 32768.0f));  // Convert to float and apply

    // cap values above 1 and below -1
    if (value > 1.0) {
      value = 1;
    } else if (value < -1.0) {
      value = -1;
    }

    LeftData[index] = value;
    value = (float)(window_modifier * ((right) / 32768.0f));

    if (value > 1.0) {
      value = 1;
    } else if (value < -1.0) {
      value = -1;
    }
    rightData[index] = value;
    index++;
  }

  float *leftData2 = (float *)av_malloc(samplesPerChannel * 2 * sizeof (float));
  memcpy(leftData2, LeftData, sizeof(float) * samplesPerChannel * 2);
  float *rightData2 = (float *)av_malloc(samplesPerChannel * 2 * sizeof (float));
  memcpy(rightData2, rightData, sizeof(float) * samplesPerChannel * 2);

  av_rdft_calc(rdftContext, leftData2);
  av_rdft_calc(rdftContext, rightData2);

  return std::tuple<float*, float*>(leftData2, rightData2);
}

int FftCalculator::getSamplesPerChannel() {
    return this->samplesPerChannel;
}
