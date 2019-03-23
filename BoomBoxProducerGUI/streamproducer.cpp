#include "streamproducer.h"

StreamProducer::StreamProducer(QObject* parent) : QObject(parent) {
  outputFormat = NULL;

  inputFormatContext = NULL;
  outputFormatContext = NULL;
  streamIndex = 1;
  abandoned = false;
  paused = false;
  outStreamName = "udp://127.0.0.1:2300";
}

StreamProducer::~StreamProducer() {
  delete outputFormat;
  delete inputFormatContext;
  delete outputFormat;
  delete &outStreamName;
  delete &inFilePath;
  delete &packet;
}

void StreamProducer::startStreaming() {
  av_register_all();

  avformat_network_init();

  prepareInputSource();
  prepareOoutputStream();
  streamLoop();
}

void StreamProducer::prepareInputSource() {
  int ret;
  if ((ret = avformat_open_input(&inputFormatContext, inFilePath.c_str(), 0,
                                 0)) < 0) {
    errorHandler("Could not open input file", ret);
  }

  if ((ret = avformat_find_stream_info(inputFormatContext, 0)) < 0) {
    errorHandler("Failed to retrieve input stream information", ret);
  }

  for (int i = 0; i < inputFormatContext->nb_streams; i++) {
    if (inputFormatContext->streams[i]->codec->codec_type ==
        AVMEDIA_TYPE_AUDIO) {
      streamIndex = i;
      break;
    }
  }

  av_dump_format(inputFormatContext, 0, inFilePath.c_str(), 0);
}

void StreamProducer::prepareOoutputStream() {
  avformat_alloc_output_context2(&outputFormatContext, NULL, "mp2",
                                 outStreamName.c_str());  // UDP

  if (!outputFormatContext) {
    errorHandler("Could not create output context", AVERROR_UNKNOWN);
  }
  outputFormat = outputFormatContext->oformat;
  for (int i = 0; i < inputFormatContext->nb_streams; i++) {
    AVStream* inStream = inputFormatContext->streams[i];
    AVStream* outStream =
        avformat_new_stream(outputFormatContext, inStream->codec->codec);
    if (!outStream) {
      errorHandler("Failed allocating output stream", AVERROR_UNKNOWN);
    }
    int ret = avcodec_copy_context(outStream->codec, inStream->codec);
    if (ret < 0) {
      errorHandler(
          "Failed to copy context from input to output stream codec context",
          ret);
    }
    outStream->codec->codec_tag = 0;
    if (outputFormatContext->oformat->flags & AVFMT_GLOBALHEADER) {
      outStream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
    }
  }
  av_dump_format(outputFormatContext, 0, outStreamName.c_str(), 1);
  if (!(outputFormat->flags & AVFMT_NOFILE)) {
    if (int ret = avio_open(&outputFormatContext->pb, outStreamName.c_str(),
                            AVIO_FLAG_WRITE) < 0) {
      std::string errorMessage = "Could not open output URL: ";
      errorHandler(errorMessage.append(outStreamName.c_str()), ret);
    }
  }

  if (int ret = avformat_write_header(outputFormatContext, NULL) < 0) {
    std::string errorMessage = "Error occurred when opening output URL: ";
    errorHandler(errorMessage.append(outStreamName.c_str()), ret);
  }
}

void StreamProducer::streamLoop() {
  int frameIndex = 0;
  int64_t startTime = av_gettime();
  while (1) {
    AVStream* inStream;
    AVStream* outStream;
    if (int ret = av_read_frame(inputFormatContext, &packet) < 0) {
      break;
    }
    if (packet.pts == AV_NOPTS_VALUE) {
      AVRational time_base1 =
          inputFormatContext->streams[streamIndex]->time_base;
      // Duration between 2 frames (us)
      int64_t calc_duration =
          (double)AV_TIME_BASE /
          av_q2d(inputFormatContext->streams[streamIndex]->r_frame_rate);
      packet.pts = (double)(frameIndex * calc_duration) /
                   (double)(av_q2d(time_base1) * AV_TIME_BASE);
      packet.dts = packet.pts;
      packet.duration =
          (double)calc_duration / (double)(av_q2d(time_base1) * AV_TIME_BASE);
    }
    if (packet.stream_index == streamIndex) {
      AVRational time_base =
          inputFormatContext->streams[streamIndex]->time_base;
      AVRational time_base_q = {1, AV_TIME_BASE};
      int64_t pts_time = av_rescale_q(packet.dts, time_base, time_base_q);
      int64_t now_time = av_gettime() - startTime;
      if (pts_time > now_time) {
        av_usleep(pts_time - now_time);
      }
    }

    inStream = inputFormatContext->streams[packet.stream_index];
    outStream = outputFormatContext->streams[packet.stream_index];

    packet.pts = av_rescale_q_rnd(
        packet.pts, inStream->time_base, outStream->time_base,
        (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
    packet.dts = av_rescale_q_rnd(
        packet.dts, inStream->time_base, outStream->time_base,
        (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
    packet.duration = av_rescale_q(packet.duration, inStream->time_base,
                                   outStream->time_base);
    packet.pos = -1;

    if (packet.stream_index == streamIndex) {
      printf("Sended: %8d\n", frameIndex);
      frameIndex++;
    }

    if (int ret =
            av_interleaved_write_frame(outputFormatContext, &packet) < 0) {
      printf("Error muxing packet\n");
      break;
    }

    av_free_packet(&packet);
    if (abandoned) {
      errorHandler("Quit before time", 0);
      return;
    }

    if (paused) {
      pauseLoop();
    }
  }
  av_write_trailer(outputFormatContext);
}

void StreamProducer::pauseLoop() {
  while (paused) {
  }
}

void StreamProducer::errorHandler(std::string errorMessage, int errorCode) {
  avformat_close_input(&inputFormatContext);

  if (outputFormatContext && !(outputFormat->flags & AVFMT_NOFILE)) {
    avio_close(outputFormatContext->pb);
  }
  avformat_free_context(outputFormatContext);

  throw errorMessage;
}

void StreamProducer::setInFilePath(std::string pathToFile) {
  inFilePath = pathToFile;
}

void StreamProducer::setAbandoned(bool abandoned) {
  this->abandoned = abandoned;
}

void StreamProducer::setPaused(bool paused) {
  this->paused = paused;
}

bool StreamProducer::isAbandoned() {
  return abandoned;
}

bool StreamProducer::isPaused() {
  return paused;
}
