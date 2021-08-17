/*
 * Copyright (c) 2008-2011 Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <linux/nl80211.h>
#include <linux/delay.h>
#include "ath9k.h"
#include "btcoex.h"

<<<<<<< HEAD
static void ath9k_set_assoc_state(struct ath_softc *sc,
				  struct ieee80211_vif *vif);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
u8 ath9k_parse_mpdudensity(u8 mpdudensity)
{
	/*
	 * 802.11n D2.0 defined values for "Minimum MPDU Start Spacing":
	 *   0 for no restriction
	 *   1 for 1/4 us
	 *   2 for 1/2 us
	 *   3 for 1 us
	 *   4 for 2 us
	 *   5 for 4 us
	 *   6 for 8 us
	 *   7 for 16 us
	 */
	switch (mpdudensity) {
	case 0:
		return 0;
	case 1:
	case 2:
	case 3:
		/* Our lower layer calculations limit our precision to
		   1 microsecond */
		return 1;
	case 4:
		return 2;
	case 5:
		return 4;
	case 6:
		return 8;
	case 7:
		return 16;
	default:
		return 0;
	}
}

<<<<<<< HEAD
static bool ath9k_has_pending_frames(struct ath_softc *sc, struct ath_txq *txq)
=======
static bool ath9k_has_pending_frames(struct ath_softc *sc, struct ath_txq *txq,
				     bool sw_pending)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	bool pending = false;

	spin_lock_bh(&txq->axq_lock);

<<<<<<< HEAD
	if (txq->axq_depth || !list_empty(&txq->axq_acq))
		pending = true;

=======
	if (txq->axq_depth) {
		pending = true;
		goto out;
	}

	if (!sw_pending)
		goto out;

	if (txq->mac80211_qnum >= 0) {
		struct list_head *list;

		list = &sc->cur_chan->acq[txq->mac80211_qnum];
		if (!list_empty(list))
			pending = true;
	}
out:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	spin_unlock_bh(&txq->axq_lock);
	return pending;
}

static bool ath9k_setpower(struct ath_softc *sc, enum ath9k_power_mode mode)
{
	unsigned long flags;
	bool ret;

	spin_lock_irqsave(&sc->sc_pm_lock, flags);
	ret = ath9k_hw_setpower(sc->sc_ah, mode);
	spin_unlock_irqrestore(&sc->sc_pm_lock, flags);

	return ret;
}

<<<<<<< HEAD
=======
void ath_ps_full_sleep(unsigned long data)
{
	struct ath_softc *sc = (struct ath_softc *) data;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	bool reset;

	spin_lock(&common->cc_lock);
	ath_hw_cycle_counters_update(common);
	spin_unlock(&common->cc_lock);

	ath9k_hw_setrxabort(sc->sc_ah, 1);
	ath9k_hw_stopdmarecv(sc->sc_ah, &reset);

	ath9k_hw_setpower(sc->sc_ah, ATH9K_PM_FULL_SLEEP);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void ath9k_ps_wakeup(struct ath_softc *sc)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	unsigned long flags;
	enum ath9k_power_mode power_mode;

	spin_lock_irqsave(&sc->sc_pm_lock, flags);
	if (++sc->ps_usecount != 1)
		goto unlock;

<<<<<<< HEAD
=======
	del_timer_sync(&sc->sleep_timer);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	power_mode = sc->sc_ah->power_mode;
	ath9k_hw_setpower(sc->sc_ah, ATH9K_PM_AWAKE);

	/*
	 * While the hardware is asleep, the cycle counters contain no
	 * useful data. Better clear them now so that they don't mess up
	 * survey data results.
	 */
	if (power_mode != ATH9K_PM_AWAKE) {
		spin_lock(&common->cc_lock);
		ath_hw_cycle_counters_update(common);
		memset(&common->cc_survey, 0, sizeof(common->cc_survey));
		memset(&common->cc_ani, 0, sizeof(common->cc_ani));
		spin_unlock(&common->cc_lock);
	}

 unlock:
	spin_unlock_irqrestore(&sc->sc_pm_lock, flags);
}

void ath9k_ps_restore(struct ath_softc *sc)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	enum ath9k_power_mode mode;
	unsigned long flags;
<<<<<<< HEAD
	bool reset;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	spin_lock_irqsave(&sc->sc_pm_lock, flags);
	if (--sc->ps_usecount != 0)
		goto unlock;

	if (sc->ps_idle) {
<<<<<<< HEAD
		ath9k_hw_setrxabort(sc->sc_ah, 1);
		ath9k_hw_stopdmarecv(sc->sc_ah, &reset);
		mode = ATH9K_PM_FULL_SLEEP;
	} else if (sc->ps_enabled &&
=======
		mod_timer(&sc->sleep_timer, jiffies + HZ / 10);
		goto unlock;
	}

	if (sc->ps_enabled &&
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		   !(sc->ps_flags & (PS_WAIT_FOR_BEACON |
				     PS_WAIT_FOR_CAB |
				     PS_WAIT_FOR_PSPOLL_DATA |
				     PS_WAIT_FOR_TX_ACK |
				     PS_WAIT_FOR_ANI))) {
		mode = ATH9K_PM_NETWORK_SLEEP;
		if (ath9k_hw_btcoex_is_enabled(sc->sc_ah))
			ath9k_btcoex_stop_gen_timer(sc);
	} else {
		goto unlock;
	}

	spin_lock(&common->cc_lock);
	ath_hw_cycle_counters_update(common);
	spin_unlock(&common->cc_lock);

	ath9k_hw_setpower(sc->sc_ah, mode);

 unlock:
	spin_unlock_irqrestore(&sc->sc_pm_lock, flags);
}

static void __ath_cancel_work(struct ath_softc *sc)
{
	cancel_work_sync(&sc->paprd_work);
<<<<<<< HEAD
	cancel_work_sync(&sc->hw_check_work);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	cancel_delayed_work_sync(&sc->tx_complete_work);
	cancel_delayed_work_sync(&sc->hw_pll_work);

#ifdef CONFIG_ATH9K_BTCOEX_SUPPORT
	if (ath9k_hw_mci_is_enabled(sc->sc_ah))
		cancel_work_sync(&sc->mci_work);
#endif
}

<<<<<<< HEAD
static void ath_cancel_work(struct ath_softc *sc)
=======
void ath_cancel_work(struct ath_softc *sc)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	__ath_cancel_work(sc);
	cancel_work_sync(&sc->hw_reset_work);
}

<<<<<<< HEAD
static void ath_restart_work(struct ath_softc *sc)
=======
void ath_restart_work(struct ath_softc *sc)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	ieee80211_queue_delayed_work(sc->hw, &sc->tx_complete_work, 0);

	if (AR_SREV_9340(sc->sc_ah) || AR_SREV_9330(sc->sc_ah))
		ieee80211_queue_delayed_work(sc->hw, &sc->hw_pll_work,
				     msecs_to_jiffies(ATH_PLL_WORK_INTERVAL));

<<<<<<< HEAD
	ath_start_rx_poll(sc, 3);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ath_start_ani(sc);
}

