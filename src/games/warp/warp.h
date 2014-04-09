/* $Header: warp.h,v 7.0.1.3 95/21/1 17:08:42 lwall Exp $ */

/* $Log:	warp.h,v $
 * Revision 7.0.1.3 95/21/1 18:40:00 sms
 * Remove ifdefs around pwd.h
 *
 * Revision 7.0.1.2  86/12/12  17:08:42  lwall
 * Baseline for net release.
 *
 * Revision 7.0.1.1  86/10/16  10:54:26  lwall
 * Added Damage.  Fixed random bugs.
 *
 * Revision 7.0  86/10/08  15:17:55  lwall
 * Split into separate files.  Added amoebas and pirates.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

/* WARPLIB must be readable and writeable by warp, but not by anyone who you
 * don't trust.  In other words, to set up warp so everyone can play and
 * no one can cheat, give warp a uid of its own and make warp setuid to
 * that uid.  WARPLIB must then NOT be made writeable by the world,
 * since no attempt is made to encrypt saved games or anything.
 * (It must be readable by the world, however, due to a strangeness in
 * access.)
 */

#define SAVEDIR "./"
#define NEWSFILE "warp.news"
#define HELPFILE "warp.doc"
#define LOCKFILE ".warp.lock"
#define LOGFILE "warp.log"
#define SCOREBOARD "warp.top"
#define LSCOREBOARD "warp.lowtop"
#define FSCOREBOARD "warp.funtop"
#define TMPSCOREBOARD "warp.topnew"
#define WARPMACRO "%X/Kbmap.%{TERM}"

/* warp library */
#ifndef WARPLIB		/* ~ and %l only ("~%l" is permissable) */
#   define WARPLIB "/usr/games/warp"
#endif

EXT char *warplib;

#define PERMMAPS 8	/* how many starmaps are permanent */
#define MAPS 20		/* how many starmaps to choose from */
			/* (MAPS - PERMMAPS is # of half-gone universes) */

/*
 * Screen size info, minimum screen size is 23x40 (actually 24x80).
 * YSIZE and XSIZE should be relatively prime so that a torpedo launched
 * at an angle will eventually cover the whole screen.
 * To calculate a new position for something:
 * new_position = (current_position + delta + ?SIZE00) % ?SIZE
 * This allows for negative deltas of up to ?SIZE00 (% doesn't work right
 * on negative numbers).
 * ?SIZE01, etc. are fudges for efficiency--they already include a delta.
 */

#define XYSIZE 920
#define XYSIZEx4 3680

#define YSIZE   23
#define YSIZE00 2300
#define YSIZE01 2301
#define YSIZE99 2299

#define XSIZE   40
#define XSIZE00 4000
#define XSIZE01 4001
#define XSIZE99 3999
#define XSIZE02 4002
#define XSIZE98 3998
#define XSIZE03 4003
#define XSIZE97 3997
#define XSIZE08 4008
#define XSIZE92 3992

EXT char amb[YSIZE][XSIZE];

#ifndef isalnum
#   define isalnum(c) (isalpha(c) || isdigit(c))
#endif

#include <sys/ioctl.h>
#include <fcntl.h>
#include <pwd.h>

#ifdef TERMIO
#   include <termio.h>
#else
#   include <sgtty.h>
#endif

#define BITSPERBYTE 8
#define LBUFLEN 512	/* line buffer length */

#define CBUFLEN 256	/* command buffer length */
#define PUSHSIZE 128
#define MAXFILENAME 128
#define FINISHCMD 0177

/* some handy defs */

#define bool char
#define TRUE (1)
#define FALSE (0)
#define Null(t) ((t)0)
#define Nullch Null(char *)
#define Nullfp Null(FILE *)

#define Ctl(ch) (ch & 037)

#define strNE(s1,s2) (strcmp(s1,s2))
#define strEQ(s1,s2) (!strcmp(s1,s2))
#define strnNE(s1,s2,l) (strncmp(s1,s2,l))
#define strnEQ(s1,s2,l) (!strncmp(s1,s2,l))

#define sgn(x) ((x) < 0 ? -1 : (x) > 0)

