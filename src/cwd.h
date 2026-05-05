//stolen from skeeto on Reddit https://www.reddit.com/user/skeeto/

#ifndef CWD_H
#define CWD_H

#include <stddef.h>

/**
* Write the current working directory to the given buffer.
*
* Returns CWD_SUCCESS on success.
* Returns CWD_ERANGE if the buffer is too small.
* Returns CWD_FAILURE if the operation cannot succeed.
*/
enum cwd_result {CWD_SUCCESS, CWD_ERANGE, CWD_FAILURE}
cwd(char *, size_t);

#endif