static bool ath_prepare_reset(struct ath_softc *sc)
{
	struct ath_hw *ah = sc->sc_ah;
	bool ret = true;

	ieee80211_stop_queues(sc->hw);
<<<<<<< HEAD

	sc->hw_busy_count = 0;
	ath_stop_ani(sc);
	del_timer_sync(&sc->rx_poll_timer);

	ath9k_debug_samp_bb_mac(sc);
	ath9k_hw_disable_interrupts(ah);

	if (!ath_drain_all_txq(sc))
		ret = false;

	if (!ath_stoprecv(sc))
		ret = false;
=======
	ath_stop_ani(sc);
	ath9k_hw_disable_interrupts(ah);

	if (AR_SREV_9300_20_OR_LATER(ah)) {
		ret &= ath_stoprecv(sc);
		ret &= ath_drain_all_txq(sc);
	} else {
		ret &= ath_drain_all_txq(sc);
		ret &= ath_stoprecv(sc);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return ret;
}

static bool ath_complete_reset(struct ath_softc *sc, bool start)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	unsigned long flags;
<<<<<<< HEAD
	int i;

	if (ath_startrecv(sc) != 0) {
		ath_err(common, "Unable to restart recv logic\n");
		return false;
	}

	ath9k_cmn_update_txpow(ah, sc->curtxpow,
			       sc->config.txpowlimit, &sc->curtxpow);

	clear_bit(SC_OP_HW_RESET, &sc->sc_flags);
	ath9k_hw_set_interrupts(ah);
	ath9k_hw_enable_interrupts(ah);

	if (!(sc->hw->conf.flags & IEEE80211_CONF_OFFCHANNEL) && start) {
		if (!test_bit(SC_OP_BEACONS, &sc->sc_flags))
			goto work;

		if (ah->opmode == NL80211_IFTYPE_STATION &&
		    test_bit(SC_OP_PRIM_STA_VIF, &sc->sc_flags)) {
=======

	ath9k_calculate_summary_state(sc, sc->cur_chan);
	ath_startrecv(sc);
	ath9k_cmn_update_txpow(ah, sc->cur_chan->cur_txpower,
			       sc->cur_chan->txpower,
			       &sc->cur_chan->cur_txpower);
	clear_bit(ATH_OP_HW_RESET, &common->op_flags);

	if (!sc->cur_chan->offchannel && start) {
		/* restore per chanctx TSF timer */
		if (sc->cur_chan->tsf_val) {
			u32 offset;

			offset = ath9k_hw_get_tsf_offset(&sc->cur_chan->tsf_ts,
							 NULL);
			ath9k_hw_settsf64(ah, sc->cur_chan->tsf_val + offset);
		}


		if (!test_bit(ATH_OP_BEACONS, &common->op_flags))
			goto work;

		if (ah->opmode == NL80211_IFTYPE_STATION &&
		    test_bit(ATH_OP_PRIM_STA_VIF, &common->op_flags)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			spin_lock_irqsave(&sc->sc_pm_lock, flags);
			sc->ps_flags |= PS_BEACON_SYNC | PS_WAIT_FOR_BEACON;
			spin_unlock_irqrestore(&sc->sc_pm_lock, flags);
		} else {
			ath9k_set_beacon(sc);
		}
	work:
		ath_restart_work(sc);
<<<<<<< HEAD

		for (i = 0; i < ATH9K_NUM_TX_QUEUES; i++) {
			if (!ATH_TXQ_SETUP(sc, i))
				continue;

			spin_lock_bh(&sc->tx.txq[i].axq_lock);
			ath_txq_schedule(sc, &sc->tx.txq[i]);
			spin_unlock_bh(&sc->tx.txq[i].axq_lock);
		}
	}

	if ((ah->caps.hw_caps & ATH9K_HW_CAP_ANT_DIV_COMB) && sc->ant_rx != 3)
		ath_ant_comb_update(sc);

	ieee80211_wake_queues(sc->hw);
=======
		ath_txq_schedule_all(sc);
	}

	sc->gtt_cnt = 0;

	ath9k_hw_set_interrupts(ah);
	ath9k_hw_enable_interrupts(ah);
	ieee80211_wake_queues(sc->hw);
	ath9k_p2p_ps_timer(sc);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return true;
}

static int ath_reset_internal(struct ath_softc *sc, struct ath9k_channel *hchan)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ath9k_hw_cal_data *caldata = NULL;
	bool fastcc = true;
	int r;

	__ath_cancel_work(sc);

<<<<<<< HEAD
	tasklet_disable(&sc->intr_tq);
	spin_lock_bh(&sc->sc_pcu_lock);

	if (!(sc->hw->conf.flags & IEEE80211_CONF_OFFCHANNEL)) {
		fastcc = false;
		caldata = &sc->caldata;
=======
	disable_irq(sc->irq);
	tasklet_disable(&sc->intr_tq);
	tasklet_disable(&sc->bcon_tasklet);
	spin_lock_bh(&sc->sc_pcu_lock);

	if (!sc->cur_chan->offchannel) {
		fastcc = false;
		caldata = &sc->cur_chan->caldata;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	if (!hchan) {
		fastcc = false;
		hchan = ah->curchan;
	}

<<<<<<< HEAD
	if (!ath_prepare_reset(sc))
		fastcc = false;

=======
	if (!hchan) {
		fastcc = false;
		hchan = ath9k_cmn_get_channel(sc->hw, ah, &sc->cur_chan->chandef);
	}

	if (!ath_prepare_reset(sc))
		fastcc = false;

	if (ath9k_is_chanctx_enabled())
		fastcc = false;

	spin_lock_bh(&sc->chan_lock);
	sc->cur_chandef = sc->cur_chan->chandef;
	spin_unlock_bh(&sc->chan_lock);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ath_dbg(common, CONFIG, "Reset to %u MHz, HT40: %d fastcc: %d\n",
		hchan->channel, IS_CHAN_HT40(hchan), fastcc);

	r = ath9k_hw_reset(ah, hchan, caldata, fastcc);
	if (r) {
		ath_err(common,
			"Unable to reset channel, reset status %d\n", r);

		ath9k_hw_enable_interrupts(ah);
		ath9k_queue_reset(sc, RESET_TYPE_BB_HANG);

		goto out;
	}

	if (ath9k_hw_mci_is_enabled(sc->sc_ah) &&
<<<<<<< HEAD
	    (sc->hw->conf.flags & IEEE80211_CONF_OFFCHANNEL))
=======
	    sc->cur_chan->offchannel)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ath9k_mci_set_txpower(sc, true, false);

	if (!ath_complete_reset(sc, true))
		r = -EIO;

out:
<<<<<<< HEAD
	spin_unlock_bh(&sc->sc_pcu_lock);
=======
	enable_irq(sc->irq);
	spin_unlock_bh(&sc->sc_pcu_lock);
	tasklet_enable(&sc->bcon_tasklet);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	tasklet_enable(&sc->intr_tq);

	return r;
}

<<<<<<< HEAD

/*
 * Set/change channels.  If the channel is really being changed, it's done
 * by reseting the chip.  To accomplish this we must first cleanup any pending
 * DMA, then restart stuff.
*/
static int ath_set_channel(struct ath_softc *sc, struct ieee80211_hw *hw,
		    struct ath9k_channel *hchan)
{
	int r;

	if (test_bit(SC_OP_INVALID, &sc->sc_flags))
		return -EIO;

	r = ath_reset_internal(sc, hchan);

	return r;
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void ath_node_attach(struct ath_softc *sc, struct ieee80211_sta *sta,
			    struct ieee80211_vif *vif)
{
	struct ath_node *an;
	an = (struct ath_node *)sta->drv_priv;

	an->sc = sc;
	an->sta = sta;
	an->vif = vif;
<<<<<<< HEAD

	ath_tx_node_init(sc, an);

	if (sta->ht_cap.ht_supported) {
		an->maxampdu = 1 << (IEEE80211_HT_MAX_AMPDU_FACTOR +
				     sta->ht_cap.ampdu_factor);
		an->mpdudensity = ath9k_parse_mpdudensity(sta->ht_cap.ampdu_density);
	}
=======
	memset(&an->key_idx, 0, sizeof(an->key_idx));

	ath_tx_node_init(sc, an);

	ath_dynack_node_init(sc->sc_ah, an);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void ath_node_detach(struct ath_softc *sc, struct ieee80211_sta *sta)
{
	struct ath_node *an = (struct ath_node *)sta->drv_priv;
	ath_tx_node_cleanup(sc, an);
<<<<<<< HEAD
=======

	ath_dynack_node_deinit(sc->sc_ah, an);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void ath9k_tasklet(unsigned long data)
{
	struct ath_softc *sc = (struct ath_softc *)data;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	enum ath_reset_type type;
	unsigned long flags;
<<<<<<< HEAD
	u32 status = sc->intrstatus;
	u32 rxmask;

	ath9k_ps_wakeup(sc);
	spin_lock(&sc->sc_pcu_lock);

	if ((status & ATH9K_INT_FATAL) ||
	    (status & ATH9K_INT_BB_WATCHDOG)) {

		if (status & ATH9K_INT_FATAL)
			type = RESET_TYPE_FATAL_INT;
		else
			type = RESET_TYPE_BB_WATCHDOG;

		ath9k_queue_reset(sc, type);
		goto out;
=======
	u32 status;
	u32 rxmask;

	spin_lock_irqsave(&sc->intr_lock, flags);
	status = sc->intrstatus;
	sc->intrstatus = 0;
	spin_unlock_irqrestore(&sc->intr_lock, flags);

	ath9k_ps_wakeup(sc);
	spin_lock(&sc->sc_pcu_lock);

	if (status & ATH9K_INT_FATAL) {
		type = RESET_TYPE_FATAL_INT;
		ath9k_queue_reset(sc, type);
		ath_dbg(common, RESET, "FATAL: Skipping interrupts\n");
		goto out;
	}

	if ((ah->config.hw_hang_checks & HW_BB_WATCHDOG) &&
	    (status & ATH9K_INT_BB_WATCHDOG)) {
		spin_lock(&common->cc_lock);
		ath_hw_cycle_counters_update(common);
		ar9003_hw_bb_watchdog_dbg_info(ah);
		spin_unlock(&common->cc_lock);

		if (ar9003_hw_bb_watchdog_check(ah)) {
			type = RESET_TYPE_BB_WATCHDOG;
			ath9k_queue_reset(sc, type);

			ath_dbg(common, RESET,
				"BB_WATCHDOG: Skipping interrupts\n");
			goto out;
		}
	}

	if (status & ATH9K_INT_GTT) {
		sc->gtt_cnt++;

		if ((sc->gtt_cnt >= MAX_GTT_CNT) && !ath9k_hw_check_alive(ah)) {
			type = RESET_TYPE_TX_GTT;
			ath9k_queue_reset(sc, type);
			ath_dbg(common, RESET,
				"GTT: Skipping interrupts\n");
			goto out;
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	spin_lock_irqsave(&sc->sc_pm_lock, flags);
	if ((status & ATH9K_INT_TSFOOR) && sc->ps_enabled) {
		/*
		 * TSF sync does not look correct; remain awake to sync with
		 * the next Beacon.
		 */
		ath_dbg(common, PS, "TSFOOR - Sync with next Beacon\n");
		sc->ps_flags |= PS_WAIT_FOR_BEACON | PS_BEACON_SYNC;
	}
	spin_unlock_irqrestore(&sc->sc_pm_lock, flags);

	if (ah->caps.hw_caps & ATH9K_HW_CAP_EDMA)
		rxmask = (ATH9K_INT_RXHP | ATH9K_INT_RXLP | ATH9K_INT_RXEOL |
			  ATH9K_INT_RXORN);
	else
		rxmask = (ATH9K_INT_RX | ATH9K_INT_RXEOL | ATH9K_INT_RXORN);

	if (status & rxmask) {
		/* Check for high priority Rx first */
		if ((ah->caps.hw_caps & ATH9K_HW_CAP_EDMA) &&
		    (status & ATH9K_INT_RXHP))
			ath_rx_tasklet(sc, 0, true);

		ath_rx_tasklet(sc, 0, false);
	}

	if (status & ATH9K_INT_TX) {
<<<<<<< HEAD
		if (ah->caps.hw_caps & ATH9K_HW_CAP_EDMA)
			ath_tx_edma_tasklet(sc);
		else
			ath_tx_tasklet(sc);
	}

	ath9k_btcoex_handle_interrupt(sc, status);

out:
	/* re-enable hardware interrupt */
	ath9k_hw_enable_interrupts(ah);

=======
		if (ah->caps.hw_caps & ATH9K_HW_CAP_EDMA) {
			/*
			 * For EDMA chips, TX completion is enabled for the
			 * beacon queue, so if a beacon has been transmitted
			 * successfully after a GTT interrupt, the GTT counter
			 * gets reset to zero here.
			 */
			sc->gtt_cnt = 0;

			ath_tx_edma_tasklet(sc);
		} else {
			ath_tx_tasklet(sc);
		}

		wake_up(&sc->tx_wait);
	}

	if (status & ATH9K_INT_GENTIMER)
		ath_gen_timer_isr(sc->sc_ah);

	ath9k_btcoex_handle_interrupt(sc, status);

	/* re-enable hardware interrupt */
	ath9k_hw_resume_interrupts(ah);
out:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	spin_unlock(&sc->sc_pcu_lock);
	ath9k_ps_restore(sc);
}

irqreturn_t ath_isr(int irq, void *dev)
{
#define SCHED_INTR (				\
		ATH9K_INT_FATAL |		\
		ATH9K_INT_BB_WATCHDOG |		\
		ATH9K_INT_RXORN |		\
		ATH9K_INT_RXEOL |		\
		ATH9K_INT_RX |			\
		ATH9K_INT_RXLP |		\
		ATH9K_INT_RXHP |		\
		ATH9K_INT_TX |			\
		ATH9K_INT_BMISS |		\
		ATH9K_INT_CST |			\
<<<<<<< HEAD
=======
		ATH9K_INT_GTT |			\
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ATH9K_INT_TSFOOR |		\
		ATH9K_INT_GENTIMER |		\
		ATH9K_INT_MCI)

	struct ath_softc *sc = dev;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	enum ath9k_int status;
<<<<<<< HEAD
=======
	u32 sync_cause = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	bool sched = false;

	/*
	 * The hardware is not ready/present, don't
	 * touch anything. Note this can happen early
	 * on if the IRQ is shared.
	 */
<<<<<<< HEAD
	if (test_bit(SC_OP_INVALID, &sc->sc_flags))
		return IRQ_NONE;

	/* shared irq, not for us */

	if (!ath9k_hw_intrpend(ah))
		return IRQ_NONE;

	if (test_bit(SC_OP_HW_RESET, &sc->sc_flags)) {
		ath9k_hw_kill_interrupts(ah);
		return IRQ_HANDLED;
	}

=======
	if (!ah || test_bit(ATH_OP_INVALID, &common->op_flags))
		return IRQ_NONE;

	/* shared irq, not for us */
	if (!ath9k_hw_intrpend(ah))
		return IRQ_NONE;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * Figure out the reason(s) for the interrupt.  Note
	 * that the hal returns a pseudo-ISR that may include
	 * bits we haven't explicitly enabled so we mask the
	 * value to insure we only process bits we requested.
	 */
<<<<<<< HEAD
	ath9k_hw_getisr(ah, &status);	/* NB: clears ISR too */
	status &= ah->imask;	/* discard unasked-for bits */

=======
	ath9k_hw_getisr(ah, &status, &sync_cause); /* NB: clears ISR too */
	ath9k_debug_sync_cause(sc, sync_cause);
	status &= ah->imask;	/* discard unasked-for bits */

	if (test_bit(ATH_OP_HW_RESET, &common->op_flags))
		return IRQ_HANDLED;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * If there are no status bits set, then this interrupt was not
	 * for me (should have been caught above).
	 */
	if (!status)
		return IRQ_NONE;

	/* Cache the status */
<<<<<<< HEAD
	sc->intrstatus = status;
=======
	spin_lock(&sc->intr_lock);
	sc->intrstatus |= status;
	spin_unlock(&sc->intr_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (status & SCHED_INTR)
		sched = true;

	/*
<<<<<<< HEAD
	 * If a FATAL or RXORN interrupt is received, we have to reset the
	 * chip immediately.
	 */
	if ((status & ATH9K_INT_FATAL) || ((status & ATH9K_INT_RXORN) &&
	    !(ah->caps.hw_caps & ATH9K_HW_CAP_EDMA)))
		goto chip_reset;

	if ((ah->caps.hw_caps & ATH9K_HW_CAP_EDMA) &&
	    (status & ATH9K_INT_BB_WATCHDOG)) {

		spin_lock(&common->cc_lock);
		ath_hw_cycle_counters_update(common);
		ar9003_hw_bb_watchdog_dbg_info(ah);
		spin_unlock(&common->cc_lock);

		goto chip_reset;
	}
#ifdef CONFIG_PM_SLEEP
	if (status & ATH9K_INT_BMISS) {
		if (atomic_read(&sc->wow_sleep_proc_intr) == 0) {
			ath_dbg(common, ANY, "during WoW we got a BMISS\n");
			atomic_inc(&sc->wow_got_bmiss_intr);
			atomic_dec(&sc->wow_sleep_proc_intr);
		}
	}
#endif
=======
	 * If a FATAL interrupt is received, we have to reset the chip
	 * immediately.
	 */
	if (status & ATH9K_INT_FATAL)
		goto chip_reset;

	if ((ah->config.hw_hang_checks & HW_BB_WATCHDOG) &&
	    (status & ATH9K_INT_BB_WATCHDOG))
		goto chip_reset;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (status & ATH9K_INT_SWBA)
		tasklet_schedule(&sc->bcon_tasklet);

	if (status & ATH9K_INT_TXURN)
		ath9k_hw_updatetxtriglevel(ah, true);

	if (status & ATH9K_INT_RXEOL) {
		ah->imask &= ~(ATH9K_INT_RXEOL | ATH9K_INT_RXORN);
		ath9k_hw_set_interrupts(ah);
	}

	if (!(ah->caps.hw_caps & ATH9K_HW_CAP_AUTOSLEEP))
		if (status & ATH9K_INT_TIM_TIMER) {
			if (ATH_DBG_WARN_ON_ONCE(sc->ps_idle))
				goto chip_reset;
			/* Clear RxAbort bit so that we can
			 * receive frames */
			ath9k_setpower(sc, ATH9K_PM_AWAKE);
			spin_lock(&sc->sc_pm_lock);
			ath9k_hw_setrxabort(sc->sc_ah, 0);
			sc->ps_flags |= PS_WAIT_FOR_BEACON;
			spin_unlock(&sc->sc_pm_lock);
		}

chip_reset:

	ath_debug_stat_interrupt(sc, status);

	if (sched) {
		/* turn off every interrupt */
<<<<<<< HEAD
		ath9k_hw_disable_interrupts(ah);
=======
		ath9k_hw_kill_interrupts(ah);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		tasklet_schedule(&sc->intr_tq);
	}

	return IRQ_HANDLED;

#undef SCHED_INTR
}

<<<<<<< HEAD
static int ath_reset(struct ath_softc *sc)
{
	int r;

	ath9k_ps_wakeup(sc);
	r = ath_reset_internal(sc, NULL);
=======
/*
 * This function is called when a HW reset cannot be deferred
 * and has to be immediate.
 */
int ath_reset(struct ath_softc *sc, struct ath9k_channel *hchan)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	int r;

	ath9k_hw_kill_interrupts(sc->sc_ah);
	set_bit(ATH_OP_HW_RESET, &common->op_flags);

	ath9k_ps_wakeup(sc);
	r = ath_reset_internal(sc, hchan);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ath9k_ps_restore(sc);

	return r;
}

<<<<<<< HEAD
void ath9k_queue_reset(struct ath_softc *sc, enum ath_reset_type type)
{
#ifdef CONFIG_ATH9K_DEBUGFS
	RESET_STAT_INC(sc, type);
#endif
	set_bit(SC_OP_HW_RESET, &sc->sc_flags);
=======
/*
 * When a HW reset can be deferred, it is added to the
 * hw_reset_work workqueue, but we set ATH_OP_HW_RESET before
 * queueing.
 */
void ath9k_queue_reset(struct ath_softc *sc, enum ath_reset_type type)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
#ifdef CONFIG_ATH9K_DEBUGFS
	RESET_STAT_INC(sc, type);
#endif
	ath9k_hw_kill_interrupts(sc->sc_ah);
	set_bit(ATH_OP_HW_RESET, &common->op_flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ieee80211_queue_work(sc->hw, &sc->hw_reset_work);
}

void ath_reset_work(struct work_struct *work)
{
	struct ath_softc *sc = container_of(work, struct ath_softc, hw_reset_work);

<<<<<<< HEAD
	ath_reset(sc);
=======
	ath9k_ps_wakeup(sc);
	ath_reset_internal(sc, NULL);
	ath9k_ps_restore(sc);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**********************/
/* mac80211 callbacks */
/**********************/

static int ath9k_start(struct ieee80211_hw *hw)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
<<<<<<< HEAD
	struct ieee80211_channel *curchan = hw->conf.chandef.chan;
=======
	struct ieee80211_channel *curchan = sc->cur_chan->chandef.chan;
	struct ath_chanctx *ctx = sc->cur_chan;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct ath9k_channel *init_channel;
	int r;

	ath_dbg(common, CONFIG,
		"Starting driver with initial channel: %d MHz\n",
		curchan->center_freq);

	ath9k_ps_wakeup(sc);
	mutex_lock(&sc->mutex);

<<<<<<< HEAD
	init_channel = ath9k_cmn_get_curchannel(hw, ah);
=======
	init_channel = ath9k_cmn_get_channel(hw, ah, &ctx->chandef);
	sc->cur_chandef = hw->conf.chandef;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Reset SERDES registers */
	ath9k_hw_configpcipowersave(ah, false);

	/*
	 * The basic interface to setting the hardware in a good
	 * state is ``reset''.  On return the hardware is known to
	 * be powered up and with interrupts disabled.  This must
	 * be followed by initialization of the appropriate bits
	 * and then setup of the interrupt mask.
	 */
	spin_lock_bh(&sc->sc_pcu_lock);

	atomic_set(&ah->intr_ref_cnt, -1);

	r = ath9k_hw_reset(ah, init_channel, ah->caldata, false);
	if (r) {
		ath_err(common,
			"Unable to reset hardware; reset status %d (freq %u MHz)\n",
			r, curchan->center_freq);
		ah->reset_power_on = false;
	}

	/* Setup our intr mask. */
	ah->imask = ATH9K_INT_TX | ATH9K_INT_RXEOL |
		    ATH9K_INT_RXORN | ATH9K_INT_FATAL |
		    ATH9K_INT_GLOBAL;

	if (ah->caps.hw_caps & ATH9K_HW_CAP_EDMA)
		ah->imask |= ATH9K_INT_RXHP |
<<<<<<< HEAD
			     ATH9K_INT_RXLP |
			     ATH9K_INT_BB_WATCHDOG;
	else
		ah->imask |= ATH9K_INT_RX;

	ah->imask |= ATH9K_INT_GTT;
=======
			     ATH9K_INT_RXLP;
	else
		ah->imask |= ATH9K_INT_RX;

	if (ah->config.hw_hang_checks & HW_BB_WATCHDOG)
		ah->imask |= ATH9K_INT_BB_WATCHDOG;

	/*
	 * Enable GTT interrupts only for AR9003/AR9004 chips
	 * for now.
	 */
	if (AR_SREV_9300_20_OR_LATER(ah))
		ah->imask |= ATH9K_INT_GTT;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (ah->caps.hw_caps & ATH9K_HW_CAP_HT)
		ah->imask |= ATH9K_INT_CST;

	ath_mci_enable(sc);

<<<<<<< HEAD
	clear_bit(SC_OP_INVALID, &sc->sc_flags);
=======
	clear_bit(ATH_OP_INVALID, &common->op_flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	sc->sc_ah->is_monitoring = false;

	if (!ath_complete_reset(sc, false))
		ah->reset_power_on = false;

	if (ah->led_pin >= 0) {
<<<<<<< HEAD
		ath9k_hw_cfg_output(ah, ah->led_pin,
				    AR_GPIO_OUTPUT_MUX_AS_OUTPUT);
		ath9k_hw_set_gpio(ah, ah->led_pin, 0);
=======
		ath9k_hw_set_gpio(ah, ah->led_pin,
				  (ah->config.led_active_high) ? 1 : 0);
		ath9k_hw_gpio_request_out(ah, ah->led_pin, NULL,
					  AR_GPIO_OUTPUT_MUX_AS_OUTPUT);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/*
	 * Reset key cache to sane defaults (all entries cleared) instead of
	 * semi-random values after suspend/resume.
	 */
	ath9k_cmn_init_crypto(sc->sc_ah);

<<<<<<< HEAD
=======
	ath9k_hw_reset_tsf(ah);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	spin_unlock_bh(&sc->sc_pcu_lock);

	mutex_unlock(&sc->mutex);

	ath9k_ps_restore(sc);

<<<<<<< HEAD
=======
	ath9k_rng_start(sc);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static void ath9k_tx(struct ieee80211_hw *hw,
		     struct ieee80211_tx_control *control,
		     struct sk_buff *skb)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_tx_control txctl;
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *) skb->data;
	unsigned long flags;

	if (sc->ps_enabled) {
		/*
		 * mac80211 does not set PM field for normal data frames, so we
		 * need to update that based on the current PS mode.
		 */
		if (ieee80211_is_data(hdr->frame_control) &&
		    !ieee80211_is_nullfunc(hdr->frame_control) &&
		    !ieee80211_has_pm(hdr->frame_control)) {
			ath_dbg(common, PS,
				"Add PM=1 for a TX frame while in PS mode\n");
			hdr->frame_control |= cpu_to_le16(IEEE80211_FCTL_PM);
		}
	}

	if (unlikely(sc->sc_ah->power_mode == ATH9K_PM_NETWORK_SLEEP)) {
		/*
		 * We are using PS-Poll and mac80211 can request TX while in
		 * power save mode. Need to wake up hardware for the TX to be
		 * completed and if needed, also for RX of buffered frames.
		 */
		ath9k_ps_wakeup(sc);
		spin_lock_irqsave(&sc->sc_pm_lock, flags);
		if (!(sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_AUTOSLEEP))
			ath9k_hw_setrxabort(sc->sc_ah, 0);
		if (ieee80211_is_pspoll(hdr->frame_control)) {
			ath_dbg(common, PS,
				"Sending PS-Poll to pick a buffered frame\n");
			sc->ps_flags |= PS_WAIT_FOR_PSPOLL_DATA;
		} else {
			ath_dbg(common, PS, "Wake up to complete TX\n");
			sc->ps_flags |= PS_WAIT_FOR_TX_ACK;
		}
		/*
		 * The actual restore operation will happen only after
		 * the ps_flags bit is cleared. We are just dropping
		 * the ps_usecount here.
		 */
		spin_unlock_irqrestore(&sc->sc_pm_lock, flags);
		ath9k_ps_restore(sc);
	}

	/*
	 * Cannot tx while the hardware is in full sleep, it first needs a full
	 * chip reset to recover from that
	 */
	if (unlikely(sc->sc_ah->power_mode == ATH9K_PM_FULL_SLEEP)) {
		ath_err(common, "TX while HW is in FULL_SLEEP mode\n");
		goto exit;
	}

	memset(&txctl, 0, sizeof(struct ath_tx_control));
	txctl.txq = sc->tx.txq_map[skb_get_queue_mapping(skb)];
	txctl.sta = control->sta;

	ath_dbg(common, XMIT, "transmitting packet, skb: %p\n", skb);

	if (ath_tx_start(hw, skb, &txctl) != 0) {
		ath_dbg(common, XMIT, "TX failed\n");
		TX_STAT_INC(txctl.txq->axq_qnum, txfailed);
		goto exit;
	}

	return;
exit:
	ieee80211_free_txskb(hw, skb);
}

static void ath9k_stop(struct ieee80211_hw *hw)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	bool prev_idle;

<<<<<<< HEAD
	mutex_lock(&sc->mutex);

	ath_cancel_work(sc);
	del_timer_sync(&sc->rx_poll_timer);

	if (test_bit(SC_OP_INVALID, &sc->sc_flags)) {
=======
	ath9k_deinit_channel_context(sc);

	ath9k_rng_stop(sc);

	mutex_lock(&sc->mutex);

	ath_cancel_work(sc);

	if (test_bit(ATH_OP_INVALID, &common->op_flags)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ath_dbg(common, ANY, "Device not present\n");
		mutex_unlock(&sc->mutex);
		return;
	}

	/* Ensure HW is awake when we try to shut it down. */
	ath9k_ps_wakeup(sc);

	spin_lock_bh(&sc->sc_pcu_lock);

	/* prevent tasklets to enable interrupts once we disable them */
	ah->imask &= ~ATH9K_INT_GLOBAL;

	/* make sure h/w will not generate any interrupt
	 * before setting the invalid flag. */
	ath9k_hw_disable_interrupts(ah);

	spin_unlock_bh(&sc->sc_pcu_lock);

	/* we can now sync irq and kill any running tasklets, since we already
	 * disabled interrupts and not holding a spin lock */
	synchronize_irq(sc->irq);
	tasklet_kill(&sc->intr_tq);
	tasklet_kill(&sc->bcon_tasklet);

	prev_idle = sc->ps_idle;
	sc->ps_idle = true;

	spin_lock_bh(&sc->sc_pcu_lock);

	if (ah->led_pin >= 0) {
<<<<<<< HEAD
		ath9k_hw_set_gpio(ah, ah->led_pin, 1);
		ath9k_hw_cfg_gpio_input(ah, ah->led_pin);
=======
		ath9k_hw_set_gpio(ah, ah->led_pin,
				  (ah->config.led_active_high) ? 0 : 1);
		ath9k_hw_gpio_request_in(ah, ah->led_pin, NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	ath_prepare_reset(sc);

	if (sc->rx.frag) {
		dev_kfree_skb_any(sc->rx.frag);
		sc->rx.frag = NULL;
	}

	if (!ah->curchan)
<<<<<<< HEAD
		ah->curchan = ath9k_cmn_get_curchannel(hw, ah);

	ath9k_hw_reset(ah, ah->curchan, ah->caldata, false);
=======
		ah->curchan = ath9k_cmn_get_channel(hw, ah,
						    &sc->cur_chan->chandef);

	ath9k_hw_reset(ah, ah->curchan, ah->caldata, false);

	set_bit(ATH_OP_INVALID, &common->op_flags);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ath9k_hw_phy_disable(ah);

	ath9k_hw_configpcipowersave(ah, true);

	spin_unlock_bh(&sc->sc_pcu_lock);

	ath9k_ps_restore(sc);

<<<<<<< HEAD
	set_bit(SC_OP_INVALID, &sc->sc_flags);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	sc->ps_idle = prev_idle;

	mutex_unlock(&sc->mutex);

	ath_dbg(common, CONFIG, "Driver halt\n");
}

<<<<<<< HEAD
bool ath9k_uses_beacons(int type)
=======
static bool ath9k_uses_beacons(int type)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	switch (type) {
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_ADHOC:
	case NL80211_IFTYPE_MESH_POINT:
		return true;
	default:
		return false;
	}
}

<<<<<<< HEAD
static void ath9k_vif_iter(void *data, u8 *mac, struct ieee80211_vif *vif)
{
	struct ath9k_vif_iter_data *iter_data = data;
=======
static void ath9k_vif_iter_set_beacon(struct ath9k_vif_iter_data *iter_data,
				      struct ieee80211_vif *vif)
{
	/* Use the first (configured) interface, but prefering AP interfaces. */
	if (!iter_data->primary_beacon_vif) {
		iter_data->primary_beacon_vif = vif;
	} else {
		if (iter_data->primary_beacon_vif->type != NL80211_IFTYPE_AP &&
		    vif->type == NL80211_IFTYPE_AP)
			iter_data->primary_beacon_vif = vif;
	}

	iter_data->beacons = true;
	iter_data->nbcnvifs += 1;
}

static void ath9k_vif_iter(struct ath9k_vif_iter_data *iter_data,
			   u8 *mac, struct ieee80211_vif *vif)
{
	struct ath_vif *avp = (struct ath_vif *)vif->drv_priv;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int i;

	if (iter_data->has_hw_macaddr) {
		for (i = 0; i < ETH_ALEN; i++)
			iter_data->mask[i] &=
				~(iter_data->hw_macaddr[i] ^ mac[i]);
	} else {
		memcpy(iter_data->hw_macaddr, mac, ETH_ALEN);
		iter_data->has_hw_macaddr = true;
	}

<<<<<<< HEAD
	switch (vif->type) {
	case NL80211_IFTYPE_AP:
		iter_data->naps++;
		break;
	case NL80211_IFTYPE_STATION:
		iter_data->nstations++;
		break;
	case NL80211_IFTYPE_ADHOC:
		iter_data->nadhocs++;
		break;
	case NL80211_IFTYPE_MESH_POINT:
		iter_data->nmeshes++;
=======
	if (!vif->bss_conf.use_short_slot)
		iter_data->slottime = 20;

	switch (vif->type) {
	case NL80211_IFTYPE_AP:
		iter_data->naps++;
		if (vif->bss_conf.enable_beacon)
			ath9k_vif_iter_set_beacon(iter_data, vif);
		break;
	case NL80211_IFTYPE_STATION:
		iter_data->nstations++;
		if (avp->assoc && !iter_data->primary_sta)
			iter_data->primary_sta = vif;
		break;
	case NL80211_IFTYPE_OCB:
		iter_data->nocbs++;
		break;
	case NL80211_IFTYPE_ADHOC:
		iter_data->nadhocs++;
		if (vif->bss_conf.enable_beacon)
			ath9k_vif_iter_set_beacon(iter_data, vif);
		break;
	case NL80211_IFTYPE_MESH_POINT:
		iter_data->nmeshes++;
		if (vif->bss_conf.enable_beacon)
			ath9k_vif_iter_set_beacon(iter_data, vif);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;
	case NL80211_IFTYPE_WDS:
		iter_data->nwds++;
		break;
	default:
		break;
	}
}

<<<<<<< HEAD
static void ath9k_sta_vif_iter(void *data, u8 *mac, struct ieee80211_vif *vif)
{
	struct ath_softc *sc = data;
	struct ath_vif *avp = (void *)vif->drv_priv;

	if (vif->type != NL80211_IFTYPE_STATION)
		return;

	if (avp->primary_sta_vif)
		ath9k_set_assoc_state(sc, vif);
}

/* Called with sc->mutex held. */
void ath9k_calculate_iter_data(struct ieee80211_hw *hw,
			       struct ieee80211_vif *vif,
			       struct ath9k_vif_iter_data *iter_data)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);

	/*
	 * Pick the MAC address of the first interface as the new hardware
	 * MAC address. The hardware will use it together with the BSSID mask
	 * when matching addresses.
	 */
	memset(iter_data, 0, sizeof(*iter_data));
	memset(&iter_data->mask, 0xff, ETH_ALEN);

	if (vif)
		ath9k_vif_iter(iter_data, vif->addr, vif);

	/* Get list of all active MAC addresses */
	ieee80211_iterate_active_interfaces_atomic(
		sc->hw, IEEE80211_IFACE_ITER_RESUME_ALL,
		ath9k_vif_iter, iter_data);

	memcpy(common->macaddr, iter_data->hw_macaddr, ETH_ALEN);
}

/* Called with sc->mutex held. */
static void ath9k_calculate_summary_state(struct ieee80211_hw *hw,
					  struct ieee80211_vif *vif)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ath9k_vif_iter_data iter_data;
	enum nl80211_iftype old_opmode = ah->opmode;

	ath9k_calculate_iter_data(hw, vif, &iter_data);
=======
static void ath9k_update_bssid_mask(struct ath_softc *sc,
				    struct ath_chanctx *ctx,
				    struct ath9k_vif_iter_data *iter_data)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_vif *avp;
	int i;

	if (!ath9k_is_chanctx_enabled())
		return;

	list_for_each_entry(avp, &ctx->vifs, list) {
		if (ctx->nvifs_assigned != 1)
			continue;

		if (!iter_data->has_hw_macaddr)
			continue;

		ether_addr_copy(common->curbssid, avp->bssid);

		/* perm_addr will be used as the p2p device address. */
		for (i = 0; i < ETH_ALEN; i++)
			iter_data->mask[i] &=
				~(iter_data->hw_macaddr[i] ^
				  sc->hw->wiphy->perm_addr[i]);
	}
}

/* Called with sc->mutex held. */
void ath9k_calculate_iter_data(struct ath_softc *sc,
			       struct ath_chanctx *ctx,
			       struct ath9k_vif_iter_data *iter_data)
{
	struct ath_vif *avp;

	/*
	 * The hardware will use primary station addr together with the
	 * BSSID mask when matching addresses.
	 */
	memset(iter_data, 0, sizeof(*iter_data));
	eth_broadcast_addr(iter_data->mask);
	iter_data->slottime = 9;

	list_for_each_entry(avp, &ctx->vifs, list)
		ath9k_vif_iter(iter_data, avp->vif->addr, avp->vif);

	ath9k_update_bssid_mask(sc, ctx, iter_data);
}

static void ath9k_set_assoc_state(struct ath_softc *sc,
				  struct ieee80211_vif *vif, bool changed)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_vif *avp = (struct ath_vif *)vif->drv_priv;
	unsigned long flags;

	set_bit(ATH_OP_PRIM_STA_VIF, &common->op_flags);

	ether_addr_copy(common->curbssid, avp->bssid);
	common->curaid = avp->aid;
	ath9k_hw_write_associd(sc->sc_ah);

	if (changed) {
		common->last_rssi = ATH_RSSI_DUMMY_MARKER;
		sc->sc_ah->stats.avgbrssi = ATH_RSSI_DUMMY_MARKER;

		spin_lock_irqsave(&sc->sc_pm_lock, flags);
		sc->ps_flags |= PS_BEACON_SYNC | PS_WAIT_FOR_BEACON;
		spin_unlock_irqrestore(&sc->sc_pm_lock, flags);
	}

	if (ath9k_hw_mci_is_enabled(sc->sc_ah))
		ath9k_mci_update_wlan_channels(sc, false);

	ath_dbg(common, CONFIG,
		"Primary Station interface: %pM, BSSID: %pM\n",
		vif->addr, common->curbssid);
}

#ifdef CONFIG_ATH9K_CHANNEL_CONTEXT
static void ath9k_set_offchannel_state(struct ath_softc *sc)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ieee80211_vif *vif = NULL;

	ath9k_ps_wakeup(sc);

	if (sc->offchannel.state < ATH_OFFCHANNEL_ROC_START)
		vif = sc->offchannel.scan_vif;
	else
		vif = sc->offchannel.roc_vif;

	if (WARN_ON(!vif))
		goto exit;

	eth_zero_addr(common->curbssid);
	eth_broadcast_addr(common->bssidmask);
	memcpy(common->macaddr, vif->addr, ETH_ALEN);
	common->curaid = 0;
	ah->opmode = vif->type;
	ah->imask &= ~ATH9K_INT_SWBA;
	ah->imask &= ~ATH9K_INT_TSFOOR;
	ah->slottime = 9;

	ath_hw_setbssidmask(common);
	ath9k_hw_setopmode(ah);
	ath9k_hw_write_associd(sc->sc_ah);
	ath9k_hw_set_interrupts(ah);
	ath9k_hw_init_global_settings(ah);

exit:
	ath9k_ps_restore(sc);
}
#endif

/* Called with sc->mutex held. */
void ath9k_calculate_summary_state(struct ath_softc *sc,
				   struct ath_chanctx *ctx)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ath9k_vif_iter_data iter_data;

	ath_chanctx_check_active(sc, ctx);

	if (ctx != sc->cur_chan)
		return;

#ifdef CONFIG_ATH9K_CHANNEL_CONTEXT
	if (ctx == &sc->offchannel.chan)
		return ath9k_set_offchannel_state(sc);
#endif

	ath9k_ps_wakeup(sc);
	ath9k_calculate_iter_data(sc, ctx, &iter_data);

	if (iter_data.has_hw_macaddr)
		memcpy(common->macaddr, iter_data.hw_macaddr, ETH_ALEN);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	memcpy(common->bssidmask, iter_data.mask, ETH_ALEN);
	ath_hw_setbssidmask(common);

	if (iter_data.naps > 0) {
		ath9k_hw_set_tsfadjust(ah, true);
		ah->opmode = NL80211_IFTYPE_AP;
	} else {
		ath9k_hw_set_tsfadjust(ah, false);
<<<<<<< HEAD

		if (iter_data.nmeshes)
			ah->opmode = NL80211_IFTYPE_MESH_POINT;
=======
		if (iter_data.beacons)
			ath9k_beacon_ensure_primary_slot(sc);

		if (iter_data.nmeshes)
			ah->opmode = NL80211_IFTYPE_MESH_POINT;
		else if (iter_data.nocbs)
			ah->opmode = NL80211_IFTYPE_OCB;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		else if (iter_data.nwds)
			ah->opmode = NL80211_IFTYPE_AP;
		else if (iter_data.nadhocs)
			ah->opmode = NL80211_IFTYPE_ADHOC;
		else
			ah->opmode = NL80211_IFTYPE_STATION;
	}

	ath9k_hw_setopmode(ah);

<<<<<<< HEAD
	if ((iter_data.nstations + iter_data.nadhocs + iter_data.nmeshes) > 0)
		ah->imask |= ATH9K_INT_TSFOOR;
	else
		ah->imask &= ~ATH9K_INT_TSFOOR;

	ath9k_hw_set_interrupts(ah);

	/*
	 * If we are changing the opmode to STATION,
	 * a beacon sync needs to be done.
	 */
	if (ah->opmode == NL80211_IFTYPE_STATION &&
	    old_opmode == NL80211_IFTYPE_AP &&
	    test_bit(SC_OP_PRIM_STA_VIF, &sc->sc_flags)) {
		ieee80211_iterate_active_interfaces_atomic(
			sc->hw, IEEE80211_IFACE_ITER_RESUME_ALL,
			ath9k_sta_vif_iter, sc);
	}
=======
	ctx->switch_after_beacon = false;
	if ((iter_data.nstations + iter_data.nadhocs + iter_data.nmeshes) > 0)
		ah->imask |= ATH9K_INT_TSFOOR;
	else {
		ah->imask &= ~ATH9K_INT_TSFOOR;
		if (iter_data.naps == 1 && iter_data.beacons)
			ctx->switch_after_beacon = true;
	}

	if (ah->opmode == NL80211_IFTYPE_STATION) {
		bool changed = (iter_data.primary_sta != ctx->primary_sta);

		if (iter_data.primary_sta) {
			iter_data.primary_beacon_vif = iter_data.primary_sta;
			iter_data.beacons = true;
			ath9k_set_assoc_state(sc, iter_data.primary_sta,
					      changed);
			ctx->primary_sta = iter_data.primary_sta;
		} else {
			ctx->primary_sta = NULL;
			eth_zero_addr(common->curbssid);
			common->curaid = 0;
			ath9k_hw_write_associd(sc->sc_ah);
			if (ath9k_hw_mci_is_enabled(sc->sc_ah))
				ath9k_mci_update_wlan_channels(sc, true);
		}
	}
	sc->nbcnvifs = iter_data.nbcnvifs;
	ath9k_beacon_config(sc, iter_data.primary_beacon_vif,
			    iter_data.beacons);
	ath9k_hw_set_interrupts(ah);

	if (ah->slottime != iter_data.slottime) {
		ah->slottime = iter_data.slottime;
		ath9k_hw_init_global_settings(ah);
	}

	if (iter_data.primary_sta)
		set_bit(ATH_OP_PRIM_STA_VIF, &common->op_flags);
	else
		clear_bit(ATH_OP_PRIM_STA_VIF, &common->op_flags);

	ath_dbg(common, CONFIG,
		"macaddr: %pM, bssid: %pM, bssidmask: %pM\n",
		common->macaddr, common->curbssid, common->bssidmask);

	ath9k_ps_restore(sc);
}

static void ath9k_tpc_vif_iter(void *data, u8 *mac, struct ieee80211_vif *vif)
{
	int *power = (int *)data;

	if (*power < vif->bss_conf.txpower)
		*power = vif->bss_conf.txpower;
}

/* Called with sc->mutex held. */
void ath9k_set_txpower(struct ath_softc *sc, struct ieee80211_vif *vif)
{
	int power;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_regulatory *reg = ath9k_hw_regulatory(ah);

	ath9k_ps_wakeup(sc);
	if (ah->tpc_enabled) {
		power = (vif) ? vif->bss_conf.txpower : -1;
		ieee80211_iterate_active_interfaces_atomic(
				sc->hw, IEEE80211_IFACE_ITER_RESUME_ALL,
				ath9k_tpc_vif_iter, &power);
		if (power == -1)
			power = sc->hw->conf.power_level;
	} else {
		power = sc->hw->conf.power_level;
	}
	sc->cur_chan->txpower = 2 * power;
	ath9k_hw_set_txpowerlimit(ah, sc->cur_chan->txpower, false);
	sc->cur_chan->cur_txpower = reg->max_power_level;
	ath9k_ps_restore(sc);
}

static void ath9k_assign_hw_queues(struct ieee80211_hw *hw,
				   struct ieee80211_vif *vif)
{
	int i;

	if (!ath9k_is_chanctx_enabled())
		return;

	for (i = 0; i < IEEE80211_NUM_ACS; i++)
		vif->hw_queue[i] = i;

	if (vif->type == NL80211_IFTYPE_AP ||
	    vif->type == NL80211_IFTYPE_MESH_POINT)
		vif->cab_queue = hw->queues - 2;
	else
		vif->cab_queue = IEEE80211_INVAL_HW_QUEUE;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int ath9k_add_interface(struct ieee80211_hw *hw,
			       struct ieee80211_vif *vif)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
<<<<<<< HEAD

	mutex_lock(&sc->mutex);

	ath_dbg(common, CONFIG, "Attach a VIF of type: %d\n", vif->type);
	sc->nvifs++;

	ath9k_ps_wakeup(sc);
	ath9k_calculate_summary_state(hw, vif);
	ath9k_ps_restore(sc);
=======
	struct ath_vif *avp = (void *)vif->drv_priv;
	struct ath_node *an = &avp->mcast_node;

	mutex_lock(&sc->mutex);
	if (IS_ENABLED(CONFIG_ATH9K_TX99)) {
		if (sc->cur_chan->nvifs >= 1) {
			mutex_unlock(&sc->mutex);
			return -EOPNOTSUPP;
		}
		sc->tx99_vif = vif;
	}

	ath_dbg(common, CONFIG, "Attach a VIF of type: %d\n", vif->type);
	sc->cur_chan->nvifs++;

	if (vif->type == NL80211_IFTYPE_STATION && ath9k_is_chanctx_enabled())
		vif->driver_flags |= IEEE80211_VIF_GET_NOA_UPDATE;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (ath9k_uses_beacons(vif->type))
		ath9k_beacon_assign_slot(sc, vif);

<<<<<<< HEAD
	mutex_unlock(&sc->mutex);
	return 0;
}
=======
	avp->vif = vif;
	if (!ath9k_is_chanctx_enabled()) {
		avp->chanctx = sc->cur_chan;
		list_add_tail(&avp->list, &avp->chanctx->vifs);
	}

	ath9k_calculate_summary_state(sc, avp->chanctx);

	ath9k_assign_hw_queues(hw, vif);

	ath9k_set_txpower(sc, vif);

	an->sc = sc;
	an->sta = NULL;
	an->vif = vif;
	an->no_ps_filter = true;
	ath_tx_node_init(sc, an);

	mutex_unlock(&sc->mutex);
	return 0;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static int ath9k_change_interface(struct ieee80211_hw *hw,
				  struct ieee80211_vif *vif,
				  enum nl80211_iftype new_type,
				  bool p2p)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
<<<<<<< HEAD

	ath_dbg(common, CONFIG, "Change Interface\n");
	mutex_lock(&sc->mutex);

=======
	struct ath_vif *avp = (void *)vif->drv_priv;

	mutex_lock(&sc->mutex);

	if (IS_ENABLED(CONFIG_ATH9K_TX99)) {
		mutex_unlock(&sc->mutex);
		return -EOPNOTSUPP;
	}

	ath_dbg(common, CONFIG, "Change Interface\n");

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (ath9k_uses_beacons(vif->type))
		ath9k_beacon_remove_slot(sc, vif);

	vif->type = new_type;
	vif->p2p = p2p;

<<<<<<< HEAD
	ath9k_ps_wakeup(sc);
	ath9k_calculate_summary_state(hw, vif);
	ath9k_ps_restore(sc);

	if (ath9k_uses_beacons(vif->type))
		ath9k_beacon_assign_slot(sc, vif);

=======
	if (ath9k_uses_beacons(vif->type))
		ath9k_beacon_assign_slot(sc, vif);

	ath9k_assign_hw_queues(hw, vif);
	ath9k_calculate_summary_state(sc, avp->chanctx);

	ath9k_set_txpower(sc, vif);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mutex_unlock(&sc->mutex);
	return 0;
}

static void ath9k_remove_interface(struct ieee80211_hw *hw,
				   struct ieee80211_vif *vif)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
<<<<<<< HEAD
=======
	struct ath_vif *avp = (void *)vif->drv_priv;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ath_dbg(common, CONFIG, "Detach Interface\n");

	mutex_lock(&sc->mutex);

<<<<<<< HEAD
	sc->nvifs--;
=======
	ath9k_p2p_remove_vif(sc, vif);

	sc->cur_chan->nvifs--;
	sc->tx99_vif = NULL;
	if (!ath9k_is_chanctx_enabled())
		list_del(&avp->list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (ath9k_uses_beacons(vif->type))
		ath9k_beacon_remove_slot(sc, vif);

<<<<<<< HEAD
	ath9k_ps_wakeup(sc);
	ath9k_calculate_summary_state(hw, NULL);
	ath9k_ps_restore(sc);
=======
	ath_tx_node_cleanup(sc, &avp->mcast_node);

	ath9k_calculate_summary_state(sc, avp->chanctx);

	ath9k_set_txpower(sc, NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	mutex_unlock(&sc->mutex);
}

static void ath9k_enable_ps(struct ath_softc *sc)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);

<<<<<<< HEAD
=======
	if (IS_ENABLED(CONFIG_ATH9K_TX99))
		return;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	sc->ps_enabled = true;
	if (!(ah->caps.hw_caps & ATH9K_HW_CAP_AUTOSLEEP)) {
		if ((ah->imask & ATH9K_INT_TIM_TIMER) == 0) {
			ah->imask |= ATH9K_INT_TIM_TIMER;
			ath9k_hw_set_interrupts(ah);
		}
		ath9k_hw_setrxabort(ah, 1);
	}
	ath_dbg(common, PS, "PowerSave enabled\n");
}

static void ath9k_disable_ps(struct ath_softc *sc)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);

<<<<<<< HEAD
=======
	if (IS_ENABLED(CONFIG_ATH9K_TX99))
		return;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	sc->ps_enabled = false;
	ath9k_hw_setpower(ah, ATH9K_PM_AWAKE);
	if (!(ah->caps.hw_caps & ATH9K_HW_CAP_AUTOSLEEP)) {
		ath9k_hw_setrxabort(ah, 0);
		sc->ps_flags &= ~(PS_WAIT_FOR_BEACON |
				  PS_WAIT_FOR_CAB |
				  PS_WAIT_FOR_PSPOLL_DATA |
				  PS_WAIT_FOR_TX_ACK);
		if (ah->imask & ATH9K_INT_TIM_TIMER) {
			ah->imask &= ~ATH9K_INT_TIM_TIMER;
			ath9k_hw_set_interrupts(ah);
		}
	}
	ath_dbg(common, PS, "PowerSave disabled\n");
}

<<<<<<< HEAD
void ath9k_spectral_scan_trigger(struct ieee80211_hw *hw)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	u32 rxfilter;

	if (!ath9k_hw_ops(ah)->spectral_scan_trigger) {
		ath_err(common, "spectrum analyzer not implemented on this hardware\n");
		return;
	}

	ath9k_ps_wakeup(sc);
	rxfilter = ath9k_hw_getrxfilter(ah);
	ath9k_hw_setrxfilter(ah, rxfilter |
				 ATH9K_RX_FILTER_PHYRADAR |
				 ATH9K_RX_FILTER_PHYERR);

	/* TODO: usually this should not be neccesary, but for some reason
	 * (or in some mode?) the trigger must be called after the
	 * configuration, otherwise the register will have its values reset
	 * (on my ar9220 to value 0x01002310)
	 */
	ath9k_spectral_scan_config(hw, sc->spectral_mode);
	ath9k_hw_ops(ah)->spectral_scan_trigger(ah);
	ath9k_ps_restore(sc);
}

