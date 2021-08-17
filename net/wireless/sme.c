/*
<<<<<<< HEAD
 * SME code for cfg80211's connect emulation.
=======
 * SME code for cfg80211
 * both driver SME event handling and the SME implementation
 * (for nl80211's connect() and wext)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Copyright 2009	Johannes Berg <johannes@sipsolutions.net>
 * Copyright (C) 2009   Intel Corporation. All rights reserved.
 */

#include <linux/etherdevice.h>
#include <linux/if_arp.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/wireless.h>
#include <linux/export.h>
#include <net/iw_handler.h>
#include <net/cfg80211.h>
#include <net/rtnetlink.h>
#include "nl80211.h"
#include "reg.h"
#include "rdev-ops.h"

<<<<<<< HEAD
#ifndef MAC2STR
#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02X:%02X:%02X:%02X:%02X:%02X \n"
#endif
=======
/*
 * Software SME in cfg80211, using auth/assoc/deauth calls to the
 * driver. This is is for implementing nl80211's connect/disconnect
 * and wireless extensions (if configured.)
 */

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct cfg80211_conn {
	struct cfg80211_connect_params params;
	/* these are sub-states of the _CONNECTING sme_state */
	enum {
<<<<<<< HEAD
		CFG80211_CONN_IDLE,
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		CFG80211_CONN_SCANNING,
		CFG80211_CONN_SCAN_AGAIN,
		CFG80211_CONN_AUTHENTICATE_NEXT,
		CFG80211_CONN_AUTHENTICATING,
<<<<<<< HEAD
		CFG80211_CONN_ASSOCIATE_NEXT,
		CFG80211_CONN_ASSOCIATING,
		CFG80211_CONN_DEAUTH_ASSOC_FAIL,
	} state;
	u8 bssid[ETH_ALEN], prev_bssid[ETH_ALEN];
	u8 *ie;
=======
		CFG80211_CONN_AUTH_FAILED,
		CFG80211_CONN_ASSOCIATE_NEXT,
		CFG80211_CONN_ASSOCIATING,
		CFG80211_CONN_ASSOC_FAILED,
		CFG80211_CONN_DEAUTH,
		CFG80211_CONN_ABANDON,
		CFG80211_CONN_CONNECTED,
	} state;
	u8 bssid[ETH_ALEN], prev_bssid[ETH_ALEN];
	const u8 *ie;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	size_t ie_len;
	bool auto_auth, prev_bssid_valid;
};

<<<<<<< HEAD
static bool cfg80211_is_all_idle(void)
{
	struct cfg80211_registered_device *rdev;
	struct wireless_dev *wdev;
	bool is_all_idle = true;

	mutex_lock(&cfg80211_mutex);

	/*
	 * All devices must be idle as otherwise if you are actively
	 * scanning some new beacon hints could be learned and would
	 * count as new regulatory hints.
	 */
	list_for_each_entry(rdev, &cfg80211_rdev_list, list) {
		cfg80211_lock_rdev(rdev);
		list_for_each_entry(wdev, &rdev->wdev_list, list) {
			wdev_lock(wdev);
			if (wdev->sme_state != CFG80211_SME_IDLE)
				is_all_idle = false;
			wdev_unlock(wdev);
		}
		cfg80211_unlock_rdev(rdev);
	}

	mutex_unlock(&cfg80211_mutex);

	return is_all_idle;
}

static void disconnect_work(struct work_struct *work)
{
	if (!cfg80211_is_all_idle())
		return;

	regulatory_hint_disconnect();
}

static DECLARE_WORK(cfg80211_disconnect_work, disconnect_work);

static int cfg80211_conn_scan(struct wireless_dev *wdev)
{
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
=======
static void cfg80211_sme_free(struct wireless_dev *wdev)
{
	if (!wdev->conn)
		return;

	kfree(wdev->conn->ie);
	kfree(wdev->conn);
	wdev->conn = NULL;
}

static int cfg80211_conn_scan(struct wireless_dev *wdev)
{
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct cfg80211_scan_request *request;
	int n_channels, err;

	ASSERT_RTNL();
<<<<<<< HEAD
	ASSERT_RDEV_LOCK(rdev);
	ASSERT_WDEV_LOCK(wdev);
	lockdep_assert_held(&rdev->sched_scan_mtx);

	if (rdev->scan_req)
		return -EBUSY;

	if (wdev->conn->params.channel) {
		n_channels = 1;
	} else {
		enum ieee80211_band band;
		n_channels = 0;

		for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
			if (!wdev->wiphy->bands[band])
				continue;
			n_channels += wdev->wiphy->bands[band]->n_channels;
		}
	}
=======
	ASSERT_WDEV_LOCK(wdev);

	if (rdev->scan_req || rdev->scan_msg)
		return -EBUSY;

	if (wdev->conn->params.channel)
		n_channels = 1;
	else
		n_channels = ieee80211_get_num_supported_channels(wdev->wiphy);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	request = kzalloc(sizeof(*request) + sizeof(request->ssids[0]) +
			  sizeof(request->channels[0]) * n_channels,
			  GFP_KERNEL);
	if (!request)
		return -ENOMEM;

<<<<<<< HEAD
	if (wdev->conn->params.channel)
		request->channels[0] = wdev->conn->params.channel;
	else {
		int i = 0, j;
		enum ieee80211_band band;
		struct ieee80211_supported_band *bands;
		struct ieee80211_channel *channel;

		for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
=======
	if (wdev->conn->params.channel) {
		enum nl80211_band band = wdev->conn->params.channel->band;
		struct ieee80211_supported_band *sband =
			wdev->wiphy->bands[band];

		if (!sband) {
			kfree(request);
			return -EINVAL;
		}
		request->channels[0] = wdev->conn->params.channel;
		request->rates[band] = (1 << sband->n_bitrates) - 1;
	} else {
		int i = 0, j;
		enum nl80211_band band;
		struct ieee80211_supported_band *bands;
		struct ieee80211_channel *channel;

		for (band = 0; band < NUM_NL80211_BANDS; band++) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			bands = wdev->wiphy->bands[band];
			if (!bands)
				continue;
			for (j = 0; j < bands->n_channels; j++) {
				channel = &bands->channels[j];
				if (channel->flags & IEEE80211_CHAN_DISABLED)
					continue;
				request->channels[i++] = channel;
			}
			request->rates[band] = (1 << bands->n_bitrates) - 1;
		}
		n_channels = i;
	}
	request->n_channels = n_channels;
	request->ssids = (void *)&request->channels[n_channels];
	request->n_ssids = 1;

	memcpy(request->ssids[0].ssid, wdev->conn->params.ssid,
		wdev->conn->params.ssid_len);
	request->ssids[0].ssid_len = wdev->conn->params.ssid_len;

<<<<<<< HEAD
=======
	eth_broadcast_addr(request->bssid);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	request->wdev = wdev;
	request->wiphy = &rdev->wiphy;
	request->scan_start = jiffies;

	rdev->scan_req = request;

	err = rdev_scan(rdev, request);
	if (!err) {
		wdev->conn->state = CFG80211_CONN_SCANNING;
		nl80211_send_scan_start(rdev, wdev);
		dev_hold(wdev->netdev);
	} else {
		rdev->scan_req = NULL;
		kfree(request);
	}
	return err;
}

