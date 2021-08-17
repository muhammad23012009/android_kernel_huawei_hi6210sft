/*
 * Copyright (c) 1996, 2003 VIA Networking Technologies, Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
<<<<<<< HEAD
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * File: rxtx.h
 *
 * Purpose:
 *
 * Author: Jerry Chen
 *
 * Date: Jun. 27, 2002
 *
 */

#ifndef __RXTX_H__
#define __RXTX_H__

#include "device.h"
#include "wcmd.h"
<<<<<<< HEAD

//
// RTS buffer header
//
typedef struct tagSRTSDataF {
    u16    wFrameControl;
    u16    wDurationID;
    u8    abyRA[ETH_ALEN];
    u8    abyTA[ETH_ALEN];
} SRTSDataF, *PSRTSDataF;

//
// CTS buffer header
//
typedef struct tagSCTSDataF {
    u16    wFrameControl;
    u16    wDurationID;
    u8    abyRA[ETH_ALEN];
    u16    wReserved;
} SCTSDataF, *PSCTSDataF;

//
// MICHDR data header
//
typedef struct tagSMICHDR {
	u32 adwHDR0[4];
	u32 adwHDR1[4];
	u32 adwHDR2[4];
} SMICHDR, *PSMICHDR;

typedef struct tagSTX_NAF_G_RTS
{
    //RsvTime
    u16            wRTSTxRrvTime_ba;
    u16            wRTSTxRrvTime_aa;
    u16            wRTSTxRrvTime_bb;
    u16            wReserved2;
    u16            wTxRrvTime_b;
    u16            wTxRrvTime_a;

    //RTS
    u8            byRTSSignalField_b;
    u8            byRTSServiceField_b;
    u16            wRTSTransmitLength_b;
    u8            byRTSSignalField_a;
    u8            byRTSServiceField_a;
    u16            wRTSTransmitLength_a;
    u16            wRTSDuration_ba;
    u16            wRTSDuration_aa;
    u16            wRTSDuration_bb;
    u16            wReserved3;
    SRTSDataF       sRTS;

    //Data
    u8            bySignalField_b;
    u8            byServiceField_b;
    u16            wTransmitLength_b;
    u8            bySignalField_a;
    u8            byServiceField_a;
    u16            wTransmitLength_a;
    u16            wDuration_b;
    u16            wDuration_a;
    u16            wTimeStampOff_b;
    u16            wTimeStampOff_a;

} TX_NAF_G_RTS, *PTX_NAF_G_RTS;

typedef struct tagSTX_NAF_G_RTS_MIC
{
    //RsvTime
    u16            wRTSTxRrvTime_ba;
    u16            wRTSTxRrvTime_aa;
    u16            wRTSTxRrvTime_bb;
    u16            wReserved2;
    u16            wTxRrvTime_b;
    u16            wTxRrvTime_a;

    SMICHDR         sMICHDR;

    //RTS
    u8            byRTSSignalField_b;
    u8            byRTSServiceField_b;
    u16            wRTSTransmitLength_b;
    u8            byRTSSignalField_a;
    u8            byRTSServiceField_a;
    u16            wRTSTransmitLength_a;
    u16            wRTSDuration_ba;
    u16            wRTSDuration_aa;
    u16            wRTSDuration_bb;
    u16            wReserved3;
    SRTSDataF       sRTS;

    //Data
    u8            bySignalField_b;
    u8            byServiceField_b;
    u16            wTransmitLength_b;
    u8            bySignalField_a;
    u8            byServiceField_a;
    u16            wTransmitLength_a;
    u16            wDuration_b;
    u16            wDuration_a;
    u16            wTimeStampOff_b;
    u16            wTimeStampOff_a;

} TX_NAF_G_RTS_MIC, *PTX_NAF_G_RTS_MIC;