int ath9k_spectral_scan_config(struct ieee80211_hw *hw,
			       enum spectral_mode spectral_mode)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);

	if (!ath9k_hw_ops(ah)->spectral_scan_trigger) {
		ath_err(common, "spectrum analyzer not implemented on this hardware\n");
		return -1;
	}

	switch (spectral_mode) {
	case SPECTRAL_DISABLED:
		sc->spec_config.enabled = 0;
		break;
	case SPECTRAL_BACKGROUND:
		/* send endless samples.
		 * TODO: is this really useful for "background"?
		 */
		sc->spec_config.endless = 1;
		sc->spec_config.enabled = 1;
		break;
	case SPECTRAL_CHANSCAN:
	case SPECTRAL_MANUAL:
		sc->spec_config.endless = 0;
		sc->spec_config.enabled = 1;
		break;
	default:
		return -1;
	}

	ath9k_ps_wakeup(sc);
	ath9k_hw_ops(ah)->spectral_scan_config(ah, &sc->spec_config);
	ath9k_ps_restore(sc);

	sc->spectral_mode = spectral_mode;

	return 0;
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int ath9k_config(struct ieee80211_hw *hw, u32 changed)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ieee80211_conf *conf = &hw->conf;
<<<<<<< HEAD
	bool reset_channel = false;
