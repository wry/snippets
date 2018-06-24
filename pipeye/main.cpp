// a pipe watcher
// use : ./0 <comand to read the pipe of> <regex to match with> <command to execute on match>

#include <iostream>
#include <memory>
#include <cstdlib>

#include <regex.h>
#include <stdio.h>

#if defined(HAS_NO_GETLINE)
static ssize_t getline(char **lineptr, size_t *n, FILE *stream);
#endif /* HAS_GETLINE */

int main(int argc, char *argv[])
{
    char *buf = NULL;
    size_t buflen = 0;
    ssize_t read = 0;
    regex_t regex;

    if (argc < 4)
    {
        std::cerr << "./0 <cmd> <regex> <exec>" << std::endl;
        return EXIT_FAILURE;
    }
    
    // compile regex
    if (regcomp(&regex, argv[2], 0))
    {
        std::cerr << "error : regcomp" << std::endl;
        return EXIT_FAILURE;
    }

    // create pipe to logcat
    auto fdtor = [](FILE *p) { pclose(p); };
    std::unique_ptr<FILE, decltype(fdtor)> fileptr(std::move(popen(argv[1], "r")), fdtor);

    if (!fileptr)
    {
        return EXIT_FAILURE;
    }

    // start logcat reaction loop
    while ((read = getline(&buf, &buflen, fileptr.get())) != -1)
    {
        std::string str(buf, read);
        if (regexec(&regex, buf, 0, NULL, 0) == 0)
        {
            std::cout << "match found : '" << str << "'. executing task : " << argv[3] << std::endl;
            system(argv[3]);
        }
    }

    free(buf);
    regfree(&regex);

    return EXIT_SUCCESS;
}

#ifndef HAS_GETLINE
// from https://android.googlesource.com/platform/external/elfutils/+/android-4.2.2_r1/bionic-fixup/getline.c
ssize_t getline(char **lineptr, size_t *n, FILE *stream)
{
    char *ptr;
    ptr = fgetln(stream, n);
    if (ptr == NULL)
    {
        return -1;
    }
    /* Free the original ptr */
    if (*lineptr != NULL)
        free(*lineptr);
    /* Add one more space for '\0' */
    size_t len = n[0] + 1;
    /* Update the length */
    n[0] = len;
    /* Allocate a new buffer */
    *lineptr = (char *)malloc(len);
    /* Copy over the string */
    memcpy(*lineptr, ptr, len - 1);
    /* Write the NULL character */
    (*lineptr)[len - 1] = '\0';
    /* Return the length of the new buffer */
    return len;
}
#endif /* DXP_STD_HAS_NO_GETLINE */
