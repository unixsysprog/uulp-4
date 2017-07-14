/*  bounce2d 1.0	
 *	bounce a character (default is 'o') around the screen
 *	defined by some parameters
 *
 *	user input: 	s slow down x component, S: slow y component
 *		 	f speed up x component,  F: speed y component
 *			Q quit
 *
 *	blocks on read, but timer tick sends SIGALRM caught by ball_move
 *	build:   cc bounce2d.c set_ticker.c -lcurses -o bounce2d
 */

#include	<curses.h>
#include	<signal.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	"bounce.h"
#include	"bar.h"

struct ppball the_ball ;
struct bar the_bar;
	
void	set_ticker( int  );
int bounce_or_lose(struct ppball *);

/**  the main loop  **/

void set_up();
void wrap_up();

int main()
{
	int	c;

	set_up();

	while ( ( c = getchar()) != 'Q' ){
		if ( c == 'f' )	     the_ball.x_ttm--;
		else if ( c == 's' ) the_ball.x_ttm++;
		else if ( c == 'F' ) the_ball.y_ttm--;
		else if ( c == 'S' ) the_ball.y_ttm++;

		if ( c == UP_C && the_bar.bottom_y != BOT_ROW)
			the_bar.dir = BAR_DOWN;
		if ( c == DOWN_C && the_bar.top_y != TOP_ROW)
			the_bar.dir = BAR_UP;
	}

	wrap_up();
}


void set_up()
/*
 *	init structure and other stuff
 */
{
	void	ball_move(int);

	the_ball.y_pos = Y_INIT;
	the_ball.x_pos = X_INIT;
	the_ball.y_ttg = the_ball.y_ttm = Y_TTM ;
	the_ball.x_ttg = the_ball.x_ttm = X_TTM ;
	the_ball.y_dir = 1  ;
	the_ball.x_dir = 1  ;
	the_ball.symbol = DFL_SYMBOL ;

	the_bar.top_y = BAR_TOP_Y;
	the_bar.bottom_y = BAR_BOTTOM_Y;
	the_bar.dir = BAR_STOP;

	initscr();
	noecho();
	crmode();

	signal( SIGINT , SIG_IGN );
	mvaddch( the_ball.y_pos, the_ball.x_pos, the_ball.symbol  );
	for ( int i = the_bar.top_y; i <= the_bar.bottom_y; ++i )
		mvaddch(i, RIGHT_EDGE, BAR_SYMBOL );
	refresh();
	
	signal( SIGALRM, ball_move );
	set_ticker( 1000 / TICKS_PER_SEC );	/* send millisecs per tick */
}

void wrap_up()
{

	set_ticker( 0 );
	endwin();		/* put back to normal	*/
}


void ball_move(int signum)
{
	int	y_cur, x_cur, moved;

	signal( SIGALRM , SIG_IGN );		/* dont get caught now 	*/
	y_cur = the_ball.y_pos ;		/* old spot		*/
	x_cur = the_ball.x_pos ;
	moved = 0 ;

	if ( the_ball.y_ttm > 0 && the_ball.y_ttg-- == 1 ){
		the_ball.y_pos += the_ball.y_dir ;	/* move	*/
		the_ball.y_ttg = the_ball.y_ttm  ;	/* reset*/
		moved = 1;
	}

	if ( the_ball.x_ttm > 0 && the_ball.x_ttg-- == 1 ){
		the_ball.x_pos += the_ball.x_dir ;	/* move	*/
		the_ball.x_ttg = the_ball.x_ttm  ;	/* reset*/
		moved = 1;
	}

	if ( moved ){
//		mvaddch( y_cur, x_cur, BLANK );
		mvaddch( y_cur, x_cur, BLANK );
		mvaddch( the_ball.y_pos, the_ball.x_pos, the_ball.symbol );
		bounce_or_lose( &the_ball );
		move(LINES-1,COLS-1);
//		refresh();
	}

	if ( the_bar.dir == BAR_UP && the_bar.top_y != TOP_ROW ) {
		mvaddch(the_bar.top_y + the_bar.dir, RIGHT_EDGE, BAR_SYMBOL );
		mvaddch(the_bar.bottom_y, RIGHT_EDGE, BLANK);
		the_bar.top_y += the_bar.dir;
		the_bar.bottom_y += the_bar.dir;
	}

	if ( the_bar.dir == BAR_DOWN && the_bar.bottom_y != BOT_ROW ) {
		mvaddch(the_bar.top_y, RIGHT_EDGE, BLANK);
		mvaddch(the_bar.bottom_y + the_bar.dir, RIGHT_EDGE, BAR_SYMBOL);
		the_bar.top_y += the_bar.dir;
		the_bar.bottom_y += the_bar.dir;
	}

	if ( moved || the_bar.dir != BAR_STOP ) {
		refresh();
		the_bar.dir = BAR_STOP;
	}


	signal( SIGALRM, ball_move);		/* for unreliable systems */

}

int bounce_or_lose(struct ppball *bp)
{
	int	return_val = 0 ;

	if ( bp->y_pos == TOP_ROW ){
		bp->y_dir = 1 ; 
		return_val = 1 ;
	} else if ( bp->y_pos == BOT_ROW ){
		bp->y_dir = -1 ;
	       	return_val = 1;
	}
	if ( bp->x_pos == LEFT_EDGE ){
		bp->x_dir = 1 ;
	       	return_val = 1 ;
	} else if ( bp->x_pos == RIGHT_EDGE ){
			if ( bp->y_pos >= the_bar.top_y &&
						the_ball.y_pos <= the_bar.bottom_y ) {
					mvaddch(the_ball.y_pos, the_ball.x_pos, BAR_SYMBOL);
					bp->x_dir = -1;
	       	return_val = 1;
			} else {
				move (0, 0);
				addstr("You lost!");
				refresh();
				sleep(1);
				wrap_up();
				exit(0);
			}
	}

	return return_val;
}

