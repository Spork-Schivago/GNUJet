/* ini.c -- reads values from BBS.ini and stores them in struct syscfg
 *
 * source originally from inih -- simple .INI file parser (By Ben Hoyt)
 *
 * inih is released under the New BSD license (see LICENSE.txt). Go to the project
 * home page for more info:
 *
 * http://code.google.com/p/inih/
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>		/* for strcasecmp and strdup*/
#include <inttypes.h>		/* for uint_t's and PRIu32 */
#include "ini.h"		/* typedef struct syscfg and function prototypes */


/* use heap instead of stack */
#include <stdlib.h>

#define MAX_NAME 50

/* Strip whitespace chars off end of given string, in place. Return s. */
static char* rstrip(char* s)
{
	char* p = s + strlen(s);

	while (p > s && isspace((unsigned char)(*--p))) {
		*p = '\0';
	}

	return s;
}

/* Return pointer to first non-whitespace char in given string. */
static char* lskip(const char* s)
{
	while (*s && isspace((unsigned char)(*s))) {
		s++;
	}

	return (char*)s;
}

/* Return pointer to first char c or ';' comment in given string, or pointer to
   null at end of string if neither found. ';' must be prefixed by a whitespace
   character to register as a comment. */
static char* find_char_or_comment(const char* s, char c)
{
	int was_whitespace = 0;

	while (*s && *s != c && !(was_whitespace && *s == ';')) {
		was_whitespace = isspace((unsigned char)(*s));
		s++;
	}

	return (char*)s;
}

/* Version of strncpy that ensures dest (size bytes) is null-terminated. */
static char* strncpy0(char* dest, const char* src, size_t size)
{
	strncpy(dest, src, size);
	dest[size - 1] = '\0';

	return dest;
}

/* See documentation in header file. */
int ini_parse_file(FILE* file,
                   int (*handler)(void*, const char*, const char*),
                   void* user)
{
	/* Use the heap */
	char* line;

	char prev_name[MAX_NAME] = "";

	char* start;
	char* end;
	char* name;
	char* value;
	int lineno = 0;
	int error = 0;

	/* Use the heap */
	line = (char*)malloc(INI_MAX_LINE);
	if (!line) {
		/* malloc error */
		return -2;
	}

	/* Scan through file line by line */
	while (fgets(line, INI_MAX_LINE, file) != NULL) {
		lineno++;

		start = line;
		start = lskip(rstrip(start));

		if (*start == ';') {
			/* Allow ';' comments at start of line */
		}
		else if (*start) {
			/* Not a comment, must be a name[:]value pair */
			end = find_char_or_comment(start, ':');

			if (*end == ':') {
				*end = '\0';
				name = rstrip(start);
				value = lskip(end + 1);
				end = find_char_or_comment(value, '\0');
				if (*end == ';') {
					*end = '\0';
				}
				rstrip(value);

				/* Valid name[=:]value pair found, call handler */
				strncpy0(prev_name, name, sizeof(prev_name));
				if (!handler(user, name, value) && !error) {
					error = lineno;
				}
			}
			else if (!error) {
				/* No ':' found on name[:]value line */
				error = lineno;
			}
		}
	}

	/* use heap */
	free(line);

	return error;
}

/* See documentation in header file. */
int ini_parse(const char* filename,
              int (*handler)(void*, const char*, const char*),
              void* user)
{
	FILE* file;
	int error;

	file = fopen(filename, "r");

	if (!file) {
		return -1;
	}

	error = ini_parse_file(file, handler, user);
	fclose(file);

	return error;
}