=======
	struct ath_chanctx *ctx = sc->cur_chan;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ath9k_ps_wakeup(sc);
	mutex_lock(&sc->mutex);

	if (changed & IEEE80211_CONF_CHANGE_IDLE) {
		sc->ps_idle = !!(conf->flags & IEEE80211_CONF_IDLE);
		if (sc->ps_idle) {
			ath_cancel_work(sc);
			ath9k_stop_btcoex(sc);
		} else {
			ath9k_start_btcoex(sc);
			/*
			 * The chip needs a reset to properly wake up from
			 * full sleep
			 */
<<<<<<< HEAD
			reset_channel = ah->chip_fullsleep;
=======
			ath_chanctx_set_channel(sc, ctx, &ctx->chandef);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}

	/*
	 * We just prepare to enable PS. We have to wait until our AP has
	 * ACK'd our null data frame to disable RX otherwise we'll ignore
	 * those ACKs and end up retransmitting the same null data frames.
	 * IEEE80211_CONF_CHANGE_PS is only passed by mac80211 for STA mode.
	 */
	if (changed & IEEE80211_CONF_CHANGE_PS) {
		unsigned long flags;
		spin_lock_irqsave(&sc->sc_pm_lock, flags);
		if (conf->flags & IEEE80211_CONF_PS)
			ath9k_enable_ps(sc);
		else
			ath9k_disable_ps(sc);
		spin_unlock_irqrestore(&sc->sc_pm_lock, flags);
	}

	if (changed & IEEE80211_CONF_CHANGE_MONITOR) {
		if (conf->flags & IEEE80211_CONF_MONITOR) {
			ath_dbg(common, CONFIG, "Monitor mode is enabled\n");
			sc->sc_ah->is_monitoring = true;
		} else {
			ath_dbg(common, CONFIG, "Monitor mode is disabled\n");
			sc->sc_ah->is_monitoring = false;
		}
	}

<<<<<<< HEAD
	if ((changed & IEEE80211_CONF_CHANGE_CHANNEL) || reset_channel) {
		struct ieee80211_channel *curchan = hw->conf.chandef.chan;
		enum nl80211_channel_type channel_type =
			cfg80211_get_chandef_type(&conf->chandef);
		int pos = curchan->hw_value;
		int old_pos = -1;
		unsigned long flags;

		if (ah->curchan)
			old_pos = ah->curchan - &ah->channels[0];

		ath_dbg(common, CONFIG, "Set channel: %d MHz type: %d\n",
			curchan->center_freq, channel_type);

		/* update survey stats for the old channel before switching */
		spin_lock_irqsave(&common->cc_lock, flags);
		ath_update_survey_stats(sc);
		spin_unlock_irqrestore(&common->cc_lock, flags);

		ath9k_cmn_update_ichannel(&sc->sc_ah->channels[pos],
					  curchan, channel_type);

		/*
		 * If the operating channel changes, change the survey in-use flags
		 * along with it.
		 * Reset the survey data for the new channel, unless we're switching
		 * back to the operating channel from an off-channel operation.
		 */
		if (!(hw->conf.flags & IEEE80211_CONF_OFFCHANNEL) &&
		    sc->cur_survey != &sc->survey[pos]) {

			if (sc->cur_survey)
				sc->cur_survey->filled &= ~SURVEY_INFO_IN_USE;

			sc->cur_survey = &sc->survey[pos];

			memset(sc->cur_survey, 0, sizeof(struct survey_info));
			sc->cur_survey->filled |= SURVEY_INFO_IN_USE;
		} else if (!(sc->survey[pos].filled & SURVEY_INFO_IN_USE)) {
			memset(&sc->survey[pos], 0, sizeof(struct survey_info));
		}

		if (ath_set_channel(sc, hw, &sc->sc_ah->channels[pos]) < 0) {
			ath_err(common, "Unable to set channel\n");
			mutex_unlock(&sc->mutex);
			ath9k_ps_restore(sc);
			return -EINVAL;
		}

		/*
		 * The most recent snapshot of channel->noisefloor for the old
		 * channel is only available after the hardware reset. Copy it to
		 * the survey stats now.
		 */
		if (old_pos >= 0)
			ath_update_survey_nf(sc, old_pos);

		/*
		 * Enable radar pulse detection if on a DFS channel. Spectral
		 * scanning and radar detection can not be used concurrently.
		 */
		if (hw->conf.radar_enabled) {
			u32 rxfilter;

			/* set HW specific DFS configuration */
			ath9k_hw_set_radar_params(ah);
			rxfilter = ath9k_hw_getrxfilter(ah);
			rxfilter |= ATH9K_RX_FILTER_PHYRADAR |
				    ATH9K_RX_FILTER_PHYERR;
			ath9k_hw_setrxfilter(ah, rxfilter);
			ath_dbg(common, DFS, "DFS enabled at freq %d\n",
				curchan->center_freq);
		} else {
			/* perform spectral scan if requested. */
			if (sc->scanning &&
			    sc->spectral_mode == SPECTRAL_CHANSCAN)
				ath9k_spectral_scan_trigger(hw);
		}
	}

	if (changed & IEEE80211_CONF_CHANGE_POWER) {
		ath_dbg(common, CONFIG, "Set power: %d\n", conf->power_level);
		sc->config.txpowlimit = 2 * conf->power_level;
		ath9k_cmn_update_txpow(ah, sc->curtxpow,
				       sc->config.txpowlimit, &sc->curtxpow);
	}
=======
	if (!ath9k_is_chanctx_enabled() && (changed & IEEE80211_CONF_CHANGE_CHANNEL)) {
		ctx->offchannel = !!(conf->flags & IEEE80211_CONF_OFFCHANNEL);
		ath_chanctx_set_channel(sc, ctx, &hw->conf.chandef);
	}

	if (changed & IEEE80211_CONF_CHANGE_POWER)
		ath9k_set_txpower(sc, NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	mutex_unlock(&sc->mutex);
	ath9k_ps_restore(sc);

	return 0;
}

#define SUPPORTED_FILTERS			\
<<<<<<< HEAD
	(FIF_PROMISC_IN_BSS |			\
	FIF_ALLMULTI |				\
=======
	(FIF_ALLMULTI |				\
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	FIF_CONTROL |				\
	FIF_PSPOLL |				\
	FIF_OTHER_BSS |				\
	FIF_BCN_PRBRESP_PROMISC |		\
	FIF_PROBE_REQ |				\
	FIF_FCSFAIL)

/* FIXME: sc->sc_full_reset ? */
static void ath9k_configure_filter(struct ieee80211_hw *hw,
				   unsigned int changed_flags,
				   unsigned int *total_flags,
				   u64 multicast)
{
	struct ath_softc *sc = hw->priv;
<<<<<<< HEAD
=======
	struct ath_chanctx *ctx;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	u32 rfilt;

	changed_flags &= SUPPORTED_FILTERS;
	*total_flags &= SUPPORTED_FILTERS;

<<<<<<< HEAD
	sc->rx.rxfilter = *total_flags;
=======
	spin_lock_bh(&sc->chan_lock);
	ath_for_each_chanctx(sc, ctx)
		ctx->rxfilter = *total_flags;
#ifdef CONFIG_ATH9K_CHANNEL_CONTEXT
	sc->offchannel.chan.rxfilter = *total_flags;
#endif
	spin_unlock_bh(&sc->chan_lock);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ath9k_ps_wakeup(sc);
	rfilt = ath_calcrxfilter(sc);
	ath9k_hw_setrxfilter(sc->sc_ah, rfilt);
	ath9k_ps_restore(sc);

	ath_dbg(ath9k_hw_common(sc->sc_ah), CONFIG, "Set HW RX filter: 0x%x\n",
		rfilt);
}

static int ath9k_sta_add(struct ieee80211_hw *hw,
			 struct ieee80211_vif *vif,
			 struct ieee80211_sta *sta)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_node *an = (struct ath_node *) sta->drv_priv;
	struct ieee80211_key_conf ps_key = { };
	int key;

	ath_node_attach(sc, sta, vif);

	if (vif->type != NL80211_IFTYPE_AP &&
	    vif->type != NL80211_IFTYPE_AP_VLAN)
		return 0;

	key = ath_key_config(common, vif, sta, &ps_key);
<<<<<<< HEAD
	if (key > 0)
		an->ps_key = key;
=======
	if (key > 0) {
		an->ps_key = key;
		an->key_idx[0] = key;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

static void ath9k_del_ps_key(struct ath_softc *sc,
			     struct ieee80211_vif *vif,
			     struct ieee80211_sta *sta)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_node *an = (struct ath_node *) sta->drv_priv;
	struct ieee80211_key_conf ps_key = { .hw_key_idx = an->ps_key };

	if (!an->ps_key)
	    return;

	ath_key_delete(common, &ps_key);
	an->ps_key = 0;
<<<<<<< HEAD
=======
	an->key_idx[0] = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int ath9k_sta_remove(struct ieee80211_hw *hw,
			    struct ieee80211_vif *vif,
			    struct ieee80211_sta *sta)
{
	struct ath_softc *sc = hw->priv;

	ath9k_del_ps_key(sc, vif, sta);
	ath_node_detach(sc, sta);

	return 0;
}

<<<<<<< HEAD
=======
static int ath9k_sta_state(struct ieee80211_hw *hw,
			   struct ieee80211_vif *vif,
			   struct ieee80211_sta *sta,
			   enum ieee80211_sta_state old_state,
			   enum ieee80211_sta_state new_state)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	int ret = 0;

	if (old_state == IEEE80211_STA_NOTEXIST &&
	    new_state == IEEE80211_STA_NONE) {
		ret = ath9k_sta_add(hw, vif, sta);
		ath_dbg(common, CONFIG,
			"Add station: %pM\n", sta->addr);
	} else if (old_state == IEEE80211_STA_NONE &&
		   new_state == IEEE80211_STA_NOTEXIST) {
		ret = ath9k_sta_remove(hw, vif, sta);
		ath_dbg(common, CONFIG,
			"Remove station: %pM\n", sta->addr);
	}

	if (ath9k_is_chanctx_enabled()) {
		if (vif->type == NL80211_IFTYPE_STATION) {
			if (old_state == IEEE80211_STA_ASSOC &&
			    new_state == IEEE80211_STA_AUTHORIZED)
				ath_chanctx_event(sc, vif,
						  ATH_CHANCTX_EVENT_AUTHORIZED);
		}
	}

	return ret;
}

static void ath9k_sta_set_tx_filter(struct ath_hw *ah,
				    struct ath_node *an,
				    bool set)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(an->key_idx); i++) {
		if (!an->key_idx[i])
			continue;
		ath9k_hw_set_tx_filter(ah, an->key_idx[i], set);
	}
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void ath9k_sta_notify(struct ieee80211_hw *hw,
			 struct ieee80211_vif *vif,
			 enum sta_notify_cmd cmd,
			 struct ieee80211_sta *sta)
{
	struct ath_softc *sc = hw->priv;
	struct ath_node *an = (struct ath_node *) sta->drv_priv;

<<<<<<< HEAD
	if (!sta->ht_cap.ht_supported)
		return;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	switch (cmd) {
	case STA_NOTIFY_SLEEP:
		an->sleeping = true;
		ath_tx_aggr_sleep(sta, sc, an);
<<<<<<< HEAD
		break;
	case STA_NOTIFY_AWAKE:
=======
		ath9k_sta_set_tx_filter(sc->sc_ah, an, true);
		break;
	case STA_NOTIFY_AWAKE:
		ath9k_sta_set_tx_filter(sc->sc_ah, an, false);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		an->sleeping = false;
		ath_tx_aggr_wakeup(sc, an);
		break;
	}
}

static int ath9k_conf_tx(struct ieee80211_hw *hw,
			 struct ieee80211_vif *vif, u16 queue,
			 const struct ieee80211_tx_queue_params *params)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_txq *txq;
	struct ath9k_tx_queue_info qi;
	int ret = 0;

	if (queue >= IEEE80211_NUM_ACS)
		return 0;

	txq = sc->tx.txq_map[queue];

	ath9k_ps_wakeup(sc);
	mutex_lock(&sc->mutex);

	memset(&qi, 0, sizeof(struct ath9k_tx_queue_info));

	qi.tqi_aifs = params->aifs;
	qi.tqi_cwmin = params->cw_min;
	qi.tqi_cwmax = params->cw_max;
	qi.tqi_burstTime = params->txop * 32;

	ath_dbg(common, CONFIG,
		"Configure tx [queue/halq] [%d/%d], aifs: %d, cw_min: %d, cw_max: %d, txop: %d\n",
		queue, txq->axq_qnum, params->aifs, params->cw_min,
		params->cw_max, params->txop);

	ath_update_max_aggr_framelen(sc, queue, qi.tqi_burstTime);
	ret = ath_txq_update(sc, txq->axq_qnum, &qi);
	if (ret)
		ath_err(common, "TXQ Update failed\n");

	mutex_unlock(&sc->mutex);
	ath9k_ps_restore(sc);

	return ret;
}

