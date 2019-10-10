/*
    FUSE: Filesystem in Userspace
    Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

    This program can be distributed under the terms of the GNU LGPLv2.
    See the file COPYING.LIB
*/

#include "fuse.h"
#include "fuse_lowlevel.h"

struct fuse_lowlevel_ops;
struct fuse_req;

struct fuse_session {
    struct fuse_session_ops op;
    void *data;
    volatile int exited;
    struct fuse_chan *ch;
};

struct fuse_chan {
    struct fuse_chan_ops op;
    struct fuse_session *se;
    int fd;
    size_t bufsize;
    void *data;
};

struct fuse_cmd {
    char *buf;
    size_t buflen;
    struct fuse_chan *ch;
};

/**
 * Query the file descriptor of the channel
 *
 * @param ch the channel
 * @return the file descriptor passed to fuse_chan_new()
 */
static inline int fuse_chan_fd(struct fuse_chan *ch);
int fuse_chan_fd(struct fuse_chan *ch) { return ch-> fd; }

/**
 * Query the minimal receive buffer size
 *
 * @param ch the channel
 * @return the buffer size passed to fuse_chan_new()
 */
static inline size_t fuse_chan_bufsize(struct fuse_chan *ch);
size_t fuse_chan_bufsize(struct fuse_chan *ch) { return ch->bufsize; }

/**
 * Query the user data
 *
 * @param ch the channel
 * @return the user data passed to fuse_chan_new()
 */
static inline void *fuse_chan_data(struct fuse_chan *ch);
void *fuse_chan_data(struct fuse_chan *ch) { return ch->data; }

/**
 * Query the session to which this channel is assigned
 *
 * @param ch the channel
 * @return the session, or NULL if the channel is not assigned
 */
static inline struct fuse_session *fuse_chan_session(struct fuse_chan *ch);
struct fuse_session *fuse_chan_session(struct fuse_chan *ch) { return ch->se; }

struct fuse_chan *fuse_kern_chan_new(int fd);
void fuse_kern_unmount(const char *mountpoint, int fd);
int fuse_kern_mount(const char *mountpoint, struct fuse_args *args);

#define FUSE_MAX_MAX_PAGES 256
#define FUSE_DEFAULT_MAX_PAGES_PER_REQ 32

/* room needed in buffer to accommodate header */
#define FUSE_BUFFER_HEADER_SIZE 0x1000
