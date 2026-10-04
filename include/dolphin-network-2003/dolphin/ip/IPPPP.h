/* Older PPP control prefix; observed offsets and unresolved intervals are
 * documented in docs/Network.md. No complete control-object size is asserted.
 */
#ifndef __DOLPHIN_OS_IP_PPP_H__
#define __DOLPHIN_OS_IP_PPP_H__

#include <dolphin/ip/IP.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LCPHeader {
    u8 code;
    u8 id;
    u16 len;
} LCPHeader;

typedef struct CHAPHeader {
    u8 code;
    u8 id;
    u16 len;
} CHAPHeader;

typedef struct PPPConf PPPConf;
typedef void (*PPPConfCallback)(PPPConf*);
typedef int (*PPPConfRecvCallback)(PPPConf*, LCPHeader*);
typedef int (*PPPConfTransCallback)(PPPConf*);

struct PPPConf {
    u16 protocol;
    int state;
    u8 id;
    u8 idTerminate;
    u8 idReject;
    u8 idEcho;
    int rxmit;
    int configure;
    int terminate;
    int failure;
    u8 unknown_01c[512]; // Unresolved interval before the negotiation data.
    u8 data[20];
    u16 len;
    OSAlarm alarm;
    IPInterface* interface;
    PPPConfRecvCallback receiveConfigureRequest;
    PPPConfRecvCallback receiveConfigureAck;
    PPPConfRecvCallback receiveConfigureNak;
    PPPConfRecvCallback receiveConfigureReject;
    PPPConfTransCallback up;
    PPPConfTransCallback down;
    PPPConfTransCallback started;
    PPPConfTransCallback finished;
    u8 unknown_284[4]; // Unresolved interval between finished and mru.
    u16 mru;

};

typedef struct PPPAuth {
    u8 peerIdLen;
    u8 passwordLen;
    u8 messageLen;
    char peerId[256];
    char password[256];
    char message[256];
} PPPAuth;

#define PPP_STATE_INITIAL 0
#define PPP_STATE_STARTING 1
#define PPP_STATE_CLOSED 2
#define PPP_STATE_STOPPED 3
#define PPP_STATE_CLOSING 4
#define PPP_STATE_STOPPING 5
#define PPP_STATE_REQ_SENT 6
#define PPP_STATE_ACK_RCVD 7
#define PPP_STATE_ACK_SENT 8
#define PPP_STATE_OPENED 9

#define PPP_IP 0x0021
#define PPP_IPCP 0x8021
#define PPP_LCP 0xC021
#define PPP_PAP 0xC023
#define PPP_CHAP 0xC223

typedef struct LCPOpt {
    u8 type;
    u8 len;
} LCPOpt;

typedef struct LCPOptPAP {
    u8 type;
    u8 len;
    u16 auth;
} LCPOptPAP;

typedef struct LCPOptCHAP {
    u8 type;
    u8 len;
    u16 auth;
    u8 algorithm;
} LCPOptCHAP;

typedef struct PAPHeader {
    u8 code;
    u8 id;
    u16 len;
} PAPHeader;

// IPPPP.c
int PPPLayerUp(PPPConf* conf);
int PPPLayerDown(PPPConf* conf);
int PPPLayerStarted(PPPConf* conf);
int PPPLayerFinished(PPPConf* conf);
void PPPInitializeRestartCount(PPPConf* conf);
void PPPZeroRestartCount(PPPConf* conf);
void PPPSetState(PPPConf* conf, int state);
int PPPGetState(PPPConf* conf);
void PPPIn(IPInterface* interface, u8* ppp, s32 len, u32 flag);
void PPPOpen(PPPConf* conf);
void PPPUp(PPPConf* conf);
void PPPDown(PPPConf* conf);
void PPPClose(PPPConf* conf);

u16 PPPDeleteOpt(u8* data, u16 len, LCPOpt* opt);
u16 PPPInsertOpt(u8* data, u16 len, LCPOpt* at, LCPOpt* opt);
int PPPInit(IPInterface* interface, PPPConf* lcp, PPPConf* ipcp, const char* peerid, const char* password);
char* PPPGetMessage(void);

// IPLcp.c
void PPPDumpLCP(LCPHeader* lcp);
int PPPInitLCP(PPPConf* conf, IPInterface* interface);
void PPPDumpLCP(LCPHeader* lcp);

// IPIpcp.c
void PPPInitIPCP(PPPConf* ipcp, IPInterface* interface);

// IPPap.c
void PAPOpen(PPPConf* conf);
void PAPUp(PPPConf* conf);
void PAPDown(PPPConf* conf);
void PAPClose(PPPConf* conf);
int PAPTimeout(PPPConf* conf);
void PAPIn(PPPConf* conf, PAPHeader* pap, s32 len, u32 flag);
void PAPInit(PPPConf* conf, IPInterface* interface);

// IPChap.c
void CHAPOpen(PPPConf* conf);
void CHAPUp(PPPConf* conf);
void CHAPDown(PPPConf* conf);
void CHAPClose(PPPConf* conf);
int CHAPTimeout(PPPConf* conf);
void CHAPIn(PPPConf* conf, CHAPHeader* chap, s32 len, u32 flag);
void CHAPInit(PPPConf* conf, IPInterface* interface);

#ifdef __cplusplus
}
#endif

#endif
