/* inih -- simple .INI file parser

inih is released under the New BSD license (see LICENSE.txt). Go to the project
home page for more info:

http://code.google.com/p/inih/

*/

#ifndef __INI_H__
#define __INI_H__

/* Make this header file easier to include in C++ code */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <sys/param.h>		/* for MAXPATHLEN */


/* struct to hold data from BBS.ini */
typedef struct {
	uint8_t		ComPort,	/* Com port the modem is on */
			LockBaudRate,	/* LockbaudRate to allow slower modems communicate at higher speed */
			AnswerOnRing,	/* Alternative to using modem's auto-answer mode */
			LogEnabled,	/* Enable logging to text file */
			RefndPostTime,	/* If 1, time spent posting is free */
			PayCrForPost,	/* If 1, pay 25cr per 80 chars of post, if 0, free */
			NewUserLevel,	/* User level new users start at */
			AllowEscChar,	/* If 1, let users use ANSI escape characters */
			FgColor,	/* Local foreground screen colour, doesn't effect remote user */
			BgColor,	/* Local background screen colour, doesn't effect remote user */
			StatusBarOffWhenLocal, /* If 1, status bar disappears when logged on locally */
			AutoLogon,	/* If not 0, log the account # on automatically */
			RefndULTime,	/* If 1, time spent uploading is free */
			AttachmentEnabled, /* If 1, enables attaching files to e-mail */
			CLSBeforeHelp,	/* If 1, clear screen before each help menu */
			FSEditClearScreen, /* If 1, clear screen before starting full screen editor */
			AskLongDesc,	/* If 1, BBS will ask for an extended description of uploaded files */
			AlwaysExpertMode; /* If 1, BBS will only display menu's when user presses ? */

	uint16_t	MdmDelayInit,	/* Delay in milliseconds before sending each init string */
			MdmDelayCnct,	/* Delay between init strings and logon screen */
			SleepMinutes,	/* Minutes before BBS automatically logs off inactive user */
			TransmitBuffer,	/* Numb of chars stored in modem buffer before being sent */
			ULCreditsPerK,	/* Credits given per kilobyte uploaded */
			DLCreditsPerMin,  /* Credits charged per minute of downloading */
			NewUserBonusTime; /* Extra time given to new accounts */

	uint32_t	MaxUsers,	/* Max users accounts allowed in userlog database */
			BaudRate,	/* Baud rate for modem */
			CarbonCopy;	/* User number to send a copy of feedback to, in addition to account 1 */

	uint64_t	MsgSpace,	/* Max disk space message base is allowed */
			LogSizeMax,	/* Max disk space for log file */
			CallsPerDayLevel0, /* Max numb of calls allowed per day for access level 0 */
			CallsPerDayLevel1, /* Max numb of calls allowed per day for access level 1 */
			CallsPerDayLevel2, /* Max numb of calls allowed per day for access level 2 */
			CallsPerDayLevel3, /* Max numb of calls allowed per day for access level 3 */
			CallsPerDayLevel4, /* Max numb of calls allowed per day for access level 4 */
			CallsPerDayLevel5, /* Max numb of calls allowed per day for access level 5 */
			CallsPerDayLevel6, /* Max numb of calls allowed per day for access level 6 */
			CallsPerDayLevel7, /* Max numb of calls allowed per day for access level 7 */
			CallsPerDayLevel8, /* Max numb of calls allowed per day for access level 8 */
			CallsPerDayLevel9, /* Max numb of calls allowed per day for access level 9 */
			MinutesPerDayLevel0, /* Max numb of mins allowed per day for access level 0 */
			MinutesPerDayLevel1, /* Max numb of mins allowed per day for access level 1 */
			MinutesPerDayLevel2, /* Max numb of mins allowed per day for access level 2 */
			MinutesPerDayLevel3, /* Max numb of mins allowed per day for access level 3 */
			MinutesPerDayLevel4, /* Max numb of mins allowed per day for access level 4 */
			MinutesPerDayLevel5, /* Max numb of mins allowed per day for access level 5 */
			MinutesPerDayLevel6, /* Max numb of mins allowed per day for access level 6 */
			MinutesPerDayLevel7, /* Max numb of mins allowed per day for access level 7 */
			MinutesPerDayLevel8, /* Max numb of mins allowed per day for access level 8 */
			MinutesPerDayLevel9, /* Max numb of mins allowed per day for access level 9 */
			NewUserCr,	/* Credits new users get */
			OldMailExpireDays, /* Days after old mail was created before it gets purged */
			NewMailExpireDays; /* Days after new mail was created before it gets purged */

	const char	*MdmInitStr1,	/* Modem initialization string 1 */
			*MdmInitStr2,	/* Modem initialization string 2 */
			*MdmInitStr3,	/* Modem initialization string 3 */
			*MdmDontAnswer,	/* Command sent to modem when you log on locally */
			*LogFile,	/* Name of log file */
			*SysDir,		/* Directory for data files */
			*TextDir,	/* Directory for text and help files */
			*MenuDir,	/* Directory for MNU files */
			*BBSMailDir,	/* Directory for e-mail files */
			*MsgDir,		/* Directory for networked message files */
			*NewUserString,	/* String needed to type at logon to create a new user */
			*UserInfo1,	/* User info field 1 */
			*UserInfo2,	/* User info field 2 */
			*UserInfo3,	/* User info field 3 */
			*UserInfo4,	/* User info field 4 */
			*UserInfo5,	/* User info field 5 */
			*UserInfo6,	/* User info field 6 */
			*UserInfo7,	/* User info field 7 */
			*UserInfo8,	/* User info field 8 */
			*UserInfo9,	/* User info field 9 */
			*ChatCommand,	/* Command to be executed as a chat call */
			*ForceLogoffPrint, /* Printed when the sysop disconnects a user */
			*SysopOnLine,	/* Displayed when sysop enters or leaves chat */
			*SysopOffLine,	/* Dispalyed when sysop leaves chat */
			*RemoteSysopPassword, /* Password used to access shell remotely, etc */
			*MidnightEvent,	/* Command to be executed every night at midnight */
			*GlobalMenu,	/* Menu file that lists the commands that are always available at any menu */
			*FirstMenu,	/* Menu file that gets read before main menu (ie, new e-mail, etc) */
			*RamDriveDir,	/* Directory to be used for RAM Drive */
			*BBSName,	/* Name of the BBS given on networked messages */
			*LineName0,	/* Name 0 of the phone lines displayed in the /who cmd and caller log */
			*LineName1,	/* Name 1 of the phone lines displayed in the /who cmd and caller log */
			*LineName2,	/* Name 2 of the phone lines displayed in the /who cmd and caller log */
			*LineName3,	/* Name 3 of the phone lines displayed in the /who cmd and caller log */
			*LineName4,	/* Name 4 of the phone lines displayed in the /who cmd and caller log */
			*LineName5,	/* Name 5 of the phone lines displayed in the /who cmd and caller log */
			*LineName6,	/* Name 6 of the phone lines displayed in the /who cmd and caller log */
			*LineName7,	/* Name 7 of the phone lines displayed in the /who cmd and caller log */
			*LineName8,	/* Name 8 of the phone lines displayed in the /who cmd and caller log */
			*LineName9,	/* Name 9 of the phone lines displayed in the /who cmd and caller log */
			*LineName10,	/* Name 10 of the phone lines displayed in the /who cmd and caller log */
			*LineName11,	/* Name 11 of the phone lines displayed in the /who cmd and caller log */
			*LineName12,	/* Name 12 of the phone lines displayed in the /who cmd and caller log */
			*LineName13,	/* Name 13 of the phone lines displayed in the /who cmd and caller log */
			*LineName14,	/* Name 14 of the phone lines displayed in the /who cmd and caller log */
			*LineName15,	/* Name 15 of the phone lines displayed in the /who cmd and caller log */
			*LineName16,	/* Name 16 of the phone lines displayed in the /who cmd and caller log */
			*LineName17,	/* Name 17 of the phone lines displayed in the /who cmd and caller log */
			*LineName18,	/* Name 18 of the phone lines displayed in the /who cmd and caller log */
			*LineName19;	/* Name 20 of the phone lines displayed in the /who cmd and caller log */
} syscfg;

/* Parse given INI-style file. May have name=value pairs (whitespace stripped),
   and comments starting with ';' (semicolon).

   For each name=value pair parsed, call handler function with given user
   pointer as well as name and value (data only valid for duration
   of handler call). Handler should return nonzero on success, zero on error.

   Returns 0 on success, line number of first error on parse error (doesn't
   stop on first error), -1 on file open error, or -2 on memory allocation
   error.
*/
int ini_parse(const char* filename,
              int (*handler)(void* user, const char* name, const char* value),
              void* user);

/* Same as ini_parse(), but takes a FILE* instead of filename. This doesn't
   close the file when it's finished -- the caller must do that. */
int ini_parse_file(FILE* file,
                   int (*handler)(void* user, const char* name, const char* value),
                   void* user);

int handler(void* user, const char* name, const char* value);

/* Function to fill in default values in case they're missing or not set in BBS.ini */
void default_ini(void* user);

/* Function to print BBS config values, can be used before BBS.ini is read to see   */
/* defaults or after to see what's read in */
void print_config(void* user);

/* Maximum line length for any line in INI file. */
#ifndef INI_MAX_LINE
#define INI_MAX_LINE 200
#endif

#ifdef __cplusplus
}
#endif

#endif /* __INI_H__ */