static int cfg80211_conn_do_work(struct wireless_dev *wdev)
{
<<<<<<< HEAD
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
=======
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct cfg80211_connect_params *params;
	struct cfg80211_assoc_request req = {};
	int err;

	ASSERT_WDEV_LOCK(wdev);

	if (!wdev->conn)
		return 0;

	params = &wdev->conn->params;

	switch (wdev->conn->state) {
<<<<<<< HEAD
	case CFG80211_CONN_SCAN_AGAIN:
		return cfg80211_conn_scan(wdev);
	case CFG80211_CONN_AUTHENTICATE_NEXT:
		BUG_ON(!rdev->ops->auth);
		wdev->conn->state = CFG80211_CONN_AUTHENTICATING;
		return __cfg80211_mlme_auth(rdev, wdev->netdev,
					    params->channel, params->auth_type,
					    params->bssid,
					    params->ssid, params->ssid_len,
					    NULL, 0,
					    params->key, params->key_len,
					    params->key_idx, NULL, 0);
	case CFG80211_CONN_ASSOCIATE_NEXT:
		BUG_ON(!rdev->ops->assoc);
=======
	case CFG80211_CONN_SCANNING:
		/* didn't find it during scan ... */
		return -ENOENT;
	case CFG80211_CONN_SCAN_AGAIN:
		return cfg80211_conn_scan(wdev);
	case CFG80211_CONN_AUTHENTICATE_NEXT:
		if (WARN_ON(!rdev->ops->auth))
			return -EOPNOTSUPP;
		wdev->conn->state = CFG80211_CONN_AUTHENTICATING;
		return cfg80211_mlme_auth(rdev, wdev->netdev,
					  params->channel, params->auth_type,
					  params->bssid,
					  params->ssid, params->ssid_len,
					  NULL, 0,
					  params->key, params->key_len,
					  params->key_idx, NULL, 0);
	case CFG80211_CONN_AUTH_FAILED:
		return -ENOTCONN;
	case CFG80211_CONN_ASSOCIATE_NEXT:
		if (WARN_ON(!rdev->ops->assoc))
			return -EOPNOTSUPP;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		wdev->conn->state = CFG80211_CONN_ASSOCIATING;
		if (wdev->conn->prev_bssid_valid)
			req.prev_bssid = wdev->conn->prev_bssid;
		req.ie = params->ie;
		req.ie_len = params->ie_len;
		req.use_mfp = params->mfp != NL80211_MFP_NO;
		req.crypto = params->crypto;
		req.flags = params->flags;
		req.ht_capa = params->ht_capa;
		req.ht_capa_mask = params->ht_capa_mask;
		req.vht_capa = params->vht_capa;
		req.vht_capa_mask = params->vht_capa_mask;

<<<<<<< HEAD
		err = __cfg80211_mlme_assoc(rdev, wdev->netdev, params->channel,
					    params->bssid, params->ssid,
					    params->ssid_len, &req);
		if (err)
			__cfg80211_mlme_deauth(rdev, wdev->netdev, params->bssid,
					       NULL, 0,
					       WLAN_REASON_DEAUTH_LEAVING,
					       false);
		return err;
	case CFG80211_CONN_DEAUTH_ASSOC_FAIL:
		__cfg80211_mlme_deauth(rdev, wdev->netdev, params->bssid,
				       NULL, 0,
				       WLAN_REASON_DEAUTH_LEAVING, false);
		/* return an error so that we call __cfg80211_connect_result() */
		return -EINVAL;
=======
		err = cfg80211_mlme_assoc(rdev, wdev->netdev, params->channel,
					  params->bssid, params->ssid,
					  params->ssid_len, &req);
		if (err)
			cfg80211_mlme_deauth(rdev, wdev->netdev, params->bssid,
					     NULL, 0,
					     WLAN_REASON_DEAUTH_LEAVING,
					     false);
		return err;
	case CFG80211_CONN_ASSOC_FAILED:
		cfg80211_mlme_deauth(rdev, wdev->netdev, params->bssid,
				     NULL, 0,
				     WLAN_REASON_DEAUTH_LEAVING, false);
		return -ENOTCONN;
	case CFG80211_CONN_DEAUTH:
		cfg80211_mlme_deauth(rdev, wdev->netdev, params->bssid,
				     NULL, 0,
				     WLAN_REASON_DEAUTH_LEAVING, false);
		/* fall through */
	case CFG80211_CONN_ABANDON:
		/* free directly, disconnected event already sent */
		cfg80211_sme_free(wdev);
		return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	default:
		return 0;
	}
}

void cfg80211_conn_work(struct work_struct *work)
{
	struct cfg80211_registered_device *rdev =
		container_of(work, struct cfg80211_registered_device, conn_work);
	struct wireless_dev *wdev;
	u8 bssid_buf[ETH_ALEN], *bssid = NULL;

	rtnl_lock();
<<<<<<< HEAD
	cfg80211_lock_rdev(rdev);
	mutex_lock(&rdev->devlist_mtx);
	mutex_lock(&rdev->sched_scan_mtx);

	list_for_each_entry(wdev, &rdev->wdev_list, list) {
=======

	list_for_each_entry(wdev, &rdev->wiphy.wdev_list, list) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (!wdev->netdev)
			continue;

		wdev_lock(wdev);
		if (!netif_running(wdev->netdev)) {
			wdev_unlock(wdev);
			continue;
		}
<<<<<<< HEAD
		if (wdev->sme_state != CFG80211_SME_CONNECTING || !wdev->conn) {
=======
		if (!wdev->conn ||
		    wdev->conn->state == CFG80211_CONN_CONNECTED) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			wdev_unlock(wdev);
			continue;
		}
		if (wdev->conn->params.bssid) {
			memcpy(bssid_buf, wdev->conn->params.bssid, ETH_ALEN);
			bssid = bssid_buf;
		}
<<<<<<< HEAD
		if (cfg80211_conn_do_work(wdev))
			__cfg80211_connect_result(
					wdev->netdev, bssid,
					NULL, 0, NULL, 0,
					WLAN_STATUS_UNSPECIFIED_FAILURE,
					false, NULL);
		wdev_unlock(wdev);
	}

	mutex_unlock(&rdev->sched_scan_mtx);
	mutex_unlock(&rdev->devlist_mtx);
	cfg80211_unlock_rdev(rdev);
	rtnl_unlock();
}

static struct cfg80211_bss *cfg80211_get_conn_bss(struct wireless_dev *wdev)
{
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	struct cfg80211_bss *bss;
	u16 capa = WLAN_CAPABILITY_ESS;

	ASSERT_WDEV_LOCK(wdev);

	if (wdev->conn->params.privacy)
		capa |= WLAN_CAPABILITY_PRIVACY;

=======
		if (cfg80211_conn_do_work(wdev)) {
			__cfg80211_connect_result(
					wdev->netdev, bssid,
					NULL, 0, NULL, 0, -1, false, NULL);
		}
		wdev_unlock(wdev);
	}

	rtnl_unlock();
}

/* Returned bss is reference counted and must be cleaned up appropriately. */
static struct cfg80211_bss *cfg80211_get_conn_bss(struct wireless_dev *wdev)
{
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
	struct cfg80211_bss *bss;

	ASSERT_WDEV_LOCK(wdev);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	bss = cfg80211_get_bss(wdev->wiphy, wdev->conn->params.channel,
			       wdev->conn->params.bssid,
			       wdev->conn->params.ssid,
			       wdev->conn->params.ssid_len,
<<<<<<< HEAD
			       WLAN_CAPABILITY_ESS | WLAN_CAPABILITY_PRIVACY,
			       capa);
=======
			       wdev->conn_bss_type,
			       IEEE80211_PRIVACY(wdev->conn->params.privacy));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!bss)
		return NULL;

	memcpy(wdev->conn->bssid, bss->bssid, ETH_ALEN);
	wdev->conn->params.bssid = wdev->conn->bssid;
	wdev->conn->params.channel = bss->channel;
	wdev->conn->state = CFG80211_CONN_AUTHENTICATE_NEXT;
	schedule_work(&rdev->conn_work);

	return bss;
}

