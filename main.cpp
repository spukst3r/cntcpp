#include <curses.h>
#include <cstdio>
#include <cstdlib>

#include "usb.h"

#define VENDOR_ID   0x0925
#define PRODUCT_ID  0x7001

int main(int argc, char *argv[])
{
	TankChassis chassis(VENDOR_ID, PRODUCT_ID);
	int to = 50;

	if (argv[1])
		to = atoi(argv[1]);

	if (!chassis.connected()) {
		fprintf(stderr, "Unable to connect to usb device!");
		return -1;
	}

	int c = 0;
	bool quit = false;

	initscr();
	timeout(to);

	printw("Tank control prog. v0.1\n\r\nPress WASD to move\n\r");

	noecho();

	while (true) {
		c = getch();

		switch (c) {
			case 'q':
				quit = true;
				break;
			case 's':
				chassis.move_backward();
				break;
			case 'w':
				chassis.move_forward();
				break;
			case 'a':
				chassis.rotate_cw();
				break;
			case 'd':
				chassis.rotate_ccw();
				break;

			default:
				chassis.stop();
		}

		if (quit)
			break;
	}

	endwin();

	return 0;
}

