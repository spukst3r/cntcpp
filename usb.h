#ifndef _USB_H_
#define _USB_H_

#include <libusb-1.0/libusb.h>

const static int PACKET_INT_LEN 		= 3;
const static int INTERFACE 				= 0;
const static int ENDPOINT_INT_IN	 	= 0x81;
const static int ENDPOINT_INT_OUT 		= 0x01;
const static int TIMEOUT 				= 5000;


class TankChassis
{
private:
	libusb_device_handle *devh;
	int vendor_id,
		product_id;

	int find_lvr_hidusb(int vendor, int product);
	int interrupt_transfer(unsigned char cmd_byte);
	int prepare_usb_device();
	void usb_dispose();

	TankChassis(const TankChassis &ob);
	bool ok;

public:
	TankChassis(int vendor, int product);
	~TankChassis();

	bool connected();
	int sendByte(unsigned char b);
	void move_forward();
	void move_backward();
	void rotate_cw();
	void rotate_ccw();
	void rotate_tower_cw();
	void rotate_tower_ccw();
	void stop();
	void stop_tower();
};

#endif

