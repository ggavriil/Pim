#include <sys/stat.h>
#include "uart.h"

#include <errno.h>
#undef errno
extern int errno;



/* A pointer to a list of environment variables and their values. For a minimal
   environment, this empty list is adequate: */
char *__env[1] = {0};
char **environ = __env;

extern caddr_t _get_stack_pointer(void);

extern int errno;


/* Never return from _exit as there's no OS to exit to, so instead we trap
   here */
void _exit(int status){
    (void)status;

    while(1) {
        //Do Nothing
    }
}


/* Increase program data space. As malloc and related functions depend on this,
 * it is useful to have a working implementation. The following suffices for a
 * standalone system; it exploits the symbol _end automatically defined by the
 * GNU linker.
 */

caddr_t _sbrk(int incr) {
  extern char _end;
  static char* heap_end = 0;
  char* prev_heap_end;

  if(heap_end == 0) {
    heap_end = &_end;
  }

  prev_heap_end = heap_end;

  heap_end += incr;
  return (caddr_t)prev_heap_end;
}

/* There's currently no implementation of a file system because there's no
   file system! */
int _close(int file) {
    return -1;
}


/* Transfer control to a new process. Minimal implementation (for a system
   without processes): */
int execve(char *name, char **argv, char **env) {
    errno = ENOMEM;
    return -1;
}


/* Create a new process. Minimal implementation (for a system without
   processes): */
int fork(void) {
    errno = EAGAIN;
    return -1;
}


/* Status of an open file. For consistency with other minimal implementations
   in these examples, all files are regarded as character special devices. The
   sys/stat.h header file required is distributed in the include subdirectory
   for this C library. */
int _fstat(int file, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}


/* Process-ID; this is sometimes used to generate strings unlikely to conflict
   with other processes. Minimal implementation, for a system without
   processes: */
int getpid(void) {
    return 1;
}


/* Query whether output stream is a terminal. For consistency with the other
   minimal implementations, which only support output to stdout, this minimal
   implementation is suggested: */
int _isatty(int file) {
    return 1;
}


/* Send a signal. Minimal implementation: */
int kill(int pid, int sig) {
    errno = EINVAL;
    return -1;
}


/* Establish a new name for an existing file. Minimal implementation: */
int link(char *old, char *new) {
    errno = EMLINK;
    return -1;
}


/* Set position in a file. Minimal implementation: */
int _lseek(int file, int ptr, int dir) {
    return 0;
}


/* Open a file. Minimal implementation: */
int open(const char *name, int flags, int mode) {
    return -1;
}


/* Read from a file. Minimal implementation: */
int _read(int file, char *ptr, int len) {
    return 0;
}

/* Status of a file (by name). Minimal implementation: */
int stat(const char *file, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}



/* Remove a file's directory entry. Minimal implementation: */
int unlink(char *name) {
    errno = ENOENT;
    return -1;
}


/* Wait for a child process. Minimal implementation: */
int wait(int *status) {
    errno = ECHILD;
    return -1;
}


int _write( int file, char *ptr, int len ) {
    for(int i = 0; i < len; i++) {
      uart_putc(*(ptr + i));
    }
}
