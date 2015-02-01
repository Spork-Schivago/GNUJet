#include <stdio.h>
#include <stdlib.h>
#include <string.h>             /* for strcasecmp and strdup*/
#include <getopt.h>		/* for getopt and getopt_long */
#include <inttypes.h>		/* for uint#_t and PRIu#_t */

#include "ini.h"
#include "bbs.h"

#define POSIXLY_CORRECT	1

int main(int argc, char *argv[]) {
	char *mnu_file;
	int x, interrupt = -1;
	uint32_t baudrate = -1;

	syscfg config;

	/* Populate typedef struct syscfg with default values
	 * in case they're missing from config file.  We do this
	 * now so if the user specifies something on the command-line,
	 * it doesn't get over-written with our default ones. */
	default_ini(&config);

	if (argc>=2) {	/* Check to see if they provided switches. */
		for (x = 1; x < argc; x++) {	/* First check all variables to see
						 * if we have any unknown switches.
						 */
			if (strcasecmp("COM1",argv[x])!=0 &&
			    strcasecmp("COM2",argv[x])!=0 &&
			    strcasecmp("COM3",argv[x])!=0 &&
			    strcasecmp("COM4",argv[x])!=0 &&
			    strcasecmp("LOCAL",argv[x])!=0 &&
			    strcasecmp("/o",argv[x])!=0 &&
			    strcasecmp("/d",argv[x])!=0 &&
			    strncasecmp("/i:",argv[x],3)!=0 &&
			    strncasecmp("/b:",argv[x],3)!=0 &&
			    strcasecmp("/q",argv[x])!=0 &&
			    strncasecmp("/r:",argv[x],3)!=0 &&
			    strcasecmp("/c",argv[x])!=0 &&
			    strncasecmp("/e:",argv[x],3)!=0 &&
			    strcasecmp("--dump-ini",argv[x])!=0 &&
			    strcasecmp("--default-config",argv[x])!=0 &&
			    strcasecmp("/?",argv[x])!=0 &&
			    strcasecmp("-h",argv[x])!=0 &&
			    strcasecmp("--help",argv[x])!=0 &&
			    IsValidNumber(argv[x])!=0) {
				printf("Invalid switch: %s\n",argv[x]);
				usage(argv);
				exit(EXIT_FAILURE);
			}
		}
		for (x = 1; x < argc; x++) {	/* No unknown switches, parse known switches. */
			if (strcasecmp("/?",argv[x])==0 ||
					strcasecmp("--help",argv[x])==0 ||
					strcasecmp("-h",argv[x])==0) {	/* They want help. */
				usage(argv);	/* Display usage and exit. */
				exit(EXIT_SUCCESS);
			} else if (strncasecmp("/b:",argv[x],3)==0) {	/* Check to see if they want to use */
				if(strlen(argv[x])==3) {		/* a custom BBS.ini. */
					printf("/b:<file> requires a file name.\n");
					exit(EXIT_FAILURE);
				} else {
					sprintf(INI_File,"%s",&argv[x][3]);
					printf("using '%s' for BBS.ini.\n",INI_File);
				}
			} else if (strcasecmp("--dump-ini",argv[x])==0) {	 /* Dump INI file using a */
				if (ini_parse(INI_File, handler, &config) < 0) { /* modified version of   */
					printf("Can't load '%s'\n", INI_File);	 /* Ben Hoyt's inih lib.  */
					exit(EXIT_FAILURE);
				}
				print_config(&config);
				printf("\nPrinted %s config.\n",INI_File);
				exit(EXIT_SUCCESS);
			} else if (strcasecmp("--default-config",argv[x])==0) {		/* Dump default's, these are in
				print_config(&config);					 * case there's no INI file.
											 */
				printf("\nPrinted default config.\n");
				exit(EXIT_SUCCESS);
			} else if (strcasecmp("COM1",argv[x])==0) {	/* Modem is on COM1. */
				printf("Found COM1");
				if(argc-1 > x) {	/* Check to see if they specified a baud rate for COM1. */
					if(IsValidNumber(argv[x+1])==0) {	/* They did, set it. */
						baudrate = atoi(argv[x+1]);
						x = x + 1;
						printf(", baudrate: %"PRIu32".",baudrate);
					}
				}
				printf("\n");
			} else if (strcasecmp("COM2",argv[x])==0) {	/* Modem is on COM2. */
				printf("Found COM2");
				if(argc-1 > x) {	/* Check to see if they specified a baud rate for COM2. */
					if(IsValidNumber(argv[x+1])==0) {	/* They did, set it. */
						baudrate = atoi(argv[x+1]);
						x = x + 1;
						printf(", baudrate: %"PRIu32".",baudrate);
					}
				}
				printf("\n");
			} else if (strcasecmp("COM3",argv[x])==0) {	/* Modem is on COM3. */
				printf("Found COM3");
				if(argc-1 > x) {	/* Check to see if they specified a baud rate for COM3. */
					if(IsValidNumber(argv[x+1])==0) {	/* They did, set it. */
						baudrate = atoi(argv[x+1]);
						x = x + 1;
						printf(", baudrate: %"PRIu32".",baudrate);
					}
				}
				printf("\n");
			} else if (strcasecmp("COM4",argv[x])==0) {	/* Modem is on COM4. */
				printf("Found COM4");
				if(argc-1 > x) {	/* Check to see if they specified a baud rate for COM4. */
					if(IsValidNumber(argv[x+1])==0) {	/* They did, set it. */
						baudrate = atoi(argv[x+1]);
						x = x + 1;
						printf(", baudrate: %"PRIu32".",baudrate);
					}
				}
				printf("\n");
			} else if (strcasecmp("LOCAL",argv[x])==0) {	/* They want to log on locally. */
				printf("Found LOCAL");
				if(argc-1 > x) {	/* We don't allow a baud rate for local logons. */
					if(IsValidNumber(argv[x+1])==0) {
						baudrate = atoi(argv[x+1]);
						x = x + 1;
						printf(", baudrate: %"PRIu32".\n",baudrate);
						printf("Please do not set baudrate when using the LOCAL option.\n");
						exit(EXIT_FAILURE);
					}
				}
				printf("\n");
			} else if (strcasecmp("/o",argv[x])==0) {	/* Check for one user mode. */
				printf("Take one user, don't restart after logoff.\n");
			} else if (strcasecmp("/d",argv[x])==0) {	/* Check for doorway mode. */
				printf("Doorway mode, call is already connected (use with /o).\n");
			} else if (strncasecmp("/i:",argv[x],3)==0) {	/* Check if IRQ is provided. */
				if(strlen(argv[x])==3) {	/* Make sure they gave us a number with the switch. */
					printf("/i:# requires a number in place of #.\n");
					exit(EXIT_FAILURE);
				} else {
					if(IsValidNumber(&argv[x][3])==0) {
						interrupt = atoi(&argv[x][3]);
						printf("Set IRQ (interrupt) number to %d.\n",interrupt);
					} else {
						printf("/i:# requires a number in place of #.\n");
						exit(EXIT_FAILURE);
					}
				}
			} else if (strcasecmp("/q",argv[x])==0) {	/* Check for quiet mode. */
				printf("Found quit mode.\n");
			} else if (strncasecmp("/r:",argv[x],3)==0) {	/* Check for answer ring-type. */
				if (strlen(argv[x])!=4) {
					printf("switch /r:# requires either a 1 or 2 where # is.\n");
					exit(EXIT_FAILURE);
				}
				if (argv[x][4]!=1 || argv[x][4]!=2) {	/* Make sure they specify one or two rings. */
					printf("switch /r:# requires either a 1 or a 2 where # is.\n");
					exit(EXIT_FAILURE);
				}
				printf("Setting ring mode to %d.\n",argv[x][4]);
			} else if (strcasecmp("/c",argv[x])==0) {	/* Turn capture log on. */
				printf("Beginning with capture log turned on.\n");
			} else if (strncasecmp("/e:",argv[x],3)==0) {	/* Check to see if they want to run an MNU file. */
				if (strlen(argv[x])==3 || argc<= x + 1) {	/* Make sure they gave us a file and user acct num. */
					printf("/e: switch requires <file.mnu> and <account#>.\n");
					exit(EXIT_FAILURE);
				} else if(argc-1 > x) {		/* they provided something. */
					if(IsValidNumber(argv[x+1])!=0) {	/* Make sure it's a number. */
						printf("please specify an account number to use with the /e: switch\n");
					} else {
						mnu_file = &argv[x][3];
						printf("MNU file: %s, account #: %s.\n",mnu_file,argv[x+1]);
					}
					x = x + 1;
				}
			} else if (IsValidNumber(argv[x])==0) {		/* They're setting a default baud rate. */
				baudrate = atoi(argv[x]);
				printf("Found baudrate for default COM: %"PRIu32".\n",baudrate);
			} else {	/* If we did everything write, users should never see this. */
				printf("You should never be seeing this.\nUnknown switch: %s.\n",argv[x]);
				usage(argv);
				exit(EXIT_FAILURE);
			}

		}
	}

	return 0;
}

