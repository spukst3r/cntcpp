#include <curses.h>
#include <stdio.h>

#include "usb.h"

#define VENDOR_ID   0x0925
#define PRODUCT_ID  0x7001

int main()
{
	TankChassis chassis(VENDOR_ID, PRODUCT_ID);

	if (!chassis.connected()) {
		fprintf(stderr, "Unable to connect to usb device!");
		return -1;
	}

	int c = 0;
	bool quit = false;

	initscr();
	timeout(50);

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

