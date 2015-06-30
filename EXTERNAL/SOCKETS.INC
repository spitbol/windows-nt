*  Copyright 1994-2003, Mark Emmer, Catspaw, Inc.  All rights reserved.
*  Use of this source code is governed by a BSD-style
*  license that can be found in the LICENSE file.

* SOCKETS.INC
*
* WARNING: THIS IS A WORK IN PROGRESS AND IS BOTH INCOMPLETE AND
* UNDEBUGGED.  MANY OF THE INTERFACES ARE SUBJECT TO CHANGE.
*
* Load the Socket APIs
*
        load("EXT_CLOSESOCKET_(INTEGER)INTEGER","nt95only.slf")
        load("EXT_CONNECT_(INTEGER,INTEGER,INTEGER,INTEGER)INTEGER","nt95only.slf")
        load("EXT_GETHOSTBYNAME_(STRING)INTEGER","nt95only.slf")
        load("EXT_GETHOSTNAME_()STRING","nt95only.slf")
        load("EXT_HTONL_(INTEGER)INTEGER","nt95only.slf")
        load("EXT_HTONS_(INTEGER)INTEGER","nt95only.slf")
        load("EXT_INET_ADDR_(STRING)INTEGER","nt95only.slf")
        load("EXT_INET_NTOA_(INTEGER)STRING","nt95only.slf")
        load("EXT_NTOHL_(INTEGER)INTEGER","nt95only.slf")
        load("EXT_NTOHS_(INTEGER)INTEGER","nt95only.slf")
        load("EXT_RECV_(INTEGER,STRING,INTEGER)INTEGER","nt95only.slf")
        load("EXT_SEND_(INTEGER,STRING,INTEGER)INTEGER","nt95only.slf")
        load("EXT_SOCKET_(INTEGER,INTEGER,INTEGER)INTEGER","nt95only.slf")
        load("EXT_SOCKETCLEANUP_()","nt95only.slf")
        load("EXT_SOCKETSTARTUP_()","nt95only.slf")

* Socket Data Types
*
        data("sockaddr_in(sin_family, sin_port, sin_addr)")

* Define Primary Socket APIs
*
        opsyn("closesocket", "EXT_CLOSESOCKET_")
        define("connect(socket, addr, namelen)")          :(connect_end)
connect connect = EXT_CONNECT_(socket, sin_family(addr), sin_port(addr),
+       sin_addr(addr))                                 :s(RETURN)f(FRETURN)
connect_end
        opsyn("htonl", "EXT_HTONL_")
        opsyn("htons", "EXT_HTONS_")
        opsyn("inet_addr", "EXT_INET_ADDR_")
        opsyn("inet_ntoa", "EXT_INET_NTOA_")
        opsyn("ntohl", "EXT_NTOHL_")
        opsyn("ntohs", "EXT_NTOHS_")
        define("recv(socket,var,length,flags)")         :(recv_end)
recv    $var = dupl(char(0),length)
        recv = EXT_RECV_(socket, $var, flags)           :f(FRETURN)
        $var = substr($var, 1, recv)                    :(RETURN)
recv_end
        opsyn("send", "EXT_SEND_")
        opsyn("socket", "EXT_SOCKET_")

* Define Database Socket APIs
* 
        opsyn("gethostbyname", "EXT_GETHOSTBYNAME_")
        opsyn("gethostname", "EXT_GETHOSTNAME_")

* Define Socket Extension APIs
*
        opsyn("SocketCleanup", "EXT_SOCKETCLEANUP_")
        opsyn("SocketStartup", "EXT_SOCKETSTARTUP_")

* end of SOCKETS.INC