/* Function to print default usage information */
void usage(char **argv) {
	char tmp_prg_name[strlen(argv[0])];	/* temporary character array to hold full path name. */
	char program_name[strlen(argv[0])];	/* character array to hold just program name. */
	int x;


	sprintf(tmp_prg_name,"%s",argv[0]);	/* Copy full program name to temp char array. */

	/* This for loop strips the leading directory(ies) the program name
	 * and leaves us with just the program name.  For example, instead of ./GNUJet
	 * we have GNUJet.
	 */
	for(x=strlen(argv[0]); x > 0; x--) {
		if (tmp_prg_name[x]=='/') {				/* found our last slash.             */
			sprintf(program_name,"%s",&tmp_prg_name[x+1]);	/* copy the program name to buffer   */
			x = -1;						/* we found the last slash, no sense */
									/* continuing to run our for loop.   */

		}
	}

	printf("Usage: %s [portname] [baudrate] [switches]\n"
	       "Switches:\n"
	       "  /o        = Take one user, don't restart after logoff.\n"
	       "  /d        = Doorway mode, call is already connected (use with /o).\n"
	       "  /i:#      = Set IRQ (interrupt) number.\n"
	       "  /b:<file> = Set a different file to use as BBS.INI.\n"
	       "  /q        = Quiet, save CPU time by not displaying locally (alt+q toggles).\n"
	       "  /r:#      = Answers only one type of custom ringing: 1=single, 2=double-ring.\n"
	       "  /c        = Begin with capture log turned on.\n"
	       "  /e:<file.mnu> <account#> = Logon with account# and execute MNU file.  Execute\n"
	       "      an MNU file with the command \"qwkexportqwkfile filename.qwk\" to export\n"
	       "      a QWK file or \"qwkimportrepfile filename.rep\" to import a REP file.\n"
	       "      You can import a QWK file with \"qwkimportqwkfile filename.qwk\".\n"
	       "Examples:\n"
	       "  %s                   (the normal way of starting the BBS to take calls)\n"
	       "  %s COM2 2400         (use 2400 baud modem on COM2)\n"
	       "  %s COM3 2400 /i:5    (2400 baud, COM3's I/O address, and IRQ5)\n"
	       "  %s COM1 2400 /d /o   (run as a door inside another BBS or mail front end)\n"
	       "  %s LOCAL             (start local session)\n"
	       "  %s /e:run.mnu 25     (execute the MNU com mands in the file MNU\\RUN.MNU)\n"
	       "  %s --default-config  (display default ini config values)\n"
	       "  %s --dump-ini        (display values from BBS.ini file)\n"
	       , program_name, program_name, program_name, program_name
	       , program_name, program_name, program_name, program_name, program_name);
}

/* Function to check if a string is all numbers */
/* Return -1 if it isn't, 0 if it is */
int IsValidNumber(char *string) {
	int i;

	for(i = 0; i < strlen(string); i++) {
		/* ASCII value of 0 = 48, 9 = 57 */
		/* if we find something that isn't a number */
		/* return false */
		if(string[i] < 48 || string[i] > 57) {
			return -1;
		}
	}
	return 0;
}
