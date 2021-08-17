obj-$(CONFIG_MAC80211) += mac80211.o

# mac80211 objects
mac80211-y := \
	main.o status.o \
<<<<<<< HEAD
=======
	driver-ops.o \
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	sta_info.o \
	wep.o \
	wpa.o \
	scan.o offchannel.o \
	ht.o agg-tx.o agg-rx.o \
	vht.o \
	ibss.o \
	iface.o \
	rate.o \
	michael.o \
	tkip.o \
	aes_ccm.o \
<<<<<<< HEAD
	aes_cmac.o \
	cfg.o \
=======
	aes_gcm.o \
	aes_cmac.o \
	aes_gmac.o \
	cfg.o \
	ethtool.o \
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rx.o \
	spectmgmt.o \
	tx.o \
	key.o \
	util.o \
	wme.o \
<<<<<<< HEAD
	event.o \
	chan.o \
	trace.o mlme.o
=======
	chan.o \
	trace.o mlme.o \
	tdls.o \
	ocb.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

mac80211-$(CONFIG_MAC80211_LEDS) += led.o
mac80211-$(CONFIG_MAC80211_DEBUGFS) += \
	debugfs.o \
	debugfs_sta.o \
	debugfs_netdev.o \
	debugfs_key.o

mac80211-$(CONFIG_MAC80211_MESH) += \
	mesh.o \
	mesh_pathtbl.o \
	mesh_plink.o \
	mesh_hwmp.o \
	mesh_sync.o \
	mesh_ps.o

mac80211-$(CONFIG_PM) += pm.o

CFLAGS_trace.o := -I$(src)

<<<<<<< HEAD
# objects for PID algorithm
rc80211_pid-y := rc80211_pid_algo.o
rc80211_pid-$(CONFIG_MAC80211_DEBUGFS) += rc80211_pid_debugfs.o

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
rc80211_minstrel-y := rc80211_minstrel.o
rc80211_minstrel-$(CONFIG_MAC80211_DEBUGFS) += rc80211_minstrel_debugfs.o

rc80211_minstrel_ht-y := rc80211_minstrel_ht.o
rc80211_minstrel_ht-$(CONFIG_MAC80211_DEBUGFS) += rc80211_minstrel_ht_debugfs.o

<<<<<<< HEAD
mac80211-$(CONFIG_MAC80211_RC_PID) += $(rc80211_pid-y)
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
mac80211-$(CONFIG_MAC80211_RC_MINSTREL) += $(rc80211_minstrel-y)
mac80211-$(CONFIG_MAC80211_RC_MINSTREL_HT) += $(rc80211_minstrel_ht-y)

ccflags-y += -D__CHECK_ENDIAN__ -DDEBUG
