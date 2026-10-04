/* Older TCP ABI subset; provenance and observed offsets: docs/Network.md.
 * The unknown trailing interval does not assert the later SDK field layout.
 */
#ifndef __DOLPHIN_OS_IP_TCP_H__
#define __DOLPHIN_OS_IP_TCP_H__

#include <dolphin/ip/IP.h>
#include <dolphin/ip/IFFifo.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TCP_STATE_CLOSED       0
#define TCP_STATE_LISTEN       1
#define TCP_STATE_SYN_SENT     2
#define TCP_STATE_SYN_RECEIVED 3
#define TCP_STATE_ESTABLISHED  4
#define TCP_STATE_FIN_WAIT1    5
#define TCP_STATE_FIN_WAIT2    6
#define TCP_STATE_CLOSE_WAIT   7
#define TCP_STATE_CLOSING      8
#define TCP_STATE_LAST_ACK     9
#define TCP_STATE_TIME_WAIT    10

#define TCP_MIN_HLEN 20
#define TCP_MAX_HLEN 60
#define TCP_HLEN(tcp) (((tcp)->flag & 0xF000) >> 10)

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#define TCP_OPT_EOL            0
#define TCP_OPT_NOP            1
#define TCP_OPT_MSS            2
#define TCP_OPT_WS             3
#define TCP_OPT_SACK_PERMITTED 4
#define TCP_OPT_SACK           5

// NOTE: argument order as used by the SDK asserts: TCP_SEQ_GT(a, b) is true when b is after a.
#define TCP_SEQ_LT(a, b) ((s32)((b) - (a)) < 0)
#define TCP_SEQ_LE(a, b) ((s32)((b) - (a)) <= 0)
#define TCP_SEQ_GT(a, b) ((s32)((b) - (a)) > 0)
#define TCP_SEQ_GE(a, b) ((s32)((b) - (a)) >= 0)

#define TCP_FLAG_FIN (1 << 0)
#define TCP_FLAG_SYN (1 << 1)
#define TCP_FLAG_RST (1 << 2)
#define TCP_FLAG_PSH (1 << 3)
#define TCP_FLAG_ACK (1 << 4)
#define TCP_FLAG_URG (1 << 5)

typedef struct TCPHeader {
    u16 src;
    u16 dst;
    s32 seq;
    s32 ack;
    u16 flag;
    u16 win;
    u16 sum;
    u16 urg;
} TCPHeader;

typedef struct TCPSackHole {
    s32 start;
    s32 end;
    int dupAcks;
    s32 rxmit;
} TCPSackHole;

typedef struct TCPStatistics {
    u32 sendTotal;
    u32 recvTotal;
    u32 rxmitTimeout;
    u32 rxmitPackets;
    u32 rxmitBytes;
} TCPStatistics;

typedef struct TCPInfo TCPInfo;
typedef void (*TCPCallback)(TCPInfo*, s32);