static void __cfg80211_sme_scan_done(struct net_device *dev)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
<<<<<<< HEAD
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
=======
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct cfg80211_bss *bss;

	ASSERT_WDEV_LOCK(wdev);

<<<<<<< HEAD
	if (wdev->sme_state != CFG80211_SME_CONNECTING)
		return;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!wdev->conn)
		return;

	if (wdev->conn->state != CFG80211_CONN_SCANNING &&
	    wdev->conn->state != CFG80211_CONN_SCAN_AGAIN)
		return;

	bss = cfg80211_get_conn_bss(wdev);
<<<<<<< HEAD
	if (bss) {
		cfg80211_put_bss(&rdev->wiphy, bss);
	} else {
		/* not found */
		if (wdev->conn->state == CFG80211_CONN_SCAN_AGAIN)
			schedule_work(&rdev->conn_work);
		else
			__cfg80211_connect_result(
					wdev->netdev,
					wdev->conn->params.bssid,
					NULL, 0, NULL, 0,
					WLAN_STATUS_UNSPECIFIED_FAILURE,
					false, NULL);
	}
=======
	if (bss)
		cfg80211_put_bss(&rdev->wiphy, bss);
	else
		schedule_work(&rdev->conn_work);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void cfg80211_sme_scan_done(struct net_device *dev)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
<<<<<<< HEAD
	if(NULL == wdev)
	{
		return;
	}
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	wdev_lock(wdev);
	__cfg80211_sme_scan_done(dev);
	wdev_unlock(wdev);
}

<<<<<<< HEAD
void cfg80211_sme_rx_auth(struct net_device *dev,
			  const u8 *buf, size_t len)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);
=======
void cfg80211_sme_rx_auth(struct wireless_dev *wdev, const u8 *buf, size_t len)
{
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wiphy);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)buf;
	u16 status_code = le16_to_cpu(mgmt->u.auth.status_code);

	ASSERT_WDEV_LOCK(wdev);

<<<<<<< HEAD
	/* should only RX auth frames when connecting */
	if (wdev->sme_state != CFG80211_SME_CONNECTING)
		return;

	if (WARN_ON(!wdev->conn))
=======
	if (!wdev->conn || wdev->conn->state == CFG80211_CONN_CONNECTED)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return;

	if (status_code == WLAN_STATUS_NOT_SUPPORTED_AUTH_ALG &&
	    wdev->conn->auto_auth &&
	    wdev->conn->params.auth_type != NL80211_AUTHTYPE_NETWORK_EAP) {
		/* select automatically between only open, shared, leap */
		switch (wdev->conn->params.auth_type) {
		case NL80211_AUTHTYPE_OPEN_SYSTEM:
			if (wdev->connect_keys)
				wdev->conn->params.auth_type =
					NL80211_AUTHTYPE_SHARED_KEY;
			else
				wdev->conn->params.auth_type =
					NL80211_AUTHTYPE_NETWORK_EAP;
			break;
		case NL80211_AUTHTYPE_SHARED_KEY:
			wdev->conn->params.auth_type =
				NL80211_AUTHTYPE_NETWORK_EAP;
			break;
		default:
			/* huh? */
			wdev->conn->params.auth_type =
				NL80211_AUTHTYPE_OPEN_SYSTEM;
			break;
		}
		wdev->conn->state = CFG80211_CONN_AUTHENTICATE_NEXT;
		schedule_work(&rdev->conn_work);
	} else if (status_code != WLAN_STATUS_SUCCESS) {
<<<<<<< HEAD
		__cfg80211_connect_result(dev, mgmt->bssid, NULL, 0, NULL, 0,
					  status_code, false, NULL);
	} else if (wdev->sme_state == CFG80211_SME_CONNECTING &&
		 wdev->conn->state == CFG80211_CONN_AUTHENTICATING) {
=======
		__cfg80211_connect_result(wdev->netdev, mgmt->bssid,
					  NULL, 0, NULL, 0,
					  status_code, false, NULL);
	} else if (wdev->conn->state == CFG80211_CONN_AUTHENTICATING) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		wdev->conn->state = CFG80211_CONN_ASSOCIATE_NEXT;
		schedule_work(&rdev->conn_work);
	}
}

<<<<<<< HEAD
bool cfg80211_sme_failed_reassoc(struct wireless_dev *wdev)
{
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	if (WARN_ON(!wdev->conn))
		return false;

	if (!wdev->conn->prev_bssid_valid)
		return false;

	/*
	 * Some stupid APs don't accept reassoc, so we
	 * need to fall back to trying regular assoc.
	 */
	wdev->conn->prev_bssid_valid = false;
	wdev->conn->state = CFG80211_CONN_ASSOCIATE_NEXT;
	schedule_work(&rdev->conn_work);

	return true;
}

void cfg80211_sme_failed_assoc(struct wireless_dev *wdev)
{
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	wdev->conn->state = CFG80211_CONN_DEAUTH_ASSOC_FAIL;
	schedule_work(&rdev->conn_work);
}

void bss_dump(struct wiphy *wiphy)
{
    if(!wiphy)
       return;
    struct cfg80211_registered_device *dev = wiphy_to_dev(wiphy);
    struct cfg80211_internal_bss *bss;
    int count = 0;
    printk("in bss_dump\n");
    spin_lock_bh(&dev->bss_lock);
    list_for_each_entry(bss, &dev->bss_list, list)
    {
       printk(MACSTR,  MAC2STR(bss->pub.bssid));
       count++;
       if(count >= 100)
          break;
    }
    spin_unlock_bh(&dev->bss_lock);
}
void __cfg80211_connect_result(struct net_device *dev, const u8 *bssid,
			       const u8 *req_ie, size_t req_ie_len,
			       const u8 *resp_ie, size_t resp_ie_len,
			       u16 status, bool wextev,
=======
bool cfg80211_sme_rx_assoc_resp(struct wireless_dev *wdev, u16 status)
{
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);

	if (!wdev->conn)
		return false;

	if (status == WLAN_STATUS_SUCCESS) {
		wdev->conn->state = CFG80211_CONN_CONNECTED;
		return false;
	}

	if (wdev->conn->prev_bssid_valid) {
		/*
		 * Some stupid APs don't accept reassoc, so we
		 * need to fall back to trying regular assoc;
		 * return true so no event is sent to userspace.
		 */
		wdev->conn->prev_bssid_valid = false;
		wdev->conn->state = CFG80211_CONN_ASSOCIATE_NEXT;
		schedule_work(&rdev->conn_work);
		return true;
	}

	wdev->conn->state = CFG80211_CONN_ASSOC_FAILED;
	schedule_work(&rdev->conn_work);
	return false;
}

void cfg80211_sme_deauth(struct wireless_dev *wdev)
{
	cfg80211_sme_free(wdev);
}

void cfg80211_sme_auth_timeout(struct wireless_dev *wdev)
{
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);

	if (!wdev->conn)
		return;

	wdev->conn->state = CFG80211_CONN_AUTH_FAILED;
	schedule_work(&rdev->conn_work);
}

void cfg80211_sme_disassoc(struct wireless_dev *wdev)
{
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);

	if (!wdev->conn)
		return;

	wdev->conn->state = CFG80211_CONN_DEAUTH;
	schedule_work(&rdev->conn_work);
}

void cfg80211_sme_assoc_timeout(struct wireless_dev *wdev)
{
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);

	if (!wdev->conn)
		return;

	wdev->conn->state = CFG80211_CONN_ASSOC_FAILED;
	schedule_work(&rdev->conn_work);
}

