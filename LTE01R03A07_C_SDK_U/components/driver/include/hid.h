#ifndef _HID_H
#define _HID_H

#include "ql_api_common.h"
#include <stdint.h>
#include <stddef.h>

#ifndef CONFIG_QUEC_PROJECT_FEATURE
#include <usb/usb_device.h>
#include <usb/usb_composite_device.h>
#include <stdint.h>
#include <stddef.h>
#include "osi_api.h"
#include "osi_compiler.h"

#include "usb_utils.h"
#endif

/*
 * HID class requests
 */

#define HID_REQ_GET_REPORT 0x01
#define HID_REQ_GET_IDLE 0x02
#define HID_REQ_GET_PROTOCOL 0x03
#define HID_REQ_SET_REPORT 0x09
#define HID_REQ_SET_IDLE 0x0A
#define HID_REQ_SET_PROTOCOL 0x0B

/*
 * HID class descriptor types
 */

#define HID_DT_HID (UT_CLASS | 0x01)
#define HID_DT_REPORT (UT_CLASS | 0x02)
#define HID_DT_PHYSICAL (UT_CLASS | 0x03)

#define USB_HID_GENERIC_DESKTOP 0x01
#define USB_HID_BUTTONS 0x09
#define USB_HID_X 0x30
#define USB_HID_Y 0x31

#define USB_HID_POINTER 0x01
#define USB_HID_MOUSE 0x02
#define USB_HID_KEYBOARD 0x06

#define USB_HID_PHYSICAL 0x00
#define USB_HID_APPLICATION 0x01

#define USB_HID_USAGE_POINTER 0x0109
#define USB_HID_USAGE_BUTTONS 0x0509
#define USB_HID_USAGE_LEDS 0x0508
#define USB_HID_USAGE_KEYCODES 0x0507

#define USB_HID_INPUT_DATA 0x0000
#define USB_HID_INPUT_CONSTANT 0x0001
#define USB_HID_INPUT_ARRAY 0x0000
#define USB_HID_INPUT_VARIABLE 0x0002
#define USB_HID_INPUT_ABS 0x0000
#define USB_HID_INPUT_RELATIVE 0x0004
#define USB_HID_INPUT_NOWRAP 0x0000
#define USB_HID_INPUT_WRAP 0x0008
#define USB_HID_INPUT_LINEAR 0x0000
#define USB_HID_INPUT_NONLINEAR 0x0010
#define USB_HID_INPUT_PREFER 0x0000
#define USB_HID_INPUT_NONPREFER 0x0020
#define USB_HID_INPUT_NONULL 0x0000
#define USB_HID_INPUT_NULL 0x0040
#define USB_HID_INPUT_BITF 0x0100
#define USB_HID_INPUT_BYTES 0x0000

#define USB_HID_OUTPUT_DATA 0x0000
#define USB_HID_OUTPUT_CONSTANT 0x0001
#define USB_HID_OUTPUT_ARRAY 0x0000
#define USB_HID_OUTPUT_VARIABLE 0x0002
#define USB_HID_OUTPUT_ABS 0x0000
#define USB_HID_OUTPUT_RELATIVE 0x0004
#define USB_HID_OUTPUT_NOWRAP 0x0000
#define USB_HID_OUTPUT_WRAP 0x0008
#define USB_HID_OUTPUT_LINEAR 0x0000
#define USB_HID_OUTPUT_NONLINEAR \
    0x0010
#define USB_HID_OUTPUT_PREFER 0x0000
#define USB_HID_OUTPUT_NONPREFER \
    0x0020
#define USB_HID_OUTPUT_NONULL 0x0000
#define USB_HID_OUTPUT_NULL 0x0040
#define USB_HID_OUTPUT_BITF 0x0100
#define USB_HID_OUTPUT_BYTES 0x0000

#define USB_HID_FEATURE_VARIABLE \
    0x0002

#define USB_DESC_BYTE(b) ((unsigned char)(b))
#define UsagePage(ucValue) 0x05, ((ucValue)&0xff)
#define Usage(ucValue) 0x09, ((ucValue)&0xff)
#define Collection(ucValue) 0xa1, ((ucValue)&0xff)
#define LogicalMinimum(cValue) 0x15, ((cValue)&0xff)
#define LogicalMaximum(cValue) 0x25, ((cValue)&0xff)

#define ReportSize(ucValue) 0x75, ((ucValue)&0xff)
#define ReportCount(ucValue) 0x95, ((ucValue)&0xff)
#define Input(ucValue) 0x81, ((ucValue)&0xff)
#define Output(ucValue) 0x91, ((ucValue)&0xff)
#define EndCollection 0xc0

#define Feature(ucValue) 0xB1, ((ucValue)&0xff)

#define USB_DESC_WORD(w) \
    (uint8_t)((w)&255),  \
        (uint8_t)(((w) >> 8) & 255)

struct hid_class_descriptor
{
    uint8_t bDescriptorType;
    uint16_t wDescriptorLength;
} __attribute__((packed));

struct hid_descriptor
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bcdHID;
    uint8_t bCountryCode;
    uint8_t bNumDescriptors;

    struct hid_class_descriptor desc[1];
} __attribute__((packed));

struct hidg_func_descriptor
{
    uint8_t subclass;
    uint8_t protocol;
    uint16_t report_length;
    uint16_t report_desc_length;
    uint8_t report_desc[];
};

#endif /* _HID_H */

