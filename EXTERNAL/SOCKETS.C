/*
  Copyright 1994-2003 Mark Emmer, Catspaw, Inc.  All Rights Reserved.
  Use Of this source code is governed by a BSD-style
  license that can be found in the LICENSE file.
*/

/*
 * SOCKETS.C
 *
 * WARNING: THIS IS A WORK IN PROGRESS AND IS BOTH INCOMPLETE AND
 * UNDEBUGGED.  MANY OF THE INTERFACES ARE SUBJECT TO CHANGE.
 *
 * 
 * To build under Borland C++ 4.x:
 *   bcc32 -WXD -a1 -d -w-par -M sockets.c extrnlib.obj
 *
 */
#include <windows.h>
#include <winnt.h>
#include <stdio.h>
#include <winsock.h>
#include "system.h"
#include "extern32.h"

#define WS_VERSION_REQD 0x0101
static init = 0;
static void socketshutdown(void);

/* CLOSESOCKET(INTEGER,INTEGER,INTEGER,INTEGER) -- Establish connection to a port.
 */
entry(EXT_CLOSESOCKET_)(union block *presult, misc *pinfo, mword s)
{
    if (closesocket(s))
      return FAIL;
    else
      return retstrf("", presult);
}       
     
/* CONNECT(INTEGER,INTEGER,INTEGER,INTEGER) -- Establish connection to a port.
 */
entry(EXT_CONNECT_)(union block *presult, misc *pinfo,
      mword addr, mword port, mword family, mword s)
{
    struct sockaddr_in sktin;

    sktin.sin_family = family;
    sktin.sin_port = port;
    sktin.sin_addr.s_addr = addr;
    memset(sktin.sin_zero, 0, sizeof(sktin.sin_zero));

    if (connect(s, (struct sockaddr *)&sktin, sizeof(sktin)))
      return FAIL;
    else
      return retstrf("", presult);
}       
     
/* GETHOSTBYNAME(TEXT) -- Convert hostname to a 32-bit integer IP address.
 * 							  Address is in network byte order.
 */
entry(EXT_GETHOSTBYNAME_)(union block *presult, misc *pinfo, far char *parg, mword larg)
{
   struct hostent *host;

   if (!larg || larg >= buflen)
      return FAIL;
   
   strncpy(presult->scb.scstr, parg, larg);
   presult->scb.scstr[larg] = '\0';         /* create C string */
   host = gethostbyname(presult->scb.scstr);
   if (!host)
     return FAIL;
	else
	  return retint(*(unsigned long *)host->h_addr, presult);
}       

/* GETHOSTNAME() -- Get name of local host.
 */
entry(EXT_GETHOSTNAME_)(union block *presult, misc *pinfo, far char *parg, mword larg)
{
   if (gethostname(presult->scb.scstr, buflen))
     return FAIL;
   presult->scb.sclen = strlen(presult->scb.scstr);
   return BL_SC;
}       

/* HTONL(INTEGER) -- Convert integer from host to network byte order.
 */
entry(EXT_HTONL_)(union block *presult, misc *pinfo, mword i)
{
   return retint(htonl(i), presult);
}       

/* HTONS(INTEGER) -- Convert short integer from host to network byte order.
 */
entry(EXT_HTONS_)(union block *presult, misc *pinfo, mword i)
{
   return retint(htonl((unsigned short)i), presult);
}       

/* INET_ADDR(STRING) -- Convert internet address in a.b.c.d string form to
 *                               integer.  Result in network byte order.
 */
entry(EXT_INET_ADDR_)(union block *presult, misc *pinfo, far char *parg, mword larg)
{
   unsigned long ip;
   if (!larg || larg >= buflen)
      return FAIL;
   
   strncpy(presult->scb.scstr, parg, larg);
   presult->scb.scstr[larg] = '\0';         /* create C string */
   ip = inet_addr(presult->scb.scstr);
   if (ip == INADDR_NONE)
     return FAIL;
   else
     return retint(ip, presult);
}       

/* INET_NTOA(INTEGER) -- Convert integer internet address to a.b.c.d string form.
 *								 Address is in network byte order.
 */
entry(EXT_INET_NTOA_)(union block *presult, misc *pinfo, mword ip)
{
	char *addr;
	struct in_addr in;

	in.S_un.S_addr = ip;
	addr = inet_ntoa(in);
	if (!addr)
	  return FAIL;
   else
     return retstrt(addr, presult);
}

/* NTOHL(INTEGER) -- Convert integer from network to host byte order.
 */
entry(EXT_NTOHL_)(union block *presult, misc *pinfo, mword i)
{
   return retint(ntohl(i), presult);
}

/* NTOHS(INTEGER) -- Convert short integer from network to host byte order.
 */
entry(EXT_NTOHS_)(union block *presult, misc *pinfo, mword i)
{
   return retint(ntohl((unsigned short)i), presult);
}

/* RECV(INTEGER,STRING,INTEGER) -- Receive data from a socket.
 */
entry(EXT_RECV_)(union block *presult, misc *pinfo,
    mword flags, far char *parg, mword larg, mword s)
{
    int result;

    result = recv(s, parg, larg, flags);
    if (result == SOCKET_ERROR)
      return FAIL;
   else
     return retint(result, presult);
}       
     
/* SEND(INTEGER,STRING,INTEGER) -- Send data to a socket.
 */
entry(EXT_SEND_)(union block *presult, misc *pinfo,
    mword flags, far char *parg, mword larg, mword s)
{
    int result;

    result = send(s, parg, larg, flags);
    if (result == SOCKET_ERROR)
      return FAIL;
   else
     return retint(result, presult);
}       
     
/* SOCKET(INTEGER,INTEGER,INTEGER) -- Create a socket.
 */
entry(EXT_SOCKET_)(union block *presult, misc *pinfo, mword protocol, mword type, mword af)
{
    SOCKET s;

    s = socket(af, type, protocol);
    if (s == INVALID_SOCKET)
      return FAIL;
   else
     return retint(s, presult);
}       
     
/* SOCKETCLEANUP -- Close down the socket system
 */
entry(EXT_SOCKETCLEANUP_)(union block *presult, misc *pinfo, far char *parg, mword larg)
{
   if (!init || WSACleanup())
	 	return FAIL;
	init--;
	return retstrf("", presult);
}

/* SOCKETSTARTUP -- Start the socket system
 */
entry(EXT_SOCKETSTARTUP_)(union block *presult, misc *pinfo, far char *parg, mword larg)
{
	WSADATA wsaData;
	if (init || WSAStartup(WS_VERSION_REQD, &wsaData))
	  return FAIL;
	init++;
	pinfo->pxnblk->xnu.ef.xncbp = socketshutdown;
	return retstrf("", presult);
}

/* socketshutdown -- Called when SPITBOL is shutting down, or if function
 * SOCKETSTARTUP is unloaded.
 */
static void socketshutdown(void)
{
   if (init && !WSACleanup())
	  init--;
}
	
/* ----------------------------  end of sockets.c ----------------------*/
