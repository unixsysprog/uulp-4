/* who3.c - who with buffered reads
 *	  - surpresses empty records
 *	  - formats time nicely
 *	  - buffers input (using utmplib)
 */
#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<utmpx.h>
#include	<time.h>

#define	SHOWHOST

void show_info(struct utmpx *);
void showtime(time_t);

int main()
{
	struct utmpx	*utbufp;

	if ( setutxdb ( UTXDB_ACTIVE, NULL ) == -1 ){
		perror("UTXDB_ACTIVE");
		exit(1);
	}

	while ( ( utbufp = getutxent()) != NULL ) {
		show_info ( utbufp );
	}
	endutxent();
	return 0;
}
/*
 *	show info()
 *			displays the contents of the utmp struct
 *			in human readable form
 *			* displays nothing if record has no user name
 */
void show_info( struct utmpx *utbufp )
{
	if ( utbufp->ut_type != USER_PROCESS )
		return;

//	printf("%-12hd", utbufp->ut_type);
//	printf(" ");					/* a space	*/
	printf("%-16s", utbufp->ut_user);		/* the logname	*/
	printf(" ");					/* a space	*/
	printf("%-12s", utbufp->ut_line);		/* the tty	*/
	printf(" ");					/* a space	*/
	showtime( utbufp->ut_tv.tv_sec );			/* display time	*/
#ifdef SHOWHOST
	if ( utbufp->ut_host[0] != '\0' )
		printf(" (%s)", utbufp->ut_host);	/* the host	*/
#endif
	printf("\n");					/* newline	*/
}

void showtime( time_t timeval )
/*
 *	displays time in a format fit for human consumption
 *	uses ctime to build a string then picks parts out of it
 *      Note: %12.12s prints a string 12 chars wide and LIMITS
 *      it to 12chars.
 */
{
	char	*ctime();		/* convert long to ascii	*/
	char	*cp;			/* to hold address of time	*/

	cp = ctime( &timeval );		/* convert time to string	*/
					/* string looks like		*/
					/* Mon Feb  4 00:46:40 EST 1991 */
					/* 0123456789012345.		*/
	printf("%12.12s", cp+4 );	/* pick 12 chars from pos 4	*/
}