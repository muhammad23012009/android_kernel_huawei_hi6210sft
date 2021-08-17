#
# Makefile for physical layer USB drivers
#
<<<<<<< HEAD

ccflags-$(CONFIG_USB_DEBUG) := -DDEBUG

obj-$(CONFIG_USB_PHY)			+= phy.o
obj-$(CONFIG_USB_OTG_WAKELOCK)		+= otg-wakelock.o
=======
obj-$(CONFIG_USB_PHY)			+= phy.o
obj-$(CONFIG_OF)			+= of.o
obj-$(CONFIG_USB_OTG_WAKELOCK)		+= otg-wakelock.o
obj-$(CONFIG_DUAL_ROLE_USB_INTF)	+= class-dual-role.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

# transceiver drivers, keep the list sorted

obj-$(CONFIG_AB8500_USB)		+= phy-ab8500-usb.o
<<<<<<< HEAD
phy-fsl-usb2-objs			:= phy-fsl-usb.o phy-fsm-usb.o
obj-$(CONFIG_FSL_USB2_OTG)		+= phy-fsl-usb2.o
obj-$(CONFIG_ISP1301_OMAP)		+= phy-isp1301-omap.o
obj-$(CONFIG_MV_U3D_PHY)		+= phy-mv-u3d-usb.o
obj-$(CONFIG_NOP_USB_XCEIV)		+= phy-nop.o
obj-$(CONFIG_OMAP_CONTROL_USB)		+= phy-omap-control.o
obj-$(CONFIG_OMAP_USB2)			+= phy-omap-usb2.o
obj-$(CONFIG_OMAP_USB3)			+= phy-omap-usb3.o
obj-$(CONFIG_SAMSUNG_USBPHY)		+= phy-samsung-usb.o
obj-$(CONFIG_SAMSUNG_USB2PHY)		+= phy-samsung-usb2.o
obj-$(CONFIG_SAMSUNG_USB3PHY)		+= phy-samsung-usb3.o
obj-$(CONFIG_TWL4030_USB)		+= phy-twl4030-usb.o
=======
obj-$(CONFIG_FSL_USB2_OTG)		+= phy-fsl-usb.o
obj-$(CONFIG_ISP1301_OMAP)		+= phy-isp1301-omap.o
obj-$(CONFIG_NOP_USB_XCEIV)		+= phy-generic.o
obj-$(CONFIG_TAHVO_USB)			+= phy-tahvo.o
obj-$(CONFIG_AM335X_CONTROL_USB)	+= phy-am335x-control.o
obj-$(CONFIG_AM335X_PHY_USB)		+= phy-am335x.o
obj-$(CONFIG_OMAP_OTG)			+= phy-omap-otg.o
obj-$(CONFIG_SAMSUNG_USBPHY)		+= phy-samsung-usb.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
obj-$(CONFIG_TWL6030_USB)		+= phy-twl6030-usb.o
obj-$(CONFIG_USB_EHCI_TEGRA)		+= phy-tegra-usb.o
obj-$(CONFIG_USB_GPIO_VBUS)		+= phy-gpio-vbus-usb.o
obj-$(CONFIG_USB_ISP1301)		+= phy-isp1301.o
obj-$(CONFIG_USB_MSM_OTG)		+= phy-msm-usb.o
<<<<<<< HEAD
obj-$(CONFIG_USB_MV_OTG)		+= phy-mv-usb.o
obj-$(CONFIG_USB_MXS_PHY)		+= phy-mxs-usb.o
obj-$(CONFIG_USB_RCAR_PHY)		+= phy-rcar-usb.o
obj-$(CONFIG_USB_ULPI)			+= phy-ulpi.o
obj-$(CONFIG_USB_ULPI_VIEWPORT)		+= phy-ulpi-viewport.o
=======
obj-$(CONFIG_USB_QCOM_8X16_PHY)	+= phy-qcom-8x16-usb.o
obj-$(CONFIG_USB_MV_OTG)		+= phy-mv-usb.o
obj-$(CONFIG_USB_MXS_PHY)		+= phy-mxs-usb.o
obj-$(CONFIG_USB_ULPI)			+= phy-ulpi.o
obj-$(CONFIG_USB_ULPI_VIEWPORT)		+= phy-ulpi-viewport.o
obj-$(CONFIG_KEYSTONE_USB_PHY)		+= phy-keystone.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
