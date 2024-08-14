#pragma once

#define IOCTL_ADD_VIRTUAL_DISPLAY CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_REMOVE_VIRTUAL_DISPLAY CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SET_RENDER_ADAPTER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_GET_WATCHDOG CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_DRIVER_PING CTL_CODE(FILE_DEVICE_UNKNOWN, 0x888, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_GET_PROTOCOL_VERSION CTL_CODE(FILE_DEVICE_UNKNOWN, 0x8FF, METHOD_BUFFERED, FILE_ANY_ACCESS)

static const char* SUVDA_HARDWARE_ID = "root\\sudomaker\\sudovda";

// DO NOT CHANGE
// {4d36e968-e325-11ce-bfc1-08002be10318}
static const GUID SUVDA_CLASS_GUID = { 0x4d36e968, 0xe325, 0x11ce, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } };
// {e5bcc234-1e0c-418a-a0d4-ef8b7501414d}
static const GUID SUVDA_INTERFACE_GUID = { 0xe5bcc234, 0x1e0c, 0x418, { 0xa0, 0xd4, 0xef, 0x8b, 0x75, 0x01, 0x41, 0x4d } };

typedef struct _SUVDA_PROTOCAL_VERSION {
	uint8_t Major;
	uint8_t Minor;
	uint8_t Incremental;
	bool TestBuild;
} SUVDA_PROTOCAL_VERSION, * PSUVDA_PROTOCAL_VERSION;

// Please update the version after ioctl changed
static const SUVDA_PROTOCAL_VERSION VDAProtocolVersion = { 0, 2, 0, true };

typedef struct _VIRTUAL_DISPLAY_PARAMS {
	UINT Width;
	UINT Height;
	UINT RefreshRate;
	GUID MonitorGuid;
	CHAR DeviceName[14];
	CHAR SerialNumber[14];
} VIRTUAL_DISPLAY_PARAMS, * PVIRTUAL_DISPLAY_PARAMS;

typedef struct _VIRTUAL_DISPLAY_REMOVE_PARAMS {
	GUID MonitorGuid;
} VIRTUAL_DISPLAY_REMOVE_PARAMS, * PVIRTUAL_DISPLAY_REMOVE_PARAMS;

typedef struct _VIRTUAL_DISPLAY_OUTPUT {
	LUID AdapterLuid;
	UINT TargetId;
} VIRTUAL_DISPLAY_OUTPUT, * PVIRTUAL_DISPLAY_OUTPUT;

typedef struct _VIRTUAL_DISPLAY_SET_RENDER_ADAPTER_PARAMS {
	LUID AdapterLuid;
} VIRTUAL_DISPLAY_SET_RENDER_ADAPTER_PARAMS, * PVIRTUAL_DISPLAY_SET_RENDER_ADAPTER_PARAMS;

typedef struct _VIRTUAL_DISPLAY_GET_WATCHDOG_OUT {
	uint32_t Timeout;
	uint32_t Countdown;
} VIRTUAL_DISPLAY_GET_WATCHDOG_OUT, * PVIRTUAL_DISPLAY_GET_WATCHDOG_OUT;

typedef struct _VIRTUAL_DISPLAY_GET_PROTOCOL_VERSION_OUT {
	SUVDA_PROTOCAL_VERSION Version;
} VIRTUAL_DISPLAY_GET_PROTOCOL_VERSION_OUT, * PVIRTUAL_DISPLAY_GET_PROTOCOL_VERSION_OUT;

bool isProtocolCompatible(const SUVDA_PROTOCAL_VERSION& otherVersion) {
	// Changes to existing ioctl must be marked as major
	if (VDAProtocolVersion.Major != otherVersion.Major) {
		return false;
	}

	// We shouldn't break compatibility with minor/incremental changes
	// e.g. add new ioctl
	if (VDAProtocolVersion.Minor < otherVersion.Minor) {
		return false;
	}

	if (
		VDAProtocolVersion.Minor == otherVersion.Minor &&
		VDAProtocolVersion.Incremental < otherVersion.Incremental
	) {
		return false;
	}

	return true;
};