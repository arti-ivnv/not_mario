#ifndef RESOURCE_PATH_HPP
#define RESOURCE_PATH_HPP

#include <string>

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

inline std::string getResourcePath()
{
#ifdef __APPLE__
    CFBundleRef mainBundle   = CFBundleGetMainBundle();
    CFURLRef    resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char        path[PATH_MAX];
    if (CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        CFRelease(resourcesURL);
        return std::string(path) + "/";
    }
    CFRelease(resourcesURL);
#endif
    return "./"; // Fallback for Linux/Windows or terminal runs
}

#endif