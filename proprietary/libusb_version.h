#ifndef LIBUSB_VERSION_H
#define LIBUSB_VERSION_H

#include <stdint.h>

struct libusb_version {
	/** Library major version. */
	const uint16_t major;

	/** Library minor version. */
	const uint16_t minor;

	/** Library micro version. */
	const uint16_t micro;

	/** Library nano version. */
	const uint16_t nano;

	/** Library release candidate suffix string, e.g. "-rc4". */
	const char *rc;

	/** For ABI compatibility only. */
	const char *describe;
};

const struct libusb_version *libusb_get_version(void);


#endif
