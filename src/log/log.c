/*-
 * Copyright (c) 2008-2013 WiredTiger, Inc.
 *	All rights reserved.
 *
 * See the file LICENSE for redistribution information.
 */

#include "wt_internal.h"

static int
__log_record_size(WT_SESSION_IMPL *session,
    size_t *sizep, WT_LOGREC_DESC *recdesc, va_list ap)
{
	return (__wt_struct_sizev(session, sizep, recdesc->fmt, ap));
}

int
__wt_log_file_name(WT_SESSION_IMPL *session, WT_LOG *log, WT_ITEM *buf)
{
	WT_CONNECTION_IMPL *conn;
	WT_DECL_RET;
	WT_ITEM path;

	conn = S2C(session);
	WT_ERR(__wt_buf_init(session, &path,
	    strlen(conn->log_path) + ENTRY_SIZE));
	WT_RET(__wt_buf_fmt(session, buf, "log:%s.%10" PRIu32,
	    conn->log_path, WT_ATOMIC_ADD(log->fileid, 1)));
}

#ifdef	NOTDEF
int
__wt_log_put(WT_SESSION_IMPL *session, WT_LOGREC_DESC *recdesc, ...)
{
	WT_DECL_RET;
	WT_ITEM *buf;
	va_list ap;
	size_t size;

	buf = &session->logrec_buf;
	size = 0;

	va_start(ap, recdesc);
	ret = __log_record_size(session, &size, recdesc, ap);
	va_end(ap);
	WT_RET(ret);

	WT_RET(__wt_buf_initsize(session, buf, size));

	va_start(ap, recdesc);
	ret = __wt_struct_packv(session, buf->mem, size, recdesc->fmt, ap);
	va_end(ap);
	return (ret);
}
#endif

int
__wt_log_vprintf(WT_SESSION_IMPL *session, const char *fmt, va_list ap)
{
	WT_CONNECTION_IMPL *conn;
	WT_ITEM *buf;
	va_list ap_copy;
	size_t len;

	conn = S2C(session);

	if (conn->log_fh == NULL)
		return (0);

	buf = &session->logprint_buf;

	va_copy(ap_copy, ap);
	len = (size_t)vsnprintf(NULL, 0, fmt, ap_copy) + 2;
	va_end(ap_copy);

	WT_RET(__wt_buf_initsize(session, buf, len));

	(void)vsnprintf(buf->mem, len, fmt, ap);

	/*
	 * For now, just dump the text into the file.  Later, we will use
	 * __wt_logput_debug to wrap this in a log header.
	 */
#if 1
	strcpy((char *)buf->mem + len - 2, "\n");
	return ((write(conn->log_fh->fd, buf->mem, len - 1) ==
	    (ssize_t)len - 1) ? 0 : WT_ERROR);
#else
	return (__wt_logput_debug(session, (char *)buf->mem));
#endif
}

int
__wt_log_printf(WT_SESSION_IMPL *session, const char *fmt, ...)
    WT_GCC_FUNC_ATTRIBUTE((format (printf, 2, 3)))
{
	WT_DECL_RET;
	va_list ap;

	va_start(ap, fmt);
	ret = __wt_log_vprintf(session, fmt, ap);
	va_end(ap);

	return (ret);
}

int
__wt_log_open(WT_SESSION_IMPL *session)
{
	WT_DECL_RET;
	WT_CONNECTION_IMPL *conn;
	WT_ITEM path;
	WT_LOG *log;
	char

	conn = S2C(session);
	log = conn->log;
	WT_RET(__wt_open());
}

int
__wt_log_close(WT_SESSION_IMPL *session)
{
	WT_DECL_RET;
	WT_CONNECTION_IMPL *conn;
	WT_LOG *log;

	conn = S2C(session);
	log = conn->log;
}

static int
__log_acquire()
{
}

static int
__log_fill()
{
}

static int
__log_release()
{
}

int
__wt_log_read(WT_SESSION_IMPL *session, WT_ITEM *record, WT_LSN *lsnp,
    uint32_t flags)
{
}

int
__wt_log_scan(WT_SESSION_IMPL *session, WT_ITEM *record, uint32_t flags,
    int (*func)(WT_SESSION_IMPL *session, WT_ITEM *record, void *cookie),
    void *cookie)
{
}

int
__wt_log_write(WT_SESSION_IMPL *session, WT_ITEM *record, WT_LSN *lsnp,
    uint32_t flags)
{
}