// Recovered older control block; the interval before node remains unresolved.
struct TCPInfo {
    IPInfo pair;
    OSThreadQueue queueThread;
    IPInterface* interface;
    s32 err;
    s32 sendUna;
    s32 sendNext;
    s32 sendWin;
    s32 sendUp;
    s32 sendWL1;
    s32 sendWL2;
    s32 iss;
    s32 sendMaxWin;
    s32 sendMax;
    s32 recvNext;
    s32 recvWin;
    s32 recvUp;
    s32 irs;
    s32 segLen;
    u8* segBegin;
    IFBlock asb[4];
    s32 state;
    u32 flag;
    TCPCallback closeCallback;
    s32* closeResult;
    s32 mss;
    volatile s32 sendBusy;
    u8 header[120];
    u8* sendData;
    s32 sendBuff;
    u8* sendPtr;
    s32 sendLen;
    IFDatagram datagram;
    IFVec vec[3];
    TCPCallback sendCallback;
    s32* sendResult;
    s32 userAcked;
    u8* userSendData;
    s32 userSendLen;
    OSTime lastSend;
    u8* recvData;
    s32 recvBuff;
    s32 recvUser;
    u8* recvPtr;
    s32 recvAcked; // Last acknowledged receive sequence at 0x1b0.
    TCPCallback recvCallback;
    s32* recvResult;
    u8* userData;
    s32 userBuff;
    s32 userLen;
    u8 oob;
    s32 recvUrg;
    TCPCallback urgCallback;
    s32* urgResult;
    u8* urgData;
    s32 rxmitCount;
    OSTime rto;
    OSTime r0;
    OSTime r2;
    OSAlarm rxmitAlarm;
    s32 cWin;
    s32 ssThresh;
    BOOL rttTiming;
    s32 rttSeq;
    OSTime rtt;
    OSTime srtt;
    OSTime rttDe;
    OSTime rttMin;
    OSTime rttMax;
    TCPInfo* listening;
    IPSocket* local;
    IPSocket* remote;
    IFQueue queueListen;
    IFLink linkListen;
    TCPCallback openCallback;
    s32* openResult;
    int linger;
    OSAlarm lingerAlarm;
    u8 unknown_2a8[32]; // Trailing fields not accessed by the retained subset.
    void* node; // GetNode reads offset 0x2c8; GetNode/PutNode free 0x2d0 bytes.
};

// IPTcp.c
s32 TCPIsn(IPInfo* info);
u16 TCPCheckSum(IFVec* vec, s32 nVec);
void TCPDumpHeader(const IPHeader* ip, const TCPHeader* tcp);
s32 TCPGetSegmentLength(IPHeader* ip, TCPHeader* tcp);
void TCPRespond(IPInterface* interface, u8* dstAddr, u16 dst, u8* srcAddr, u16 src, s32 seq, s32 ack, u16 flag, u16 win);
void TCPIn(IPInterface * interface /* r28 */, IPHeader * ip /* r29 */, u32 flag);
s32 TCPSendIn(TCPInfo* info, BOOL nonblock);
s32 TCPPeekOut(TCPInfo* info, void* ptr, s32 len, BOOL peek);
s32 TCPRecvOut(TCPInfo* info);
void TCPNotify(IPHeader* ip, const u8* gateway, s32 err);
void TCPSourceQuench(IPHeader* ip, const u8* gateway);
void TCPDeleteSackHoles(TCPInfo* info, TCPHeader* tcp);
void TCPUpdateScoreboard(TCPInfo* info, TCPHeader* tcp, u8* opt, int optlen);
BOOL __TCPTrimSegment(TCPInfo* info, IPHeader* ip, u16* flag);

// IPTcpTimeWait.c
BOOL TCPTestTimeWait(IPInterface* interface, IPHeader* ip, TCPHeader* tcp);
void TCPStartTimeWait(IPInterface* interface, TCPInfo* info);

// IPTcpOutput.c
TCPSackHole* TCPSackOutput(const TCPInfo* info);
int TCPMakeOption(TCPHeader* tcp, TCPInfo* info, u16 flag);
void TCPOutput(TCPInfo* info, u16 flag);
s32 __TCPCalcSendSize(TCPInfo* info, s32 effSendMss, u16* pflag);

// IPTcpTimer.c
void TCPStartRxmitTimer(TCPInfo* info);
void TCPStopRxmitTimer(TCPInfo* info, TCPHeader* tcp); // 2nd param unused (IPTcp.c passes tcp in r4)
void TCPCancelRxmitTimer(TCPInfo* info);
void TCPUpdateRtt(TCPInfo* info, OSTime rtt);
void TCPInitRtt(TCPInfo* info);

