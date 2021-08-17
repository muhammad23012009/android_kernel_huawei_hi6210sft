/*
 * Wireless configuration interface internals.
 *
 * Copyright 2006-2010	Johannes Berg <johannes@sipsolutions.net>
 */
#ifndef __NET_WIRELESS_CORE_H
#define __NET_WIRELESS_CORE_H
<<<<<<< HEAD
#include <linux/mutex.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/list.h>
#include <linux/netdevice.h>
#include <linux/rbtree.h>
#include <linux/debugfs.h>
#include <linux/rfkill.h>
#include <linux/workqueue.h>
#include <linux/rtnetlink.h>
#include <net/genetlink.h>
#include <net/cfg80211.h>
#include "reg.h"


#define WIPHY_IDX_INVALID	-1

struct cfg80211_registered_device {
	const struct cfg80211_ops *ops;
	struct list_head list;
<<<<<<< HEAD
	/* we hold this mutex during any call so that
	 * we cannot do multiple calls at once, and also
	 * to avoid the deregister call to proceed while
	 * any call is in progress */
	struct mutex mtx;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* rfkill support */
	struct rfkill_ops rfkill_ops;
	struct rfkill *rfkill;
	struct work_struct rfkill_sync;

	/* ISO / IEC 3166 alpha2 for which this device is receiving
	 * country IEs on, this can help disregard country IEs from APs
	 * on the same alpha2 quickly. The alpha2 may differ from
	 * cfg80211_regdomain's alpha2 when an intersection has occurred.
	 * If the AP is reconfigured this can also be used to tell us if
	 * the country on the country IE changed. */
	char country_ie_alpha2[2];

<<<<<<< HEAD
=======
	/*
	 * the driver requests the regulatory core to set this regulatory
	 * domain as the wiphy's. Only used for %REGULATORY_WIPHY_SELF_MANAGED
	 * devices using the regulatory_set_wiphy_regd() API
	 */
	const struct ieee80211_regdomain *requested_regd;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* If a Country IE has been received this tells us the environment
	 * which its telling us its in. This defaults to ENVIRON_ANY */
	enum environment_cap env;

	/* wiphy index, internal only */
	int wiphy_idx;

<<<<<<< HEAD
	/* associated wireless interfaces */
	struct mutex devlist_mtx;
	/* protected by devlist_mtx or RCU */
	struct list_head wdev_list;
	int devlist_generation, wdev_id;
	int opencount; /* also protected by devlist_mtx */
=======
	/* protected by RTNL */
	int devlist_generation, wdev_id;
	int opencount;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	wait_queue_head_t dev_wait;

	struct list_head beacon_registrations;
	spinlock_t beacon_registrations_lock;

<<<<<<< HEAD
=======
	struct list_head mlme_unreg;
	spinlock_t mlme_unreg_lock;
	struct work_struct mlme_unreg_wk;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* protected by RTNL only */
	int num_running_ifaces;
	int num_running_monitor_ifaces;

	/* BSSes/scanning */
	spinlock_t bss_lock;
	struct list_head bss_list;
	struct rb_root bss_tree;
	u32 bss_generation;
	u32 bss_entries;
	struct cfg80211_scan_request *scan_req; /* protected by RTNL */
	struct sk_buff *scan_msg;
<<<<<<< HEAD
	struct cfg80211_sched_scan_request *sched_scan_req;
=======
	struct cfg80211_sched_scan_request __rcu *sched_scan_req;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long suspend_at;
	struct work_struct scan_done_wk;
	struct work_struct sched_scan_results_wk;

<<<<<<< HEAD
	struct mutex sched_scan_mtx;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct genl_info *cur_cmd_info;

	struct work_struct conn_work;
	struct work_struct event_work;

<<<<<<< HEAD
	struct cfg80211_wowlan *wowlan;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct delayed_work dfs_update_channels_wk;

	/* netlink port which started critical protocol (0 means not started) */
	u32 crit_proto_nlportid;

<<<<<<< HEAD
=======
	struct cfg80211_coalesce *coalesce;

	spinlock_t destroy_list_lock;
	struct list_head destroy_list;
	struct work_struct destroy_work;