typedef struct tagSTX_NAF_G_CTS
{
    //RsvTime
    u16            wCTSTxRrvTime_ba;
    u16            wReserved2;
    u16            wTxRrvTime_b;
    u16            wTxRrvTime_a;

    //CTS
    u8            byCTSSignalField_b;
    u8            byCTSServiceField_b;
    u16            wCTSTransmitLength_b;
    u16            wCTSDuration_ba;
    u16            wReserved3;
    SCTSDataF       sCTS;

    //Data
    u8            bySignalField_b;
    u8            byServiceField_b;
    u16            wTransmitLength_b;
    u8            bySignalField_a;
    u8            byServiceField_a;
    u16            wTransmitLength_a;
    u16            wDuration_b;
    u16            wDuration_a;
    u16            wTimeStampOff_b;
    u16            wTimeStampOff_a;

} TX_NAF_G_CTS, *PTX_NAF_G_CTS;

typedef struct tagSTX_NAF_G_CTS_MIC
{
    //RsvTime
    u16            wCTSTxRrvTime_ba;
    u16            wReserved2;
    u16            wTxRrvTime_b;
    u16            wTxRrvTime_a;

    SMICHDR         sMICHDR;

    //CTS
    u8            byCTSSignalField_b;
    u8            byCTSServiceField_b;
    u16            wCTSTransmitLength_b;
    u16            wCTSDuration_ba;
    u16            wReserved3;
    SCTSDataF       sCTS;

    //Data
    u8            bySignalField_b;
    u8            byServiceField_b;
    u16            wTransmitLength_b;
    u8            bySignalField_a;
    u8            byServiceField_a;
    u16            wTransmitLength_a;
    u16            wDuration_b;
    u16            wDuration_a;
    u16            wTimeStampOff_b;
    u16            wTimeStampOff_a;

} TX_NAF_G_CTS_MIC, *PTX_NAF_G_CTS_MIC;

typedef struct tagSTX_NAF_G_BEACON
{
    u16            wFIFOCtl;
    u16            wTimeStamp;

    //CTS
    u8            byCTSSignalField_b;
    u8            byCTSServiceField_b;
    u16            wCTSTransmitLength_b;
    u16            wCTSDuration_ba;
    u16            wReserved1;
    SCTSDataF       sCTS;

    //Data
    u8            bySignalField_a;
    u8            byServiceField_a;
    u16            wTransmitLength_a;
    u16            wDuration_a;
    u16            wTimeStampOff_a;

} TX_NAF_G_BEACON, *PTX_NAF_G_BEACON;

typedef struct tagSTX_NAF_AB_RTS
{
    //RsvTime
    u16            wRTSTxRrvTime_ab;
    u16            wTxRrvTime_ab;

    //RTS
    u8            byRTSSignalField_ab;
    u8            byRTSServiceField_ab;
    u16            wRTSTransmitLength_ab;
    u16            wRTSDuration_ab;
    u16            wReserved2;
    SRTSDataF       sRTS;

    //Data
    u8            bySignalField_ab;
    u8            byServiceField_ab;
    u16            wTransmitLength_ab;
    u16            wDuration_ab;
    u16            wTimeStampOff_ab;

} TX_NAF_AB_RTS, *PTX_NAF_AB_RTS;

typedef struct tagSTX_NAF_AB_RTS_MIC
{
    //RsvTime
    u16            wRTSTxRrvTime_ab;
    u16            wTxRrvTime_ab;

    SMICHDR         sMICHDR;

    //RTS
    u8            byRTSSignalField_ab;
    u8            byRTSServiceField_ab;
    u16            wRTSTransmitLength_ab;
    u16            wRTSDuration_ab;
    u16            wReserved2;
    SRTSDataF       sRTS;

    //Data
    u8            bySignalField_ab;
    u8            byServiceField_ab;
    u16            wTransmitLength_ab;
    u16            wDuration_ab;
    u16            wTimeStampOff_ab;

} TX_NAF_AB_RTS_MIC, *PTX_NAF_AB_RTS_MIC;

