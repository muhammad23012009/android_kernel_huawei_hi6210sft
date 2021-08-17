NIC_SELECT = RTL8192U

<<<<<<< HEAD
ccflags-y := -I$(TOPDIR)/drivers/net/wireless
ccflags-y += -O2
=======
ccflags-y := -O2
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
ccflags-y += -DJACKSON_NEW_8187 -DJACKSON_NEW_RX

ieee80211-rsl-objs := ieee80211_rx.o \
		      ieee80211_softmac.o \
		      ieee80211_tx.o \
		      ieee80211_wx.o \
		      ieee80211_module.o \
		      ieee80211_softmac_wx.o\
		      rtl819x_HTProc.o\
		      rtl819x_TSProc.o\
		      rtl819x_BAProc.o\
		      dot11d.o

ieee80211_crypt-rsl-objs := ieee80211_crypt.o
ieee80211_crypt_tkip-rsl-objs := ieee80211_crypt_tkip.o
ieee80211_crypt_ccmp-rsl-objs := ieee80211_crypt_ccmp.o
ieee80211_crypt_wep-rsl-objs := ieee80211_crypt_wep.o

obj-m +=ieee80211-rsl.o
obj-m +=ieee80211_crypt-rsl.o
obj-m +=ieee80211_crypt_wep-rsl.o
obj-m +=ieee80211_crypt_tkip-rsl.o
obj-m +=ieee80211_crypt_ccmp-rsl.o