	struct work_struct sched_scan_stop_wk;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* must be last because of the way we do wiphy_priv(),
	 * and it should at least be aligned to NETDEV_ALIGN */
	struct wiphy wiphy __aligned(NETDEV_ALIGN);
};

static inline
<<<<<<< HEAD
struct cfg80211_registered_device *wiphy_to_dev(struct wiphy *wiphy)
=======
struct cfg80211_registered_device *wiphy_to_rdev(struct wiphy *wiphy)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	BUG_ON(!wiphy);
	return container_of(wiphy, struct cfg80211_registered_device, wiphy);
}

static inline void
cfg80211_rdev_free_wowlan(struct cfg80211_registered_device *rdev)
{
<<<<<<< HEAD
	int i;

	if (!rdev->wowlan)
		return;
	for (i = 0; i < rdev->wowlan->n_patterns; i++)
		kfree(rdev->wowlan->patterns[i].mask);
	kfree(rdev->wowlan->patterns);
	if (rdev->wowlan->tcp && rdev->wowlan->tcp->sock)
		sock_release(rdev->wowlan->tcp->sock);
	kfree(rdev->wowlan->tcp);
	kfree(rdev->wowlan);
}

extern struct workqueue_struct *cfg80211_wq;
extern struct mutex cfg80211_mutex;
extern struct list_head cfg80211_rdev_list;
extern int cfg80211_rdev_list_generation;

static inline void assert_cfg80211_lock(void)
{
	lockdep_assert_held(&cfg80211_mutex);
}

=======
#ifdef CONFIG_PM
	int i;

	if (!rdev->wiphy.wowlan_config)
		return;
	for (i = 0; i < rdev->wiphy.wowlan_config->n_patterns; i++)
		kfree(rdev->wiphy.wowlan_config->patterns[i].mask);
	kfree(rdev->wiphy.wowlan_config->patterns);
	if (rdev->wiphy.wowlan_config->tcp &&
	    rdev->wiphy.wowlan_config->tcp->sock)
		sock_release(rdev->wiphy.wowlan_config->tcp->sock);
	kfree(rdev->wiphy.wowlan_config->tcp);
	kfree(rdev->wiphy.wowlan_config->nd_config);
	kfree(rdev->wiphy.wowlan_config);
#endif
}

extern struct workqueue_struct *cfg80211_wq;
extern struct list_head cfg80211_rdev_list;
extern int cfg80211_rdev_list_generation;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct cfg80211_internal_bss {
	struct list_head list;
	struct list_head hidden_list;
	struct rb_node rbn;
<<<<<<< HEAD
=======
	u64 ts_boottime;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long ts;
	unsigned long refcount;
	atomic_t hold;

<<<<<<< HEAD
=======
	/* time at the start of the reception of the first octet of the
	 * timestamp field of the last beacon/probe received for this BSS.
	 * The time is the TSF of the BSS specified by %parent_bssid.
	 */
	u64 parent_tsf;

	/* the BSS according to which %parent_tsf is set. This is set to
	 * the BSS that the interface that requested the scan was connected to
	 * when the beacon/probe was received.
	 */
	u8 parent_bssid[ETH_ALEN] __aligned(2);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* must be last because of priv member */
	struct cfg80211_bss pub;
};

static inline struct cfg80211_internal_bss *bss_from_pub(struct cfg80211_bss *pub)
{
	return container_of(pub, struct cfg80211_internal_bss, pub);
}

static inline void cfg80211_hold_bss(struct cfg80211_internal_bss *bss)
{
	atomic_inc(&bss->hold);
}

static inline void cfg80211_unhold_bss(struct cfg80211_internal_bss *bss)
{
	int r = atomic_dec_return(&bss->hold);
	WARN_ON(r < 0);
}


struct cfg80211_registered_device *cfg80211_rdev_by_wiphy_idx(int wiphy_idx);
int get_wiphy_idx(struct wiphy *wiphy);

<<<<<<< HEAD
/* requires cfg80211_rdev_mutex to be held! */
struct wiphy *wiphy_idx_to_wiphy(int wiphy_idx);

/* identical to cfg80211_get_dev_from_info but only operate on ifindex */
extern struct cfg80211_registered_device *
cfg80211_get_dev_from_ifindex(struct net *net, int ifindex);

