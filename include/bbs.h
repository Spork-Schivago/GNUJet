#ifndef __BBS_H__
#define __BBS_H__

char INI_File[MAXPATHLEN] = "./BBS50.ini";	/* Holds the name of our BBS.ini file */
void usage(char **argv);			/* Print detailed usage information */
int IsValidNumber(char *string);		/* Checks if a string contains only numbers */

#endif /* __BBS_H__ */