typedef struct tagSTX_NAF_AB_CTS
{
    //RsvTime
    u16            wReserved2;
    u16            wTxRrvTime_ab;

    //Data
    u8            bySignalField_ab;
    u8            byServiceField_ab;
    u16            wTransmitLength_ab;
    u16            wDuration_ab;
    u16            wTimeStampOff_ab;

} TX_NAF_AB_CTS, *PTX_NAF_AB_CTS;

typedef struct tagSTX_NAF_AB_CTS_MIC
{
    //RsvTime
    u16            wReserved2;
    u16            wTxRrvTime_ab;

    SMICHDR         sMICHDR;

    //Data
    u8            bySignalField_ab;
    u8            byServiceField_ab;
    u16            wTransmitLength_ab;
    u16            wDuration_ab;
    u16            wTimeStampOff_ab;

} TX_NAF_AB_CTS_MIC, *PTX_NAF_AB_CTS_MIC;

typedef struct tagSTX_NAF_AB_BEACON
{
    u16            wFIFOCtl;
    u16            wTimeStamp;

   //Data
    u8            bySignalField_ab;
    u8            byServiceField_ab;
    u16            wTransmitLength_ab;
    u16            wDuration_ab;
    u16            wTimeStampOff_ab;

} TX_NAF_AB_BEACON, *PTX_NAF_AB_BEACON;

typedef struct tagSTX_AF_G_RTS
{
    //RsvTime
    u16            wRTSTxRrvTime_ba;
    u16            wRTSTxRrvTime_aa;
    u16            wRTSTxRrvTime_bb;
    u16            wReserved2;
    u16            wTxRrvTime_b;
    u16            wTxRrvTime_a;

    //RTS
    u8            byRTSSignalField_b;
    u8            byRTSServiceField_b;
    u16            wRTSTransmitLength_b;
    u8            byRTSSignalField_a;
    u8            byRTSServiceField_a;
    u16            wRTSTransmitLength_a;
    u16            wRTSDuration_ba;
    u16            wRTSDuration_aa;
    u16            wRTSDuration_bb;
    u16            wReserved3;
    u16            wRTSDuration_ba_f0;
    u16            wRTSDuration_aa_f0;
    u16            wRTSDuration_ba_f1;
    u16            wRTSDuration_aa_f1;
    SRTSDataF       sRTS;

    //Data
    u8            bySignalField_b;
    u8            byServiceField_b;
    u16            wTransmitLength_b;
    u8            bySignalField_a;
    u8            byServiceField_a;
    u16            wTransmitLength_a;
    u16            wDuration_b;
    u16            wDuration_a;
    u16            wDuration_a_f0;
    u16            wDuration_a_f1;
    u16            wTimeStampOff_b;
    u16            wTimeStampOff_a;

} TX_AF_G_RTS, *PTX_AF_G_RTS;

typedef struct tagSTX_AF_G_RTS_MIC
{
    //RsvTime
    u16            wRTSTxRrvTime_ba;
    u16            wRTSTxRrvTime_aa;
    u16            wRTSTxRrvTime_bb;
    u16            wReserved2;
    u16            wTxRrvTime_b;
    u16            wTxRrvTime_a;

    SMICHDR         sMICHDR;

    //RTS
    u8            byRTSSignalField_b;
    u8            byRTSServiceField_b;
    u16            wRTSTransmitLength_b;
    u8            byRTSSignalField_a;
    u8            byRTSServiceField_a;
    u16            wRTSTransmitLength_a;
    u16            wRTSDuration_ba;
    u16            wRTSDuration_aa;
    u16            wRTSDuration_bb;
    u16            wReserved3;
    u16            wRTSDuration_ba_f0;
    u16            wRTSDuration_aa_f0;
    u16            wRTSDuration_ba_f1;
    u16            wRTSDuration_aa_f1;
    SRTSDataF       sRTS;

    //Data
    u8            bySignalField_b;
    u8            byServiceField_b;
    u16            wTransmitLength_b;
    u8            bySignalField_a;
    u8            byServiceField_a;
    u16            wTransmitLength_a;
    u16            wDuration_b;
    u16            wDuration_a;
    u16            wDuration_a_f0;
    u16            wDuration_a_f1;
    u16            wTimeStampOff_b;
    u16            wTimeStampOff_a;

} TX_AF_G_RTS_MIC, *PTX_AF_G_RTS_MIC;

