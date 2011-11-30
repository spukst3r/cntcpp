#include <errno.h>
#include <signal.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

#include "usb.h"

TankChassis::TankChassis(int vendor, int product):
	vendor_id(vendor), product_id(product)
{
	ok = false;

	if (libusb_init(0) < 0)
		throw std::runtime_error("init");

	libusb_set_debug(0, 0);

	if (find_lvr_hidusb(vendor_id, product_id) < 0)
		throw std::runtime_error("Unable to openusb device!");

	libusb_detach_kernel_driver(devh, 0);

	if (libusb_set_configuration(devh, 1) < 0)
		throw std::runtime_error("libusb_set_configuration");

	if (libusb_claim_interface(devh, 0) < 0)
		throw std::runtime_error("libusb_claim_interface");

	ok = true;
}

bool TankChassis::connected()
{
	return ok;
}

int TankChassis::sendByte(unsigned char b)
{
	return interrupt_transfer(b);
}

int TankChassis::find_lvr_hidusb(int vendor, int product)
{
	devh = libusb_open_device_with_vid_pid(0, vendor, product);
	return devh ? 0 : -EIO;
}

int TankChassis::interrupt_transfer(unsigned char cmd_byte)
{
	int r;
	int transferred;
	unsigned char question[PACKET_INT_LEN] = { cmd_byte };

	r = libusb_interrupt_transfer(devh, ENDPOINT_INT_OUT, question, PACKET_INT_LEN, &transferred, TIMEOUT);

	if (r < 0) {
		fprintf(stderr, "libusb_interrupt_transfer: error\n");
		return r;
	}

	if (transferred < PACKET_INT_LEN) {
		fprintf(stderr, "libusb_interrupt_transfer: transfer incomplete\n");
		return -1;
	}

	return 0;
}

int TankChassis::prepare_usb_device()
{
	if (libusb_init(0) < 0) {
		fprintf(stderr, "libusb_init\n");
		return -1;
	}

	if (find_lvr_hidusb(vendor_id, product_id) < 0) {
		fprintf(stderr, "find_lvr_hidusb\n");
		return -1;
	}

	libusb_detach_kernel_driver(devh, 0);

	if (libusb_set_configuration(devh, 1) < 0) {
		fprintf(stderr, "libusb_set_configuration\n");
		return -1;
	}

	if (libusb_claim_interface(devh, 0) < 0) {
		fprintf(stderr, "libusb_claim_interface\n");
		return -1;
	}

	return 0;
}

TankChassis::~TankChassis()
{
	libusb_release_interface(devh, 0);
    libusb_close(devh);
    libusb_exit(0);
}

void TankChassis::move_forward()
{
	interrupt_transfer(0x81);
}

void TankChassis::move_backward()
{
	interrupt_transfer(0x80);
}

void TankChassis::rotate_cw()
{
	interrupt_transfer(0x83);
}

void TankChassis::rotate_ccw()
{
	interrupt_transfer(0x82);
}

void TankChassis::rotate_tower_cw()
{
	interrupt_transfer(0x84);
}

void TankChassis::rotate_tower_ccw()
{
	interrupt_transfer(0x85);
}

void TankChassis::stop()
{
	interrupt_transfer(0x90);
}

void TankChassis::stop_tower()
{
	interrupt_transfer(0x91);
}

