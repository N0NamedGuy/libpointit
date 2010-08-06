#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>

#include <linux/videodev2.h>

#include "v4l2grabber.h"
#include "../color.h"

#define CLEAR(x) memset (&(x), 0, sizeof(x))

struct buffer {
    void * start;
    size_t length;
};

static char*        dev_name    = NULL;
static int          fd          = -1;
struct buffer*      buffers     = NULL;
static unsigned int n_buffers   = 0;

static int width = 0;
static int height = 0;

static char* yuv_bitmap = NULL;

static int xioctl(int fd, int req, void* arg) {
    int r;

    do r = ioctl(fd, req, arg);
    while (-1 == r && EINTR == errno);

    return r;
}

int open_device() {
    /* Check if the video device in fact exists */
    struct stat st;

    if (stat(dev_name, &st) == -1 || !S_ISCHR(st.st_mode)) {
        return -1;
    }

    /* Opens the video device */
    fd = open(dev_name, O_RDWR | O_NONBLOCK, 0);

    if (fd == -1) {
        return -1;
    }

    return 0;
}

int init_mmap() {
    struct v4l2_requestbuffers req;

    CLEAR(req);

    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (xioctl(fd, VIDIOC_REQBUFS, &req) == -1) {
        /* No memory mapping support */
        return -1;
    }

    if (req.count < 2) {
        /* Not enough device mem */
        return -1;
    }

    buffers = calloc(req.count, sizeof(*buffers));

    if (!buffers) {
        /* Aw snap! No system mem! */
        return -1;
    }

    /* Now let's stuff our pretty buffers */
    for (n_buffers = 0; n_buffers < req.count; n_buffers++) {
        struct v4l2_buffer buf;

        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffers;

        if (xioctl(fd, VIDIOC_QUERYBUF, &buf) == -1) {
            return -1;
        }

        buffers[n_buffers].length = buf.length;
        buffers[n_buffers].start =
            mmap(NULL,
                buf.length,
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                fd, buf.m.offset);

        if (buffers[n_buffers].start == MAP_FAILED) {
            /* mmap did not work :( */
            return -1;
        }
    }

    return 0;
}

/* Pre-condition: the video device must be open */
int init_device() {
    struct v4l2_capability cap;
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    struct v4l2_format fmt;

    if (xioctl(fd, VIDIOC_QUERYCAP, &cap) == -1) {
        /* The device is not a V4L2 device */
        return -1;
    }

    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        /* The device doesn't support video capture */
        return -1;
    }

    /* We'll be using only mmap, because it seems to work everywhere */
    /* Select video input, video standard and tune: */
    CLEAR(cropcap);
    
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    
    if (xioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect;
    }

    /* Capture format setup */
    CLEAR(fmt);

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = width;
    fmt.fmt.pix.height = height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

    if (xioctl(fd, VIDIOC_S_FMT, &fmt) == -1) {
        /* The driver does not like de YUVY format... */
        /* TODO: Try other pixel formats */
        return -1;
    }

    return init_mmap();
}

/* Pre-condition: the device must be opened and initialized */
int start_cap() {
    unsigned int i;
    enum v4l2_buf_type type;

    for (i = 0; i < n_buffers; i++) {
        struct v4l2_buffer buf;

        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (xioctl(fd, VIDIOC_QBUF, &buf) == -1)
            return -1;

    }
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    
    /* Let's do it! */
    if (xioctl(fd, VIDIOC_STREAMON, &type) == -1) {
        return -1;
    }

    return 0;
}

int pointit_init_cap(int w, int h) {
    dev_name = "/dev/video";
    if (open_device() == -1) return -1;

    width = w;
    height = h;

    if (init_device() == -1) return -1;
    
    return 0;
}

int pointit_destroy_cap(void) {
    unsigned int i;
    for (i = 0; i < n_buffers; i++) {
        /* Won't check for errors, because we're quitting, and I don't really care... */
        munmap(buffers[i].start, buffers[i].length);
    }
    free(buffers);

    /* Won't check for errors, for the reason metioned above */
    close(fd);
    fd = -1;

    /* Always a huge succes! */
    return 0;
}

static int get_frame() {
    struct v4l2_buffer buf;

    CLEAR(buf);

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    if (xioctl(fd, VIDIOC_DQBUF, &buf) == -1) {
        switch (errno) {
            case EAGAIN:
                return -2;
            case EIO:
            default:
                return -1;
        }
    }

    yuv_bitmap = (char *)(buffers[buf.index].start);

    return 0;
}

int pointit_capture(void) {
    for (;;) {
        fd_set fds;
        struct timeval tv;
        int r;
        int ret;

        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        
        tv.tv_sec = 2;
        tv.tv_usec = 0;
        
        r = select(fd + 1, &fds, NULL, NULL, &tv);
        
        if (r == -1) {
            if (errno == EINTR) {
                continue;
            }

            return -1;
        }

        if (r == 0) {
            /* Oh snap! Camera timeout... */
            return -1;
        }

        ret = get_frame();
        if (ret == 0) break;
        else if (ret == -1) return -1;
    }

    return 0;
}

struct hsv_color yuv_to_hsv(int y, int u, int v) {
    struct rgb_color rgb;

    /* Convert YUV to RGB */
    rgb.r = y + (1.370705 * (v-128));
    rgb.g = y - (0.698001 * (v-128)) - (0.33763 * (u - 128));
    rgb.b = y + (1.732446 * (u-128));

    if (rgb.r > 255) rgb.r = 255;
    if (rgb.g > 255) rgb.g = 255;
    if (rgb.b > 255) rgb.b = 255;
    
    if (rgb.r < 0) rgb.r = 0;
    if (rgb.g < 0) rgb.g = 0;
    if (rgb.b < 0) rgb.b = 0;

    return rgb_to_hsv(rgb);
}

struct hsv_color pointit_get_color(int x, int y) {
    unsigned int pos = (y * width) + x; 
    unsigned int pixel_16;
    int y0, u, y1, v;
    

    /* I hope the compiler optimizes this... */
    pixel_16 =
        yuv_bitmap[pos + 3] << 24 |
        yuv_bitmap[pos + 2] << 16 |
        yuv_bitmap[pos + 1] <<  8 |
        yuv_bitmap[pos + 0];

        y0 = (pixel_16 & 0x000000ff);
        u  = (pixel_16 & 0x0000ff00) >> 8;
        y1 = (pixel_16 & 0x00ff0000) >> 16;
        v  = (pixel_16 & 0xff000000) >> 24;
        
    if (pos % 2 == 0) {
        return yuv_to_hsv(y1, u, v);
    } else {
        return yuv_to_hsv(y0, u, v);
    }
}

void pointit_show_cam(void) {
    /* FIXME: Do me */
    printf("Do me! Show cam!");
}

void pointit_hide_cam(void) {
    /* FIXME: Do me */
    printf("Do me! Hide cam!");
}