/* Things we can figure out ourselves */

#ifdef SIGPROF
#   define BSD42		/* do we have Berkeley 4.2? */
#endif

#ifdef FIONREAD
#   define PENDING
#else
#   ifdef O_NDELAY
#	define PENDING
#   endif
#endif

#ifdef EUNICE
#   define UNLINK(victim) while (!unlink(victim))
#else
#   define UNLINK(victim) unlink(victim)
#endif

/* Valid substitutions for strings marked with % comment are:
 *	%H	Host name (yours)
 *	%L	Login name (yours)
 *	%N	Full name (yours)
 *	%O	Original working directory (where you ran warp from)
 *	%X	Warp library directory
 *	%~	Home directory
 *	%.	Directory containing . files
 *	%$	current process number
 *	%{name} Environment variable "name".  %{name-default} form allowed.
 *	%"prompt"
 *		Print prompt and insert what is typed.
 *	%`command`
 *		Insert output of command.
 *	%(test_text=pattern?if_text:else_text)
 *		Substitute if_text if test_text matches pattern, otherwise
 *		substitute else_text.  Use != for negated match.
 *		% substitutions are done on test_text, if_text, and else_text.
 *		(Note: %() only works if CONDSUB defined.)
 */

/* *** System Dependent Stuff *** */

/* NOTE: many of these are defined in the config.h file */

#ifndef LOGDIRFIELD
#   define LOGDIRFIELD 6		/* Which field (origin 1) is the */
					/* login directory in /etc/passwd? */
					/* (If it is not kept in passwd, */
					/* but getpwnam() returns it, */
					/* define the symbol GETPWENT) */
#endif
#ifndef GCOSFIELD
#   define GCOSFIELD 5
#endif

/* Undefine any of the following features to save both I and D space */
/* In general, earlier ones are easier to get along without */
/* Pdp11's without split I and D may have to undefine them all */
#define DEBUGGING	/* include debugging code */
#define PUSHBACK	/* macros and keymaps using pushback buffer */
#define CONDSUB		/* allow %(cond?text:text) */
#define BACKTICK	/* allow %`command` */
#define PROMPTTTY	/* allow %"prompt" */
#define GETLOGIN	/* use getlogin() routine as backup to environment */
			/* variables USER or LOGNAME */
#define TILDENAME	/* allow ~logname expansion */
#define SETUIDGID	/* substitute eaccess() for access() so that rn */
			/* can run setuid or setgid */
			/* if not setuid or setgid, you don't need it */
#define VERBOSE		/* compile in more informative messages */
#define TERSE		/* compile in shorter messages */

/* some dependencies among options */

#ifndef SETUIDGID
#   define eaccess access
#endif

#ifdef VERBOSE
#   ifdef TERSE
#	define IF(c) if (c)
#	define ELSE else
#   else /* !TERSE */
#	define IF(c)
#	define ELSE
#   endif
#else /* !VERBOSE */
#   ifndef TERSE
#	define TERSE
#   endif
#   define IF(c) "IF" outside of VERBOSE???
#   define ELSE "ELSE" outside of VERBOSE???
#endif

#ifdef DEBUGGING
#   define assert(ex) {if (!(ex)){fprintf(stderr,"Assertion failed: file %s, line %d\r\n", __FILE__, __LINE__);sig_catcher(0);}}
#else
#   define assert(ex) ;
#endif

#define TCSIZE 512	/* capacity for termcap strings */

/* End of Space Conservation Section */

/* More System Dependencies */
#define sigset signal
#define sigignore(sig) signal(sig,SIG_IGN)

/* preferred shell for use in doshell routine */
/*  ksh or sh would be okay here */
#ifndef PREFSHELL
#   define PREFSHELL "/bin/csh"
#endif

/* path to fastest starting shell */
#ifndef SH
#   define SH "/bin/sh"
#endif

/* location of macro file */
#ifndef WARPMACRO
#   ifdef PUSHBACK
#	define WARPMACRO "%./.warpmac"
#   endif
#endif

