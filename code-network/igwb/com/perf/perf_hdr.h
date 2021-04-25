#ifndef __PERF_HEADER_H_
#define __PERF_HEADER_H_

#ifdef _PLATFORM_WIN32
#define DELIMIT_CHAR    '\\'
#else
#define DELIMIT_CHAR    '/'
#endif /* _PLATFORM_WIN32 */

#define MAKE_QUALIFIED_PATH(path, filename) \
( ( *((path).end() - 1) != DELIMIT_CHAR )? \
    (path) + DELIMIT_CHAR + (filename) : \
    (path) + (filename) )

#define ALARM_DISK_ROOM_ID          0
#define ALARM_FRONT_SAVE_PARA       0
#define ALARM_BACK_SAVE_PARA        1
#define ALARM_BACKUP_DEV_PARA       2


#endif /* __PERF_HEADER_H_ */