typedef struct tagSTX_AF_G_CTS
{
    //RsvTime
    u16            wCTSTxRrvTime_ba;
    u16            wReserved2;
    u16            wTxRrvTime_b;
    u16            wTxRrvTime_a;

    //CTS
    u8            byCTSSignalField_b;
    u8            byCTSServiceField_b;
    u16            wCTSTransmitLength_b;
    u16            wCTSDuration_ba;
    u16            wReserved3;
    u16            wCTSDuration_ba_f0;
    u16            wCTSDuration_ba_f1;
    SCTSDataF       sCTS;

    //Data
    u8            bySignalField_b;
    u8            byServiceField_b;
    u16            wTransmitLength_b;
    u8            bySignalField_a;
    u8            byServiceField_a;
    u16            wTransmitLength_a;
    u16            wDuration_b;
    u16            wDuration_a;
    u16            wDuration_a_f0;
    u16            wDuration_a_f1;
    u16            wTimeStampOff_b;
    u16            wTimeStampOff_a;

} TX_AF_G_CTS, *PTX_AF_G_CTS;

typedef struct tagSTX_AF_G_CTS_MIC
{
    //RsvTime
    u16            wCTSTxRrvTime_ba;
    u16            wReserved2;
    u16            wTxRrvTime_b;
    u16            wTxRrvTime_a;

    SMICHDR         sMICHDR;

    //CTS
    u8            byCTSSignalField_b;
    u8            byCTSServiceField_b;
    u16            wCTSTransmitLength_b;
    u16            wCTSDuration_ba;
    u16            wReserved3;
    u16            wCTSDuration_ba_f0;
    u16            wCTSDuration_ba_f1;
    SCTSDataF       sCTS;

    //Data
    u8            bySignalField_b;
    u8            byServiceField_b;
    u16            wTransmitLength_b;
    u8            bySignalField_a;
    u8            byServiceField_a;
    u16            wTransmitLength_a;
    u16            wDuration_b;
    u16            wDuration_a;
    u16            wDuration_a_f0;
    u16            wDuration_a_f1;
    u16            wTimeStampOff_b;
    u16            wTimeStampOff_a;

} TX_AF_G_CTS_MIC, *PTX_AF_G_CTS_MIC;

typedef struct tagSTX_AF_A_RTS
{
    //RsvTime
    u16            wRTSTxRrvTime_a;
    u16            wTxRrvTime_a;

    //RTS
    u8            byRTSSignalField_a;
    u8            byRTSServiceField_a;
    u16            wRTSTransmitLength_a;
    u16            wRTSDuration_a;
    u16            wReserved2;
    u16            wRTSDuration_a_f0;
    u16            wRTSDuration_a_f1;
    SRTSDataF       sRTS;

    //Data
    u8            bySignalField_a;
    u8            byServiceField_a;
    u16            wTransmitLength_a;
    u16            wDuration_a;
    u16            wTimeStampOff_a;
    u16            wDuration_a_f0;
    u16            wDuration_a_f1;

} TX_AF_A_RTS, *PTX_AF_A_RTS;