void cfg80211_sme_abandon_assoc(struct wireless_dev *wdev)
{
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);

	if (!wdev->conn)
		return;

	wdev->conn->state = CFG80211_CONN_ABANDON;
	schedule_work(&rdev->conn_work);
}

static int cfg80211_sme_get_conn_ies(struct wireless_dev *wdev,
				     const u8 *ies, size_t ies_len,
				     const u8 **out_ies, size_t *out_ies_len)
{
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
	u8 *buf;
	size_t offs;

	if (!rdev->wiphy.extended_capabilities_len ||
	    (ies && cfg80211_find_ie(WLAN_EID_EXT_CAPABILITY, ies, ies_len))) {
		*out_ies = kmemdup(ies, ies_len, GFP_KERNEL);
		if (!*out_ies)
			return -ENOMEM;
		*out_ies_len = ies_len;
		return 0;
	}

	buf = kmalloc(ies_len + rdev->wiphy.extended_capabilities_len + 2,
		      GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	if (ies_len) {
		static const u8 before_extcapa[] = {
			/* not listing IEs expected to be created by driver */
			WLAN_EID_RSN,
			WLAN_EID_QOS_CAPA,
			WLAN_EID_RRM_ENABLED_CAPABILITIES,
			WLAN_EID_MOBILITY_DOMAIN,
			WLAN_EID_SUPPORTED_REGULATORY_CLASSES,
			WLAN_EID_BSS_COEX_2040,
		};

		offs = ieee80211_ie_split(ies, ies_len, before_extcapa,
					  ARRAY_SIZE(before_extcapa), 0);
		memcpy(buf, ies, offs);
		/* leave a whole for extended capabilities IE */
		memcpy(buf + offs + rdev->wiphy.extended_capabilities_len + 2,
		       ies + offs, ies_len - offs);
	} else {
		offs = 0;
	}

	/* place extended capabilities IE (with only driver capabilities) */
	buf[offs] = WLAN_EID_EXT_CAPABILITY;
	buf[offs + 1] = rdev->wiphy.extended_capabilities_len;
	memcpy(buf + offs + 2,
	       rdev->wiphy.extended_capabilities,
	       rdev->wiphy.extended_capabilities_len);

	*out_ies = buf;
	*out_ies_len = ies_len + rdev->wiphy.extended_capabilities_len + 2;

	return 0;
}

static int cfg80211_sme_connect(struct wireless_dev *wdev,
				struct cfg80211_connect_params *connect,
				const u8 *prev_bssid)
{
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
	struct cfg80211_bss *bss;
	int err;

	if (!rdev->ops->auth || !rdev->ops->assoc)
		return -EOPNOTSUPP;

	if (wdev->current_bss) {
		cfg80211_unhold_bss(wdev->current_bss);
		cfg80211_put_bss(wdev->wiphy, &wdev->current_bss->pub);
		wdev->current_bss = NULL;

		cfg80211_sme_free(wdev);
	}

	if (wdev->conn)
		return -EINPROGRESS;

	wdev->conn = kzalloc(sizeof(*wdev->conn), GFP_KERNEL);
	if (!wdev->conn)
		return -ENOMEM;

	/*
	 * Copy all parameters, and treat explicitly IEs, BSSID, SSID.
	 */
	memcpy(&wdev->conn->params, connect, sizeof(*connect));
	if (connect->bssid) {
		wdev->conn->params.bssid = wdev->conn->bssid;
		memcpy(wdev->conn->bssid, connect->bssid, ETH_ALEN);
	}

	if (cfg80211_sme_get_conn_ies(wdev, connect->ie, connect->ie_len,
				      &wdev->conn->ie,
				      &wdev->conn->params.ie_len)) {
		kfree(wdev->conn);
		wdev->conn = NULL;
		return -ENOMEM;
	}
	wdev->conn->params.ie = wdev->conn->ie;

	if (connect->auth_type == NL80211_AUTHTYPE_AUTOMATIC) {
		wdev->conn->auto_auth = true;
		/* start with open system ... should mostly work */
		wdev->conn->params.auth_type =
			NL80211_AUTHTYPE_OPEN_SYSTEM;
	} else {
		wdev->conn->auto_auth = false;
	}

	wdev->conn->params.ssid = wdev->ssid;
	wdev->conn->params.ssid_len = wdev->ssid_len;

	/* see if we have the bss already */
	bss = cfg80211_get_conn_bss(wdev);

	if (prev_bssid) {
		memcpy(wdev->conn->prev_bssid, prev_bssid, ETH_ALEN);
		wdev->conn->prev_bssid_valid = true;
	}

	/* we're good if we have a matching bss struct */
	if (bss) {
		err = cfg80211_conn_do_work(wdev);
		cfg80211_put_bss(wdev->wiphy, bss);
	} else {
		/* otherwise we'll need to scan for the AP first */
		err = cfg80211_conn_scan(wdev);

		/*
		 * If we can't scan right now, then we need to scan again
		 * after the current scan finished, since the parameters
		 * changed (unless we find a good AP anyway).
		 */
		if (err == -EBUSY) {
			err = 0;
			wdev->conn->state = CFG80211_CONN_SCAN_AGAIN;
		}
	}

	if (err)
		cfg80211_sme_free(wdev);

	return err;
}

static int cfg80211_sme_disconnect(struct wireless_dev *wdev, u16 reason)
{
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
	int err;

	if (!wdev->conn)
		return 0;

	if (!rdev->ops->deauth)
		return -EOPNOTSUPP;

	if (wdev->conn->state == CFG80211_CONN_SCANNING ||
	    wdev->conn->state == CFG80211_CONN_SCAN_AGAIN) {
		err = 0;
		goto out;
	}

	/* wdev->conn->params.bssid must be set if > SCANNING */
	err = cfg80211_mlme_deauth(rdev, wdev->netdev,
				   wdev->conn->params.bssid,
				   NULL, 0, reason, false);
 out:
	cfg80211_sme_free(wdev);
	return err;
}

/*
 * code shared for in-device and software SME
 */

static bool cfg80211_is_all_idle(void)
{
	struct cfg80211_registered_device *rdev;
	struct wireless_dev *wdev;
	bool is_all_idle = true;

	/*
	 * All devices must be idle as otherwise if you are actively
	 * scanning some new beacon hints could be learned and would
	 * count as new regulatory hints.
	 */
	list_for_each_entry(rdev, &cfg80211_rdev_list, list) {
		list_for_each_entry(wdev, &rdev->wiphy.wdev_list, list) {
			wdev_lock(wdev);
			if (wdev->conn || wdev->current_bss)
				is_all_idle = false;
			wdev_unlock(wdev);
		}
	}

	return is_all_idle;
}

static void disconnect_work(struct work_struct *work)
{
	rtnl_lock();
	if (cfg80211_is_all_idle())
		regulatory_hint_disconnect();
	rtnl_unlock();
}

static DECLARE_WORK(cfg80211_disconnect_work, disconnect_work);


/*
 * API calls for drivers implementing connect/disconnect and
 * SME event handling
 */

/* This method must consume bss one way or another */
void __cfg80211_connect_result(struct net_device *dev, const u8 *bssid,
			       const u8 *req_ie, size_t req_ie_len,
			       const u8 *resp_ie, size_t resp_ie_len,
			       int status, bool wextev,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			       struct cfg80211_bss *bss)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	const u8 *country_ie;
#ifdef CONFIG_CFG80211_WEXT
	union iwreq_data wrqu;
#endif
<<<<<<< HEAD
        if(bssid)
           printk("MAC:" MACSTR, MAC2STR(bssid));
        printk("cfg80211_connect_result status: %d\n", status);
	ASSERT_WDEV_LOCK(wdev);

	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_STATION &&
		    wdev->iftype != NL80211_IFTYPE_P2P_CLIENT))
		return;

	if (wdev->sme_state != CFG80211_SME_CONNECTING)
		return;

	nl80211_send_connect_result(wiphy_to_dev(wdev->wiphy), dev,
=======

	ASSERT_WDEV_LOCK(wdev);

	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_STATION &&
		    wdev->iftype != NL80211_IFTYPE_P2P_CLIENT)) {
		cfg80211_put_bss(wdev->wiphy, bss);
		return;
	}

	nl80211_send_connect_result(wiphy_to_rdev(wdev->wiphy), dev,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				    bssid, req_ie, req_ie_len,
				    resp_ie, resp_ie_len,
				    status, GFP_KERNEL);

