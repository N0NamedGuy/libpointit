/*
    v4l2grabber.c
    V4L2 picture grabber for libpointit

    It uses the memory mapping method and expects the camera to support the
    YUV4:4:2 format
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "v4l2grabber.h"
#include "../color.h"

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>
#include <linux/videodev2.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x) )

struct buffer {
    void*   start;
    size_t  len;
};

static char *       cam_dev_name = NULL;
static int          cam_fd = -1;
struct buffer*      cam_buffers = NULL;
static unsigned int cam_n_buffers = 0;

int cam_width, cam_height;

/*===========================================================================*/
/*=  Util stuff                                                             =*/
/*===========================================================================*/

static int errno_report(const char* s) {
    fprintf(stderr, "%s error %d, %s\n",
        s, errno, strerror(errno));

    return -1;
}

static int xioctl (int fd, int request, void* arg) {
    int r;
    do r = ioctl(fd, request, arg);
    while (r == -1 && EINTR == errno);
    
    return r;   
}

/*===========================================================================*/
/*=  Image processing stuff                                                 =*/
/*===========================================================================*/
static void process_image(const void* p) {
    /* TODO: convert to RGB image */
}

static int read_frame(void) {
    struct v4l2_buffer buf;

    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    if (xioctl(cam_fd, VIDIOC_DQBUF, &buf)) {
        switch (errno) {
        case EAGAIN: return -1;

        case EIO:
        default: return errno_report("VIDIOC_DQBUF");
        }
    }

    process_image(cam_buffers[buf.index].start);

    if (xioctl(cam_fd, VIDIOC_QBUF, &buf) == -1) {
        return errno_report("VIDIOC_QBUF");
    }

    return 0;
}

static int do_capture(void) {
    fd_set fds;
    struct timeval tv;
    int r;

    for (;;) { 
        FD_ZERO(&fds);
        FD_SET(cam_fd, &fds);

        /* Timeout. */
        tv.tv_sec = 2;
        tv.tv_usec = 0;

        r = select(cam_fd + 1, &fds, NULL, NULL, &tv);

        if (r == -1) {
            if (errno != EINTR) return errno_report("select");
        }
   
        if (r == 0) {
            fprintf(stderr, "select timeout\n");
            return -1;
        }

        if (read_frame() == 0) break;
    }

    return 0;
}

/*===========================================================================*/
/*=  Init/Destroy stuff                                                     =*/
/*===========================================================================*/

static int start_capturing(void) {
    unsigned int i;
    enum v4l2_buf_type type;

    for (i = 0; i < cam_n_buffers; i++) {
        struct v4l2_buffer buf;

        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (xioctl(cam_fd, VIDIOC_QBUF, &buf) == -1) {
            return errno_report("VIDIOC_QBUF");
        }

    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (xioctl(cam_fd, VIDIOC_STREAMON, &type)) {
        return errno_report("VIDIOC_STREAMON");
    }

    return 0;
}

static int stop_capturing(void) {
    enum v4l2_buf_type type;

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (xioctl(cam_fd, VIDIOC_STREAMOFF, &type) == -1)
        return errno_report("VIDIOC_STREAMOFF");

    return 0;
}

static int uninit_device(void) {
    unsigned int i;

    for (i = 0; i < cam_n_buffers; i++) {
        if (munmap(cam_buffers[i].start, cam_buffers[i].len) == -1) {
            return errno_report("munmap");
        }
    }

    return 0;
}

static int init_mmap(void) {
    struct v4l2_requestbuffers req;

    CLEAR(req);
    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (xioctl(cam_fd, VIDIOC_REQBUFS, &req) == -1) {
        if (errno == EINVAL) {
            fprintf(stderr, "%s does not support memory mapping\n", cam_dev_name);
            return -1;
        } else {
            return errno_report("VIDIOC_REQBUFS");
        }
    }

    if (req.count < 2) {
        fprintf(stderr, "Insufficient buffer memory on %s\n", cam_dev_name);
        return -1;
    }

    cam_buffers = calloc (req.count, sizeof(*cam_buffers));

    if (!cam_buffers) {
        fprintf(stderr, "Out of memory\n");
        return -1;
    }

    /* Stuff buffers with pretty things */
    for (cam_n_buffers = 0; cam_n_buffers < req.count; cam_n_buffers++) {
        struct v4l2_buffer buf;

        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = cam_n_buffers;

        if (xioctl(cam_fd, VIDIOC_QUERYBUF, &buf) == -1)
            return errno_report("VIDIOC_QUERYBUF");

        cam_buffers[cam_n_buffers].len = buf.length;
        cam_buffers[cam_n_buffers].start =
            mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED,
                cam_fd, buf.m.offset);

        if (cam_buffers[cam_n_buffers].start == MAP_FAILED)
            return errno_report("mmap");
    }

    return 0;
}

