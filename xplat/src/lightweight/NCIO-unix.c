/****************************************************************************
 * Copyright � 2003-2010 Dorian C. Arnold, Philip C. Roth, Barton P. Miller *
 *                  Detailed MRNet usage rights in "LICENSE" file.          *
 ****************************************************************************/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/socket.h>

#include "xplat_lightweight/NCIO.h"

const int XPlat_NCBlockingRecvFlag = MSG_WAITALL;

#ifndef IOV_MAX
#define IOV_MAX sysconf(_SC_IOV_MAX)
#endif

int NCSend(XPSOCKET s, NCBuf_t* ncbufs, unsigned int nBufs)
{
    int sret, ret = 0;
    unsigned int i;
    
    for( i = 0; i < nBufs; i++ ) {
        // do the send
        sret = write(s, ncbufs[i].buf, ncbufs[i].len);
        if( sret < 0 ) {
            ret = sret;
            break;
        }
        ret += sret;
    }
    return ret;
}

int NCRecv(XPSOCKET s, NCBuf_t* ncbufs, unsigned int nBufs)
{

    int err, ret = 0;

    unsigned int nBufsLeftToRecv = nBufs;
    NCBuf_t* currBuf = ncbufs;
    while (nBufsLeftToRecv > 0) {
        // determine how many bufs we will try to receive
        //int IOV_MAX = 1000; //this should get loaded from inc'd file
        unsigned int nBufsToRecv = ((nBufsLeftToRecv > IOV_MAX) ? IOV_MAX : nBufsLeftToRecv);

        // convert our buffer spec to recvmsg/readv's buffer spec
        struct msghdr msg;
    
        msg.msg_name = NULL;
        msg.msg_namelen = 0;
        struct iovec * iovec_new = (struct iovec *)malloc(sizeof(struct iovec) * nBufsToRecv);
        assert(iovec_new);
        msg.msg_iov = iovec_new;
        msg.msg_iovlen = nBufsToRecv;
#if defined(os_solaris)
        msg.msg_accrights = NULL;
        msg.msg_accrightslen = 0;
#else
        msg.msg_control = NULL;
        msg.msg_controllen = 0;
        msg.msg_flags = 0;
#endif 

        unsigned int i;
        for (i = 0; i < nBufsToRecv; i++) {
            msg.msg_iov[i].iov_base = currBuf[i].buf;
            msg.msg_iov[i].iov_len = currBuf[i].len;
        }

        // do the receive
        int sret = recvmsg(s, &msg, XPlat_NCBlockingRecvFlag);
        if (sret < 0) {
            perror("recvmsg()");
            ret = sret;
#ifndef os_solaris
            err = msg.msg_flags;
            fprintf(stderr, "NCRecv error msg_flags=%x\n", err);
#endif
            free( iovec_new );
            break;
        } else {
            ret += sret;
        }
        free( iovec_new );

        // advance through buffers
        nBufsLeftToRecv -= nBufsToRecv;
        currBuf+= nBufsToRecv;
    }

    return ret;
}