#ifdef CONFIG_CFG80211_WEXT
	if (wextev) {
		if (req_ie && status == WLAN_STATUS_SUCCESS) {
			memset(&wrqu, 0, sizeof(wrqu));
			wrqu.data.length = req_ie_len;
			wireless_send_event(dev, IWEVASSOCREQIE, &wrqu, req_ie);
		}

		if (resp_ie && status == WLAN_STATUS_SUCCESS) {
			memset(&wrqu, 0, sizeof(wrqu));
			wrqu.data.length = resp_ie_len;
			wireless_send_event(dev, IWEVASSOCRESPIE, &wrqu, resp_ie);
		}

		memset(&wrqu, 0, sizeof(wrqu));
		wrqu.ap_addr.sa_family = ARPHRD_ETHER;
		if (bssid && status == WLAN_STATUS_SUCCESS) {
			memcpy(wrqu.ap_addr.sa_data, bssid, ETH_ALEN);
			memcpy(wdev->wext.prev_bssid, bssid, ETH_ALEN);
			wdev->wext.prev_bssid_valid = true;
		}
		wireless_send_event(dev, SIOCGIWAP, &wrqu, NULL);
	}
#endif

<<<<<<< HEAD
=======
	if (!bss && (status == WLAN_STATUS_SUCCESS)) {
		WARN_ON_ONCE(!wiphy_to_rdev(wdev->wiphy)->ops->connect);
		bss = cfg80211_get_bss(wdev->wiphy, NULL, bssid,
				       wdev->ssid, wdev->ssid_len,
				       wdev->conn_bss_type,
				       IEEE80211_PRIVACY_ANY);
		if (bss)
			cfg80211_hold_bss(bss_from_pub(bss));
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (wdev->current_bss) {
		cfg80211_unhold_bss(wdev->current_bss);
		cfg80211_put_bss(wdev->wiphy, &wdev->current_bss->pub);
		wdev->current_bss = NULL;
	}

<<<<<<< HEAD
	if (wdev->conn)
		wdev->conn->state = CFG80211_CONN_IDLE;

	if (status != WLAN_STATUS_SUCCESS) {
		wdev->sme_state = CFG80211_SME_IDLE;
		if (wdev->conn)
			kfree(wdev->conn->ie);
		kfree(wdev->conn);
		wdev->conn = NULL;
		kfree(wdev->connect_keys);
		wdev->connect_keys = NULL;
		wdev->ssid_len = 0;
		cfg80211_put_bss(wdev->wiphy, bss);
		return;
	}

	if (!bss)
		bss = cfg80211_get_bss(wdev->wiphy,
				       wdev->conn ? wdev->conn->params.channel :
				       NULL,
				       bssid,
				       wdev->ssid, wdev->ssid_len,
				       WLAN_CAPABILITY_ESS,
				       WLAN_CAPABILITY_ESS);
        printk("dump_ap info\n"); 
        bss_dump(wdev->wiphy);
	if (WARN_ON(!bss))
        {
	    bss_dump(wdev->wiphy);
		return;
        }
	cfg80211_hold_bss(bss_from_pub(bss));
	wdev->current_bss = bss_from_pub(bss);

	wdev->sme_state = CFG80211_SME_CONNECTED;
	cfg80211_upload_connect_keys(wdev);
=======
	if (status != WLAN_STATUS_SUCCESS) {
		kzfree(wdev->connect_keys);
		wdev->connect_keys = NULL;
		wdev->ssid_len = 0;
		if (bss) {
			cfg80211_unhold_bss(bss_from_pub(bss));
			cfg80211_put_bss(wdev->wiphy, bss);
		}
		cfg80211_sme_free(wdev);
		return;
	}

	if (WARN_ON(!bss))
		return;

	wdev->current_bss = bss_from_pub(bss);

	if (!(wdev->wiphy->flags & WIPHY_FLAG_HAS_STATIC_WEP))
		cfg80211_upload_connect_keys(wdev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	rcu_read_lock();
	country_ie = ieee80211_bss_get_ie(bss, WLAN_EID_COUNTRY);
	if (!country_ie) {
		rcu_read_unlock();
		return;
	}

	country_ie = kmemdup(country_ie, 2 + country_ie[1], GFP_ATOMIC);
	rcu_read_unlock();

	if (!country_ie)
		return;

	/*
	 * ieee80211_bss_get_ie() ensures we can access:
	 * - country_ie + 2, the start of the country ie data, and
	 * - and country_ie[1] which is the IE length
	 */
<<<<<<< HEAD
	regulatory_hint_11d(wdev->wiphy, bss->channel->band,
			    country_ie + 2, country_ie[1]);
	kfree(country_ie);
}

void cfg80211_connect_result(struct net_device *dev, const u8 *bssid,
			     const u8 *req_ie, size_t req_ie_len,
			     const u8 *resp_ie, size_t resp_ie_len,
			     u16 status, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	struct cfg80211_event *ev;
	unsigned long flags;

	CFG80211_DEV_WARN_ON(wdev->sme_state != CFG80211_SME_CONNECTING);

	ev = kzalloc(sizeof(*ev) + req_ie_len + resp_ie_len, gfp);
	if (!ev)
		return;
=======
	regulatory_hint_country_ie(wdev->wiphy, bss->channel->band,
				   country_ie + 2, country_ie[1]);
	kfree(country_ie);
}

/* Consumes bss object one way or another */
void cfg80211_connect_bss(struct net_device *dev, const u8 *bssid,
			  struct cfg80211_bss *bss, const u8 *req_ie,
			  size_t req_ie_len, const u8 *resp_ie,
			  size_t resp_ie_len, int status, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
	struct cfg80211_event *ev;
	unsigned long flags;

	if (bss) {
		/* Make sure the bss entry provided by the driver is valid. */
		struct cfg80211_internal_bss *ibss = bss_from_pub(bss);

		if (WARN_ON(list_empty(&ibss->list))) {
			cfg80211_put_bss(wdev->wiphy, bss);
			return;
		}
	}

	ev = kzalloc(sizeof(*ev) + req_ie_len + resp_ie_len, gfp);
	if (!ev) {
		cfg80211_put_bss(wdev->wiphy, bss);
		return;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ev->type = EVENT_CONNECT_RESULT;
	if (bssid)
		memcpy(ev->cr.bssid, bssid, ETH_ALEN);
	if (req_ie_len) {
		ev->cr.req_ie = ((u8 *)ev) + sizeof(*ev);
		ev->cr.req_ie_len = req_ie_len;
		memcpy((void *)ev->cr.req_ie, req_ie, req_ie_len);
	}
	if (resp_ie_len) {
		ev->cr.resp_ie = ((u8 *)ev) + sizeof(*ev) + req_ie_len;
		ev->cr.resp_ie_len = resp_ie_len;
		memcpy((void *)ev->cr.resp_ie, resp_ie, resp_ie_len);
	}
<<<<<<< HEAD
=======
	if (bss)
		cfg80211_hold_bss(bss_from_pub(bss));
	ev->cr.bss = bss;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ev->cr.status = status;

	spin_lock_irqsave(&wdev->event_lock, flags);
	list_add_tail(&ev->list, &wdev->event_list);
	spin_unlock_irqrestore(&wdev->event_lock, flags);
	queue_work(cfg80211_wq, &rdev->event_work);
}
<<<<<<< HEAD
EXPORT_SYMBOL(cfg80211_connect_result);

=======
EXPORT_SYMBOL(cfg80211_connect_bss);

/* Consumes bss object one way or another */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void __cfg80211_roamed(struct wireless_dev *wdev,
		       struct cfg80211_bss *bss,
		       const u8 *req_ie, size_t req_ie_len,
		       const u8 *resp_ie, size_t resp_ie_len)
{
#ifdef CONFIG_CFG80211_WEXT
	union iwreq_data wrqu;
#endif
	ASSERT_WDEV_LOCK(wdev);

	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_STATION &&
		    wdev->iftype != NL80211_IFTYPE_P2P_CLIENT))
		goto out;

<<<<<<< HEAD
	if (wdev->sme_state != CFG80211_SME_CONNECTED)
		goto out;

	/* internal error -- how did we get to CONNECTED w/o BSS? */
	if (WARN_ON(!wdev->current_bss)) {
		goto out;
	}
=======
	if (WARN_ON(!wdev->current_bss))
		goto out;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	cfg80211_unhold_bss(wdev->current_bss);
	cfg80211_put_bss(wdev->wiphy, &wdev->current_bss->pub);
	wdev->current_bss = NULL;

	cfg80211_hold_bss(bss_from_pub(bss));
	wdev->current_bss = bss_from_pub(bss);

<<<<<<< HEAD
	nl80211_send_roamed(wiphy_to_dev(wdev->wiphy), wdev->netdev, bss->bssid,
=======
	nl80211_send_roamed(wiphy_to_rdev(wdev->wiphy),
			    wdev->netdev, bss->bssid,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			    req_ie, req_ie_len, resp_ie, resp_ie_len,
			    GFP_KERNEL);

#ifdef CONFIG_CFG80211_WEXT
	if (req_ie) {
		memset(&wrqu, 0, sizeof(wrqu));
		wrqu.data.length = req_ie_len;
		wireless_send_event(wdev->netdev, IWEVASSOCREQIE,
				    &wrqu, req_ie);
	}

	if (resp_ie) {
		memset(&wrqu, 0, sizeof(wrqu));
		wrqu.data.length = resp_ie_len;
		wireless_send_event(wdev->netdev, IWEVASSOCRESPIE,
				    &wrqu, resp_ie);
	}

	memset(&wrqu, 0, sizeof(wrqu));
	wrqu.ap_addr.sa_family = ARPHRD_ETHER;
	memcpy(wrqu.ap_addr.sa_data, bss->bssid, ETH_ALEN);
	memcpy(wdev->wext.prev_bssid, bss->bssid, ETH_ALEN);
	wdev->wext.prev_bssid_valid = true;
	wireless_send_event(wdev->netdev, SIOCGIWAP, &wrqu, NULL);
#endif

	return;
out:
	cfg80211_put_bss(wdev->wiphy, bss);
}

void cfg80211_roamed(struct net_device *dev,
		     struct ieee80211_channel *channel,
		     const u8 *bssid,
		     const u8 *req_ie, size_t req_ie_len,
		     const u8 *resp_ie, size_t resp_ie_len, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_bss *bss;

<<<<<<< HEAD
	CFG80211_DEV_WARN_ON(wdev->sme_state != CFG80211_SME_CONNECTED);

	bss = cfg80211_get_bss(wdev->wiphy, channel, bssid, wdev->ssid,
			       wdev->ssid_len, WLAN_CAPABILITY_ESS,
			       WLAN_CAPABILITY_ESS);
=======
	bss = cfg80211_get_bss(wdev->wiphy, channel, bssid, wdev->ssid,
			       wdev->ssid_len,
			       wdev->conn_bss_type, IEEE80211_PRIVACY_ANY);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (WARN_ON(!bss))
		return;

	cfg80211_roamed_bss(dev, bss, req_ie, req_ie_len, resp_ie,
			    resp_ie_len, gfp);
}
EXPORT_SYMBOL(cfg80211_roamed);

<<<<<<< HEAD
=======
/* Consumes bss object one way or another */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void cfg80211_roamed_bss(struct net_device *dev,
			 struct cfg80211_bss *bss, const u8 *req_ie,
			 size_t req_ie_len, const u8 *resp_ie,
			 size_t resp_ie_len, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
<<<<<<< HEAD
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	struct cfg80211_event *ev;
	unsigned long flags;

	CFG80211_DEV_WARN_ON(wdev->sme_state != CFG80211_SME_CONNECTED);

=======
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
	struct cfg80211_event *ev;
	unsigned long flags;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (WARN_ON(!bss))
		return;

	ev = kzalloc(sizeof(*ev) + req_ie_len + resp_ie_len, gfp);
	if (!ev) {
		cfg80211_put_bss(wdev->wiphy, bss);
		return;
	}

	ev->type = EVENT_ROAMED;
	ev->rm.req_ie = ((u8 *)ev) + sizeof(*ev);
	ev->rm.req_ie_len = req_ie_len;
	memcpy((void *)ev->rm.req_ie, req_ie, req_ie_len);
	ev->rm.resp_ie = ((u8 *)ev) + sizeof(*ev) + req_ie_len;
	ev->rm.resp_ie_len = resp_ie_len;
	memcpy((void *)ev->rm.resp_ie, resp_ie, resp_ie_len);
	ev->rm.bss = bss;

	spin_lock_irqsave(&wdev->event_lock, flags);
	list_add_tail(&ev->list, &wdev->event_list);
	spin_unlock_irqrestore(&wdev->event_lock, flags);
	queue_work(cfg80211_wq, &rdev->event_work);
}
EXPORT_SYMBOL(cfg80211_roamed_bss);

void __cfg80211_disconnected(struct net_device *dev, const u8 *ie,
			     size_t ie_len, u16 reason, bool from_ap)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
<<<<<<< HEAD
	struct cfg80211_registered_device *rdev = NULL;
=======
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int i;
#ifdef CONFIG_CFG80211_WEXT
	union iwreq_data wrqu;
#endif
<<<<<<< HEAD
	if(NULL == wdev)
	{
		return;
	}

	ASSERT_WDEV_LOCK(wdev);

	rdev = wiphy_to_dev(wdev->wiphy);
=======

	ASSERT_WDEV_LOCK(wdev);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_STATION &&
		    wdev->iftype != NL80211_IFTYPE_P2P_CLIENT))
		return;