static int ath9k_set_key(struct ieee80211_hw *hw,
			 enum set_key_cmd cmd,
			 struct ieee80211_vif *vif,
			 struct ieee80211_sta *sta,
			 struct ieee80211_key_conf *key)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
<<<<<<< HEAD
	int ret = 0;
=======
	struct ath_node *an = NULL;
	int ret = 0, i;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (ath9k_modparam_nohwcrypt)
		return -ENOSPC;

	if ((vif->type == NL80211_IFTYPE_ADHOC ||
	     vif->type == NL80211_IFTYPE_MESH_POINT) &&
	    (key->cipher == WLAN_CIPHER_SUITE_TKIP ||
	     key->cipher == WLAN_CIPHER_SUITE_CCMP) &&
	    !(key->flags & IEEE80211_KEY_FLAG_PAIRWISE)) {
		/*
		 * For now, disable hw crypto for the RSN IBSS group keys. This
		 * could be optimized in the future to use a modified key cache
		 * design to support per-STA RX GTK, but until that gets
		 * implemented, use of software crypto for group addressed
		 * frames is a acceptable to allow RSN IBSS to be used.
		 */
		return -EOPNOTSUPP;
	}

	mutex_lock(&sc->mutex);
	ath9k_ps_wakeup(sc);
<<<<<<< HEAD
	ath_dbg(common, CONFIG, "Set HW Key\n");
=======
	ath_dbg(common, CONFIG, "Set HW Key %d\n", cmd);
	if (sta)
		an = (struct ath_node *)sta->drv_priv;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	switch (cmd) {
	case SET_KEY:
		if (sta)
			ath9k_del_ps_key(sc, vif, sta);

<<<<<<< HEAD
=======
		key->hw_key_idx = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ret = ath_key_config(common, vif, sta, key);
		if (ret >= 0) {
			key->hw_key_idx = ret;
			/* push IV and Michael MIC generation to stack */
			key->flags |= IEEE80211_KEY_FLAG_GENERATE_IV;
			if (key->cipher == WLAN_CIPHER_SUITE_TKIP)
				key->flags |= IEEE80211_KEY_FLAG_GENERATE_MMIC;
<<<<<<< HEAD
			if (sc->sc_ah->sw_mgmt_crypto &&
=======
			if (sc->sc_ah->sw_mgmt_crypto_tx &&
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			    key->cipher == WLAN_CIPHER_SUITE_CCMP)
				key->flags |= IEEE80211_KEY_FLAG_SW_MGMT_TX;
			ret = 0;
		}
<<<<<<< HEAD
		break;
	case DISABLE_KEY:
		ath_key_delete(common, key);
=======
		if (an && key->hw_key_idx) {
			for (i = 0; i < ARRAY_SIZE(an->key_idx); i++) {
				if (an->key_idx[i])
					continue;
				an->key_idx[i] = key->hw_key_idx;
				break;
			}
			WARN_ON(i == ARRAY_SIZE(an->key_idx));
		}
		break;
	case DISABLE_KEY:
		ath_key_delete(common, key);
		if (an) {
			for (i = 0; i < ARRAY_SIZE(an->key_idx); i++) {
				if (an->key_idx[i] != key->hw_key_idx)
					continue;
				an->key_idx[i] = 0;
				break;
			}
		}
		key->hw_key_idx = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;
	default:
		ret = -EINVAL;
	}

	ath9k_ps_restore(sc);
	mutex_unlock(&sc->mutex);

	return ret;
}

<<<<<<< HEAD
static void ath9k_set_assoc_state(struct ath_softc *sc,
				  struct ieee80211_vif *vif)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_vif *avp = (void *)vif->drv_priv;
	struct ieee80211_bss_conf *bss_conf = &vif->bss_conf;
	unsigned long flags;

	set_bit(SC_OP_PRIM_STA_VIF, &sc->sc_flags);
	avp->primary_sta_vif = true;

	/*
	 * Set the AID, BSSID and do beacon-sync only when
	 * the HW opmode is STATION.
	 *
	 * But the primary bit is set above in any case.
	 */
	if (sc->sc_ah->opmode != NL80211_IFTYPE_STATION)
		return;

	memcpy(common->curbssid, bss_conf->bssid, ETH_ALEN);
	common->curaid = bss_conf->aid;
	ath9k_hw_write_associd(sc->sc_ah);

	sc->last_rssi = ATH_RSSI_DUMMY_MARKER;
	sc->sc_ah->stats.avgbrssi = ATH_RSSI_DUMMY_MARKER;

	spin_lock_irqsave(&sc->sc_pm_lock, flags);
	sc->ps_flags |= PS_BEACON_SYNC | PS_WAIT_FOR_BEACON;
	spin_unlock_irqrestore(&sc->sc_pm_lock, flags);

	if (ath9k_hw_mci_is_enabled(sc->sc_ah))
		ath9k_mci_update_wlan_channels(sc, false);

	ath_dbg(common, CONFIG,
		"Primary Station interface: %pM, BSSID: %pM\n",
		vif->addr, common->curbssid);
}

