/*! \file netstack.h \brief Network Stack. */
//*****************************************************************************
//
// File Name	: 'netstack.h'
// Title		: Network Stack
// Author		: Pascal Stang
// Created		: 6/28/2005
// Revised		: 7/3/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
///	\ingroup network
///	\defgroup netstack Network Stack (netstack.c)
///	\code #include "net/netstack.h" \endcode
///	\par Description
///		This library co-ordinates the various pieces of a typical IP network
///		stack into one unit.  Included are handling for ARP, ICMP, and IP
///		packets.  UDP and TCP packets are processed and passed to the user.
///
/// \note This is an example of how to use the various network libraries, and
///		is meant to be useful out-of-the-box for most users.  However, some
///		users may find it restrictive and write their own handlers instead.
///
///	\note This is NOT a full-blown TCP/IP stack.  It merely handles lower
///		level stack functions so that UDP and TCP packets can be sent
///		and received easily.  End-to-end TCP functionality may be added
///		in a future version.  Until then, I can recommend using other embedded
///		TCP/IP stacks like Adam Dunkel's uIP.
//
//	This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//*****************************************************************************
//@{

#ifndef NETSTACK_H
#define NETSTACK_H

#include "net.h"
#include "arp.h"
#include "icmp.h"
#include "ip.h"
#include "nic.h"

//#define NETSTACK_DEBUG

/// NET_BUFFERSIZE is the common receive/process/transmit buffer
/// Network packets larger than NET_BUFFERSIZE will not be accepted.
#define NET_BUFFERSIZE		500

/// netstackService should be called in the main loop of the user program.
/// The function will process one received network packet per call.
void netstackService(void);

/// netstackIPProcess handles distribution of IP received packets.
///
void netstackIPProcess(unsigned int len, ip_hdr* packet);

/// Replace this weakly-defined function with a user function that accepts UDP/IP packets.
///
void netstackUDPIPProcess(unsigned int len, udpip_hdr* packet) __attribute__ ((weak));

/// Replace this weakly-defined function with a user function that accepts TCP/IP packets.
///
void netstackTCPIPProcess(unsigned int len, tcpip_hdr* packet) __attribute__ ((weak));

#endif
//@}
