#include <stddef.h>
#ifndef RoS
#define RoS

struct SmallRoomTag;

typedef struct SmallRoomTag {
    wchar_t roomArr[8][8];
    wchar_t collisionArr[8][8];
} SmallRoom;

struct LargeRoomTag;

typedef struct LargeRoomTag {
    wchar_t roomArr[8][16];
    wchar_t collisionArr[8][16];
} LargeRoom;

#endif