typedef struct tagSTX_AF_A_RTS_MIC
{
    //RsvTime
    u16            wRTSTxRrvTime_a;
    u16            wTxRrvTime_a;

    SMICHDR         sMICHDR;

    //RTS
    u8            byRTSSignalField_a;
    u8            byRTSServiceField_a;
    u16            wRTSTransmitLength_a;
    u16            wRTSDuration_a;
    u16            wReserved2;
    u16            wRTSDuration_a_f0;
    u16            wRTSDuration_a_f1;
    SRTSDataF       sRTS;

    //Data
    u8            bySignalField_a;
    u8            byServiceField_a;
    u16            wTransmitLength_a;
    u16            wDuration_a;
    u16            wTimeStampOff_a;
    u16            wDuration_a_f0;
    u16            wDuration_a_f1;

} TX_AF_A_RTS_MIC, *PTX_AF_A_RTS_MIC;

typedef struct tagSTX_AF_A_CTS
{
    //RsvTime
    u16            wReserved2;
    u16            wTxRrvTime_a;

    //Data
    u8            bySignalField_a;
    u8            byServiceField_a;
    u16            wTransmitLength_a;
    u16            wDuration_a;
    u16            wTimeStampOff_a;
    u16            wDuration_a_f0;
    u16            wDuration_a_f1;

} TX_AF_A_CTS, *PTX_AF_A_CTS;

typedef struct tagSTX_AF_A_CTS_MIC
{
    //RsvTime
    u16            wReserved2;
    u16            wTxRrvTime_a;

    SMICHDR         sMICHDR;

    //Data
    u8            bySignalField_a;
    u8            byServiceField_a;
    u16            wTransmitLength_a;
    u16            wDuration_a;
    u16            wTimeStampOff_a;
    u16            wDuration_a_f0;
    u16            wDuration_a_f1;

} TX_AF_A_CTS_MIC, *PTX_AF_A_CTS_MIC;

//
// union with all of the TX Buffer Type
//
typedef union tagUTX_BUFFER_CONTAINER
{
    TX_NAF_G_RTS                    RTS_G;
    TX_NAF_G_RTS_MIC                RTS_G_MIC;
    TX_NAF_G_CTS                    CTS_G;
    TX_NAF_G_CTS_MIC                CTS_G_MIC;
    //TX_NAF_G_BEACON                 Beacon_G;
    TX_NAF_AB_RTS                   RTS_AB;
    TX_NAF_AB_RTS_MIC               RTS_AB_MIC;
    TX_NAF_AB_CTS                   CTS_AB;
    TX_NAF_AB_CTS_MIC               CTS_AB_MIC;
    //TX_NAF_AB_BEACON                Beacon_AB;
    TX_AF_G_RTS                     RTS_G_AutoFB;
    TX_AF_G_RTS_MIC                 RTS_G_AutoFB_MIC;
    TX_AF_G_CTS                     CTS_G_AutoFB;
    TX_AF_G_CTS_MIC                 CTS_G_AutoFB_MIC;
    TX_AF_A_RTS                     RTS_A_AutoFB;
    TX_AF_A_RTS_MIC                 RTS_A_AutoFB_MIC;
    TX_AF_A_CTS                     CTS_A_AutoFB;
    TX_AF_A_CTS_MIC                 CTS_A_AutoFB_MIC;

} TX_BUFFER_CONTAINER, *PTX_BUFFER_CONTAINER;

//
// Remote NDIS message format
//
typedef struct tagSTX_BUFFER
{
    u8                            byType;
    u8                            byPKTNO;
    u16                            wTxByteCount;

	u32 adwTxKey[4];
    u16                            wFIFOCtl;
    u16                            wTimeStamp;
    u16                            wFragCtl;
    u16                            wReserved;

    // Actual message
    TX_BUFFER_CONTAINER             BufferHeader;

} TX_BUFFER, *PTX_BUFFER;

