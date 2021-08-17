#ifndef __USBAUDIO_FORMAT_H
#define __USBAUDIO_FORMAT_H

int snd_usb_parse_audio_format(struct snd_usb_audio *chip,
			       struct audioformat *fp, unsigned int format,
			       struct uac_format_type_i_continuous_descriptor *fmt,
<<<<<<< HEAD
			       int stream, struct usb_host_interface *iface);
=======
			       int stream);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /*  __USBAUDIO_FORMAT_H */
