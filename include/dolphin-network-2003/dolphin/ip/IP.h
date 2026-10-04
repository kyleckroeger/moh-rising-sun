/* Older network ABI subset recovered from the pinned Rising Sun executable.
 * Adapted from the reference recorded in dolphin-network-source.json.
 * Unknown byte intervals are intentionally unnamed; this is not a recovered
 * historical header. See docs/Network.md for evidence and scope.
 */
#ifndef __DOLPHIN_OS_IP_IP_H__
#define __DOLPHIN_OS_IP_IP_H__

#include <dolphin/types.h>
#include <dolphin/os.h>
#include <dolphin/ip/IFQueue.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IP_SOCKLEN 8
#define IP_ALEN 4

#ifndef IP_ERR_NONE
#define IP_ERR_NONE 0
#endif
#ifndef IP_ERR_BUSY
#define IP_ERR_BUSY (-1)
#endif
#define IP_MIN_HLEN 20
#define IF_MAX_VEC 4

#define IP_HLEN(ip) (((ip)->verlen & 0xF) << 2)

#define IP_INET 2

// TODO: where does this go? IPEth? IPArp?
#define ETH_IP 0x0800

/* Supported IP protocols */
#define IP_PROTO_ICMP 0x01
#define IP_PROTO_IGMP 0x02
#define IP_PROTO_TCP  0x06
#define IP_PROTO_UDP  0x11

typedef struct IPSocket {
    u8 len;
    u8 family;
    u16 port;
    u8 addr[4];
} IPSocket;

typedef struct IPInfo {
    u8 proto;
    u8 ttl;
    u8 tos;
    s32 poll; // Older API uses a 32-bit polling count at offset 4.
    IPSocket local;
    IPSocket remote;
    IFLink link;
} IPInfo;

typedef struct IFVec {
    void* data;
    s32 len;
} IFVec;

typedef struct IPInterface IPInterface;

typedef struct IFDatagram {
    IPInterface* interface;
    IFQueue* queue;
    IFLink link;
    u16 type; // Original IPOut stores the Ethernet type at 0x10.
    u8 unknown_12[6]; // Unresolved original fields; not accessed by this subset.
    u8 dst[4]; // Original IPOut passes this address at 0x18 to IPGetRoute.
    u8 unknown_1c[12]; // Unresolved original fields; not accessed by this subset.
    void (*callback)(void*, s32);
    void* param;
    s32 nVec;
    IFVec vec[1];
} IFDatagram;

typedef struct IPInterfaceConf {
    IPInterface* interface;
    IFQueue link;
    u8 addr[4];
    OSAlarm alarm;
    s32 count;
    void (* callback)(void *, long);
} IPInterfaceConf;

typedef struct IPInterfaceStat {
    u32 inUcastPackets;
    u32 inNonUcastPackets;
    u32 inDiscards;
    u32 inErrors;
    u32 outUcastPackets;
    u32 outNonUcastPackets;
    u32 outDiscards;
    u32 outErrors;
    u32 outCollisions;
} IPInterfaceStat;

// Observed prefix through free; the original trailing fields remain unresolved.
struct IPInterface {
    s32 type;
    BOOL up;
    s32 err;
    OSAlarm gratuitousAlarm;
    u8 mac[6];
    s32 mtu;
    u8 addr[4];
    u8 netmask[4];
    u8 broadcast[4];
    u8 gateway[4];
    u8 alias[4];
    IFQueue ppp;
    void (*out)(IPInterface*, IFDatagram*);
    void (*cancel)(IPInterface*, IFDatagram*);
    void* (*alloc)(IPInterface*, s32);
    BOOL (*free)(IPInterface*, void*, s32);

};

typedef struct IPHeader {
    u8 verlen;
    u8 tos;
    u16 len;
    u16 id;
    u16 frag;
    u8 ttl;
    u8 proto;
    u16 sum;
    u8 src[4];
    u8 dst[4];
} IPHeader;

char* IPNtoA(const u8* addr);
void IFInitDatagram(IFDatagram* datagram, u16 type, int nVec);
s32 IPOut(IFDatagram* datagram);
u16 IPCheckSum(IPHeader* ip);
void IPIn(IPInterface* interface, IPHeader* ip, s32 len, u32 flag);
IPInfo* IPLookupInfo(IFQueue* queue, u8* srcAddr, u8* dstAddr, u16 src, u16 dst);
BOOL __IPIsMember(IFQueue* queue, IPInfo* info);
BOOL IPBind(IFQueue* queue, IPInfo* info, const IPSocket* socket, BOOL reuse);
u16 IPGetAnonPort(IFQueue* queue, u16* last);
s32 IPConnect(IFQueue* queue, IPInfo* info, const IPSocket* socket, u16* last);
s32 IPGetRemoteSocket(IPInfo* info, IPSocket* socket);
s32 IPGetLocalSocket(IPInfo* info, IPSocket* socket);
s32 IPGetSockOpt(IPInfo* info, int level, int optname, void* optval, int* optlen);
s32 IPSetSockOpt(IPInfo* info, int level, int optname, void* optval, int optlen);
BOOL IPSetOption(IPInfo* info, u8 ttl, u8 tos);
void IPCancel(IFDatagram* datagram);

#ifdef __cplusplus
}
#endif

#endif
