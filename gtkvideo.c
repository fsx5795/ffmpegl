#include <stdlib.h>
#include <signal.h>
#ifdef linux
#include <threads.h>
#endif
#ifdef WIN32
#include <gtk/gtk.h>
#endif
#include <time.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include "gtkvideo.h"
AVFrame* frame = NULL;
void play_video(void *data)
{
    (void*)data;
    //封装格式上下文
    AVFormatContext *pFormat = avformat_alloc_context();
    char *dir = g_get_current_dir();
    #ifdef linux
    char *fileName = "/abc.mp4";
    #endif
    #ifdef WIN32
    char *fileName = "\\abc.mp4";
    #endif
    char *path = alloca(strlen(dir) + strlen(fileName) + 1);
    strset(path, 0);
    strcat(path, dir);
    strcat(path, fileName);
    //打开输入文件，解封装
    if (avformat_open_input(&pFormat, path, NULL, NULL) != 0)
    {
        puts("avformat_open_input failed");
        return;
    }
    //寻找音视频流信息（解码方式、宽高）网络无法寻找流信息
    if (avformat_find_stream_info(pFormat, NULL) < 0)
    {
        puts("avformat_find_stream_info failed");
        return;
    }
    //寻找流
    int videoStream = av_find_best_stream(pFormat, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    int audioStream = av_find_best_stream(pFormat, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    //寻找解码器
    const AVCodec *avCodec = avcodec_find_decoder(pFormat->streams[videoStream]->codecpar->codec_id);
    if (avCodec == NULL)
    {
        puts("avcodec_find_decoder failed");
        return;
    }
    //创建解码器上下文
    AVCodecContext *codecContext = avcodec_alloc_context3(avCodec);
    //由于codec成员已废弃，因此需通过此函数将新视频流信息拷贝到新的context结构体中
    if (avcodec_parameters_to_context(codecContext, pFormat->streams[videoStream]->codecpar) != 0) {
        puts("avcodec_parameters_to_context failed");
        return;
    }
    //用avCodec解码器初始化context上下文
    if (avcodec_open2(codecContext, avCodec, NULL) < 0) {
        puts("avcodec_open2 failed");
        return;
    }
    //存放编码数据
    AVPacket *packet = av_packet_alloc();
    //存放解码后的数据
    frame = av_frame_alloc();
    //读取帧
    while (av_read_frame(pFormat, packet) >= 0)
    {
        //判断当前帧流类型是否为视频流
        if (packet->stream_index == AVMEDIA_TYPE_VIDEO)
        {
            //将要解码的数据包送入解码器
            if (avcodec_send_packet(codecContext, packet) < 0)
            {
                puts("avcodec_send_packet failed");
                return;
            }
            //获取解码数据
            int res = avcodec_receive_frame(codecContext, frame);
            if (res == AVERROR(EAGAIN) || res == AVERROR_EOF)
            {
                av_packet_unref(packet);
                continue;
            }
            else if (res < 0)
            {
                puts("avcodec_receive_frame failed");
                return;
            }
            #ifdef linux
            raise(SIGUSR1);
            thrd_sleep(&(struct timespec){.tv_nsec = 1000 * 1000 * 5}, NULL);
            #endif
            #ifdef WIN32
            raise(SIGINT);
            g_usleep(1000 * 5);
            #endif
        }
        av_packet_unref(packet);
    }
    if (packet)
    {
        av_packet_free(&packet);
        packet = NULL;
    }
    if (codecContext)
    {
        avcodec_close(codecContext);
        avcodec_free_context(&codecContext);
        codecContext = NULL;
        avCodec = NULL;
    }
    if (pFormat)
    {
        avformat_close_input(&pFormat);
        avformat_free_context(pFormat);
        pFormat = NULL;
    }
    return;
}