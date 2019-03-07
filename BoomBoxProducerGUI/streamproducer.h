#ifndef STREAMPRODUCER_H
#define STREAMPRODUCER_H

#include <stdio.h>
#include <string>
#include <string.h>
#include <QObject>

#define __STDC_CONSTANT_MACROS

#ifdef __cplusplus
extern "C"
{
#endif
#include <libavformat/avformat.h>
#include <libavutil/mathematics.h>
#include <libavutil/time.h>
#ifdef __cplusplus
};
#endif


class StreamProducer : public QObject {
    Q_OBJECT
public:
    explicit StreamProducer(QObject *parent = 0);
    void setInFilePath(std::string);
    void setAbandoned(bool);
    void setPaused(bool);
    bool isAbandoned();
    bool isPaused();

public slots:
    void startStreaming();

private:
    void prepareInputSource();
    void prepareOoutputStream();
    void streamLoop();
    int errorHandler(std::string, int);
    void pauseLoop();

private:
    AVOutputFormat *outputFormat;
    //Input AVFormatContext and Output AVFormatContext
    AVFormatContext *inputFormatContext;
    AVFormatContext *outputFormatContext;
    AVPacket packet;
    std::string outStreamName;
    std::string inFilePath;
    int streamIndex;
    bool abandoned;
    bool paused;
};

#endif // STREAMPRODUCER_H
