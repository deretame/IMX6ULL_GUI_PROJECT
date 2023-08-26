#include <fcntl.h>
#include <linux/types.h> /* for videodev2.h */
#include <linux/videodev2.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define NB_BUFFER 32
#define DBG(...)                                                               \
    fprintf(stderr, " DBG(%s, %s(), %d): ", __FILE__, __FUNCTION__, __LINE__); \
    fprintf(stderr, __VA_ARGS__)

/* ./video_test </dev/video0> */

int main(int argc, char ** argv)
{
    int fd;
    int i;
    int type;
    struct v4l2_fmtdesc fmtdesc;
    struct v4l2_frmsizeenum fsenum;
    int fmt_index   = 0;
    int frame_index = 0;
    void * bufs[NB_BUFFER];
    int buf_cnt;

    if (argc != 2)
    {
        printf("Usage: %s </dev/videoX>, print format detail for video device\n", argv[0]);
        return -1;
    }

    /* open */
    fd = open(argv[1], O_RDWR);
    if (fd < 0)
    {
        printf("can not open %s\n", argv[1]);
        return -1;
    }

    // 查询能力
    struct v4l2_capability cap;

    memset(&cap, 0, sizeof(struct v4l2_capability));
    if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == 0)
    {
        if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0)
        {
            fprintf(stderr, "Error opening device %s: video capture not supported.\n", argv[1]);
            return -1;
        }

        if (!(cap.capabilities & V4L2_CAP_STREAMING))
        {
            fprintf(stderr, "%s does not support streaming i/o\n", argv[1]);
            return -1;
        }
    }
    else
    {
        printf("can't get capability\n");
        return -1;
    }

    while (1)
    {
        /* 枚举格式 */
        fmtdesc.index = fmt_index;                     // 比如从0开始
        fmtdesc.type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;   // 指定type为"捕获"
        if (0 != ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc))
            break;

        frame_index = 0;
        while (1)
        {
            /* 枚举这种格式所支持的帧大小 */
            memset(&fsenum, 0, sizeof(struct v4l2_frmsizeenum));
            fsenum.pixel_format = fmtdesc.pixelformat;
            fsenum.index        = frame_index;

            if (ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &fsenum) == 0)
            {
                printf("format %s,%d, framesize %d: %d x %d\n", fmtdesc.description, fmtdesc.pixelformat, frame_index, fsenum.discrete.width, fsenum.discrete.height);
            }
            else
            {
                break;
            }

            frame_index++;
        }

        fmt_index++;
    }

    /* 设置格式 */
    struct v4l2_format fmt;
    memset(&fmt, 0, sizeof(struct v4l2_format));
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = 1024;
    fmt.fmt.pix.height      = 768;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    fmt.fmt.pix.field       = V4L2_FIELD_ANY;
    if (ioctl(fd, VIDIOC_S_FMT, &fmt) == 0)
    {
        printf("set format ok, real dpi : %d * %d\n", fmt.fmt.pix.width, fmt.fmt.pix.height);
    }
    else
    {
        printf("can't set format : dpi : 1024 * 768\n");
        return -1;
    }

    struct v4l2_requestbuffers rb;
    struct v4l2_buffer buf;

    memset(&rb, 0, sizeof(struct v4l2_requestbuffers));
    rb.count  = NB_BUFFER;
    rb.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    rb.memory = V4L2_MEMORY_MMAP;

    buf_cnt = rb.count;

    if (ioctl(fd, VIDIOC_REQBUFS, &rb) == 0)
    {
        // 申请成功后, mmap这些buffer
        for (i = 0; i < rb.count; i++)
        {
            buf_cnt = rb.count;
            memset(&buf, 0, sizeof(struct v4l2_buffer));
            buf.index  = i;
            buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_MMAP;

            if (ioctl(fd, VIDIOC_QUERYBUF, &buf) == 0)
            {
                /* mmap */
                bufs[i] = mmap(0 /* start anywhere */, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
                if (bufs[i] == MAP_FAILED)
                {
                    perror("Unable to map buffer");
                    return -1;
                }
            }
            else
            {
                printf("can't query buffer\n");
                return -1;
            }
        }
        printf("map %d buffers ok\n", rb.count);
    }
    else
    {
        printf("can't request buffers\n");
        return -1;
    }

    /* 将所有 buffer 放入空闲链表 */
    for (i = 0; i < buf_cnt; ++i)
    {
        memset(&buf, 0, sizeof(struct v4l2_buffer));
        buf.index  = i;
        buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        if (ioctl(fd, VIDIOC_QBUF, &buf) != 0)
        {
            perror("Unable to queue buffer");
            return -1;
        }
    }
    printf("queue buffer ok\n");

    /* 启动摄像头 */
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (ioctl(fd, VIDIOC_STREAMON, &type) != 0)
    {
        perror("Unable to start capture");
        return -1;
    }
    printf("start capture ok\n");

    while (1)
    {
        // poll

        // 把buffer取出队列

        // 把buffer的数据存为文件

        // 把buffer放入队列
    }

    // 关闭摄像头
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    DBG("STopping capture\n");
    if (ioctl(fd, VIDIOC_STREAMOFF, &type) != 0)
    {
        perror("Unable to stop capture");
        return -1;
    }
    printf("stop capture ok\n");

    return 0;
}
