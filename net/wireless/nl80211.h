#ifndef __NET_WIRELESS_NL80211_H
#define __NET_WIRELESS_NL80211_H

#include "core.h"

int nl80211_init(void);
void nl80211_exit(void);
<<<<<<< HEAD
void nl80211_notify_dev_rename(struct cfg80211_registered_device *rdev);
=======
void nl80211_notify_wiphy(struct cfg80211_registered_device *rdev,
			  enum nl80211_commands cmd);
void nl80211_notify_iface(struct cfg80211_registered_device *rdev,
			  struct wireless_dev *wdev,
			  enum nl80211_commands cmd);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void nl80211_send_scan_start(struct cfg80211_registered_device *rdev,
			     struct wireless_dev *wdev);
struct sk_buff *nl80211_build_scan_msg(struct cfg80211_registered_device *rdev,
				       struct wireless_dev *wdev, bool aborted);
void nl80211_send_scan_result(struct cfg80211_registered_device *rdev,
			      struct sk_buff *msg);
void nl80211_send_sched_scan(struct cfg80211_registered_device *rdev,
			     struct net_device *netdev, u32 cmd);
void nl80211_send_sched_scan_results(struct cfg80211_registered_device *rdev,
				     struct net_device *netdev);
<<<<<<< HEAD
void nl80211_send_reg_change_event(struct regulatory_request *request);
=======
void nl80211_common_reg_change_event(enum nl80211_commands cmd_id,
				     struct regulatory_request *request);

static inline void
nl80211_send_reg_change_event(struct regulatory_request *request)
{
	nl80211_common_reg_change_event(NL80211_CMD_REG_CHANGE, request);
}

static inline void
nl80211_send_wiphy_reg_change_event(struct regulatory_request *request)
{
	nl80211_common_reg_change_event(NL80211_CMD_WIPHY_REG_CHANGE, request);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void nl80211_send_rx_auth(struct cfg80211_registered_device *rdev,
			  struct net_device *netdev,
			  const u8 *buf, size_t len, gfp_t gfp);
void nl80211_send_rx_assoc(struct cfg80211_registered_device *rdev,
			   struct net_device *netdev,
<<<<<<< HEAD
			   const u8 *buf, size_t len, gfp_t gfp);
=======
			   const u8 *buf, size_t len, gfp_t gfp,
			   int uapsd_queues);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void nl80211_send_deauth(struct cfg80211_registered_device *rdev,
			 struct net_device *netdev,
			 const u8 *buf, size_t len, gfp_t gfp);
void nl80211_send_disassoc(struct cfg80211_registered_device *rdev,
			   struct net_device *netdev,
			   const u8 *buf, size_t len, gfp_t gfp);
void nl80211_send_auth_timeout(struct cfg80211_registered_device *rdev,
			       struct net_device *netdev,
			       const u8 *addr, gfp_t gfp);
void nl80211_send_assoc_timeout(struct cfg80211_registered_device *rdev,
				struct net_device *netdev,
				const u8 *addr, gfp_t gfp);
void nl80211_send_connect_result(struct cfg80211_registered_device *rdev,
				 struct net_device *netdev, const u8 *bssid,
				 const u8 *req_ie, size_t req_ie_len,
				 const u8 *resp_ie, size_t resp_ie_len,
<<<<<<< HEAD
				 u16 status, gfp_t gfp);
=======
				 int status, gfp_t gfp);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void nl80211_send_roamed(struct cfg80211_registered_device *rdev,
			 struct net_device *netdev, const u8 *bssid,
			 const u8 *req_ie, size_t req_ie_len,
			 const u8 *resp_ie, size_t resp_ie_len, gfp_t gfp);
void nl80211_send_disconnected(struct cfg80211_registered_device *rdev,
			       struct net_device *netdev, u16 reason,
			       const u8 *ie, size_t ie_len, bool from_ap);

void
nl80211_michael_mic_failure(struct cfg80211_registered_device *rdev,
			    struct net_device *netdev, const u8 *addr,
			    enum nl80211_key_type key_type,
			    int key_id, const u8 *tsc, gfp_t gfp);

void
nl80211_send_beacon_hint_event(struct wiphy *wiphy,
			       struct ieee80211_channel *channel_before,
			       struct ieee80211_channel *channel_after);

void nl80211_send_ibss_bssid(struct cfg80211_registered_device *rdev,
			     struct net_device *netdev, const u8 *bssid,
			     gfp_t gfp);

int nl80211_send_mgmt(struct cfg80211_registered_device *rdev,
		      struct wireless_dev *wdev, u32 nlpid,
		      int freq, int sig_dbm,
<<<<<<< HEAD
		      const u8 *buf, size_t len, gfp_t gfp);

void
nl80211_radar_notify(struct cfg80211_registered_device *rdev,
		     struct cfg80211_chan_def *chandef,
		     enum nl80211_radar_event event,
		     struct net_device *netdev, gfp_t gfp);

void
cfg80211_do_drv_recovery(struct net_device *dev, gfp_t gfp);
=======
		      const u8 *buf, size_t len, u32 flags, gfp_t gfp);

void
nl80211_radar_notify(struct cfg80211_registered_device *rdev,
		     const struct cfg80211_chan_def *chandef,
		     enum nl80211_radar_event event,
		     struct net_device *netdev, gfp_t gfp);

void nl80211_send_ap_stopped(struct wireless_dev *wdev);

void cfg80211_rdev_free_coalesce(struct cfg80211_registered_device *rdev);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* __NET_WIRELESS_NL80211_H */