int handler(void* user, const char* name, const char* value) {
	syscfg* pconfig = (syscfg*)user;

	#define MATCH(n) strcasecmp(name, n) == 0
	if (MATCH("MaxUsers")) {
		pconfig->MaxUsers = atoi(value);
	} else if (MATCH("MsgSpace")) {
		pconfig->MsgSpace = atoi(value);
	} else if (MATCH("ComPort")) {
		pconfig->ComPort = atoi(value);
	} else if (MATCH("BaudRate")) {
		pconfig->BaudRate = atoi(value);
	} else if (MATCH("LockBaudRate")) {
		pconfig->LockBaudRate = atoi(value);
	} else if (MATCH("ModemInitString1")) {
		pconfig->MdmInitStr1 = strdup(value);
	} else if (MATCH("ModemInitString2")) {
		pconfig->MdmInitStr2 = strdup(value);
	} else if (MATCH("ModemInitString3")) {
		pconfig->MdmInitStr3 = strdup(value);
	} else if (MATCH("ModemDontAnswer")) {
		pconfig->MdmDontAnswer = strdup(value);
	} else if (MATCH("AnswerOnRing")) {
		pconfig->AnswerOnRing = atoi(value);
	} else if (MATCH("DelayBetweenModemInit")) {
		pconfig->MdmDelayInit = atoi(value);
	} else if (MATCH("DelayAfterConnect")) {
		pconfig->MdmDelayCnct = atoi(value);
	} else if (MATCH("LogEnabled")) {
		pconfig->LogEnabled = atoi(value);
	} else if (MATCH("LogFile")) {
		pconfig->LogFile = strdup(value);
	} else if (MATCH("LogSizeMax")) {
		pconfig->LogSizeMax = atoi(value);
	} else if (MATCH("SysDir")) {
		pconfig->SysDir = strdup(value);
	} else if (MATCH("TextDir")) {
		pconfig->TextDir = strdup(value);
	} else if (MATCH("MenuDir")) {
		pconfig->MenuDir = strdup(value);
	} else if (MATCH("BbsMailDir")) {
		pconfig->BBSMailDir = strdup(value);
	} else if (MATCH("MsgDir")) {
		pconfig->MsgDir = strdup(value);
	} else if (MATCH("CallsPerDayLevel0")) {
		pconfig->CallsPerDayLevel0 = atoi(value);
	} else if (MATCH("CallsPerDayLevel1")) {
		pconfig->CallsPerDayLevel1 = atoi(value);
	} else if (MATCH("CallsPerDayLevel2")) {
		pconfig->CallsPerDayLevel2 = atoi(value);
	} else if (MATCH("CallsPerDayLevel3")) {
		pconfig->CallsPerDayLevel3 = atoi(value);
	} else if (MATCH("CallsPerDayLevel4")) {
		pconfig->CallsPerDayLevel4 = atoi(value);
	} else if (MATCH("CallsPerDayLevel5")) {
		pconfig->CallsPerDayLevel5 = atoi(value);
	} else if (MATCH("CallsPerDayLevel6")) {
		pconfig->CallsPerDayLevel6 = atoi(value);
	} else if (MATCH("CallsPerDayLevel7")) {
		pconfig->CallsPerDayLevel7 = atoi(value);
	} else if (MATCH("CallsPerDayLevel8")) {
		pconfig->CallsPerDayLevel8 = atoi(value);
	} else if (MATCH("CallsPerDayLevel9")) {
		pconfig->CallsPerDayLevel9 = atoi(value);
	} else if (MATCH("MinutesPerDayLevel0")) {
		pconfig->MinutesPerDayLevel0 = atoi(value);
	} else if (MATCH("MinutesPerDayLevel1")) {
		pconfig->MinutesPerDayLevel1 = atoi(value);
	} else if (MATCH("MinutesPerDayLevel2")) {
		pconfig->MinutesPerDayLevel2 = atoi(value);
	} else if (MATCH("MinutesPerDayLevel3")) {
		pconfig->MinutesPerDayLevel3 = atoi(value);
	} else if (MATCH("MinutesPerDayLevel4")) {
		pconfig->MinutesPerDayLevel4 = atoi(value);
	} else if (MATCH("MinutesPerDayLevel5")) {
		pconfig->MinutesPerDayLevel5 = atoi(value);
	} else if (MATCH("MinutesPerDayLevel6")) {
		pconfig->MinutesPerDayLevel6 = atoi(value);
	} else if (MATCH("MinutesPerDayLevel7")) {
		pconfig->MinutesPerDayLevel7 = atoi(value);
	} else if (MATCH("MinutesPerDayLevel8")) {
		pconfig->MinutesPerDayLevel8 = atoi(value);
	} else if (MATCH("MinutesPerDayLevel9")) {
		pconfig->MinutesPerDayLevel9 = atoi(value);
	} else if (MATCH("RefundPostingTime")) {
		pconfig->RefndPostTime = atoi(value);
	} else if (MATCH("PayCreditsForPosting")) {
		pconfig->PayCrForPost = atoi(value);
	} else if (MATCH("NewUserLevel")) {
		pconfig->NewUserLevel = atoi(value);
	} else if (MATCH("NewUserCredits")) {
		pconfig->NewUserCr = atoi(value);
	} else if (MATCH("NewUserString")) {
		pconfig->NewUserString = strdup(value);
	} else if (MATCH("UserInfo1")) {
		pconfig->UserInfo1 = strdup(value);
	} else if (MATCH("UserInfo2")) {
		pconfig->UserInfo2 = strdup(value);
	} else if (MATCH("UserInfo3")) {
		pconfig->UserInfo3 = strdup(value);
	} else if (MATCH("UserInfo4")) {
		pconfig->UserInfo4 = strdup(value);
	} else if (MATCH("UserInfo5")) {
		pconfig->UserInfo5 = strdup(value);
	} else if (MATCH("UserInfo6")) {
		pconfig->UserInfo6 = strdup(value);
	} else if (MATCH("UserInfo7")) {
		pconfig->UserInfo7 = strdup(value);
	} else if (MATCH("UserInfo8")) {
		pconfig->UserInfo8 = strdup(value);
	} else if (MATCH("UserInfo9")) {
		pconfig->UserInfo9 = strdup(value);
	} else if (MATCH("AllowEscChar")) {
		pconfig->AllowEscChar = atoi(value);
	} else if (MATCH("ChatCommand")) {
		pconfig->ChatCommand = strdup(value);
	} else if (MATCH("ForceLogoffPrint")) {
		pconfig->ForceLogoffPrint = strdup(value);
	} else if (MATCH("SysopOnLine")) {
		pconfig->SysopOnLine = strdup(value);
	} else if (MATCH("SysopOffLine")) {
		pconfig->SysopOffLine = strdup(value);
	} else if (MATCH("SleepMinutes")) {
		pconfig->SleepMinutes = atoi(value);
	} else if (MATCH("RemoteSysopPassword")) {
		pconfig->RemoteSysopPassword = strdup(value);
	} else if (MATCH("MidnightEvent")) {
		pconfig->MidnightEvent = strdup(value);
	} else if (MATCH("CarbonCopyFeedbackTo")) {
		pconfig->CarbonCopy = atoi(value);
	} else if (MATCH("ForegroundColor")) {
		pconfig->FgColor = atoi(value);
	} else if (MATCH("BackgroundColor")) {
		pconfig->BgColor = atoi(value);
	} else if (MATCH("GlobalMenu")) {
		pconfig->GlobalMenu = strdup(value);
	} else if (MATCH("FirstMenu")) {
		pconfig->FirstMenu = strdup(value);
	} else if (MATCH("StatusBarOffWhenLocal")) {
		pconfig->StatusBarOffWhenLocal = atoi(value);
	} else if (MATCH("RamDriveDir")) {
		pconfig->RamDriveDir = strdup(value);
	} else if (MATCH("BBSName")) {
		pconfig->BBSName = strdup(value);
	} else if (MATCH("TransmitBuffer")) {
		pconfig->TransmitBuffer = atoi(value);
	} else if (MATCH("UploadCreditsPerK")) {
		pconfig->ULCreditsPerK = atoi(value);
	} else if (MATCH("DownloadCreditsPerMin")) {
		pconfig->DLCreditsPerMin = atoi(value);
	} else if (MATCH("NewUserBonusTime")) {
		pconfig->NewUserBonusTime = atoi(value);
	} else if (MATCH("AutoLogon")) {
		pconfig->AutoLogon = atoi(value);
	} else if (MATCH("RefundUploadingTime")) {
		pconfig->RefndULTime = atoi(value);
	} else if (MATCH("OldMailExpireDays")) {
		pconfig->OldMailExpireDays = atoi(value);
	} else if (MATCH("NewMailExpireDays")) {
		pconfig->NewMailExpireDays = atoi(value);
	} else if (MATCH("AttachmentEnabled")) {
		pconfig->AttachmentEnabled = atoi(value);
	} else if (MATCH("ClearScreenBeforeHelp")) {
		pconfig->CLSBeforeHelp = atoi(value);
	} else if (MATCH("FSEditClearScreen")) {
		pconfig->FSEditClearScreen = atoi(value);
	} else if (MATCH("LineName0")) {
		pconfig->LineName0 = strdup(value);
	} else if (MATCH("LineName1")) {
		pconfig->LineName1 = strdup(value);
	} else if (MATCH("LineName2")) {
		pconfig->LineName2 = strdup(value);
	} else if (MATCH("LineName3")) {
		pconfig->LineName3 = strdup(value);
	} else if (MATCH("LineName4")) {
		pconfig->LineName4 = strdup(value);
	} else if (MATCH("LineName5")) {
		pconfig->LineName5 = strdup(value);
	} else if (MATCH("LineName6")) {
		pconfig->LineName6 = strdup(value);
	} else if (MATCH("LineName7")) {
		pconfig->LineName7 = strdup(value);
	} else if (MATCH("LineName8")) {
		pconfig->LineName8 = strdup(value);
	} else if (MATCH("LineName9")) {
		pconfig->LineName9 = strdup(value);
	} else if (MATCH("LineName10")) {
		pconfig->LineName10 = strdup(value);
	} else if (MATCH("LineName11")) {
		pconfig->LineName11 = strdup(value);
	} else if (MATCH("LineName12")) {
		pconfig->LineName12 = strdup(value);
	} else if (MATCH("LineName13")) {
		pconfig->LineName13 = strdup(value);
	} else if (MATCH("LineName14")) {
		pconfig->LineName14 = strdup(value);
	} else if (MATCH("LineName15")) {
		pconfig->LineName15 = strdup(value);
	} else if (MATCH("LineName16")) {
		pconfig->LineName16 = strdup(value);
	} else if (MATCH("LineName17")) {
		pconfig->LineName17 = strdup(value);
	} else if (MATCH("LineName18")) {
		pconfig->LineName18 = strdup(value);
	} else if (MATCH("LineName19")) {
		pconfig->LineName19 = strdup(value);
	} else if (MATCH("AskLongDescription")) {
		pconfig->AskLongDesc = atoi(value);
	} else if (MATCH("AlwaysExpertMode")) {
		pconfig->AlwaysExpertMode = atoi(value);
	} else	{
		printf("Error: Found unknown key: value.\n");
		return 0;  /* unknown name, error */
	}

	return 1;
}

