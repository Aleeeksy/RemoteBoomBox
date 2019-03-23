#ifndef STREAMRECEIVER_H
#define STREAMRECEIVER_H

#include <math.h>
#include <QWidget>
#include <iostream>
#include <list>
#include <fftcalculator.h>
#include <visualizationtype.h>
#include <simplevisualizationtype.h>

#define __STDC_CONSTANT_MACROS
#define MAX_AUDIO_FRAME_SIZE 192000

#ifdef __cplusplus
extern "C" {
#endif
#include <ao/ao.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/mathematics.h>
#include <libavutil/opt.h>
#include <libavutil/time.h>
#include <libavresample/avresample.h>
#include <libswresample/swresample.h>
#ifdef __cplusplus
};
#endif

/*!
 * \brief The StreamReceiver class
 *
 * Klasa odpowiada za odczytytywanie, dekodowanie oraz odtwarzanie streamu
 *
 */
class StreamReceiver : public QObject {
  Q_OBJECT
 public:
  explicit StreamReceiver(QObject* parent = 0);
    /*!
   * \brief prepareToRecevingStream
   * @param inputStream - adres URL streamu
   *
   * Metoda ta odpowiada za otwarcie streamu, odczytanie informacji, odnalezienie streamu audio, odczytanie kodeku
   *
   */
  void prepareToRecevingStream(std::string inputStream);
  /*!
   * \brief getSamplesPerChannel
   * \return - zwraca ilość próbek dla kanałów
   */
  int getSamplesPerChannel();

 public slots:
  /*!
   * \brief startRecevingStream
   *
   * Metoda ta roczpoczyna odbieranie streamu, po wcześniejszym przygotowaniu w prepareToRecevingStream
   *
   */
  void startRecevingStream();

 signals:
  void transferData(std::tuple<float*, float*>);

 private:
  ao_device* prepareAudioPlayback(AVSampleFormat&);
  void streamLoop(AVSampleFormat&, ao_device*, AVAudioResampleContext*);
  AVSampleFormat init_resampling(AVAudioResampleContext**, AVCodecContext*);

 private:
  AVFormatContext* formatContext;
  AVCodecContext* codecContext;
  AVCodec* codec;
  FftCalculator *fftCalculator;
  int streamIndex;

};

#endif  // STREAMRECEIVER_H