static void ath9k_bss_assoc_iter(void *data, u8 *mac, struct ieee80211_vif *vif)
{
	struct ath_softc *sc = data;
	struct ieee80211_bss_conf *bss_conf = &vif->bss_conf;

	if (test_bit(SC_OP_PRIM_STA_VIF, &sc->sc_flags))
		return;

	if (bss_conf->assoc)
		ath9k_set_assoc_state(sc, vif);
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void ath9k_bss_info_changed(struct ieee80211_hw *hw,
				   struct ieee80211_vif *vif,
				   struct ieee80211_bss_conf *bss_conf,
				   u32 changed)
{
#define CHECK_ANI				\
	(BSS_CHANGED_ASSOC |			\
	 BSS_CHANGED_IBSS |			\
	 BSS_CHANGED_BEACON_ENABLED)

	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ath_vif *avp = (void *)vif->drv_priv;
	int slottime;

	ath9k_ps_wakeup(sc);
	mutex_lock(&sc->mutex);

	if (changed & BSS_CHANGED_ASSOC) {
		ath_dbg(common, CONFIG, "BSSID %pM Changed ASSOC %d\n",
			bss_conf->bssid, bss_conf->assoc);

<<<<<<< HEAD
		if (avp->primary_sta_vif && !bss_conf->assoc) {
			clear_bit(SC_OP_PRIM_STA_VIF, &sc->sc_flags);
			avp->primary_sta_vif = false;

			if (ah->opmode == NL80211_IFTYPE_STATION)
				clear_bit(SC_OP_BEACONS, &sc->sc_flags);
		}

		ieee80211_iterate_active_interfaces_atomic(
			sc->hw, IEEE80211_IFACE_ITER_RESUME_ALL,
			ath9k_bss_assoc_iter, sc);

		if (!test_bit(SC_OP_PRIM_STA_VIF, &sc->sc_flags) &&
		    ah->opmode == NL80211_IFTYPE_STATION) {
			memset(common->curbssid, 0, ETH_ALEN);
			common->curaid = 0;
			ath9k_hw_write_associd(sc->sc_ah);
			if (ath9k_hw_mci_is_enabled(sc->sc_ah))
				ath9k_mci_update_wlan_channels(sc, true);
		}
	}

	if (changed & BSS_CHANGED_IBSS) {
=======
		memcpy(avp->bssid, bss_conf->bssid, ETH_ALEN);
		avp->aid = bss_conf->aid;
		avp->assoc = bss_conf->assoc;

		ath9k_calculate_summary_state(sc, avp->chanctx);
	}

	if ((changed & BSS_CHANGED_IBSS) ||
	      (changed & BSS_CHANGED_OCB)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		memcpy(common->curbssid, bss_conf->bssid, ETH_ALEN);
		common->curaid = bss_conf->aid;
		ath9k_hw_write_associd(sc->sc_ah);
	}

	if ((changed & BSS_CHANGED_BEACON_ENABLED) ||
<<<<<<< HEAD
	    (changed & BSS_CHANGED_BEACON_INT)) {
		if (ah->opmode == NL80211_IFTYPE_AP &&
		    bss_conf->enable_beacon)
			ath9k_set_tsfadjust(sc, vif);
		if (ath9k_allow_beacon_config(sc, vif))
			ath9k_beacon_config(sc, vif, changed);
	}

	if (changed & BSS_CHANGED_ERP_SLOT) {
=======
	    (changed & BSS_CHANGED_BEACON_INT) ||
	    (changed & BSS_CHANGED_BEACON_INFO)) {
		ath9k_calculate_summary_state(sc, avp->chanctx);
	}

	if ((avp->chanctx == sc->cur_chan) &&
	    (changed & BSS_CHANGED_ERP_SLOT)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (bss_conf->use_short_slot)
			slottime = 9;
		else
			slottime = 20;
<<<<<<< HEAD
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (vif->type == NL80211_IFTYPE_AP) {
			/*
			 * Defer update, so that connected stations can adjust
			 * their settings at the same time.
			 * See beacon.c for more details
			 */
			sc->beacon.slottime = slottime;
			sc->beacon.updateslot = UPDATE;
		} else {
			ah->slottime = slottime;
			ath9k_hw_init_global_settings(ah);
		}
	}

<<<<<<< HEAD
	if (changed & CHECK_ANI)
		ath_check_ani(sc);

=======
	if (changed & BSS_CHANGED_P2P_PS)
		ath9k_p2p_bss_info_changed(sc, vif);

	if (changed & CHECK_ANI)
		ath_check_ani(sc);

	if (changed & BSS_CHANGED_TXPOWER) {
		ath_dbg(common, CONFIG, "vif %pM power %d dbm power_type %d\n",
			vif->addr, bss_conf->txpower, bss_conf->txpower_type);
		ath9k_set_txpower(sc, vif);
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mutex_unlock(&sc->mutex);
	ath9k_ps_restore(sc);

#undef CHECK_ANI
}

static u64 ath9k_get_tsf(struct ieee80211_hw *hw, struct ieee80211_vif *vif)
{
	struct ath_softc *sc = hw->priv;
<<<<<<< HEAD
=======
	struct ath_vif *avp = (void *)vif->drv_priv;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	u64 tsf;

	mutex_lock(&sc->mutex);
	ath9k_ps_wakeup(sc);
<<<<<<< HEAD
	tsf = ath9k_hw_gettsf64(sc->sc_ah);
=======
	/* Get current TSF either from HW or kernel time. */
	if (sc->cur_chan == avp->chanctx) {
		tsf = ath9k_hw_gettsf64(sc->sc_ah);
	} else {
		tsf = sc->cur_chan->tsf_val +
		      ath9k_hw_get_tsf_offset(&sc->cur_chan->tsf_ts, NULL);
	}
	tsf += le64_to_cpu(avp->tsf_adjust);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ath9k_ps_restore(sc);
	mutex_unlock(&sc->mutex);

	return tsf;
}

static void ath9k_set_tsf(struct ieee80211_hw *hw,
			  struct ieee80211_vif *vif,
			  u64 tsf)
{
	struct ath_softc *sc = hw->priv;
<<<<<<< HEAD

	mutex_lock(&sc->mutex);
	ath9k_ps_wakeup(sc);
	ath9k_hw_settsf64(sc->sc_ah, tsf);
=======
	struct ath_vif *avp = (void *)vif->drv_priv;

	mutex_lock(&sc->mutex);
	ath9k_ps_wakeup(sc);
	tsf -= le64_to_cpu(avp->tsf_adjust);
	getrawmonotonic(&avp->chanctx->tsf_ts);
	if (sc->cur_chan == avp->chanctx)
		ath9k_hw_settsf64(sc->sc_ah, tsf);
	avp->chanctx->tsf_val = tsf;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ath9k_ps_restore(sc);
	mutex_unlock(&sc->mutex);
}

static void ath9k_reset_tsf(struct ieee80211_hw *hw, struct ieee80211_vif *vif)
{
	struct ath_softc *sc = hw->priv;
<<<<<<< HEAD
=======
	struct ath_vif *avp = (void *)vif->drv_priv;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	mutex_lock(&sc->mutex);

	ath9k_ps_wakeup(sc);
<<<<<<< HEAD
	ath9k_hw_reset_tsf(sc->sc_ah);
=======
	getrawmonotonic(&avp->chanctx->tsf_ts);
	if (sc->cur_chan == avp->chanctx)
		ath9k_hw_reset_tsf(sc->sc_ah);
	avp->chanctx->tsf_val = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ath9k_ps_restore(sc);

	mutex_unlock(&sc->mutex);
}

static int ath9k_ampdu_action(struct ieee80211_hw *hw,
			      struct ieee80211_vif *vif,
<<<<<<< HEAD
			      enum ieee80211_ampdu_mlme_action action,
			      struct ieee80211_sta *sta,
			      u16 tid, u16 *ssn, u8 buf_size)
{
	struct ath_softc *sc = hw->priv;
	bool flush = false;
	int ret = 0;

	local_bh_disable();
=======
			      struct ieee80211_ampdu_params *params)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	bool flush = false;
	int ret = 0;
	struct ieee80211_sta *sta = params->sta;
	enum ieee80211_ampdu_mlme_action action = params->action;
	u16 tid = params->tid;
	u16 *ssn = &params->ssn;

	mutex_lock(&sc->mutex);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	switch (action) {
	case IEEE80211_AMPDU_RX_START:
		break;
	case IEEE80211_AMPDU_RX_STOP:
		break;
	case IEEE80211_AMPDU_TX_START:
<<<<<<< HEAD
=======
		if (ath9k_is_chanctx_enabled()) {
			if (test_bit(ATH_OP_SCANNING, &common->op_flags)) {
				ret = -EBUSY;
				break;
			}
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ath9k_ps_wakeup(sc);
		ret = ath_tx_aggr_start(sc, sta, tid, ssn);
		if (!ret)
			ieee80211_start_tx_ba_cb_irqsafe(vif, sta->addr, tid);
		ath9k_ps_restore(sc);
		break;
	case IEEE80211_AMPDU_TX_STOP_FLUSH:
	case IEEE80211_AMPDU_TX_STOP_FLUSH_CONT:
		flush = true;
	case IEEE80211_AMPDU_TX_STOP_CONT:
		ath9k_ps_wakeup(sc);
		ath_tx_aggr_stop(sc, sta, tid);
		if (!flush)
			ieee80211_stop_tx_ba_cb_irqsafe(vif, sta->addr, tid);
		ath9k_ps_restore(sc);
		break;
	case IEEE80211_AMPDU_TX_OPERATIONAL:
		ath9k_ps_wakeup(sc);
		ath_tx_aggr_resume(sc, sta, tid);
		ath9k_ps_restore(sc);
		break;
	default:
		ath_err(ath9k_hw_common(sc->sc_ah), "Unknown AMPDU action\n");
	}

<<<<<<< HEAD
	local_bh_enable();
=======
	mutex_unlock(&sc->mutex);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return ret;
}

static int ath9k_get_survey(struct ieee80211_hw *hw, int idx,
			     struct survey_info *survey)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ieee80211_supported_band *sband;
	struct ieee80211_channel *chan;
<<<<<<< HEAD
	unsigned long flags;
	int pos;

	spin_lock_irqsave(&common->cc_lock, flags);
	if (idx == 0)
		ath_update_survey_stats(sc);

	sband = hw->wiphy->bands[IEEE80211_BAND_2GHZ];
=======
	int pos;

	if (IS_ENABLED(CONFIG_ATH9K_TX99))
		return -EOPNOTSUPP;

	spin_lock_bh(&common->cc_lock);
	if (idx == 0)
		ath_update_survey_stats(sc);

	sband = hw->wiphy->bands[NL80211_BAND_2GHZ];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (sband && idx >= sband->n_channels) {
		idx -= sband->n_channels;
		sband = NULL;
	}

	if (!sband)
<<<<<<< HEAD
		sband = hw->wiphy->bands[IEEE80211_BAND_5GHZ];

	if (!sband || idx >= sband->n_channels) {
		spin_unlock_irqrestore(&common->cc_lock, flags);
=======
		sband = hw->wiphy->bands[NL80211_BAND_5GHZ];

	if (!sband || idx >= sband->n_channels) {
		spin_unlock_bh(&common->cc_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -ENOENT;
	}

	chan = &sband->channels[idx];
	pos = chan->hw_value;
	memcpy(survey, &sc->survey[pos], sizeof(*survey));
	survey->channel = chan;
<<<<<<< HEAD
	spin_unlock_irqrestore(&common->cc_lock, flags);
=======
	spin_unlock_bh(&common->cc_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
static void ath9k_set_coverage_class(struct ieee80211_hw *hw, u8 coverage_class)
=======
static void ath9k_enable_dynack(struct ath_softc *sc)
{
#ifdef CONFIG_ATH9K_DYNACK
	u32 rfilt;
	struct ath_hw *ah = sc->sc_ah;

	ath_dynack_reset(ah);

	ah->dynack.enabled = true;
	rfilt = ath_calcrxfilter(sc);
	ath9k_hw_setrxfilter(ah, rfilt);
#endif
}

static void ath9k_set_coverage_class(struct ieee80211_hw *hw,
				     s16 coverage_class)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;

<<<<<<< HEAD
	mutex_lock(&sc->mutex);
	ah->coverage_class = coverage_class;

	ath9k_ps_wakeup(sc);
	ath9k_hw_init_global_settings(ah);
	ath9k_ps_restore(sc);

	mutex_unlock(&sc->mutex);
}

static void ath9k_flush(struct ieee80211_hw *hw, u32 queues, bool drop)
=======
	if (IS_ENABLED(CONFIG_ATH9K_TX99))
		return;

	mutex_lock(&sc->mutex);

	if (coverage_class >= 0) {
		ah->coverage_class = coverage_class;
		if (ah->dynack.enabled) {
			u32 rfilt;

			ah->dynack.enabled = false;
			rfilt = ath_calcrxfilter(sc);
			ath9k_hw_setrxfilter(ah, rfilt);
		}
		ath9k_ps_wakeup(sc);
		ath9k_hw_init_global_settings(ah);
		ath9k_ps_restore(sc);
	} else if (!ah->dynack.enabled) {
		ath9k_enable_dynack(sc);
	}

	mutex_unlock(&sc->mutex);
}

static bool ath9k_has_tx_pending(struct ath_softc *sc,
				 bool sw_pending)
{
	int i, npend = 0;

	for (i = 0; i < ATH9K_NUM_TX_QUEUES; i++) {
		if (!ATH_TXQ_SETUP(sc, i))
			continue;

		npend = ath9k_has_pending_frames(sc, &sc->tx.txq[i],
						 sw_pending);
		if (npend)
			break;
	}

	return !!npend;
}

static void ath9k_flush(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
			u32 queues, bool drop)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);

	if (ath9k_is_chanctx_enabled()) {
		if (!test_bit(ATH_OP_MULTI_CHANNEL, &common->op_flags))
			goto flush;

		/*
		 * If MCC is active, extend the flush timeout
		 * and wait for the HW/SW queues to become
		 * empty. This needs to be done outside the
		 * sc->mutex lock to allow the channel scheduler
		 * to switch channel contexts.
		 *
		 * The vif queues have been stopped in mac80211,
		 * so there won't be any incoming frames.
		 */
		__ath9k_flush(hw, queues, drop, true, true);
		return;
	}
flush:
	mutex_lock(&sc->mutex);
	__ath9k_flush(hw, queues, drop, true, false);
	mutex_unlock(&sc->mutex);
}

void __ath9k_flush(struct ieee80211_hw *hw, u32 queues, bool drop,
		   bool sw_pending, bool timeout_override)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
<<<<<<< HEAD
	int timeout = 200; /* ms */
	int i, j;
	bool drain_txq;

	mutex_lock(&sc->mutex);
=======
	int timeout;
	bool drain_txq;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	cancel_delayed_work_sync(&sc->tx_complete_work);

	if (ah->ah_flags & AH_UNPLUGGED) {
		ath_dbg(common, ANY, "Device has been unplugged!\n");
<<<<<<< HEAD
		mutex_unlock(&sc->mutex);
		return;
	}

	if (test_bit(SC_OP_INVALID, &sc->sc_flags)) {
		ath_dbg(common, ANY, "Device not present\n");
		mutex_unlock(&sc->mutex);
		return;
	}

	for (j = 0; j < timeout; j++) {
		bool npend = false;

		if (j)
			usleep_range(1000, 2000);

		for (i = 0; i < ATH9K_NUM_TX_QUEUES; i++) {
			if (!ATH_TXQ_SETUP(sc, i))
				continue;

			npend = ath9k_has_pending_frames(sc, &sc->tx.txq[i]);

			if (npend)
				break;
		}

		if (!npend)
		    break;
	}
=======
		return;
	}

	if (test_bit(ATH_OP_INVALID, &common->op_flags)) {
		ath_dbg(common, ANY, "Device not present\n");
		return;
	}

	spin_lock_bh(&sc->chan_lock);
	if (timeout_override)
		timeout = HZ / 5;
	else
		timeout = sc->cur_chan->flush_timeout;
	spin_unlock_bh(&sc->chan_lock);

	ath_dbg(common, CHAN_CTX,
		"Flush timeout: %d\n", jiffies_to_msecs(timeout));

	if (wait_event_timeout(sc->tx_wait, !ath9k_has_tx_pending(sc, sw_pending),
			       timeout) > 0)
		drop = false;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (drop) {
		ath9k_ps_wakeup(sc);
		spin_lock_bh(&sc->sc_pcu_lock);
		drain_txq = ath_drain_all_txq(sc);
		spin_unlock_bh(&sc->sc_pcu_lock);

		if (!drain_txq)
<<<<<<< HEAD
			ath_reset(sc);

		ath9k_ps_restore(sc);
		ieee80211_wake_queues(hw);
	}

	ieee80211_queue_delayed_work(hw, &sc->tx_complete_work, 0);
	mutex_unlock(&sc->mutex);