static int init_device(void) {
    struct v4l2_capability cap;
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    struct v4l2_format fmt;
    unsigned int min;

    if (xioctl(cam_fd, VIDIOC_QUERYCAP, &cap)) {
        if (EINVAL == errno) {
            fprintf(stderr, "%s is no V4L2 device\n", cam_dev_name);
            return -1;
        } else {
            return errno_report("VIDIOC_QUERYCAP");
        }
    }

    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        fprintf(stderr, "%s is no video capture device\n", cam_dev_name);
        return -1;
    }

    /* Using MMAP */
    if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
        fprintf(stderr, "%s does not support streaming i/o\n", cam_dev_name);
        return -1;  
    }

    CLEAR(cropcap);
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (xioctl(cam_fd, VIDIOC_CROPCAP, &cropcap) == 0) {
        /* If cropping is supported, do not crop */
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect;
        xioctl(cam_fd, VIDIOC_S_CROP, &crop);
    }

    CLEAR(fmt);

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = cam_width;
    fmt.fmt.pix.height = cam_height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_NONE; /* No interlacing, please */

    if (xioctl(cam_fd, VIDIOC_S_FMT, &fmt) == -1) {
        return errno_report("VIDIOC_S_FMT");
    }

    /* Buggy driver paranoia. */
    min = fmt.fmt.pix.width * 2;
    if (fmt.fmt.pix.bytesperline < min) fmt.fmt.pix.bytesperline = min;
    min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
    if (fmt.fmt.pix.sizeimage < min) fmt.fmt.pix.sizeimage = min;

    return init_mmap();
}

static int open_device(void) {
    struct stat st;

    if (-1 == stat(cam_dev_name, &st)) {
        fprintf(stderr, "Cannot identify '%s': %d, %s\n",
           cam_dev_name, errno, strerror(errno));
        return -1;
    }

    if (!S_ISCHR(st.st_mode)) {
        fprintf(stderr, "%s is no device\n", cam_dev_name);
        return -1;
    }

    cam_fd = open(cam_dev_name, O_RDWR | O_NONBLOCK, 0);

    if (cam_fd == -1) {
        fprintf(stderr, "Cannot open '%s': '%d', %s",
            cam_dev_name, errno, strerror(errno));
        return -1;
    }

    return 0;
}

static int close_device(void) {
    if (close(cam_fd) == -1) {
        return errno_report("close_device()");
    }
    cam_fd = -1;

    return 0;
}

/*===========================================================================*/
/*=  PointIt specific code                                                  =*/
/*===========================================================================*/
int pointit_init_cap(int w, int h) {
    
    /* TODO: Make this load from a config file */
    cam_width = w;
    cam_height = h;
    cam_dev_name = "/dev/video"; 

    if (open_device() != 0)     return -1;
    if (init_device() != 0)     return -1;
    if (start_capturing() != 0) return -1;

    return 0;
}

int pointit_destroy_cap(void) {
    if (stop_capturing() != 0) return -1;
    if (uninit_device() != 0)  return -1;
    if (close_device() != 0)   return -1;

    return 0;
}

int pointit_capture(void) {
    if (do_capture() != 0) return -1;
    return 0;
}

struct hsv_color pointit_get_color(int x, int y) {
    struct rgb_color rgb;
    rgb.r = 0;
    rgb.g = 0;
    rgb.b = 0;
    return rgb_to_hsv(rgb);
}

void pointit_show_cam(void) {
    /* FIXME: Do me */
    printf("Do me! Show cam!");
}

void pointit_hide_cam(void) {
    /* FIXME: Do me */
    printf("Do me! Hide cam!");
}
