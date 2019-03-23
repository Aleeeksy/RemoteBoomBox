#include "streamreceiver.h"

StreamReceiver::StreamReceiver(QObject* parent) : QObject(parent) {
  formatContext = NULL;
  codecContext = NULL;
  codec = NULL;
  streamIndex = 0;
  fftCalculator = new FftCalculator(
      4096 / sizeof(int16_t));  // 4096 is the default sample size of libav
}

void StreamReceiver::prepareToRecevingStream(std::string inputStream) {
  av_register_all();
  avformat_network_init();

  formatContext = avformat_alloc_context();
  if (avformat_open_input(&formatContext, inputStream.c_str(), NULL, NULL) !=
      0) {
    fprintf(stderr, "Could not open stream\n");
    return;
  }
  if (avformat_find_stream_info(formatContext, NULL) < 0) {
    fprintf(stderr, "Failed to retrieve input stream information\n");
    return;
  }

  av_dump_format(formatContext, 0, inputStream.c_str(), false);

  if ((streamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, -1,
                                         -1, &codec, 0)) < 0) {
    av_log(NULL, AV_LOG_ERROR,
           "Cannot find a audio stream in the input file\n");
    return;
  }

  codecContext = formatContext->streams[streamIndex]->codec;
  codec = avcodec_find_decoder(codecContext->codec_id);

  // find & open codec
  if (codec == NULL) {
    printf("Cannot find codec!");
  }
  if (avcodec_open2(codecContext, codec, NULL) < 0) {
    fprintf(stderr, "Failed to open decoder for stream #%u in file '%s'\n",
            streamIndex, inputStream.c_str());
    return;
  }
}

void StreamReceiver::startRecevingStream() {
  // Contexts
  AVAudioResampleContext* resample = 0;
  AVSampleFormat sampleFormat = init_resampling(&resample, codecContext);

  ao_device* aoDevice = prepareAudioPlayback(sampleFormat);
  streamLoop(sampleFormat, aoDevice, resample);

  // clean up
  // swr_free(&swr);
  avcodec_close(codecContext);
  avformat_free_context(formatContext);
  avformat_close_input(&formatContext);

  // success
  return;
}

ao_device* StreamReceiver::prepareAudioPlayback(AVSampleFormat& sampleFormat) {
  ao_initialize();

  int driver = ao_default_driver_id();

  ao_sample_format sformat;

  sformat.bits = 16;
  sformat.channels = codecContext->channels;
  sformat.rate = codecContext->sample_rate / 2;
  sformat.byte_format = AO_FMT_LITTLE;
  sformat.matrix = "L,R";

  return ao_open_live(driver, &sformat, NULL);
}

AVSampleFormat StreamReceiver::init_resampling(
    AVAudioResampleContext** out_resample,
    AVCodecContext* dec_ctx) {
  AVAudioResampleContext* resample = avresample_alloc_context();

  int64_t layout = av_get_default_channel_layout(dec_ctx->channels);
  int sample_rate = dec_ctx->sample_rate;
  AVSampleFormat sampleFormat = AV_SAMPLE_FMT_S16;

  av_opt_set_int(resample, "in_channel_layout", layout, 0);
  av_opt_set_int(resample, "out_channel_layout", layout, 0);
  av_opt_set_int(resample, "in_sample_rate", sample_rate, 0);
  av_opt_set_int(resample, "out_sample_rate", sample_rate, 0);
  av_opt_set_int(resample, "in_sample_fmt", dec_ctx->sample_fmt, 0);
  av_opt_set_int(resample, "out_sample_fmt", sampleFormat, 0);

  avresample_open(resample);

  *out_resample = resample;

  return sampleFormat;
}

void StreamReceiver::streamLoop(AVSampleFormat& sampleFormat,
                                ao_device* aoDevice,
                                AVAudioResampleContext* resample) {
  AVPacket packet;
  av_init_packet(&packet);

  int buffer_size = MAX_AUDIO_FRAME_SIZE + FF_INPUT_BUFFER_PADDING_SIZE;

  uint8_t* buffer = new uint8_t[buffer_size];
  packet.data = buffer;
  packet.size = buffer_size;

  AVFrame* frame = av_frame_alloc();

  int gotFrame = 0;

  while ((av_read_frame(formatContext, &packet)) >= 0) {
    if (packet.stream_index == streamIndex) {
      if (int ret = avcodec_decode_audio4(codecContext, frame, &gotFrame,
                                          &packet) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Error decoding audio\n");
        continue;
      }

      if (gotFrame) {
        // Normalize the stream by resampling it
        uint8_t* output;
        int outLinesize;
        int outSamples =
            avresample_get_out_samples(resample, frame->nb_samples);
        av_samples_alloc(&output, &outLinesize, 2, outSamples, sampleFormat, 0);
        avresample_convert(resample, &output, outLinesize, outSamples,
                           frame->data, frame->linesize[0], frame->nb_samples);

        auto channelsData = fftCalculator->calculate((int16_t*)output);
        emit transferData(channelsData);
        ao_play(aoDevice, (char*)frame->extended_data[0], frame->linesize[0]);
        av_freep(&output);
      }
    }
    av_free_packet(&packet);
  }

  av_frame_free(&frame);
}

int StreamReceiver::getSamplesPerChannel() {
  return fftCalculator->getSamplesPerChannel();
}
