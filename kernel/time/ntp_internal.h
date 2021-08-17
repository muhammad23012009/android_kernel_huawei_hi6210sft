#ifndef _LINUX_NTP_INTERNAL_H
#define _LINUX_NTP_INTERNAL_H

extern void ntp_init(void);
extern void ntp_clear(void);
/* Returns how long ticks are at present, in ns / 2^NTP_SCALE_SHIFT. */
extern u64 ntp_tick_length(void);
<<<<<<< HEAD
extern int second_overflow(unsigned long secs);
extern int ntp_validate_timex(struct timex *);
extern int __do_adjtimex(struct timex *, struct timespec *, s32 *);
extern void __hardpps(const struct timespec *, const struct timespec *);
=======
extern ktime_t ntp_get_next_leap(void);
extern int second_overflow(time64_t secs);
extern int ntp_validate_timex(struct timex *);
extern int __do_adjtimex(struct timex *, struct timespec64 *, s32 *);
extern void __hardpps(const struct timespec64 *, const struct timespec64 *);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* _LINUX_NTP_INTERNAL_H */