int cfg80211_switch_netns(struct cfg80211_registered_device *rdev,
			  struct net *net);

static inline void cfg80211_lock_rdev(struct cfg80211_registered_device *rdev)
{
	mutex_lock(&rdev->mtx);
}

static inline void cfg80211_unlock_rdev(struct cfg80211_registered_device *rdev)
{
	BUG_ON(IS_ERR(rdev) || !rdev);
	mutex_unlock(&rdev->mtx);
}

=======
struct wiphy *wiphy_idx_to_wiphy(int wiphy_idx);

int cfg80211_switch_netns(struct cfg80211_registered_device *rdev,
			  struct net *net);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline void wdev_lock(struct wireless_dev *wdev)
	__acquires(wdev)
{
	mutex_lock(&wdev->mtx);
	__acquire(wdev->mtx);
}

static inline void wdev_unlock(struct wireless_dev *wdev)
	__releases(wdev)
{
	__release(wdev->mtx);
	mutex_unlock(&wdev->mtx);
}

<<<<<<< HEAD
#define ASSERT_RDEV_LOCK(rdev) lockdep_assert_held(&(rdev)->mtx)
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define ASSERT_WDEV_LOCK(wdev) lockdep_assert_held(&(wdev)->mtx)

static inline bool cfg80211_has_monitors_only(struct cfg80211_registered_device *rdev)
{
	ASSERT_RTNL();

	return rdev->num_running_ifaces == rdev->num_running_monitor_ifaces &&
	       rdev->num_running_ifaces > 0;
}

enum cfg80211_event_type {
	EVENT_CONNECT_RESULT,
	EVENT_ROAMED,
	EVENT_DISCONNECTED,
	EVENT_IBSS_JOINED,
<<<<<<< HEAD
	EVENT_DRV_RECOVERY,
=======
	EVENT_STOPPED,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

struct cfg80211_event {
	struct list_head list;
	enum cfg80211_event_type type;