/* Function to fill the syscfg structure with default values. */
void default_ini(void* user) {
	syscfg* config = (syscfg*)user;

	config->MaxUsers = 299;
	config->MsgSpace = 20000000;
	config->ComPort = 1;
	config->BaudRate = 57600;
	config->LockBaudRate = 1;
	config->MdmInitStr1 = strdup("AT&F &K3 S95=1");
	config->MdmInitStr2 = strdup("AT &D2 &C1");
	config->MdmInitStr3 = strdup("AT E1 Q0 X3 V1 S2=1 S0=1");
	config->MdmDontAnswer = strdup("AT S0=0");
	config->AnswerOnRing = 0;
	config->MdmDelayInit = 1500;
	config->MdmDelayCnct = 5000;
	config->LogEnabled = 1;
	config->LogFile = strdup("LOG.TXT");
	config->LogSizeMax = 300000;
	config->SysDir = strdup("./SYS");
	config->TextDir = strdup("./TEXT");
	config->MenuDir = strdup("./MNU");
	config->BBSMailDir = strdup("./BBSMAIL");
	config->MsgDir = strdup("./MSG");
	config->CallsPerDayLevel0 = 3;
	config->CallsPerDayLevel1 = 20;
	config->CallsPerDayLevel2 = 40;
	config->CallsPerDayLevel3 = 10000;
	config->CallsPerDayLevel4 = 10000;
	config->CallsPerDayLevel5 = 10000;
	config->CallsPerDayLevel6 = 10000;
	config->CallsPerDayLevel7 = 10000;
	config->CallsPerDayLevel8 = 10000;
	config->CallsPerDayLevel9 = 10000;
	config->MinutesPerDayLevel0 = 45;
	config->MinutesPerDayLevel1 = 90;
	config->MinutesPerDayLevel2 = 90;
	config->MinutesPerDayLevel3 = 120;
	config->MinutesPerDayLevel4 = 120;
	config->MinutesPerDayLevel5 = 120;
	config->MinutesPerDayLevel6 = 10000;
	config->MinutesPerDayLevel7 = 10000;
	config->MinutesPerDayLevel8 = 10000;
	config->MinutesPerDayLevel9 = 10000;
	config->RefndPostTime = 1;
	config->PayCrForPost = 1;
	config->NewUserLevel = 1;
	config->NewUserCr = 200;
	config->NewUserString = strdup("new");
	config->UserInfo1 = strdup("Computer Type");
	config->UserInfo2 = strdup("");
	config->UserInfo3 = strdup("Phone number");
	config->UserInfo4 = strdup("Age");
	config->UserInfo5 = strdup("");
	config->UserInfo6 = strdup("");
	config->UserInfo7 = strdup("");
	config->UserInfo8 = strdup("");
	config->UserInfo9 = strdup("");
	config->AllowEscChar = 1;
	config->ChatCommand = strdup("none");
	config->ForceLogoffPrint = strdup("Have a nice day :)");
	config->SysopOnLine = strdup("-- Sysop is online --");
	config->SysopOffLine = strdup("");
	config->SleepMinutes = 9;
	config->RemoteSysopPassword = strdup("");
	config->MidnightEvent = strdup("");
	config->CarbonCopy = 0;
	config->FgColor = 7;
	config->BgColor = 0;
	config->GlobalMenu = strdup("./GLOBAL.MENU");
	config->FirstMenu = strdup("./FIRST.MENU");
	config->StatusBarOffWhenLocal = 0;
	config->RamDriveDir = strdup("");
	config->BBSName = strdup("");
	config->TransmitBuffer = 1024;
	config->ULCreditsPerK = 0;
	config->DLCreditsPerMin = 0;
	config->NewUserBonusTime = 0;
	config->AutoLogon = 0;
	config->RefndULTime = 1;
	config->OldMailExpireDays = 60;
	config->NewMailExpireDays = 99999;
	config->AttachmentEnabled = 1;
	config->CLSBeforeHelp = 0;
	config->FSEditClearScreen = 0;
	config->LineName0 = strdup("LOCAL");
	config->LineName1 = strdup("COM1");
	config->LineName2 = strdup("COM2");
	config->LineName3 = strdup("COM3");
	config->LineName4 = strdup("COM4");
	config->LineName5 = strdup("COM5");
	config->LineName6 = strdup("COM6");
	config->LineName7 = strdup("COM7");
	config->LineName8 = strdup("COM8");
	config->LineName9 = strdup("COM9");
	config->LineName10 = strdup("COM10");
	config->LineName11 = strdup("COM11");
	config->LineName12 = strdup("COM12");
	config->LineName13 = strdup("COM13");
	config->LineName14 = strdup("COM14");
	config->LineName15 = strdup("COM15");
	config->LineName16 = strdup("COM16");
	config->LineName17 = strdup("COM17");
	config->LineName18 = strdup("COM18");
	config->LineName19 = strdup("COM19");
	config->AskLongDesc = 1;
	config->AlwaysExpertMode = 0;
}