<<<<<<< HEAD
#ifndef CONFIG_CFG80211_ALLOW_RECONNECT
	if (wdev->sme_state != CFG80211_SME_CONNECTED)
		return;
#endif

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (wdev->current_bss) {
		cfg80211_unhold_bss(wdev->current_bss);
		cfg80211_put_bss(wdev->wiphy, &wdev->current_bss->pub);
	}

	wdev->current_bss = NULL;
<<<<<<< HEAD
	wdev->sme_state = CFG80211_SME_IDLE;
	wdev->ssid_len = 0;

	if (wdev->conn) {
		kfree(wdev->conn->ie);
		wdev->conn->ie = NULL;
		kfree(wdev->conn);
		wdev->conn = NULL;
	}

	nl80211_send_disconnected(rdev, dev, reason, ie, ie_len, from_ap);

=======
	wdev->ssid_len = 0;

	nl80211_send_disconnected(rdev, dev, reason, ie, ie_len, from_ap);

	/* stop critical protocol if supported */
	if (rdev->ops->crit_proto_stop && rdev->crit_proto_nlportid) {
		rdev->crit_proto_nlportid = 0;
		rdev_crit_proto_stop(rdev, wdev);
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * Delete all the keys ... pairwise keys can't really
	 * exist any more anyway, but default keys might.
	 */
	if (rdev->ops->del_key)
		for (i = 0; i < 6; i++)
			rdev_del_key(rdev, dev, i, false, NULL);

<<<<<<< HEAD
=======
	rdev_set_qos_map(rdev, dev, NULL);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_CFG80211_WEXT
	memset(&wrqu, 0, sizeof(wrqu));
	wrqu.ap_addr.sa_family = ARPHRD_ETHER;
	wireless_send_event(dev, SIOCGIWAP, &wrqu, NULL);
	wdev->wext.connect.ssid_len = 0;
#endif

	schedule_work(&cfg80211_disconnect_work);
}

