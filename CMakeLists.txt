cmake_minimum_required (VERSION 2.6)
set (PROJ_TITLE cntcpp)
project (${PROJ_TITLE})

set (CURSES_NEED_NCURSES TRUE)
find_package (PkgConfig)
find_package (Curses)

pkg_check_modules (USB libusb-1.0)

if (USB_FOUND AND CURSES_FOUND)
	set (SOURCES usb.cpp main.cpp)
	set (HEADERS usb.h)
	add_executable (${PROJ_TITLE} ${SOURCES})

	target_link_libraries (${PROJ_TITLE} ${USB_LIBRARIES} ${CURSES_LIBRARIES})

else (USB_FOUND AND CURSES_FOUND)

	message(FATAL_ERROR "libusb or ncurses not found!")

endif (USB_FOUND AND CURSES_FOUND)