	union {
		struct {
			u8 bssid[ETH_ALEN];
			const u8 *req_ie;
			const u8 *resp_ie;
			size_t req_ie_len;
			size_t resp_ie_len;
<<<<<<< HEAD
			u16 status;
=======
			struct cfg80211_bss *bss;
			int status; /* -1 = failed; 0..65535 = status code */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		} cr;
		struct {
			const u8 *req_ie;
			const u8 *resp_ie;
			size_t req_ie_len;
			size_t resp_ie_len;
			struct cfg80211_bss *bss;
		} rm;
		struct {
			const u8 *ie;
			size_t ie_len;
			u16 reason;
<<<<<<< HEAD
		} dc;
		struct {
			u8 bssid[ETH_ALEN];
=======
			bool locally_generated;
		} dc;
		struct {
			u8 bssid[ETH_ALEN];
			struct ieee80211_channel *channel;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		} ij;
	};
};

struct cfg80211_cached_keys {
<<<<<<< HEAD
	struct key_params params[6];
	u8 data[6][WLAN_MAX_KEY_LEN];
	int def, defmgmt;
=======
	struct key_params params[CFG80211_MAX_WEP_KEYS];
	u8 data[CFG80211_MAX_WEP_KEYS][WLAN_KEY_LEN_WEP104];
	int def;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

enum cfg80211_chan_mode {
	CHAN_MODE_UNDEFINED,
	CHAN_MODE_SHARED,
	CHAN_MODE_EXCLUSIVE,
};

struct cfg80211_beacon_registration {
	struct list_head list;
	u32 nlportid;
};

<<<<<<< HEAD
/* free object */
extern void cfg80211_dev_free(struct cfg80211_registered_device *rdev);

extern int cfg80211_dev_rename(struct cfg80211_registered_device *rdev,
			       char *newname);

void ieee80211_set_bitrate_flags(struct wiphy *wiphy);

void cfg80211_bss_expire(struct cfg80211_registered_device *dev);

void cfg80211_bss_expire_p2p(struct cfg80211_registered_device *dev);

void cfg80211_bss_age(struct cfg80211_registered_device *dev,
                      unsigned long age_secs);

/* IBSS */
int __cfg80211_join_ibss(struct cfg80211_registered_device *rdev,
			 struct net_device *dev,
			 struct cfg80211_ibss_params *params,
			 struct cfg80211_cached_keys *connkeys);
=======
struct cfg80211_iface_destroy {
	struct list_head list;
	u32 nlportid;
};

void cfg80211_destroy_ifaces(struct cfg80211_registered_device *rdev);

/* free object */
void cfg80211_dev_free(struct cfg80211_registered_device *rdev);

int cfg80211_dev_rename(struct cfg80211_registered_device *rdev,
			char *newname);

void ieee80211_set_bitrate_flags(struct wiphy *wiphy);

void cfg80211_bss_expire(struct cfg80211_registered_device *rdev);
void cfg80211_bss_age(struct cfg80211_registered_device *rdev,
                      unsigned long age_secs);

/* IBSS */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int cfg80211_join_ibss(struct cfg80211_registered_device *rdev,
		       struct net_device *dev,
		       struct cfg80211_ibss_params *params,
		       struct cfg80211_cached_keys *connkeys);
void cfg80211_clear_ibss(struct net_device *dev, bool nowext);
int __cfg80211_leave_ibss(struct cfg80211_registered_device *rdev,
			  struct net_device *dev, bool nowext);
int cfg80211_leave_ibss(struct cfg80211_registered_device *rdev,
			struct net_device *dev, bool nowext);
<<<<<<< HEAD
void __cfg80211_ibss_joined(struct net_device *dev, const u8 *bssid);
=======
void __cfg80211_ibss_joined(struct net_device *dev, const u8 *bssid,
			    struct ieee80211_channel *channel);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int cfg80211_ibss_wext_join(struct cfg80211_registered_device *rdev,
			    struct wireless_dev *wdev);

/* mesh */
extern const struct mesh_config default_mesh_config;
extern const struct mesh_setup default_mesh_setup;
int __cfg80211_join_mesh(struct cfg80211_registered_device *rdev,
			 struct net_device *dev,
			 struct mesh_setup *setup,
			 const struct mesh_config *conf);
int cfg80211_join_mesh(struct cfg80211_registered_device *rdev,
		       struct net_device *dev,
		       struct mesh_setup *setup,
		       const struct mesh_config *conf);
<<<<<<< HEAD
=======
int __cfg80211_leave_mesh(struct cfg80211_registered_device *rdev,
			  struct net_device *dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int cfg80211_leave_mesh(struct cfg80211_registered_device *rdev,
			struct net_device *dev);
int cfg80211_set_mesh_channel(struct cfg80211_registered_device *rdev,
			      struct wireless_dev *wdev,
			      struct cfg80211_chan_def *chandef);

<<<<<<< HEAD
/* AP */
int cfg80211_stop_ap(struct cfg80211_registered_device *rdev,
		     struct net_device *dev);

/* MLME */
int __cfg80211_mlme_auth(struct cfg80211_registered_device *rdev,
			 struct net_device *dev,
			 struct ieee80211_channel *chan,
			 enum nl80211_auth_type auth_type,
			 const u8 *bssid,
			 const u8 *ssid, int ssid_len,
			 const u8 *ie, int ie_len,
			 const u8 *key, int key_len, int key_idx,
			 const u8 *sae_data, int sae_data_len);
int cfg80211_mlme_auth(struct cfg80211_registered_device *rdev,
		       struct net_device *dev, struct ieee80211_channel *chan,
		       enum nl80211_auth_type auth_type, const u8 *bssid,
=======
/* OCB */
int __cfg80211_join_ocb(struct cfg80211_registered_device *rdev,
			struct net_device *dev,
			struct ocb_setup *setup);
int cfg80211_join_ocb(struct cfg80211_registered_device *rdev,
		      struct net_device *dev,
		      struct ocb_setup *setup);
int __cfg80211_leave_ocb(struct cfg80211_registered_device *rdev,
			 struct net_device *dev);
int cfg80211_leave_ocb(struct cfg80211_registered_device *rdev,
		       struct net_device *dev);

/* AP */
int __cfg80211_stop_ap(struct cfg80211_registered_device *rdev,
		       struct net_device *dev, bool notify);
int cfg80211_stop_ap(struct cfg80211_registered_device *rdev,
		     struct net_device *dev, bool notify);

/* MLME */
int cfg80211_mlme_auth(struct cfg80211_registered_device *rdev,
		       struct net_device *dev,
		       struct ieee80211_channel *chan,
		       enum nl80211_auth_type auth_type,
		       const u8 *bssid,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		       const u8 *ssid, int ssid_len,
		       const u8 *ie, int ie_len,
		       const u8 *key, int key_len, int key_idx,
		       const u8 *sae_data, int sae_data_len);
<<<<<<< HEAD
int __cfg80211_mlme_assoc(struct cfg80211_registered_device *rdev,
			  struct net_device *dev,
			  struct ieee80211_channel *chan,
			  const u8 *bssid,
			  const u8 *ssid, int ssid_len,
			  struct cfg80211_assoc_request *req);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int cfg80211_mlme_assoc(struct cfg80211_registered_device *rdev,
			struct net_device *dev,
			struct ieee80211_channel *chan,
			const u8 *bssid,
			const u8 *ssid, int ssid_len,
			struct cfg80211_assoc_request *req);
<<<<<<< HEAD
int __cfg80211_mlme_deauth(struct cfg80211_registered_device *rdev,
			   struct net_device *dev, const u8 *bssid,
			   const u8 *ie, int ie_len, u16 reason,
			   bool local_state_change);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int cfg80211_mlme_deauth(struct cfg80211_registered_device *rdev,
			 struct net_device *dev, const u8 *bssid,
			 const u8 *ie, int ie_len, u16 reason,
			 bool local_state_change);
int cfg80211_mlme_disassoc(struct cfg80211_registered_device *rdev,
			   struct net_device *dev, const u8 *bssid,
			   const u8 *ie, int ie_len, u16 reason,
			   bool local_state_change);
void cfg80211_mlme_down(struct cfg80211_registered_device *rdev,
			struct net_device *dev);
<<<<<<< HEAD
void __cfg80211_connect_result(struct net_device *dev, const u8 *bssid,
			       const u8 *req_ie, size_t req_ie_len,
			       const u8 *resp_ie, size_t resp_ie_len,
			       u16 status, bool wextev,
			       struct cfg80211_bss *bss);
int cfg80211_mlme_register_mgmt(struct wireless_dev *wdev, u32 snd_pid,
				u16 frame_type, const u8 *match_data,
				int match_len);
=======
int cfg80211_mlme_register_mgmt(struct wireless_dev *wdev, u32 snd_pid,
				u16 frame_type, const u8 *match_data,
				int match_len);
void cfg80211_mlme_unreg_wk(struct work_struct *wk);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void cfg80211_mlme_unregister_socket(struct wireless_dev *wdev, u32 nlpid);
void cfg80211_mlme_purge_registrations(struct wireless_dev *wdev);
int cfg80211_mlme_mgmt_tx(struct cfg80211_registered_device *rdev,
			  struct wireless_dev *wdev,
<<<<<<< HEAD
			  struct ieee80211_channel *chan, bool offchan,
			  unsigned int wait, const u8 *buf, size_t len,
			  bool no_cck, bool dont_wait_for_ack, u64 *cookie);
=======
			  struct cfg80211_mgmt_tx_params *params,
			  u64 *cookie);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void cfg80211_oper_and_ht_capa(struct ieee80211_ht_cap *ht_capa,
			       const struct ieee80211_ht_cap *ht_capa_mask);
void cfg80211_oper_and_vht_capa(struct ieee80211_vht_cap *vht_capa,
				const struct ieee80211_vht_cap *vht_capa_mask);

<<<<<<< HEAD
/* SME */
int __cfg80211_connect(struct cfg80211_registered_device *rdev,
		       struct net_device *dev,
		       struct cfg80211_connect_params *connect,
		       struct cfg80211_cached_keys *connkeys,
		       const u8 *prev_bssid);
int cfg80211_connect(struct cfg80211_registered_device *rdev,
		     struct net_device *dev,
		     struct cfg80211_connect_params *connect,
		     struct cfg80211_cached_keys *connkeys);
int __cfg80211_disconnect(struct cfg80211_registered_device *rdev,
			  struct net_device *dev, u16 reason,
			  bool wextev);
=======
/* SME events */
int cfg80211_connect(struct cfg80211_registered_device *rdev,
		     struct net_device *dev,
		     struct cfg80211_connect_params *connect,
		     struct cfg80211_cached_keys *connkeys,
		     const u8 *prev_bssid);
void __cfg80211_connect_result(struct net_device *dev, const u8 *bssid,
			       const u8 *req_ie, size_t req_ie_len,
			       const u8 *resp_ie, size_t resp_ie_len,
			       int status, bool wextev,
			       struct cfg80211_bss *bss);
void __cfg80211_disconnected(struct net_device *dev, const u8 *ie,
			     size_t ie_len, u16 reason, bool from_ap);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int cfg80211_disconnect(struct cfg80211_registered_device *rdev,
			struct net_device *dev, u16 reason,
			bool wextev);
void __cfg80211_roamed(struct wireless_dev *wdev,
		       struct cfg80211_bss *bss,
		       const u8 *req_ie, size_t req_ie_len,
		       const u8 *resp_ie, size_t resp_ie_len);
int cfg80211_mgd_wext_connect(struct cfg80211_registered_device *rdev,
			      struct wireless_dev *wdev);

<<<<<<< HEAD
void cfg80211_conn_work(struct work_struct *work);
void cfg80211_sme_failed_assoc(struct wireless_dev *wdev);
bool cfg80211_sme_failed_reassoc(struct wireless_dev *wdev);
=======
/* SME implementation */
void cfg80211_conn_work(struct work_struct *work);
void cfg80211_sme_scan_done(struct net_device *dev);
bool cfg80211_sme_rx_assoc_resp(struct wireless_dev *wdev, u16 status);
void cfg80211_sme_rx_auth(struct wireless_dev *wdev, const u8 *buf, size_t len);
void cfg80211_sme_disassoc(struct wireless_dev *wdev);
void cfg80211_sme_deauth(struct wireless_dev *wdev);
void cfg80211_sme_auth_timeout(struct wireless_dev *wdev);
void cfg80211_sme_assoc_timeout(struct wireless_dev *wdev);
void cfg80211_sme_abandon_assoc(struct wireless_dev *wdev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* internal helpers */
bool cfg80211_supported_cipher_suite(struct wiphy *wiphy, u32 cipher);
int cfg80211_validate_key_settings(struct cfg80211_registered_device *rdev,
				   struct key_params *params, int key_idx,
				   bool pairwise, const u8 *mac_addr);
<<<<<<< HEAD
void __cfg80211_disconnected(struct net_device *dev, const u8 *ie,
			     size_t ie_len, u16 reason, bool from_ap);
void cfg80211_sme_scan_done(struct net_device *dev);
void cfg80211_sme_rx_auth(struct net_device *dev, const u8 *buf, size_t len);
void cfg80211_sme_disassoc(struct net_device *dev,
			   struct cfg80211_internal_bss *bss);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void __cfg80211_scan_done(struct work_struct *wk);
void ___cfg80211_scan_done(struct cfg80211_registered_device *rdev,
			   bool send_message);
void __cfg80211_sched_scan_results(struct work_struct *wk);
int __cfg80211_stop_sched_scan(struct cfg80211_registered_device *rdev,
			       bool driver_initiated);
void cfg80211_upload_connect_keys(struct wireless_dev *wdev);
int cfg80211_change_iface(struct cfg80211_registered_device *rdev,
			  struct net_device *dev, enum nl80211_iftype ntype,
			  u32 *flags, struct vif_params *params);
void cfg80211_process_rdev_events(struct cfg80211_registered_device *rdev);
void cfg80211_process_wdev_events(struct wireless_dev *wdev);

<<<<<<< HEAD
int cfg80211_can_use_iftype_chan(struct cfg80211_registered_device *rdev,
				 struct wireless_dev *wdev,
				 enum nl80211_iftype iftype,
				 struct ieee80211_channel *chan,
				 enum cfg80211_chan_mode chanmode,
				 u8 radar_detect);

/**
 * cfg80211_chandef_dfs_required - checks if radar detection is required
 * @wiphy: the wiphy to validate against
 * @chandef: the channel definition to check
 * Return: 1 if radar detection is required, 0 if it is not, < 0 on error
 */
int cfg80211_chandef_dfs_required(struct wiphy *wiphy,
				  const struct cfg80211_chan_def *c);
=======
/**
 * cfg80211_chandef_dfs_usable - checks if chandef is DFS usable
 * @wiphy: the wiphy to validate against
 * @chandef: the channel definition to check
 *
 * Checks if chandef is usable and we can/need start CAC on such channel.
 *
 * Return: Return true if all channels available and at least
 *	   one channel require CAC (NL80211_DFS_USABLE)
 */
bool cfg80211_chandef_dfs_usable(struct wiphy *wiphy,
				 const struct cfg80211_chan_def *chandef);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

void cfg80211_set_dfs_state(struct wiphy *wiphy,
			    const struct cfg80211_chan_def *chandef,
			    enum nl80211_dfs_state dfs_state);

void cfg80211_dfs_channels_update_work(struct work_struct *work);

<<<<<<< HEAD

static inline int
cfg80211_can_change_interface(struct cfg80211_registered_device *rdev,
			      struct wireless_dev *wdev,
			      enum nl80211_iftype iftype)
{
	return cfg80211_can_use_iftype_chan(rdev, wdev, iftype, NULL,
					    CHAN_MODE_UNDEFINED, 0);
}

static inline int
cfg80211_can_add_interface(struct cfg80211_registered_device *rdev,
			   enum nl80211_iftype iftype)
{
	return cfg80211_can_change_interface(rdev, NULL, iftype);
}

static inline int
cfg80211_can_use_chan(struct cfg80211_registered_device *rdev,
		      struct wireless_dev *wdev,
		      struct ieee80211_channel *chan,
		      enum cfg80211_chan_mode chanmode)
{
	return cfg80211_can_use_iftype_chan(rdev, wdev, wdev->iftype,
					    chan, chanmode, 0);
}
=======
unsigned int
cfg80211_chandef_dfs_cac_time(struct wiphy *wiphy,
			      const struct cfg80211_chan_def *chandef);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static inline unsigned int elapsed_jiffies_msecs(unsigned long start)
{
	unsigned long end = jiffies;

	if (end >= start)
		return jiffies_to_msecs(end - start);

<<<<<<< HEAD
	return jiffies_to_msecs(end + (MAX_JIFFY_OFFSET - start) + 1);
=======
	return jiffies_to_msecs(end + (ULONG_MAX - start) + 1);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void
cfg80211_get_chan_state(struct wireless_dev *wdev,
		        struct ieee80211_channel **chan,
<<<<<<< HEAD
		        enum cfg80211_chan_mode *chanmode);
=======
		        enum cfg80211_chan_mode *chanmode,
		        u8 *radar_detect);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

int cfg80211_set_monitor_channel(struct cfg80211_registered_device *rdev,
				 struct cfg80211_chan_def *chandef);

int ieee80211_get_ratemask(struct ieee80211_supported_band *sband,
			   const u8 *rates, unsigned int n_rates,
			   u32 *mask);

int cfg80211_validate_beacon_int(struct cfg80211_registered_device *rdev,
				 u32 beacon_int);

void cfg80211_update_iface_num(struct cfg80211_registered_device *rdev,
			       enum nl80211_iftype iftype, int num);

<<<<<<< HEAD
=======
void __cfg80211_leave(struct cfg80211_registered_device *rdev,
		      struct wireless_dev *wdev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void cfg80211_leave(struct cfg80211_registered_device *rdev,
		    struct wireless_dev *wdev);

void cfg80211_stop_p2p_device(struct cfg80211_registered_device *rdev,
			      struct wireless_dev *wdev);

<<<<<<< HEAD
=======
void cfg80211_stop_nan(struct cfg80211_registered_device *rdev,
		       struct wireless_dev *wdev);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define CFG80211_MAX_NUM_DIFFERENT_CHANNELS 10

#ifdef CONFIG_CFG80211_DEVELOPER_WARNINGS
#define CFG80211_DEV_WARN_ON(cond)	WARN_ON(cond)
#else
/*
 * Trick to enable using it as a condition,
 * and also not give a warning when it's
 * not used that way.
 */
#define CFG80211_DEV_WARN_ON(cond)	({bool __r = (cond); __r; })
#endif

#endif /* __NET_WIRELESS_CORE_H */