/* a motd-like file for warp */
#ifndef WARPNEWSNAME		/* % and ~ */
#   define WARPNEWSNAME "%X/warp.news"
#endif

/* typedefs */

typedef unsigned int	MEM_SIZE;	/* for passing to malloc */

/* *** end of the machine dependent stuff *** */

/* GLOBAL THINGS */

/* file statistics area */

EXT struct stat filestat;

EXT char buf[LBUFLEN+1];	/* general purpose line buffer */

EXT char *cwd INIT(Nullch);		/* current working directory */

/* switches */

#ifdef DEBUGGING
    EXT int debug INIT(0);				/* -D */
#   define DEB_FILEXP 64
#endif

#ifdef VERBOSE
#   ifdef TERSE
	EXT bool verbose INIT(TRUE);			/* +t */
#   endif
#endif

/* miscellania */

EXT FILE *tmpfp INIT(Nullfp);	/* scratch fp */

#define NOMARKING 0
#define STANDOUT 1
#define UNDERLINE 2

/* Factored strings */

EXT char nullstr[] INIT("");
EXT char readerr[] INIT("warp read error");
EXT char cantopen[] INIT("Can't open %s\r\n");

#ifdef VERBOSE
    EXT char nocd[] INIT("Can't chdir to directory %s\r\n");
#else
    EXT char nocd[] INIT("Can't find %s\r\n");
#endif

extern int errno;

EXT bool justonemoretime INIT(TRUE);
EXT bool keepgoing INIT(TRUE);

EXT bool friendspec INIT(FALSE);
EXT bool piratespec INIT(FALSE);
EXT bool amoebaspec INIT(FALSE);
EXT bool starspec INIT(FALSE);
EXT bool klingspec INIT(FALSE);
EXT bool apolspec INIT(FALSE);
EXT bool crushspec INIT(FALSE);
EXT bool romspec INIT(FALSE);
EXT bool prespec INIT(FALSE);
EXT bool tholspec INIT(FALSE);
EXT bool gornspec INIT(FALSE);
EXT bool beginner INIT(FALSE);
EXT bool massacre INIT(FALSE);
EXT bool lowspeed INIT(FALSE);
EXT bool debugging INIT(FALSE);
EXT bool didkill INIT(FALSE);
EXT bool experimenting INIT(FALSE);
EXT bool scorespec INIT(FALSE);
EXT bool metakey INIT(FALSE);

EXT bool bombed_out;
EXT bool panic INIT(FALSE);
EXT bool madgorns;

EXT int madfriends;

EXT int inumpirates;
EXT int numpirates;
EXT int inumfriends;
EXT int numfriends;
EXT int inumamoebas;
EXT int numamoebas;
EXT int inumstars;
EXT int numstars;
EXT int inumenemies;
EXT int numenemies;
EXT int inumroms;
EXT int inumthols;
EXT int inumapollos;
EXT int numapollos;
EXT int apolloflag;
EXT int inumcrushes;
EXT int numcrushes;
EXT int inumgorns;
EXT int numgorns;
EXT int deados;
EXT int deadmudds;
EXT int smarts;
EXT int ismarts INIT(0);
EXT int numos INIT(0);
EXT int numxes INIT(0);
EXT int ient;
EXT int numents;
EXT int ibase;
EXT int numbases;
EXT int inuminhab;
EXT int numinhab;
EXT int wave;
EXT int cumsmarts;
EXT int prescene INIT(-1);
EXT int scandist;
EXT int antibase;
EXT int sm35;
EXT int sm45;
EXT int sm50;
EXT int sm55;
EXT int sm80;
EXT int sm95;
EXT int entmax;
EXT int basemax;
EXT int enemshields;
EXT int super;
EXT int whenok;
EXT int yamblast;
EXT int xamblast;
EXT int ambsize;

EXT char spbuf[512];

#define Fclose (void)fclose
#define Fflush (void)fflush
#define Sprintf (void)sprintf
#define Signal (void)signal
#define Safecpy (void)safecpy
#define Cpytill (void)cpytill
#define Tract (void)tract
#define Make_object (void)make_object
#define Read_tty (void)read_tty