void cfg80211_disconnected(struct net_device *dev, u16 reason,
<<<<<<< HEAD
			   u8 *ie, size_t ie_len, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	struct cfg80211_event *ev;
	unsigned long flags;

	CFG80211_DEV_WARN_ON(wdev->sme_state != CFG80211_SME_CONNECTED);

=======
			   const u8 *ie, size_t ie_len,
			   bool locally_generated, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
	struct cfg80211_event *ev;
	unsigned long flags;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ev = kzalloc(sizeof(*ev) + ie_len, gfp);
	if (!ev)
		return;

	ev->type = EVENT_DISCONNECTED;
	ev->dc.ie = ((u8 *)ev) + sizeof(*ev);
	ev->dc.ie_len = ie_len;
	memcpy((void *)ev->dc.ie, ie, ie_len);
	ev->dc.reason = reason;
<<<<<<< HEAD
=======
	ev->dc.locally_generated = locally_generated;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	spin_lock_irqsave(&wdev->event_lock, flags);
	list_add_tail(&ev->list, &wdev->event_list);
	spin_unlock_irqrestore(&wdev->event_lock, flags);
	queue_work(cfg80211_wq, &rdev->event_work);
}
EXPORT_SYMBOL(cfg80211_disconnected);

<<<<<<< HEAD
void cfg80211_drv_recovery(struct net_device *dev, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	struct cfg80211_event *ev;
	unsigned long flags;

	ev = kzalloc(sizeof(*ev), gfp);
	if (!ev) {
		printk(KERN_ERR "%s: malloc fail\n", __FUNCTION__);
		return;
	}

	ev->type = EVENT_DRV_RECOVERY;
	spin_lock_irqsave(&wdev->event_lock, flags);
	list_add_tail(&ev->list, &wdev->event_list);
	spin_unlock_irqrestore(&wdev->event_lock, flags);
	queue_work(cfg80211_wq, &rdev->event_work);
}
EXPORT_SYMBOL(cfg80211_drv_recovery);

int __cfg80211_connect(struct cfg80211_registered_device *rdev,
		       struct net_device *dev,
		       struct cfg80211_connect_params *connect,
		       struct cfg80211_cached_keys *connkeys,
		       const u8 *prev_bssid)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_bss *bss = NULL;
=======
/*
 * API calls for nl80211/wext compatibility code
 */
int cfg80211_connect(struct cfg80211_registered_device *rdev,
		     struct net_device *dev,
		     struct cfg80211_connect_params *connect,
		     struct cfg80211_cached_keys *connkeys,
		     const u8 *prev_bssid)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int err;

	ASSERT_WDEV_LOCK(wdev);

<<<<<<< HEAD
#ifndef CONFIG_CFG80211_ALLOW_RECONNECT
	if (wdev->sme_state != CFG80211_SME_IDLE)
		return -EALREADY;

	if (WARN_ON(wdev->connect_keys)) {
#else
	if (wdev->connect_keys) {
#endif
		kfree(wdev->connect_keys);
		wdev->connect_keys = NULL;
	}

=======
	/*
	 * If we have an ssid_len, we're trying to connect or are
	 * already connected, so reject a new SSID unless it's the
	 * same (which is the case for re-association.)
	 */
	if (wdev->ssid_len &&
	    (wdev->ssid_len != connect->ssid_len ||
	     memcmp(wdev->ssid, connect->ssid, wdev->ssid_len)))
		return -EALREADY;

	/*
	 * If connected, reject (re-)association unless prev_bssid
	 * matches the current BSSID.
	 */
	if (wdev->current_bss) {
		if (!prev_bssid)
			return -EALREADY;
		if (!ether_addr_equal(prev_bssid, wdev->current_bss->pub.bssid))
			return -ENOTCONN;
	}

	/*
	 * Reject if we're in the process of connecting with WEP,
	 * this case isn't very interesting and trying to handle
	 * it would make the code much more complex.
	 */
	if (wdev->connect_keys)
		return -EINPROGRESS;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	cfg80211_oper_and_ht_capa(&connect->ht_capa_mask,
				  rdev->wiphy.ht_capa_mod_mask);

	if (connkeys && connkeys->def >= 0) {
		int idx;
		u32 cipher;

		idx = connkeys->def;
		cipher = connkeys->params[idx].cipher;
		/* If given a WEP key we may need it for shared key auth */
		if (cipher == WLAN_CIPHER_SUITE_WEP40 ||
		    cipher == WLAN_CIPHER_SUITE_WEP104) {
			connect->key_idx = idx;
			connect->key = connkeys->params[idx].key;
			connect->key_len = connkeys->params[idx].key_len;

			/*
			 * If ciphers are not set (e.g. when going through
			 * iwconfig), we have to set them appropriately here.
			 */
			if (connect->crypto.cipher_group == 0)
				connect->crypto.cipher_group = cipher;

			if (connect->crypto.n_ciphers_pairwise == 0) {
				connect->crypto.n_ciphers_pairwise = 1;
				connect->crypto.ciphers_pairwise[0] = cipher;
			}
		}
<<<<<<< HEAD
	}

	if (!rdev->ops->connect) {
		if (!rdev->ops->auth || !rdev->ops->assoc)
			return -EOPNOTSUPP;

		if (WARN_ON(wdev->conn))
			return -EINPROGRESS;

		wdev->conn = kzalloc(sizeof(*wdev->conn), GFP_KERNEL);
		if (!wdev->conn)
			return -ENOMEM;

		/*
		 * Copy all parameters, and treat explicitly IEs, BSSID, SSID.
		 */
		memcpy(&wdev->conn->params, connect, sizeof(*connect));
		if (connect->bssid) {
			wdev->conn->params.bssid = wdev->conn->bssid;
			memcpy(wdev->conn->bssid, connect->bssid, ETH_ALEN);
		}

		if (connect->ie) {
			wdev->conn->ie = kmemdup(connect->ie, connect->ie_len,
						GFP_KERNEL);
			wdev->conn->params.ie = wdev->conn->ie;
			if (!wdev->conn->ie) {
				kfree(wdev->conn);
				wdev->conn = NULL;
				return -ENOMEM;
			}
		}

		if (connect->auth_type == NL80211_AUTHTYPE_AUTOMATIC) {
			wdev->conn->auto_auth = true;
			/* start with open system ... should mostly work */
			wdev->conn->params.auth_type =
				NL80211_AUTHTYPE_OPEN_SYSTEM;
		} else {
			wdev->conn->auto_auth = false;
		}

		memcpy(wdev->ssid, connect->ssid, connect->ssid_len);
		wdev->ssid_len = connect->ssid_len;
		wdev->conn->params.ssid = wdev->ssid;
		wdev->conn->params.ssid_len = connect->ssid_len;

		/* see if we have the bss already */
		bss = cfg80211_get_conn_bss(wdev);

		wdev->sme_state = CFG80211_SME_CONNECTING;
		wdev->connect_keys = connkeys;

		if (prev_bssid) {
			memcpy(wdev->conn->prev_bssid, prev_bssid, ETH_ALEN);
			wdev->conn->prev_bssid_valid = true;
		}

		/* we're good if we have a matching bss struct */
		if (bss) {
			wdev->conn->state = CFG80211_CONN_AUTHENTICATE_NEXT;
			err = cfg80211_conn_do_work(wdev);
			cfg80211_put_bss(wdev->wiphy, bss);
		} else {
			/* otherwise we'll need to scan for the AP first */
			err = cfg80211_conn_scan(wdev);
			/*
			 * If we can't scan right now, then we need to scan again
			 * after the current scan finished, since the parameters
			 * changed (unless we find a good AP anyway).
			 */
			if (err == -EBUSY) {
				err = 0;
				wdev->conn->state = CFG80211_CONN_SCAN_AGAIN;
			}
		}
		if (err) {
			kfree(wdev->conn->ie);
			kfree(wdev->conn);
			wdev->conn = NULL;
			wdev->sme_state = CFG80211_SME_IDLE;
			wdev->connect_keys = NULL;
			wdev->ssid_len = 0;
		}

		return err;
	} else {
		wdev->sme_state = CFG80211_SME_CONNECTING;
		wdev->connect_keys = connkeys;
		err = rdev_connect(rdev, dev, connect);
		if (err) {
			wdev->connect_keys = NULL;
			wdev->sme_state = CFG80211_SME_IDLE;
			return err;
		}

		memcpy(wdev->ssid, connect->ssid, connect->ssid_len);
		wdev->ssid_len = connect->ssid_len;

		return 0;
	}
}

