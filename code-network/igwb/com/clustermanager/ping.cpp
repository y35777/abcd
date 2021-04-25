/******************************************************************************\
* ping.c - Simple ping utility using SOCK_RAW
* 
*       This is a part of the Microsoft Source Code Samples.
*       Copyright 1996-1997 Microsoft Corporation.
*       All rights reserved.
*       This source code is only intended as a supplement to
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the
*       Microsoft samples programs.
\******************************************************************************/

#pragma pack(4)

#include "../include/frame.h"
#include "ping.h"
#include "resource.h"

#define ICMP_ECHO 8
#define ICMP_ECHOREPLY 0
#define ICMP_MIN 8 // minimum 8 byte icmp packet (just header)

/* The IP header */
typedef struct iphdr {
	unsigned int h_len:4;          // length of the header
	unsigned int version:4;        // Version of IP
	unsigned char tos;             // Type of service
	unsigned short total_len;      // total length of the packet
	unsigned short ident;          // unique identifier
	unsigned short frag_and_flags; // flags
	unsigned char  ttl; 
	unsigned char proto;           // protocol (TCP, UDP etc)
	unsigned short checksum;       // IP checksum
	unsigned int sourceIP;
	unsigned int destIP;
}IpHeader;

//
// ICMP header
//
typedef struct _ihdr {
  BYTE i_type;
  BYTE i_code; /* type sub code */
  USHORT i_cksum;
  USHORT i_id;
  USHORT i_seq;
  /* This is not the std header, but we reserve space for time */
  ULONG timestamp;
}IcmpHeader;

#define STATUS_FAILED 0xFFFF
#define DEF_PACKET_SIZE 32
#define MAX_PACKET 1024


void fill_icmp_data(char *, int);
USHORT checksum(USHORT *, int);
BOOL decode_resp(char *,int ,struct sockaddr_in *);

BOOL Ping(DWORD destaddr)
{
	SOCKET sockRaw;
	struct sockaddr_in dest,from;
	int bread,datasize;
	int fromlen = sizeof(from);
	int timeout = 1000;
	char *icmp_data;
	char *recvbuf;
	USHORT seq_no = 0;
	
	sockRaw = socket(AF_INET,
		SOCK_RAW,
		IPPROTO_ICMP);
	
	if (sockRaw == INVALID_SOCKET) 
	{
		return FALSE;
	}
	
	bread = setsockopt(sockRaw,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,
		sizeof(timeout));
	if(bread == SOCKET_ERROR) 
	{
		return FALSE;
	}
	timeout = 1000;
	bread = setsockopt(sockRaw,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,
		sizeof(timeout));
	if(bread == SOCKET_ERROR)
	{
		return FALSE;
	}

	memset(&dest,0,sizeof(dest));
	dest.sin_addr.s_addr = destaddr;
	dest.sin_family = AF_INET;
	
	datasize = DEF_PACKET_SIZE;	
	datasize += sizeof(IcmpHeader);  
	
	icmp_data = new char[MAX_PACKET];
	recvbuf = new char[MAX_PACKET];		
	memset(icmp_data, 0, MAX_PACKET);
	fill_icmp_data(icmp_data, datasize);

	BOOL bSucc = FALSE;
	for(int i = 0; i < 4; i++)
	{
		int bwrote;
		((IcmpHeader*)icmp_data)->i_cksum = 0;
		((IcmpHeader*)icmp_data)->timestamp = GetTickCount();		
		((IcmpHeader*)icmp_data)->i_seq = seq_no++;
		((IcmpHeader*)icmp_data)->i_cksum = checksum((USHORT*)icmp_data, 
			datasize);
		
		bwrote = sendto(sockRaw,icmp_data,datasize,0,(struct sockaddr*)&dest,
			sizeof(dest));
		if (bwrote == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAETIMEDOUT) 
			{
				MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, S_PING_TIME_OUT);
				continue;
			}
			break;
		}
		bread = recvfrom(sockRaw,recvbuf,MAX_PACKET,0,(struct sockaddr*)&from,
			&fromlen);
		if (bread == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAETIMEDOUT) 
			{
				MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, S_PING_TIME_OUT);
				continue;
			}
			break;
		}
		if(decode_resp(recvbuf,bread,&from))
		{
			bSucc = TRUE;
			break;
		}
	}
	closesocket(sockRaw);
	delete icmp_data;
	delete recvbuf;
	return bSucc;
}

BOOL Ping(LPCTSTR szIpAddr)
{
	return Ping(inet_addr(szIpAddr));
}

/* 
The response is an IP packet. We must decode the IP header to locate 
the ICMP data 
*/
BOOL decode_resp(char *buf, int bytes,struct sockaddr_in *from) 
{
	
	IpHeader *iphdr;
	IcmpHeader *icmphdr;
	unsigned short iphdrlen;
	
	iphdr = (IpHeader *)buf;	
	iphdrlen = iphdr->h_len * 4 ; // number of 32-bit words *4 = bytes	
	icmphdr = (IcmpHeader*)(buf + iphdrlen);
	
	if (icmphdr->i_type != ICMP_ECHOREPLY) {
		MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT,
               S_PING_RECV_NONE_ECHO, icmphdr->i_type);
		return FALSE;
	}
	if (icmphdr->i_id != (USHORT)GetCurrentProcessId()) {
		MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, S_PING_RECV_OTHER_PACKET);
		return FALSE;
	}
	MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, 
           S_PING_RECV_BYTE_FROM, bytes, inet_ntoa(from->sin_addr),
           icmphdr->i_seq, GetTickCount()-icmphdr->timestamp);
	return TRUE;
}


USHORT checksum(USHORT *buffer, int size) 
{
	
	unsigned long cksum=0;
	
	while(size >1) {
		cksum+=*buffer++;
		size -=sizeof(USHORT);
	}
	
	if(size ) {
		cksum += *(UCHAR*)buffer;
	}
	
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >>16);
	return (USHORT)(~cksum);
}
/* 
Helper function to fill in various stuff in our ICMP request.
*/
void fill_icmp_data(char * icmp_data, int datasize)
{
	
	IcmpHeader *icmp_hdr;
	char *datapart;
	
	icmp_hdr = (IcmpHeader*)icmp_data;
	
	icmp_hdr->i_type = ICMP_ECHO;
	icmp_hdr->i_code = 0;
	icmp_hdr->i_id = (USHORT)GetCurrentProcessId();
	icmp_hdr->i_cksum = 0;
	icmp_hdr->i_seq = 0;
	
	datapart = icmp_data + sizeof(IcmpHeader);
	//
	// Place some junk in the buffer.
	//
	memset(datapart,'E', datasize - sizeof(IcmpHeader));	
}
