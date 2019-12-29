#include "./lib.h"

void
err_msg(const char *buf, ...) {
	va_list parameters;
	char msg[MAXLINE];
	ssize_t ret;
	size_t len;
	char explanation[MAXLINE];
	int errno_saved;
	
	va_start(parameters, buf);
	vsnprintf(msg, MAXLINE - 1, buf, parameters);
	fflush(stderr);
	len = (size_t) 0;
	while (msg[len] != '\0') {
		len = len + (size_t) 1;
    }
	while ((len != (size_t) 0) && ((ret = write(STDERR_FILENO, msg, len)) != (ssize_t) 0)) {
		if (ret == (ssize_t) -1) {
			if (errno == EINTR) {
				errno = errno_saved;
				continue;
			}
			// err_msg("write error: %s", strerror_r(errno, explanation, MAXLINE));
			_exit(EXIT_FAILURE);
		}
		len = len - ret;
		*msg = *msg + ret;
	}
	fflush(NULL);
	va_end(parameters);
}

ssize_t 
write_msg(int fd, char *msg, size_t len) {
	ssize_t ret;
	char explanation[MAXLINE];
	int errno_saved;
	
	errno_saved = errno;
	errno = 0;
	while ((len != (size_t) 0) && ((ret = write(fd, msg, len)) != (ssize_t) 0)) {
		if (ret == (ssize_t) -1) {
			
			if (errno == EINTR) {
				errno = errno_saved;
				continue;
			}
			err_msg("write error: %s", strerror_r(errno, explanation, MAXLINE));
			_exit(EXIT_FAILURE);
		}
		len = len - ret;
		msg = msg + ret;
	}
	return ret;
}

void
print_line(int fd, const void *buf, ...) {
	va_list parameters;
	char msg[MAXLINE];
	ssize_t ret;
	size_t len;
	char explanation[MAXLINE];
	int errno_saved;
	
	errno_saved = errno;
	errno = 0;
	va_start(parameters, buf);
	vsnprintf(msg, MAXLINE - 1, buf, parameters);
	fflush(stdout);
	len = (size_t) 0;
	while (msg[len] != '\0') {
		len = len + (size_t) 1;
	}
	write_msg(fd, msg, len);
	fflush(NULL);
	va_end(parameters);
}

void *
Calloc(size_t nelem, size_t elsize) {
	void *array;
	char explanation[MAXLINE];
	int errno_saved;
	
	errno_saved = errno;
	errno = 0;
	if ((array = calloc(nelem, elsize)) == NULL) {
		err_msg("calloc error: %s", strerror_r(errno, explanation, MAXLINE));
		errno = errno_saved;
		exit(EXIT_FAILURE);
	}
	return array;
}

void *
Malloc(size_t elsize) {
	void *array;
	char explanation[MAXLINE];
	int errno_saved;
	
	errno_saved = errno;
	errno = 0;
	if ((array = malloc(elsize)) == NULL) {
		err_msg("calloc error: %s", strerror_r(errno, explanation, MAXLINE));
		errno = errno_saved;
		exit(EXIT_FAILURE);
	}
	return array;
}