int cfg80211_connect(struct cfg80211_registered_device *rdev,
		     struct net_device *dev,
		     struct cfg80211_connect_params *connect,
		     struct cfg80211_cached_keys *connkeys)
{
	int err;

	mutex_lock(&rdev->devlist_mtx);
	/* might request scan - scan_mtx -> wdev_mtx dependency */
	mutex_lock(&rdev->sched_scan_mtx);
	wdev_lock(dev->ieee80211_ptr);
	err = __cfg80211_connect(rdev, dev, connect, connkeys, NULL);
	wdev_unlock(dev->ieee80211_ptr);
	mutex_unlock(&rdev->sched_scan_mtx);
	mutex_unlock(&rdev->devlist_mtx);

	return err;
}

int __cfg80211_disconnect(struct cfg80211_registered_device *rdev,
			  struct net_device *dev, u16 reason, bool wextev)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	int err;

	ASSERT_WDEV_LOCK(wdev);

	if (wdev->sme_state == CFG80211_SME_IDLE)
		return -EINVAL;

	kfree(wdev->connect_keys);
	wdev->connect_keys = NULL;

	if (!rdev->ops->disconnect) {
		if (!rdev->ops->deauth)
			return -EOPNOTSUPP;

		/* was it connected by userspace SME? */
		if (!wdev->conn) {
			cfg80211_mlme_down(rdev, dev);
			goto disconnect;
		}

		if (wdev->sme_state == CFG80211_SME_CONNECTING &&
		    (wdev->conn->state == CFG80211_CONN_SCANNING ||
		     wdev->conn->state == CFG80211_CONN_SCAN_AGAIN)) {
			wdev->sme_state = CFG80211_SME_IDLE;
			kfree(wdev->conn->ie);
			kfree(wdev->conn);
			wdev->conn = NULL;
			wdev->ssid_len = 0;
			return 0;
		}

		/* wdev->conn->params.bssid must be set if > SCANNING */
		err = __cfg80211_mlme_deauth(rdev, dev,
					     wdev->conn->params.bssid,
					     NULL, 0, reason, false);
		if (err)
			return err;
	} else {
		err = rdev_disconnect(rdev, dev, reason);
		if (err)
			return err;
	}

 disconnect:
	if (wdev->sme_state == CFG80211_SME_CONNECTED)
		__cfg80211_disconnected(dev, NULL, 0, 0, false);
	else if (wdev->sme_state == CFG80211_SME_CONNECTING)
		__cfg80211_connect_result(dev, NULL, NULL, 0, NULL, 0,
					  WLAN_STATUS_UNSPECIFIED_FAILURE,
					  wextev, NULL);
=======

		connect->crypto.wep_keys = connkeys->params;
		connect->crypto.wep_tx_key = connkeys->def;
	} else {
		if (WARN_ON(connkeys))
			return -EINVAL;
	}

	wdev->connect_keys = connkeys;
	memcpy(wdev->ssid, connect->ssid, connect->ssid_len);
	wdev->ssid_len = connect->ssid_len;

	wdev->conn_bss_type = connect->pbss ? IEEE80211_BSS_TYPE_PBSS :
					      IEEE80211_BSS_TYPE_ESS;

	if (!rdev->ops->connect)
		err = cfg80211_sme_connect(wdev, connect, prev_bssid);
	else
		err = rdev_connect(rdev, dev, connect);

	if (err) {
		wdev->connect_keys = NULL;
		/*
		 * This could be reassoc getting refused, don't clear
		 * ssid_len in that case.
		 */
		if (!wdev->current_bss)
			wdev->ssid_len = 0;
		return err;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

int cfg80211_disconnect(struct cfg80211_registered_device *rdev,
<<<<<<< HEAD
			struct net_device *dev,
			u16 reason, bool wextev)
{
	int err;

	wdev_lock(dev->ieee80211_ptr);
	err = __cfg80211_disconnect(rdev, dev, reason, wextev);
	wdev_unlock(dev->ieee80211_ptr);

	return err;
}

void cfg80211_sme_disassoc(struct net_device *dev,
			   struct cfg80211_internal_bss *bss)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	u8 bssid[ETH_ALEN];

	ASSERT_WDEV_LOCK(wdev);

	if (!wdev->conn)
		return;

	if (wdev->conn->state == CFG80211_CONN_IDLE)
		return;

	/*
	 * Ok, so the association was made by this SME -- we don't
	 * want it any more so deauthenticate too.
	 */

	memcpy(bssid, bss->pub.bssid, ETH_ALEN);

	__cfg80211_mlme_deauth(rdev, dev, bssid, NULL, 0,
			       WLAN_REASON_DEAUTH_LEAVING, false);
=======
			struct net_device *dev, u16 reason, bool wextev)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	int err = 0;

	ASSERT_WDEV_LOCK(wdev);

	kzfree(wdev->connect_keys);
	wdev->connect_keys = NULL;

	if (wdev->conn)
		err = cfg80211_sme_disconnect(wdev, reason);
	else if (!rdev->ops->disconnect)
		cfg80211_mlme_down(rdev, dev);
	else if (wdev->current_bss)
		err = rdev_disconnect(rdev, dev, reason);

	/*
	 * Clear ssid_len unless we actually were fully connected,
	 * in which case cfg80211_disconnected() will take care of
	 * this later.
	 */
	if (!wdev->current_bss)
		wdev->ssid_len = 0;

	return err;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
