#include <unistd.h>
#include <fcntl.h>

// open function ------------

int open(const char *path, int oflag, ...);

/*
    The open() function shall establish the connection between a file and a file descriptor. 
    It shall create an open file description that refers to a file and a file descriptor that refers to that open file description. 
    The file descriptor is used by other I/O functions to refer to that file. 
    The path argument points to a pathname naming the file.

    Upon successful completion, these functions shall open the file and return a non-negative integer representing the file descriptor. 
    Otherwise, these functions shall return -1 and set errno to indicate the error. 
    If -1 is returned, no files shall be created or modified.
*/

// read function ------------

ssize_t read(int fildes, void *buf, size_t nbyte);

/*
    The read() function shall attempt to read nbyte bytes from the file associated with the open file descriptor, 
    fildes, into the buffer pointed to by buf. 
    The behavior of multiple concurrent reads on the same pipe, FIFO, or terminal device is unspecified.

    Upon successful completion, these functions shall return a non-negative integer indicating the number of bytes actually read (zero indicates end of file). 
    Otherwise, the functions shall return -1 and set errno to indicate the error.
*/ 

// write function ------------

ssize_t write(int fildes, const void *buf, size_t nbyte);

/*
    The write() function shall attempt to write nbyte bytes from the buffer pointed to by buf to the file associated with the open file descriptor, fildes.

    Upon successful completion, these functions shall return the number of bytes actually written to the file associated with fildes. 
    This number shall never be greater than nbyte. 
    Otherwise, -1 shall be returned and errno set to indicate the error.
*/

// close function ------------

int close(int fildes);

/*
    The close() function shall deallocate the file descriptor indicated by fildes. 
    To deallocate means to make the file descriptor available for return by subsequent calls to open() or other functions that allocate file descriptors. 
    All outstanding record locks owned by the process on the file associated with the file descriptor shall be removed (that is, unlocked).

    Upon successful completion, 0 shall be returned; otherwise, -1 shall be returned and errno set to indicate the error.
*/