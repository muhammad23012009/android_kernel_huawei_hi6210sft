#ifndef __WEXT_COMPAT
#define __WEXT_COMPAT

#include <net/iw_handler.h>
#include <linux/wireless.h>

<<<<<<< HEAD
=======
#ifdef CONFIG_CFG80211_WEXT_EXPORT
#define EXPORT_WEXT_HANDLER(h) EXPORT_SYMBOL_GPL(h)
#else
#define EXPORT_WEXT_HANDLER(h)
#endif /* CONFIG_CFG80211_WEXT_EXPORT */

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int cfg80211_ibss_wext_siwfreq(struct net_device *dev,
			       struct iw_request_info *info,
			       struct iw_freq *freq, char *extra);
int cfg80211_ibss_wext_giwfreq(struct net_device *dev,
			       struct iw_request_info *info,
			       struct iw_freq *freq, char *extra);
int cfg80211_ibss_wext_siwap(struct net_device *dev,
			     struct iw_request_info *info,
			     struct sockaddr *ap_addr, char *extra);
int cfg80211_ibss_wext_giwap(struct net_device *dev,
			     struct iw_request_info *info,
			     struct sockaddr *ap_addr, char *extra);
int cfg80211_ibss_wext_siwessid(struct net_device *dev,
				struct iw_request_info *info,
				struct iw_point *data, char *ssid);
int cfg80211_ibss_wext_giwessid(struct net_device *dev,
				struct iw_request_info *info,
				struct iw_point *data, char *ssid);

int cfg80211_mgd_wext_siwfreq(struct net_device *dev,
			      struct iw_request_info *info,
			      struct iw_freq *freq, char *extra);
int cfg80211_mgd_wext_giwfreq(struct net_device *dev,
			      struct iw_request_info *info,
			      struct iw_freq *freq, char *extra);
int cfg80211_mgd_wext_siwap(struct net_device *dev,
			    struct iw_request_info *info,
			    struct sockaddr *ap_addr, char *extra);
int cfg80211_mgd_wext_giwap(struct net_device *dev,
			    struct iw_request_info *info,
			    struct sockaddr *ap_addr, char *extra);
int cfg80211_mgd_wext_siwessid(struct net_device *dev,
			       struct iw_request_info *info,
			       struct iw_point *data, char *ssid);
int cfg80211_mgd_wext_giwessid(struct net_device *dev,
			       struct iw_request_info *info,
			       struct iw_point *data, char *ssid);

int cfg80211_wext_siwmlme(struct net_device *dev,
			  struct iw_request_info *info,
			  struct iw_point *data, char *extra);
int cfg80211_wext_siwgenie(struct net_device *dev,
			   struct iw_request_info *info,
			   struct iw_point *data, char *extra);


<<<<<<< HEAD
int cfg80211_wext_freq(struct wiphy *wiphy, struct iw_freq *freq);
=======
int cfg80211_wext_freq(struct iw_freq *freq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414


extern const struct iw_handler_def cfg80211_wext_handler;
#endif /* __WEXT_COMPAT */