/* Function to print the syscfg structure. */
/* This function can be used to print the  */
/* default values once default_ini has     */
/* called or to print out the values found */
/* in BBS.ini.				   */
void print_config(void* user) {
	syscfg* config = (syscfg*)user;

	printf("MaxUsers: %"PRIu32".\n", config->MaxUsers);
	printf("MsgSpace: %"PRIu64".\n", config->MsgSpace);
	printf("ComPort: %"PRIu8".\n", config->ComPort);
	printf("BaudRate: %"PRIu32".\n", config->BaudRate);
	printf("LockBaudRate: %"PRIu8".\n", config->LockBaudRate);
	printf("ModemInitString1: %s.\n", config->MdmInitStr1);
	printf("ModemInitString2: %s.\n", config->MdmInitStr2);
	printf("ModemInitString3: %s.\n", config->MdmInitStr3);
	printf("ModemDontAnswer: %s.\n", config->MdmDontAnswer);
	printf("AnswerOnRing: %"PRIu8".\n", config->AnswerOnRing);
	printf("DelayBetweenModemInit: %"PRIu16".\n", config->MdmDelayInit);
	printf("DelayAfterConnect: %"PRIu16".\n", config->MdmDelayCnct);
	printf("LogEnabled: %"PRIu8".\n", config->LogEnabled);
	printf("LogFile: %s.\n", config->LogFile);
	printf("LogSizeMax: %"PRIu64".\n", config->LogSizeMax);
	printf("SysDir: %s.\n", config->SysDir);
	printf("TextDir: %s.\n", config->TextDir);
	printf("MenuDir: %s.\n", config->MenuDir);
	printf("BbsMailDir: %s.\n", config->BBSMailDir);
	printf("MsgDir: %s.\n", config->MsgDir);
	printf("CallsPerDayLevel0: %"PRIu64".\n", config->CallsPerDayLevel0);
	printf("CallsPerDayLevel1: %"PRIu64".\n", config->CallsPerDayLevel1);
	printf("CallsPerDayLevel2: %"PRIu64".\n", config->CallsPerDayLevel2);
	printf("CallsPerDayLevel3: %"PRIu64".\n", config->CallsPerDayLevel3);
	printf("CallsPerDayLevel4: %"PRIu64".\n", config->CallsPerDayLevel4);
	printf("CallsPerDayLevel5: %"PRIu64".\n", config->CallsPerDayLevel5);
	printf("CallsPerDayLevel6: %"PRIu64".\n", config->CallsPerDayLevel6);
	printf("CallsPerDayLevel7: %"PRIu64".\n", config->CallsPerDayLevel7);
	printf("CallsPerDayLevel8: %"PRIu64".\n", config->CallsPerDayLevel8);
	printf("CallsPerDayLevel9: %"PRIu64".\n", config->CallsPerDayLevel9);
	printf("MinutesPerDayLevel0: %"PRIu64".\n", config->MinutesPerDayLevel0);
	printf("MinutesPerDayLevel1: %"PRIu64".\n", config->MinutesPerDayLevel1);
	printf("MinutesPerDayLevel2: %"PRIu64".\n", config->MinutesPerDayLevel2);
	printf("MinutesPerDayLevel3: %"PRIu64".\n", config->MinutesPerDayLevel3);
	printf("MinutesPerDayLevel4: %"PRIu64".\n", config->MinutesPerDayLevel4);
	printf("MinutesPerDayLevel5: %"PRIu64".\n", config->MinutesPerDayLevel5);
	printf("MinutesPerDayLevel6: %"PRIu64".\n", config->MinutesPerDayLevel6);
	printf("MinutesPerDayLevel7: %"PRIu64".\n", config->MinutesPerDayLevel7);
	printf("MinutesPerDayLevel8: %"PRIu64".\n", config->MinutesPerDayLevel8);
	printf("MinutesPerDayLevel9: %"PRIu64".\n", config->MinutesPerDayLevel9);
	printf("RefundPostingTime: %"PRIu8".\n", config->RefndPostTime);
	printf("PayCreditsForPosting: %"PRIu8".\n", config->PayCrForPost);
	printf("NewUserLevel: %"PRIu8".\n", config->NewUserLevel);
	printf("NewUserCredits: %"PRIu64".\n", config->NewUserCr);
	printf("NewUserString: %s.\n", config->NewUserString);
	printf("UserInfo1: %s.\n", config->UserInfo1);
	printf("UserInfo2: %s.\n", config->UserInfo2);
	printf("UserInfo3: %s.\n", config->UserInfo3);
	printf("UserInfo4: %s.\n", config->UserInfo4);
	printf("UserInfo5: %s.\n", config->UserInfo5);
	printf("UserInfo6: %s.\n", config->UserInfo6);
	printf("UserInfo7: %s.\n", config->UserInfo7);
	printf("UserInfo8: %s.\n", config->UserInfo8);
	printf("UserInfo9: %s.\n", config->UserInfo9);
	printf("AllowEscChar: %"PRIu8".\n", config->AllowEscChar);
	printf("ChatCommand: %s.\n", config->ChatCommand);
	printf("ForceLogoffPrint: %s.\n", config->ForceLogoffPrint);
	printf("SysopOnLine: %s.\n", config->SysopOnLine);
	printf("SysopOffLine: %s.\n", config->SysopOffLine);
	printf("SleepMinutes: %"PRIu16".\n", config->SleepMinutes);
	printf("RemoteSysopPassword: %s.\n", config->RemoteSysopPassword);
	printf("MidnightEvent: %s.\n", config->MidnightEvent);
	printf("CarbonCopyFeedbackTo: %"PRIu32".\n", config->CarbonCopy);
	printf("ForegroundColor: %"PRIu8".\n", config->FgColor);
	printf("BackgroundColor: %"PRIu8".\n", config->BgColor);
	printf("GlobalMenu: %s.\n", config->GlobalMenu);
	printf("FirstMenu: %s.\n", config->FirstMenu);
	printf("StatusBarOffWhenLocal: %"PRIu8".\n", config->StatusBarOffWhenLocal);
	printf("RamDriveDir: %s.\n", config->RamDriveDir);
	printf("BbsName: %s.\n", config->BBSName);
	printf("TransmitBuffer: %"PRIu16".\n", config->TransmitBuffer);
	printf("UploadCreditsPerK: %"PRIu16".\n", config->ULCreditsPerK);
	printf("DownloadCreditsPerMin: %"PRIu16".\n", config->DLCreditsPerMin);
	printf("NewUserBonusTime: %"PRIu16".\n", config->NewUserBonusTime);
	printf("AutoLogon: %"PRIu8".\n", config->AutoLogon);
	printf("RefundUploadingTime: %"PRIu8".\n", config->RefndULTime);
	printf("OldMailExpireDays: %"PRIu64".\n", config->OldMailExpireDays);
	printf("NewMailExpireDays: %"PRIu64".\n", config->NewMailExpireDays);
	printf("AttachmentEnabled: %"PRIu8".\n", config->AttachmentEnabled);
	printf("ClearScreenBeforeHelp: %"PRIu8".\n", config->CLSBeforeHelp);
	printf("FSEditClearScreen: %"PRIu8".\n", config->FSEditClearScreen);
	printf("LineName0: %s.\n", config->LineName0);
	printf("LineName1: %s.\n", config->LineName1);
	printf("LineName2: %s.\n", config->LineName2);
	printf("LineName3: %s.\n", config->LineName3);
	printf("LineName4: %s.\n", config->LineName4);
	printf("LineName5: %s.\n", config->LineName5);
	printf("LineName6: %s.\n", config->LineName6);
	printf("LineName7: %s.\n", config->LineName7);
	printf("LineName8: %s.\n", config->LineName8);
	printf("LineName9: %s.\n", config->LineName9);
	printf("LineName10: %s.\n", config->LineName10);
	printf("LineName11: %s.\n", config->LineName11);
	printf("LineName12: %s.\n", config->LineName12);
	printf("LineName13: %s.\n", config->LineName13);
	printf("LineName14: %s.\n", config->LineName14);
	printf("LineName15: %s.\n", config->LineName15);
	printf("LineName16: %s.\n", config->LineName16);
	printf("LineName17: %s.\n", config->LineName17);
	printf("LineName18: %s.\n", config->LineName18);
	printf("LineName19: %s.\n", config->LineName19);
	printf("AskLongDescription: %"PRIu8".\n", config->AskLongDesc);
	printf("AlwaysExpertMode: %"PRIu8".\n", config->AlwaysExpertMode);
}