// IPTcpUser.c
void TCPEnumInfoQueue(TCPCallback callback);
TCPInfo* TCPLookupInfo(IPHeader* ip, TCPHeader* tcp);
s32 TCPGetStatus(TCPInfo* info);
s32 TCPGetRemoteSocket(TCPInfo* info, IPSocket* socket);
s32 TCPGetLocalSocket(TCPInfo* info, IPSocket* socket);
s32 TCPBind(TCPInfo* info, const IPSocket* socket);
BOOL TCPAbort(TCPInfo* info);
s32 TCPSetSendBuff(TCPInfo* info, void* sendbuf, s32 sendbufLen);
s32 TCPSetRecvBuff(TCPInfo* info, void* recvbuf, s32 recvbufLen);
s32 TCPGetSendBuff(TCPInfo* info, void* sendbuf, s32* sendbufLen);
s32 TCPGetRecvBuff(TCPInfo* info, void* recvbuf, s32* recvbufLen);
s32 TCPOpen(TCPInfo* info, void* sendbuf, s32 sendbufLen, void* recvbuf, s32 recvbufLen);
s32 TCPListen(TCPInfo* info, IPSocket* local, IPSocket* remote, int (*callback)(TCPInfo*, s32), s32* result);
s32 TCPAcceptAsync(TCPInfo* info, TCPInfo* listening, TCPCallback callback, s32* result);
s32 TCPAccept(TCPInfo* info, TCPInfo* listening);
s32 TCPConnectAsync(TCPInfo* info, const IPSocket* socket, TCPCallback callback, s32* result);
s32 TCPConnect(TCPInfo* info, const IPSocket* socket);
s32 TCPSendAsync(TCPInfo* info, void* data, s32 len, TCPCallback callback, s32* result);
s32 TCPSendNonblock(TCPInfo* info, void* data, s32 len);
s32 TCPSendUrgAsync(TCPInfo* info, void* data, s32 len, TCPCallback callback, s32* result);
s32 TCPSendUrgNonblock(TCPInfo* info, void* data, s32 len);
s32 TCPSend(TCPInfo* info, void* data, s32 len);
s32 TCPSendUrg(TCPInfo* info, void* data, s32 len);
s32 TCPReceiveExAsync(TCPInfo* info, void* data, s32 len, u32 flag, TCPCallback callback, s32* result);
s32 TCPReceiveEx(TCPInfo* info, void* data, s32 len, u32 flag);
s32 TCPReceiveAsync(TCPInfo* info, void* data, s32 len, TCPCallback callback, s32* result);
s32 TCPReceiveNonblock(TCPInfo* info, void* data, s32 len);
s32 TCPReceive(TCPInfo* info, void* data, s32 len);
s32 TCPPeek(TCPInfo* info, void* data, s32 len);
s32 TCPCloseAsync(TCPInfo* info, TCPCallback callback, s32* result);
s32 TCPClose(TCPInfo* info);
s32 TCPShutdown(TCPInfo* info, u32 flag);
s32 TCPCancel(TCPInfo* info);
s32 TCPReceiveUrgExAsync(TCPInfo* info, void* data, s32 len, u32 flag, TCPCallback callback, s32* result);
s32 TCPReceiveUrgAsync(TCPInfo* info, void* data, s32 len, TCPCallback callback, s32* result);
s32 TCPReceiveUrgNonblock(TCPInfo* info, void* data, s32 len);
s32 TCPReceiveUrgEx(TCPInfo* info, void* data, s32 len, u32 flags);
s32 TCPReceiveUrg(TCPInfo* info, void* data, s32 len);
s32 TCPPeekUrg(TCPInfo* info, void* data, s32 len);
s32 TCPGetUrgOffset(TCPInfo* info);
s32 TCPGetSockOpt(TCPInfo* info, int level, int optname, void* optval, int* optlen);
s32 TCPSetSockOpt(TCPInfo* info, int level, int optname, void* optval, int optlen);
s32 TCPSetTimeout(TCPInfo* info, OSTime threshold);
s32 TCPControlNagle(TCPInfo* info, BOOL enable);
s32 TCPSetUrgInLine(TCPInfo* info, BOOL inLine);
s32 TCPSetOption(TCPInfo* info, u8 ttl, u8 tos);
BOOL TCPOnReset(BOOL final);
s16 __TCPPoll(TCPInfo* info);

#ifdef __cplusplus
}
#endif

#endif