//
// Remote NDIS message format
//
typedef struct tagSBEACON_BUFFER
{
    u8                            byType;
    u8                            byPKTNO;
    u16                            wTxByteCount;

    u16                            wFIFOCtl;
    u16                            wTimeStamp;

    // Actual message
    TX_BUFFER_CONTAINER             BufferHeader;

} BEACON_BUFFER, *PBEACON_BUFFER;

void vDMA0_tx_80211(struct vnt_private *, struct sk_buff *skb);
int nsDMA_tx_packet(struct vnt_private *, u32 uDMAIdx, struct sk_buff *skb);
CMD_STATUS csMgmt_xmit(struct vnt_private *, struct vnt_tx_mgmt *);
CMD_STATUS csBeacon_xmit(struct vnt_private *, struct vnt_tx_mgmt *);
int bRelayPacketSend(struct vnt_private *, u8 *pbySkbData, u32 uDataLen,
	u32 uNodeIndex);
=======
#include "baseband.h"

#define DEFAULT_MGN_LIFETIME_RES_64us	125  /* 64us */
#define DEFAULT_MSDU_LIFETIME_RES_64us  8000

/* MIC HDR data header */
struct vnt_mic_hdr {
	u8 id;
	u8 tx_priority;
	u8 mic_addr2[6];
	u8 ccmp_pn[IEEE80211_CCMP_PN_LEN];
	__be16 payload_len;
	__be16 hlen;
	__le16 frame_control;
	u8 addr1[6];
	u8 addr2[6];
	u8 addr3[6];
	__le16 seq_ctrl;
	u8 addr4[6];
	u16 packing; /* packing to 48 bytes */
} __packed;

/* RsvTime buffer header */
struct vnt_rrv_time_rts {
	__le16 rts_rrv_time_ba;
	__le16 rts_rrv_time_aa;
	__le16 rts_rrv_time_bb;
	u16 wReserved;
	__le16 rrv_time_b;
	__le16 rrv_time_a;
} __packed;

struct vnt_rrv_time_cts {
	__le16 cts_rrv_time_ba;
	u16 wReserved;
	__le16 rrv_time_b;
	__le16 rrv_time_a;
} __packed;

struct vnt_rrv_time_ab {
	__le16 rts_rrv_time;
	__le16 rrv_time;
} __packed;

/* TX data header */
struct vnt_tx_datahead_g {
	struct vnt_phy_field b;
	struct vnt_phy_field a;
	__le16 duration_b;
	__le16 duration_a;
	__le16 time_stamp_off_b;
	__le16 time_stamp_off_a;
	struct ieee80211_hdr hdr;
} __packed;

struct vnt_tx_datahead_g_fb {
	struct vnt_phy_field b;
	struct vnt_phy_field a;
	__le16 duration_b;
	__le16 duration_a;
	__le16 duration_a_f0;
	__le16 duration_a_f1;
	__le16 time_stamp_off_b;
	__le16 time_stamp_off_a;
	struct ieee80211_hdr hdr;
} __packed;

struct vnt_tx_datahead_ab {
	struct vnt_phy_field ab;
	__le16 duration;
	__le16 time_stamp_off;
	struct ieee80211_hdr hdr;
} __packed;

struct vnt_tx_datahead_a_fb {
	struct vnt_phy_field a;
	__le16 duration;
	__le16 time_stamp_off;
	__le16 duration_f0;
	__le16 duration_f1;
	struct ieee80211_hdr hdr;
} __packed;

/* RTS buffer header */
struct vnt_rts_g {
	struct vnt_phy_field b;
	struct vnt_phy_field a;
	__le16 duration_ba;
	__le16 duration_aa;
	__le16 duration_bb;
	u16 wReserved;
	struct ieee80211_rts data;
	struct vnt_tx_datahead_g data_head;
} __packed;

