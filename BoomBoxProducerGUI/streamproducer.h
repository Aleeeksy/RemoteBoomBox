#ifndef STREAMPRODUCER_H
#define STREAMPRODUCER_H

#include <stdio.h>
#include <string.h>
#include <QObject>
#include <string>

#define __STDC_CONSTANT_MACROS

#ifdef __cplusplus
extern "C" {
#endif
#include <libavformat/avformat.h>
#include <libavutil/mathematics.h>
#include <libavutil/time.h>
#ifdef __cplusplus
};
#endif

/*!
 * \brief The StreamProducer class
 *
 * Klasa odpowiada za:
 *  - odczytanie wybranego pliku audio wybranego w interfejsie graficznym
 *  - przegotowanie streamu na podstawie tego pliku
 *  - rozpoczęcie streamu
 *
 */

class StreamProducer : public QObject {
  Q_OBJECT
 public:
  explicit StreamProducer(QObject* parent = 0);
  ~StreamProducer();
  void setInFilePath(std::string);
  void setAbandoned(bool);
  void setPaused(bool);
  bool isAbandoned();
  bool isPaused();

 public slots:
  /*!
   * \brief startStreaming
   *
   * metoda odpowiada za rozpoczęcie streamowania utworu
   *
   */
  void startStreaming();

 private:
  void prepareInputSource();
  void prepareOoutputStream();
  void streamLoop();
  void errorHandler(std::string, int);
  void pauseLoop();

 private:
  AVOutputFormat* outputFormat;
  AVFormatContext* inputFormatContext;
  AVFormatContext* outputFormatContext;
  AVPacket packet;
  std::string outStreamName;
  std::string inFilePath;
  int streamIndex;
  bool abandoned;
  bool paused;
};

#endif  // STREAMPRODUCER_H
