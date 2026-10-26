#include <syslog.h>
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

#define PRINT_AND_LOG(level, message, ...)        \
    do {                                        \
    printf(message, ##__VA_ARGS__);               \
    syslog(level, message, ##__VA_ARGS__);     \
    } while(0);

#define PRINT_AND_LOG_INFO(message, ...)     PRINT_AND_LOG(LOG_INFO, message, ##__VA_ARGS__);
#define PRINT_AND_LOG_ERROR(message, ...)    PRINT_AND_LOG(LOG_ERR, message, ##__VA_ARGS__);
#define PRINT_AND_LOG_DEBUG(message, ...)    PRINT_AND_LOG(LOG_DEBUG, message, ##__VA_ARGS__);

int main(int argc, char *argv[])
{
    openlog(NULL, 0, LOG_USER);

    
    // The first argument is the command run to run the app
    int userArgc = argc - 1;
    
    if(userArgc != 2)
    {
        PRINT_AND_LOG_ERROR("Expected 2 arguments, but %d were given.\n", userArgc);
        return 1;
    }

    const char* pathToWrite = argv[1];
    const char* stringToWrite = argv[2];

    FILE* file = fopen(pathToWrite, "w+");
    if(file == NULL)
    {
        const char* errorDescription = strerror(errno);
        if(errorDescription != NULL)
        {
            PRINT_AND_LOG_ERROR("Unable to open file: %s\n", errorDescription);
        }
        return errno;
    }

    int strLength = strlen(stringToWrite);

    if(strLength < 0)
    {
        PRINT_AND_LOG_ERROR("Invalid string length: %d\n", strLength);
        return 1;
    }
    
    PRINT_AND_LOG_DEBUG("Writing %s to %s\n", stringToWrite, pathToWrite);

    int written = fwrite(stringToWrite, 1, strLength, file);
    int err = ferror(file);
    
    if(err != 0)
    {
        const char* errorDescription = strerror(errno);
        if(errorDescription != NULL)
        {
            PRINT_AND_LOG_ERROR("Unable to write: %s\n", errorDescription);
        }
        fclose(file);
        return errno;
    }

    if(written != strLength)
    {
        PRINT_AND_LOG_ERROR("Unexpected amount of bytes written. Should be %d, but only %d were written.\n", strLength, written);
        fclose(file);
        return 1;
    }


    fclose(file);
    return 0;
}
