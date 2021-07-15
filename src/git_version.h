
#ifndef AIRQUALITY_ESP32_GIT_VERSION_H
#define AIRQUALITY_ESP32_GIT_VERSION_H

#endif //AIRQUALITY_ESP32_GIT_VERSION_H

#ifndef GIT_HASH_MACRO
#define GIT_HASH_MACRO "undefined"
#endif

#define xstr(s) str(s)
#define str(s) #s
#define GIT_HASH xstr(GIT_HASH_MACRO)