=======
			ath_reset(sc, NULL);

		ath9k_ps_restore(sc);
	}

	ieee80211_queue_delayed_work(hw, &sc->tx_complete_work, 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static bool ath9k_tx_frames_pending(struct ieee80211_hw *hw)
{
	struct ath_softc *sc = hw->priv;
<<<<<<< HEAD
	int i;

	for (i = 0; i < ATH9K_NUM_TX_QUEUES; i++) {
		if (!ATH_TXQ_SETUP(sc, i))
			continue;

		if (ath9k_has_pending_frames(sc, &sc->tx.txq[i]))
			return true;
	}
	return false;
=======

	return ath9k_has_tx_pending(sc, true);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int ath9k_tx_last_beacon(struct ieee80211_hw *hw)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ieee80211_vif *vif;
	struct ath_vif *avp;
	struct ath_buf *bf;
	struct ath_tx_status ts;
	bool edma = !!(ah->caps.hw_caps & ATH9K_HW_CAP_EDMA);
	int status;

	vif = sc->beacon.bslot[0];
	if (!vif)
		return 0;

	if (!vif->bss_conf.enable_beacon)
		return 0;

	avp = (void *)vif->drv_priv;

	if (!sc->beacon.tx_processed && !edma) {
		tasklet_disable(&sc->bcon_tasklet);

		bf = avp->av_bcbuf;
		if (!bf || !bf->bf_mpdu)
			goto skip;

		status = ath9k_hw_txprocdesc(ah, bf->bf_desc, &ts);
		if (status == -EINPROGRESS)
			goto skip;

		sc->beacon.tx_processed = true;
		sc->beacon.tx_last = !(ts.ts_status & ATH9K_TXERR_MASK);

skip:
		tasklet_enable(&sc->bcon_tasklet);
	}

	return sc->beacon.tx_last;
}

static int ath9k_get_stats(struct ieee80211_hw *hw,
			   struct ieee80211_low_level_stats *stats)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath9k_mib_stats *mib_stats = &ah->ah_mibStats;

	stats->dot11ACKFailureCount = mib_stats->ackrcv_bad;
	stats->dot11RTSFailureCount = mib_stats->rts_bad;
	stats->dot11FCSErrorCount = mib_stats->fcs_bad;
	stats->dot11RTSSuccessCount = mib_stats->rts_good;
	return 0;
}

static u32 fill_chainmask(u32 cap, u32 new)
{
	u32 filled = 0;
	int i;

	for (i = 0; cap && new; i++, cap >>= 1) {
		if (!(cap & BIT(0)))
			continue;

		if (new & BIT(0))
			filled |= BIT(i);

		new >>= 1;
	}

	return filled;
}

static bool validate_antenna_mask(struct ath_hw *ah, u32 val)
{
	if (AR_SREV_9300_20_OR_LATER(ah))
		return true;

	switch (val & 0x7) {
	case 0x1:
	case 0x3:
	case 0x7:
		return true;
	case 0x2:
		return (ah->caps.rx_chainmask == 1);
	default:
		return false;
	}
}

static int ath9k_set_antenna(struct ieee80211_hw *hw, u32 tx_ant, u32 rx_ant)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;

	if (ah->caps.rx_chainmask != 1)
		rx_ant |= tx_ant;

	if (!validate_antenna_mask(ah, rx_ant) || !tx_ant)
		return -EINVAL;

	sc->ant_rx = rx_ant;
	sc->ant_tx = tx_ant;

	if (ah->caps.rx_chainmask == 1)
		return 0;

	/* AR9100 runs into calibration issues if not all rx chains are enabled */
	if (AR_SREV_9100(ah))
		ah->rxchainmask = 0x7;
	else
		ah->rxchainmask = fill_chainmask(ah->caps.rx_chainmask, rx_ant);

	ah->txchainmask = fill_chainmask(ah->caps.tx_chainmask, tx_ant);
<<<<<<< HEAD
	ath9k_reload_chainmask_settings(sc);
=======
	ath9k_cmn_reload_chainmask(ah);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

static int ath9k_get_antenna(struct ieee80211_hw *hw, u32 *tx_ant, u32 *rx_ant)
{
	struct ath_softc *sc = hw->priv;

	*tx_ant = sc->ant_tx;
	*rx_ant = sc->ant_rx;
	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_PM_SLEEP

static void ath9k_wow_map_triggers(struct ath_softc *sc,
				   struct cfg80211_wowlan *wowlan,
				   u32 *wow_triggers)
{
	if (wowlan->disconnect)
		*wow_triggers |= AH_WOW_LINK_CHANGE |
				 AH_WOW_BEACON_MISS;
	if (wowlan->magic_pkt)
		*wow_triggers |= AH_WOW_MAGIC_PATTERN_EN;

	if (wowlan->n_patterns)
		*wow_triggers |= AH_WOW_USER_PATTERN_EN;

	sc->wow_enabled = *wow_triggers;

}

static void ath9k_wow_add_disassoc_deauth_pattern(struct ath_softc *sc)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ath9k_hw_capabilities *pcaps = &ah->caps;
	int pattern_count = 0;
	int i, byte_cnt;
	u8 dis_deauth_pattern[MAX_PATTERN_SIZE];
	u8 dis_deauth_mask[MAX_PATTERN_SIZE];

	memset(dis_deauth_pattern, 0, MAX_PATTERN_SIZE);
	memset(dis_deauth_mask, 0, MAX_PATTERN_SIZE);

	/*
	 * Create Dissassociate / Deauthenticate packet filter
	 *
	 *     2 bytes        2 byte    6 bytes   6 bytes  6 bytes
	 *  +--------------+----------+---------+--------+--------+----
	 *  + Frame Control+ Duration +   DA    +  SA    +  BSSID +
	 *  +--------------+----------+---------+--------+--------+----
	 *
	 * The above is the management frame format for disassociate/
	 * deauthenticate pattern, from this we need to match the first byte
	 * of 'Frame Control' and DA, SA, and BSSID fields
	 * (skipping 2nd byte of FC and Duration feild.
	 *
	 * Disassociate pattern
	 * --------------------
	 * Frame control = 00 00 1010
	 * DA, SA, BSSID = x:x:x:x:x:x
	 * Pattern will be A0000000 | x:x:x:x:x:x | x:x:x:x:x:x
	 *			    | x:x:x:x:x:x  -- 22 bytes
	 *
	 * Deauthenticate pattern
	 * ----------------------
	 * Frame control = 00 00 1100
	 * DA, SA, BSSID = x:x:x:x:x:x
	 * Pattern will be C0000000 | x:x:x:x:x:x | x:x:x:x:x:x
	 *			    | x:x:x:x:x:x  -- 22 bytes
	 */

	/* Create Disassociate Pattern first */

	byte_cnt = 0;

	/* Fill out the mask with all FF's */

	for (i = 0; i < MAX_PATTERN_MASK_SIZE; i++)
		dis_deauth_mask[i] = 0xff;

	/* copy the first byte of frame control field */
	dis_deauth_pattern[byte_cnt] = 0xa0;
	byte_cnt++;

	/* skip 2nd byte of frame control and Duration field */
	byte_cnt += 3;

	/*
	 * need not match the destination mac address, it can be a broadcast
	 * mac address or an unicast to this station
	 */
	byte_cnt += 6;

	/* copy the source mac address */
	memcpy((dis_deauth_pattern + byte_cnt), common->curbssid, ETH_ALEN);

	byte_cnt += 6;

	/* copy the bssid, its same as the source mac address */

	memcpy((dis_deauth_pattern + byte_cnt), common->curbssid, ETH_ALEN);

	/* Create Disassociate pattern mask */

	if (pcaps->hw_caps & ATH9K_HW_WOW_PATTERN_MATCH_EXACT) {

		if (pcaps->hw_caps & ATH9K_HW_WOW_PATTERN_MATCH_DWORD) {
			/*
			 * for AR9280, because of hardware limitation, the
			 * first 4 bytes have to be matched for all patterns.
			 * the mask for disassociation and de-auth pattern
			 * matching need to enable the first 4 bytes.
			 * also the duration field needs to be filled.
			 */
			dis_deauth_mask[0] = 0xf0;

			/*
			 * fill in duration field
			 FIXME: what is the exact value ?
			 */
			dis_deauth_pattern[2] = 0xff;
			dis_deauth_pattern[3] = 0xff;
		} else {
			dis_deauth_mask[0] = 0xfe;
		}

		dis_deauth_mask[1] = 0x03;
		dis_deauth_mask[2] = 0xc0;
	} else {
		dis_deauth_mask[0] = 0xef;
		dis_deauth_mask[1] = 0x3f;
		dis_deauth_mask[2] = 0x00;
		dis_deauth_mask[3] = 0xfc;
	}

	ath_dbg(common, WOW, "Adding disassoc/deauth patterns for WoW\n");

	ath9k_hw_wow_apply_pattern(ah, dis_deauth_pattern, dis_deauth_mask,
				   pattern_count, byte_cnt);

	pattern_count++;
	/*
	 * for de-authenticate pattern, only the first byte of the frame
	 * control field gets changed from 0xA0 to 0xC0
	 */
	dis_deauth_pattern[0] = 0xC0;

	ath9k_hw_wow_apply_pattern(ah, dis_deauth_pattern, dis_deauth_mask,
				   pattern_count, byte_cnt);

}

static void ath9k_wow_add_pattern(struct ath_softc *sc,
				  struct cfg80211_wowlan *wowlan)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath9k_wow_pattern *wow_pattern = NULL;
	struct cfg80211_wowlan_trig_pkt_pattern *patterns = wowlan->patterns;
	int mask_len;
	s8 i = 0;

	if (!wowlan->n_patterns)
		return;

	/*
	 * Add the new user configured patterns
	 */
	for (i = 0; i < wowlan->n_patterns; i++) {

		wow_pattern = kzalloc(sizeof(*wow_pattern), GFP_KERNEL);

		if (!wow_pattern)
			return;

		/*
		 * TODO: convert the generic user space pattern to
		 * appropriate chip specific/802.11 pattern.
		 */

		mask_len = DIV_ROUND_UP(wowlan->patterns[i].pattern_len, 8);
		memset(wow_pattern->pattern_bytes, 0, MAX_PATTERN_SIZE);
		memset(wow_pattern->mask_bytes, 0, MAX_PATTERN_SIZE);
		memcpy(wow_pattern->pattern_bytes, patterns[i].pattern,
		       patterns[i].pattern_len);
		memcpy(wow_pattern->mask_bytes, patterns[i].mask, mask_len);
		wow_pattern->pattern_len = patterns[i].pattern_len;

		/*
		 * just need to take care of deauth and disssoc pattern,
		 * make sure we don't overwrite them.
		 */

		ath9k_hw_wow_apply_pattern(ah, wow_pattern->pattern_bytes,
					   wow_pattern->mask_bytes,
					   i + 2,
					   wow_pattern->pattern_len);
		kfree(wow_pattern);

	}

}

static int ath9k_suspend(struct ieee80211_hw *hw,
			 struct cfg80211_wowlan *wowlan)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	u32 wow_triggers_enabled = 0;
	int ret = 0;

	mutex_lock(&sc->mutex);

	ath_cancel_work(sc);
	ath_stop_ani(sc);
	del_timer_sync(&sc->rx_poll_timer);

	if (test_bit(SC_OP_INVALID, &sc->sc_flags)) {
		ath_dbg(common, ANY, "Device not present\n");
		ret = -EINVAL;
		goto fail_wow;
	}

	if (WARN_ON(!wowlan)) {
		ath_dbg(common, WOW, "None of the WoW triggers enabled\n");
		ret = -EINVAL;
		goto fail_wow;
	}

	if (!device_can_wakeup(sc->dev)) {
		ath_dbg(common, WOW, "device_can_wakeup failed, WoW is not enabled\n");
		ret = 1;
		goto fail_wow;
	}

	/*
	 * none of the sta vifs are associated
	 * and we are not currently handling multivif
	 * cases, for instance we have to seperately
	 * configure 'keep alive frame' for each
	 * STA.
	 */

	if (!test_bit(SC_OP_PRIM_STA_VIF, &sc->sc_flags)) {
		ath_dbg(common, WOW, "None of the STA vifs are associated\n");
		ret = 1;
		goto fail_wow;
	}

	if (sc->nvifs > 1) {
		ath_dbg(common, WOW, "WoW for multivif is not yet supported\n");
		ret = 1;
		goto fail_wow;
	}

	ath9k_wow_map_triggers(sc, wowlan, &wow_triggers_enabled);

	ath_dbg(common, WOW, "WoW triggers enabled 0x%x\n",
		wow_triggers_enabled);

	ath9k_ps_wakeup(sc);

	ath9k_stop_btcoex(sc);

	/*
	 * Enable wake up on recieving disassoc/deauth
	 * frame by default.
	 */
	ath9k_wow_add_disassoc_deauth_pattern(sc);

	if (wow_triggers_enabled & AH_WOW_USER_PATTERN_EN)
		ath9k_wow_add_pattern(sc, wowlan);

	spin_lock_bh(&sc->sc_pcu_lock);
	/*
	 * To avoid false wake, we enable beacon miss interrupt only
	 * when we go to sleep. We save the current interrupt mask
	 * so we can restore it after the system wakes up
	 */
	sc->wow_intr_before_sleep = ah->imask;
	ah->imask &= ~ATH9K_INT_GLOBAL;
	ath9k_hw_disable_interrupts(ah);
	ah->imask = ATH9K_INT_BMISS | ATH9K_INT_GLOBAL;
	ath9k_hw_set_interrupts(ah);
	ath9k_hw_enable_interrupts(ah);

	spin_unlock_bh(&sc->sc_pcu_lock);

	/*
	 * we can now sync irq and kill any running tasklets, since we already
	 * disabled interrupts and not holding a spin lock
	 */
	synchronize_irq(sc->irq);
	tasklet_kill(&sc->intr_tq);

	ath9k_hw_wow_enable(ah, wow_triggers_enabled);

	ath9k_ps_restore(sc);
	ath_dbg(common, ANY, "WoW enabled in ath9k\n");
	atomic_inc(&sc->wow_sleep_proc_intr);

fail_wow:
	mutex_unlock(&sc->mutex);
	return ret;
}

static int ath9k_resume(struct ieee80211_hw *hw)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	u32 wow_status;

	mutex_lock(&sc->mutex);

	ath9k_ps_wakeup(sc);

	spin_lock_bh(&sc->sc_pcu_lock);

	ath9k_hw_disable_interrupts(ah);
	ah->imask = sc->wow_intr_before_sleep;
	ath9k_hw_set_interrupts(ah);
	ath9k_hw_enable_interrupts(ah);

	spin_unlock_bh(&sc->sc_pcu_lock);

	wow_status = ath9k_hw_wow_wakeup(ah);

	if (atomic_read(&sc->wow_got_bmiss_intr) == 0) {
		/*
		 * some devices may not pick beacon miss
		 * as the reason they woke up so we add
		 * that here for that shortcoming.
		 */
		wow_status |= AH_WOW_BEACON_MISS;
		atomic_dec(&sc->wow_got_bmiss_intr);
		ath_dbg(common, ANY, "Beacon miss interrupt picked up during WoW sleep\n");
	}

	atomic_dec(&sc->wow_sleep_proc_intr);

	if (wow_status) {
		ath_dbg(common, ANY, "Waking up due to WoW triggers %s with WoW status = %x\n",
			ath9k_hw_wow_event_to_string(wow_status), wow_status);
	}

	ath_restart_work(sc);
	ath9k_start_btcoex(sc);

	ath9k_ps_restore(sc);
	mutex_unlock(&sc->mutex);

	return 0;
}

