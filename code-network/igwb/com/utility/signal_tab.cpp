#include "../include/signal_tab.h"

int g_nSignalToExit[37] = {
	0,		  //		   0	
    0,        //SIGHUP     1    /* hangup */
    0,        //SIGINT     2    /* interrupt (rubout) */
    1,        //SIGQUIT    3    /* quit (ASCII FS) */
    1,        //SIGILL     4    /* illegal instruction (not reset when caught) */
    1,        //SIGTRAP    5    /* trace trap (not reset when caught) */
    1,        //SIGABRT    6    /* used by abort, replace SIGIOT in the future */
    1,        //SIGEMT     7    /* EMT instruction */
    1,        //SIGFPE     8    /* floating point exception */
    1,        //SIGKILL    9    /* kill (cannot be caught or ignored) */
    1,        //SIGBUS     10   /* bus error */
    1,        //SIGSEGV    11   /* segmentation violation */
    1,        //SIGSYS     12   /* bad argument to system call */
    0,        //SIGPIPE    13   /* write on a pipe with no one to read it */
    0,        //SIGALRM    14   /* alarm clock */
    1,        //SIGTERM    15   /* software termination signal from kill */
    0,        //SIGUSR1    16   /* user defined signal 1 */
    0,        //SIGUSR2    17   /* user defined signal 2 */
    0,        //SIGCHLD    18   /* child status change alias (POSIX) */
    0,        //SIGPWR     19   /* power-fail restart */
    0,        //SIGWINCH   20   /* window size change */
#ifdef _PLATFORM_WIN32
    1,        //SIGURG     21   /* urgent socket condition */
#else //_PLATFORM_WIN32 
    0,        //SIGURG     21   /* urgent socket condition */
#endif //_PLATFORM_WIN32
    0,        //SIGPOLL    22   /* pollable event occured */
    1,        //SIGSTOP    23   /* stop (cannot be caught or ignored) */
    0,        //SIGTSTP    24   /* user stop requested from tty */
    0,        //SIGCONT    25   /* stopped process has been continued */
    0,        //SIGTTIN    26   /* background tty read attempted */
    0,        //SIGTTOU    27   /* background tty write attempted */
    0,        //SIGVTALRM  28   /* virtual timer expired */
    0,        //SIGPROF    29   /* profiling timer expired */
    1,        //SIGXCPU    30   /* exceeded cpu limit */
    1,        //SIGXFSZ    31   /* exceeded file size limit */
    0,        //SIGWAITING 32   /* process's lwps are blocked */
    0,        //SIGLWP     33   /* special signal used by thread library */
    0,        //SIGFREEZE  34   /* special signal used by CPR */
    0,        //SIGTHAW    35   /* special signal used by CPR */
    0         //SIGCANCEL  36   /* thread cancellation signal used by libthread */
};

const char* const g_szSignalDesp[37] = {
	"UNKNOWN",	  //0
    "SIGHUP",     //1    /* hangup */
    "SIGINT",     //2    /* interrupt (rubout) */
    "SIGQUIT",    //3    /* quit (ASCII FS) */
    "SIGILL",     //4    /* illegal instruction (not reset when caught) */
    "SIGTRAP",    //5    /* trace trap (not reset when caught) */
    "SIGABRT",    //6    /* used by abort, replace SIGIOT in the future */
    "SIGEMT",     //7    /* EMT instruction */
    "SIGFPE",     //8    /* floating point exception */
    "SIGKILL",    //9    /* kill (cannot be caught or ignored) */
    "SIGBUS",     //10   /* bus error */
    "SIGSEGV",    //11   /* segmentation violation */
    "SIGSYS",     //12   /* bad argument to system call */
    "SIGPIPE",    //13   /* write on a pipe with no one to read it */
    "SIGALRM",    //14   /* alarm clock */
    "SIGTERM",    //15   /* software termination signal from kill */
    "SIGUSR1",    //16   /* user defined signal 1 */
    "SIGUSR2",    //17   /* user defined signal 2 */
    "SIGCHLD",    //18   /* child status change alias (POSIX) */
    "SIGPWR",     //19   /* power-fail restart */
    "SIGWINCH",   //20   /* window size change */
    "SIGURG",     //21   /* urgent socket condition */
    "SIGPOLL",    //22   /* pollable event occured */
    "SIGSTOP",    //23   /* stop (cannot be caught or ignored) */
    "SIGTSTP",    //24   /* user stop requested from tty */
    "SIGCONT",    //25   /* stopped process has been continued */
    "SIGTTIN",    //26   /* background tty read attempted */
    "SIGTTOU",    //27   /* background tty write attempted */
    "SIGVTALRM",  //28   /* virtual timer expired */
    "SIGPROF",    //29   /* profiling timer expired */
    "SIGXCPU",    //30   /* exceeded cpu limit */
    "SIGXFSZ",    //31   /* exceeded file size limit */
    "SIGWAITING", //32   /* process's lwps are blocked */
    "SIGLWP",     //33   /* special signal used by thread library */
    "SIGFREEZE",  //34   /* special signal used by CPR */
    "SIGTHAW",    //35   /* special signal used by CPR */
    "SIGCANCEL"   //36   /* thread cancellation signal used by libthread */
};