struct vnt_rts_g_fb {
	struct vnt_phy_field b;
	struct vnt_phy_field a;
	__le16 duration_ba;
	__le16 duration_aa;
	__le16 duration_bb;
	u16 wReserved;
	__le16 rts_duration_ba_f0;
	__le16 rts_duration_aa_f0;
	__le16 rts_duration_ba_f1;
	__le16 rts_duration_aa_f1;
	struct ieee80211_rts data;
	struct vnt_tx_datahead_g_fb data_head;
} __packed;

struct vnt_rts_ab {
	struct vnt_phy_field ab;
	__le16 duration;
	u16 wReserved;
	struct ieee80211_rts data;
	struct vnt_tx_datahead_ab data_head;
} __packed;

struct vnt_rts_a_fb {
	struct vnt_phy_field a;
	__le16 duration;
	u16 wReserved;
	__le16 rts_duration_f0;
	__le16 rts_duration_f1;
	struct ieee80211_rts data;
	struct vnt_tx_datahead_a_fb data_head;
} __packed;

/* CTS buffer header */
struct vnt_cts {
	struct vnt_phy_field b;
	__le16 duration_ba;
	u16 wReserved;
	struct ieee80211_cts data;
	u16 reserved2;
	struct vnt_tx_datahead_g data_head;
} __packed;

struct vnt_cts_fb {
	struct vnt_phy_field b;
	__le16 duration_ba;
	u16 wReserved;
	__le16 cts_duration_ba_f0;
	__le16 cts_duration_ba_f1;
	struct ieee80211_cts data;
	u16 reserved2;
	struct vnt_tx_datahead_g_fb data_head;
} __packed;

union vnt_tx_data_head {
	/* rts g */
	struct vnt_rts_g rts_g;
	struct vnt_rts_g_fb rts_g_fb;
	/* rts a/b */
	struct vnt_rts_ab rts_ab;
	struct vnt_rts_a_fb rts_a_fb;
	/* cts g */
	struct vnt_cts cts_g;
	struct vnt_cts_fb cts_g_fb;
	/* no rts/cts */
	struct vnt_tx_datahead_a_fb data_head_a_fb;
	struct vnt_tx_datahead_ab data_head_ab;
};

struct vnt_tx_mic_hdr {
	struct vnt_mic_hdr hdr;
	union vnt_tx_data_head head;
} __packed;

union vnt_tx {
	struct vnt_tx_mic_hdr mic;
	union vnt_tx_data_head head;
};

union vnt_tx_head {
	struct {
		struct vnt_rrv_time_rts rts;
		union vnt_tx tx;
	} __packed tx_rts;
	struct {
		struct vnt_rrv_time_cts cts;
		union vnt_tx tx;
	} __packed tx_cts;
	struct {
		struct vnt_rrv_time_ab ab;
		union vnt_tx tx;
	} __packed tx_ab;
};

struct vnt_tx_fifo_head {
	u8 tx_key[WLAN_KEY_LEN_CCMP];
	__le16 fifo_ctl;
	__le16 time_stamp;
	__le16 frag_ctl;
	__le16 current_rate;
} __packed;

struct vnt_tx_buffer {
	u8 type;
	u8 pkt_no;
	__le16 tx_byte_count;
	struct vnt_tx_fifo_head fifo_head;
	union vnt_tx_head tx_head;
} __packed;

struct vnt_tx_short_buf_head {
	__le16 fifo_ctl;
	u16 time_stamp;
	struct vnt_phy_field ab;
	__le16 duration;
	__le16 time_stamp_off;
} __packed;

struct vnt_beacon_buffer {
	u8 type;
	u8 pkt_no;
	__le16 tx_byte_count;
	struct vnt_tx_short_buf_head short_head;
	struct ieee80211_mgmt mgmt_hdr;
} __packed;

int vnt_tx_packet(struct vnt_private *, struct sk_buff *);
int vnt_beacon_make(struct vnt_private *, struct ieee80211_vif *);
int vnt_beacon_enable(struct vnt_private *, struct ieee80211_vif *,
	struct ieee80211_bss_conf *);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* __RXTX_H__ */
