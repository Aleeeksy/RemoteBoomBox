#ifndef STREAMPRODUCER_H
#define STREAMPRODUCER_H

#include <stdio.h>
#include <string>
#include <string.h>

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

class StreamProducer
{
public:
    StreamProducer();
    void startStreaming(std::string);

private:
    void prepareInputSource();
    void prepareOoutputStream();
    void streamLoop();
    int errorHandler(std::string, int);

private:
    AVOutputFormat *outputFormat;
    //Input AVFormatContext and Output AVFormatContext
    AVFormatContext *inputFormatContext;
    AVFormatContext *outputFormatContext;
    AVPacket packet;
    const char *inFilePath;
    const char *outStreamName;
    int streamIndex;
};

#endif // STREAMPRODUCER_H