static void ath9k_set_wakeup(struct ieee80211_hw *hw, bool enabled)
{
	struct ath_softc *sc = hw->priv;

	mutex_lock(&sc->mutex);
	device_init_wakeup(sc->dev, 1);
	device_set_wakeup_enable(sc->dev, enabled);
	mutex_unlock(&sc->mutex);
}

#endif
static void ath9k_sw_scan_start(struct ieee80211_hw *hw)
{
	struct ath_softc *sc = hw->priv;

	sc->scanning = 1;
}

static void ath9k_sw_scan_complete(struct ieee80211_hw *hw)
{
	struct ath_softc *sc = hw->priv;

	sc->scanning = 0;
=======
static void ath9k_sw_scan_start(struct ieee80211_hw *hw,
				struct ieee80211_vif *vif,
				const u8 *mac_addr)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	set_bit(ATH_OP_SCANNING, &common->op_flags);
}

static void ath9k_sw_scan_complete(struct ieee80211_hw *hw,
				   struct ieee80211_vif *vif)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	clear_bit(ATH_OP_SCANNING, &common->op_flags);
}

#ifdef CONFIG_ATH9K_CHANNEL_CONTEXT

static void ath9k_cancel_pending_offchannel(struct ath_softc *sc)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);

	if (sc->offchannel.roc_vif) {
		ath_dbg(common, CHAN_CTX,
			"%s: Aborting RoC\n", __func__);

		del_timer_sync(&sc->offchannel.timer);
		if (sc->offchannel.state >= ATH_OFFCHANNEL_ROC_START)
			ath_roc_complete(sc, ATH_ROC_COMPLETE_ABORT);
	}

	if (test_bit(ATH_OP_SCANNING, &common->op_flags)) {
		ath_dbg(common, CHAN_CTX,
			"%s: Aborting HW scan\n", __func__);

		del_timer_sync(&sc->offchannel.timer);
		ath_scan_complete(sc, true);
	}
}

static int ath9k_hw_scan(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
			 struct ieee80211_scan_request *hw_req)
{
	struct cfg80211_scan_request *req = &hw_req->req;
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	int ret = 0;

	mutex_lock(&sc->mutex);

	if (WARN_ON(sc->offchannel.scan_req)) {
		ret = -EBUSY;
		goto out;
	}

	ath9k_ps_wakeup(sc);
	set_bit(ATH_OP_SCANNING, &common->op_flags);
	sc->offchannel.scan_vif = vif;
	sc->offchannel.scan_req = req;
	sc->offchannel.scan_idx = 0;

	ath_dbg(common, CHAN_CTX, "HW scan request received on vif: %pM\n",
		vif->addr);

	if (sc->offchannel.state == ATH_OFFCHANNEL_IDLE) {
		ath_dbg(common, CHAN_CTX, "Starting HW scan\n");
		ath_offchannel_next(sc);
	}

out:
	mutex_unlock(&sc->mutex);

	return ret;
}

static void ath9k_cancel_hw_scan(struct ieee80211_hw *hw,
				 struct ieee80211_vif *vif)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);

	ath_dbg(common, CHAN_CTX, "Cancel HW scan on vif: %pM\n", vif->addr);

	mutex_lock(&sc->mutex);
	del_timer_sync(&sc->offchannel.timer);
	ath_scan_complete(sc, true);
	mutex_unlock(&sc->mutex);
}

static int ath9k_remain_on_channel(struct ieee80211_hw *hw,
				   struct ieee80211_vif *vif,
				   struct ieee80211_channel *chan, int duration,
				   enum ieee80211_roc_type type)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	int ret = 0;

	mutex_lock(&sc->mutex);

	if (WARN_ON(sc->offchannel.roc_vif)) {
		ret = -EBUSY;
		goto out;
	}

	ath9k_ps_wakeup(sc);
	sc->offchannel.roc_vif = vif;
	sc->offchannel.roc_chan = chan;
	sc->offchannel.roc_duration = duration;

	ath_dbg(common, CHAN_CTX,
		"RoC request on vif: %pM, type: %d duration: %d\n",
		vif->addr, type, duration);

	if (sc->offchannel.state == ATH_OFFCHANNEL_IDLE) {
		ath_dbg(common, CHAN_CTX, "Starting RoC period\n");
		ath_offchannel_next(sc);
	}

out:
	mutex_unlock(&sc->mutex);

	return ret;
}

static int ath9k_cancel_remain_on_channel(struct ieee80211_hw *hw)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);

	mutex_lock(&sc->mutex);

	ath_dbg(common, CHAN_CTX, "Cancel RoC\n");
	del_timer_sync(&sc->offchannel.timer);

	if (sc->offchannel.roc_vif) {
		if (sc->offchannel.state >= ATH_OFFCHANNEL_ROC_START)
			ath_roc_complete(sc, ATH_ROC_COMPLETE_CANCEL);
	}

	mutex_unlock(&sc->mutex);

	return 0;
}

static int ath9k_add_chanctx(struct ieee80211_hw *hw,
			     struct ieee80211_chanctx_conf *conf)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_chanctx *ctx, **ptr;
	int pos;

	mutex_lock(&sc->mutex);

	ath_for_each_chanctx(sc, ctx) {
		if (ctx->assigned)
			continue;

		ptr = (void *) conf->drv_priv;
		*ptr = ctx;
		ctx->assigned = true;
		pos = ctx - &sc->chanctx[0];
		ctx->hw_queue_base = pos * IEEE80211_NUM_ACS;

		ath_dbg(common, CHAN_CTX,
			"Add channel context: %d MHz\n",
			conf->def.chan->center_freq);

		ath_chanctx_set_channel(sc, ctx, &conf->def);

		mutex_unlock(&sc->mutex);
		return 0;
	}

	mutex_unlock(&sc->mutex);
	return -ENOSPC;
}


static void ath9k_remove_chanctx(struct ieee80211_hw *hw,
				 struct ieee80211_chanctx_conf *conf)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_chanctx *ctx = ath_chanctx_get(conf);

	mutex_lock(&sc->mutex);

	ath_dbg(common, CHAN_CTX,
		"Remove channel context: %d MHz\n",
		conf->def.chan->center_freq);

	ctx->assigned = false;
	ctx->hw_queue_base = 0;
	ath_chanctx_event(sc, NULL, ATH_CHANCTX_EVENT_UNASSIGN);

	mutex_unlock(&sc->mutex);
}

static void ath9k_change_chanctx(struct ieee80211_hw *hw,
				 struct ieee80211_chanctx_conf *conf,
				 u32 changed)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_chanctx *ctx = ath_chanctx_get(conf);

	mutex_lock(&sc->mutex);
	ath_dbg(common, CHAN_CTX,
		"Change channel context: %d MHz\n",
		conf->def.chan->center_freq);
	ath_chanctx_set_channel(sc, ctx, &conf->def);
	mutex_unlock(&sc->mutex);
}

static int ath9k_assign_vif_chanctx(struct ieee80211_hw *hw,
				    struct ieee80211_vif *vif,
				    struct ieee80211_chanctx_conf *conf)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_vif *avp = (void *)vif->drv_priv;
	struct ath_chanctx *ctx = ath_chanctx_get(conf);
	int i;

	ath9k_cancel_pending_offchannel(sc);

	mutex_lock(&sc->mutex);

	ath_dbg(common, CHAN_CTX,
		"Assign VIF (addr: %pM, type: %d, p2p: %d) to channel context: %d MHz\n",
		vif->addr, vif->type, vif->p2p,
		conf->def.chan->center_freq);

	avp->chanctx = ctx;
	ctx->nvifs_assigned++;
	list_add_tail(&avp->list, &ctx->vifs);
	ath9k_calculate_summary_state(sc, ctx);
	for (i = 0; i < IEEE80211_NUM_ACS; i++)
		vif->hw_queue[i] = ctx->hw_queue_base + i;

	mutex_unlock(&sc->mutex);

	return 0;
}

static void ath9k_unassign_vif_chanctx(struct ieee80211_hw *hw,
				       struct ieee80211_vif *vif,
				       struct ieee80211_chanctx_conf *conf)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_vif *avp = (void *)vif->drv_priv;
	struct ath_chanctx *ctx = ath_chanctx_get(conf);
	int ac;

	ath9k_cancel_pending_offchannel(sc);

	mutex_lock(&sc->mutex);

	ath_dbg(common, CHAN_CTX,
		"Remove VIF (addr: %pM, type: %d, p2p: %d) from channel context: %d MHz\n",
		vif->addr, vif->type, vif->p2p,
		conf->def.chan->center_freq);

	avp->chanctx = NULL;
	ctx->nvifs_assigned--;
	list_del(&avp->list);
	ath9k_calculate_summary_state(sc, ctx);
	for (ac = 0; ac < IEEE80211_NUM_ACS; ac++)
		vif->hw_queue[ac] = IEEE80211_INVAL_HW_QUEUE;

	mutex_unlock(&sc->mutex);
}

static void ath9k_mgd_prepare_tx(struct ieee80211_hw *hw,
				 struct ieee80211_vif *vif)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_vif *avp = (struct ath_vif *) vif->drv_priv;
	struct ath_beacon_config *cur_conf;
	struct ath_chanctx *go_ctx;
	unsigned long timeout;
	bool changed = false;
	u32 beacon_int;

	if (!test_bit(ATH_OP_MULTI_CHANNEL, &common->op_flags))
		return;

	if (!avp->chanctx)
		return;

	mutex_lock(&sc->mutex);

	spin_lock_bh(&sc->chan_lock);
	if (sc->next_chan || (sc->cur_chan != avp->chanctx))
		changed = true;
	spin_unlock_bh(&sc->chan_lock);

	if (!changed)
		goto out;

	ath9k_cancel_pending_offchannel(sc);

	go_ctx = ath_is_go_chanctx_present(sc);

	if (go_ctx) {
		/*
		 * Wait till the GO interface gets a chance
		 * to send out an NoA.
		 */
		spin_lock_bh(&sc->chan_lock);
		sc->sched.mgd_prepare_tx = true;
		cur_conf = &go_ctx->beacon;
		beacon_int = TU_TO_USEC(cur_conf->beacon_interval);
		spin_unlock_bh(&sc->chan_lock);

		timeout = usecs_to_jiffies(beacon_int * 2);
		init_completion(&sc->go_beacon);

		mutex_unlock(&sc->mutex);

		if (wait_for_completion_timeout(&sc->go_beacon,
						timeout) == 0) {
			ath_dbg(common, CHAN_CTX,
				"Failed to send new NoA\n");

			spin_lock_bh(&sc->chan_lock);
			sc->sched.mgd_prepare_tx = false;
			spin_unlock_bh(&sc->chan_lock);
		}

		mutex_lock(&sc->mutex);
	}

	ath_dbg(common, CHAN_CTX,
		"%s: Set chanctx state to FORCE_ACTIVE for vif: %pM\n",
		__func__, vif->addr);

	spin_lock_bh(&sc->chan_lock);
	sc->next_chan = avp->chanctx;
	sc->sched.state = ATH_CHANCTX_STATE_FORCE_ACTIVE;
	spin_unlock_bh(&sc->chan_lock);

	ath_chanctx_set_next(sc, true);
out:
	mutex_unlock(&sc->mutex);
}

void ath9k_fill_chanctx_ops(void)
{
	if (!ath9k_is_chanctx_enabled())
		return;

	ath9k_ops.hw_scan                  = ath9k_hw_scan;
	ath9k_ops.cancel_hw_scan           = ath9k_cancel_hw_scan;
	ath9k_ops.remain_on_channel        = ath9k_remain_on_channel;
	ath9k_ops.cancel_remain_on_channel = ath9k_cancel_remain_on_channel;
	ath9k_ops.add_chanctx              = ath9k_add_chanctx;
	ath9k_ops.remove_chanctx           = ath9k_remove_chanctx;
	ath9k_ops.change_chanctx           = ath9k_change_chanctx;
	ath9k_ops.assign_vif_chanctx       = ath9k_assign_vif_chanctx;
	ath9k_ops.unassign_vif_chanctx     = ath9k_unassign_vif_chanctx;
	ath9k_ops.mgd_prepare_tx           = ath9k_mgd_prepare_tx;
}

#endif

static int ath9k_get_txpower(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
			     int *dbm)
{
	struct ath_softc *sc = hw->priv;
	struct ath_vif *avp = (void *)vif->drv_priv;

	mutex_lock(&sc->mutex);
	if (avp->chanctx)
		*dbm = avp->chanctx->cur_txpower;
	else
		*dbm = sc->cur_chan->cur_txpower;
	mutex_unlock(&sc->mutex);

	*dbm /= 2;

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

struct ieee80211_ops ath9k_ops = {
	.tx 		    = ath9k_tx,
	.start 		    = ath9k_start,
	.stop 		    = ath9k_stop,
	.add_interface 	    = ath9k_add_interface,
	.change_interface   = ath9k_change_interface,
	.remove_interface   = ath9k_remove_interface,
	.config 	    = ath9k_config,
	.configure_filter   = ath9k_configure_filter,
<<<<<<< HEAD
	.sta_add	    = ath9k_sta_add,
	.sta_remove	    = ath9k_sta_remove,
=======
	.sta_state          = ath9k_sta_state,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.sta_notify         = ath9k_sta_notify,
	.conf_tx 	    = ath9k_conf_tx,
	.bss_info_changed   = ath9k_bss_info_changed,
	.set_key            = ath9k_set_key,
	.get_tsf 	    = ath9k_get_tsf,
	.set_tsf 	    = ath9k_set_tsf,
	.reset_tsf 	    = ath9k_reset_tsf,
	.ampdu_action       = ath9k_ampdu_action,
	.get_survey	    = ath9k_get_survey,
	.rfkill_poll        = ath9k_rfkill_poll_state,
	.set_coverage_class = ath9k_set_coverage_class,
	.flush		    = ath9k_flush,
	.tx_frames_pending  = ath9k_tx_frames_pending,
	.tx_last_beacon     = ath9k_tx_last_beacon,
<<<<<<< HEAD
=======
	.release_buffered_frames = ath9k_release_buffered_frames,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.get_stats	    = ath9k_get_stats,
	.set_antenna	    = ath9k_set_antenna,
	.get_antenna	    = ath9k_get_antenna,

<<<<<<< HEAD
#ifdef CONFIG_PM_SLEEP
=======
#ifdef CONFIG_ATH9K_WOW
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.suspend	    = ath9k_suspend,
	.resume		    = ath9k_resume,
	.set_wakeup	    = ath9k_set_wakeup,
#endif

#ifdef CONFIG_ATH9K_DEBUGFS
	.get_et_sset_count  = ath9k_get_et_sset_count,
	.get_et_stats       = ath9k_get_et_stats,
	.get_et_strings     = ath9k_get_et_strings,
#endif

<<<<<<< HEAD
#if defined(CONFIG_MAC80211_DEBUGFS) && defined(CONFIG_ATH9K_DEBUGFS)
	.sta_add_debugfs    = ath9k_sta_add_debugfs,
	.sta_remove_debugfs = ath9k_sta_remove_debugfs,
#endif
	.sw_scan_start	    = ath9k_sw_scan_start,
	.sw_scan_complete   = ath9k_sw_scan_complete,
=======
#if defined(CONFIG_MAC80211_DEBUGFS) && defined(CONFIG_ATH9K_STATION_STATISTICS)
	.sta_add_debugfs    = ath9k_sta_add_debugfs,
#endif
	.sw_scan_start	    = ath9k_sw_scan_start,
	.sw_scan_complete   = ath9k_sw_scan_complete,
	.get_txpower        = ath9k_get_txpower,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};
