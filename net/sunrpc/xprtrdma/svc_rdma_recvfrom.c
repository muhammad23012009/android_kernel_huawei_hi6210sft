/*
<<<<<<< HEAD
=======
 * Copyright (c) 2014 Open Grid Computing, Inc. All rights reserved.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Copyright (c) 2005-2006 Network Appliance, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the BSD-type
 * license below:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *      Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *      Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *
 *      Neither the name of the Network Appliance, Inc. nor the names of
 *      its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written
 *      permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Tom Tucker <tom@opengridcomputing.com>
 */

#include <linux/sunrpc/debug.h>
#include <linux/sunrpc/rpc_rdma.h>
#include <linux/spinlock.h>
#include <asm/unaligned.h>
#include <rdma/ib_verbs.h>
#include <rdma/rdma_cm.h>
#include <linux/sunrpc/svc_rdma.h>

#define RPCDBG_FACILITY	RPCDBG_SVCXPRT

/*
 * Replace the pages in the rq_argpages array with the pages from the SGE in
 * the RDMA_RECV completion. The SGL should contain full pages up until the
 * last one.
 */
static void rdma_build_arg_xdr(struct svc_rqst *rqstp,
			       struct svc_rdma_op_ctxt *ctxt,
			       u32 byte_count)
{
<<<<<<< HEAD
=======
	struct rpcrdma_msg *rmsgp;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct page *page;
	u32 bc;
	int sge_no;

	/* Swap the page in the SGE with the page in argpages */
	page = ctxt->pages[0];
	put_page(rqstp->rq_pages[0]);
	rqstp->rq_pages[0] = page;

	/* Set up the XDR head */
	rqstp->rq_arg.head[0].iov_base = page_address(page);
<<<<<<< HEAD
	rqstp->rq_arg.head[0].iov_len = min(byte_count, ctxt->sge[0].length);
=======
	rqstp->rq_arg.head[0].iov_len =
		min_t(size_t, byte_count, ctxt->sge[0].length);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rqstp->rq_arg.len = byte_count;
	rqstp->rq_arg.buflen = byte_count;

	/* Compute bytes past head in the SGL */
	bc = byte_count - rqstp->rq_arg.head[0].iov_len;

	/* If data remains, store it in the pagelist */
	rqstp->rq_arg.page_len = bc;
	rqstp->rq_arg.page_base = 0;
<<<<<<< HEAD
	rqstp->rq_arg.pages = &rqstp->rq_pages[1];
=======

	/* RDMA_NOMSG: RDMA READ data should land just after RDMA RECV data */
	rmsgp = (struct rpcrdma_msg *)rqstp->rq_arg.head[0].iov_base;
	if (rmsgp->rm_type == rdma_nomsg)
		rqstp->rq_arg.pages = &rqstp->rq_pages[0];
	else
		rqstp->rq_arg.pages = &rqstp->rq_pages[1];

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	sge_no = 1;
	while (bc && sge_no < ctxt->count) {
		page = ctxt->pages[sge_no];
		put_page(rqstp->rq_pages[sge_no]);
		rqstp->rq_pages[sge_no] = page;
<<<<<<< HEAD
		bc -= min(bc, ctxt->sge[sge_no].length);
=======
		bc -= min_t(u32, bc, ctxt->sge[sge_no].length);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		rqstp->rq_arg.buflen += ctxt->sge[sge_no].length;
		sge_no++;
	}
	rqstp->rq_respages = &rqstp->rq_pages[sge_no];
<<<<<<< HEAD

	/* We should never run out of SGE because the limit is defined to
	 * support the max allowed RPC data length
	 */
	BUG_ON(bc && (sge_no == ctxt->count));
	BUG_ON((rqstp->rq_arg.head[0].iov_len + rqstp->rq_arg.page_len)
	       != byte_count);
	BUG_ON(rqstp->rq_arg.len != byte_count);
=======
	rqstp->rq_next_page = rqstp->rq_respages + 1;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* If not all pages were used from the SGL, free the remaining ones */
	bc = sge_no;
	while (sge_no < ctxt->count) {
		page = ctxt->pages[sge_no++];
		put_page(page);
	}
	ctxt->count = bc;

	/* Set up tail */
	rqstp->rq_arg.tail[0].iov_base = NULL;
	rqstp->rq_arg.tail[0].iov_len = 0;
}

<<<<<<< HEAD
/* Encode a read-chunk-list as an array of IB SGE
 *
 * Assumptions:
 * - chunk[0]->position points to pages[0] at an offset of 0
 * - pages[] is not physically or virtually contiguous and consists of
 *   PAGE_SIZE elements.
 *
 * Output:
 * - sge array pointing into pages[] array.
 * - chunk_sge array specifying sge index and count for each
 *   chunk in the read list
 *
 */
static int map_read_chunks(struct svcxprt_rdma *xprt,
			   struct svc_rqst *rqstp,
			   struct svc_rdma_op_ctxt *head,
			   struct rpcrdma_msg *rmsgp,
			   struct svc_rdma_req_map *rpl_map,
			   struct svc_rdma_req_map *chl_map,
			   int ch_count,
			   int byte_count)
{
	int sge_no;
	int sge_bytes;
	int page_off;
	int page_no;
	int ch_bytes;
	int ch_no;
	struct rpcrdma_read_chunk *ch;

	sge_no = 0;
	page_no = 0;
	page_off = 0;
	ch = (struct rpcrdma_read_chunk *)&rmsgp->rm_body.rm_chunks[0];
	ch_no = 0;
	ch_bytes = ntohl(ch->rc_target.rs_length);
	head->arg.head[0] = rqstp->rq_arg.head[0];
	head->arg.tail[0] = rqstp->rq_arg.tail[0];
	head->arg.pages = &head->pages[head->count];
	head->hdr_count = head->count; /* save count of hdr pages */
	head->arg.page_base = 0;
	head->arg.page_len = ch_bytes;
	head->arg.len = rqstp->rq_arg.len + ch_bytes;
	head->arg.buflen = rqstp->rq_arg.buflen + ch_bytes;
	head->count++;
	chl_map->ch[0].start = 0;
	while (byte_count) {
		rpl_map->sge[sge_no].iov_base =
			page_address(rqstp->rq_arg.pages[page_no]) + page_off;
		sge_bytes = min_t(int, PAGE_SIZE-page_off, ch_bytes);
		rpl_map->sge[sge_no].iov_len = sge_bytes;
		/*
		 * Don't bump head->count here because the same page
		 * may be used by multiple SGE.
		 */
		head->arg.pages[page_no] = rqstp->rq_arg.pages[page_no];
		rqstp->rq_respages = &rqstp->rq_arg.pages[page_no+1];

		byte_count -= sge_bytes;
		ch_bytes -= sge_bytes;
		sge_no++;
		/*
		 * If all bytes for this chunk have been mapped to an
		 * SGE, move to the next SGE
		 */
		if (ch_bytes == 0) {
			chl_map->ch[ch_no].count =
				sge_no - chl_map->ch[ch_no].start;
			ch_no++;
			ch++;
			chl_map->ch[ch_no].start = sge_no;
			ch_bytes = ntohl(ch->rc_target.rs_length);
			/* If bytes remaining account for next chunk */
			if (byte_count) {
				head->arg.page_len += ch_bytes;
				head->arg.len += ch_bytes;
				head->arg.buflen += ch_bytes;
			}
		}
		/*
		 * If this SGE consumed all of the page, move to the
		 * next page
		 */
		if ((sge_bytes + page_off) == PAGE_SIZE) {
			page_no++;
			page_off = 0;
			/*
			 * If there are still bytes left to map, bump
			 * the page count
			 */
			if (byte_count)
				head->count++;
		} else
			page_off += sge_bytes;
	}
	BUG_ON(byte_count != 0);
	return sge_no;
}

/* Map a read-chunk-list to an XDR and fast register the page-list.
 *
 * Assumptions:
 * - chunk[0]	position points to pages[0] at an offset of 0
 * - pages[]	will be made physically contiguous by creating a one-off memory
 *		region using the fastreg verb.
 * - byte_count is # of bytes in read-chunk-list
 * - ch_count	is # of chunks in read-chunk-list
 *
 * Output:
 * - sge array pointing into pages[] array.
 * - chunk_sge array specifying sge index and count for each
 *   chunk in the read list
 */
static int fast_reg_read_chunks(struct svcxprt_rdma *xprt,
				struct svc_rqst *rqstp,
				struct svc_rdma_op_ctxt *head,
				struct rpcrdma_msg *rmsgp,
				struct svc_rdma_req_map *rpl_map,
				struct svc_rdma_req_map *chl_map,
				int ch_count,
				int byte_count)
{
	int page_no;
	int ch_no;
	u32 offset;
	struct rpcrdma_read_chunk *ch;
	struct svc_rdma_fastreg_mr *frmr;
	int ret = 0;

	frmr = svc_rdma_get_frmr(xprt);
	if (IS_ERR(frmr))
		return -ENOMEM;

	head->frmr = frmr;
	head->arg.head[0] = rqstp->rq_arg.head[0];
	head->arg.tail[0] = rqstp->rq_arg.tail[0];
	head->arg.pages = &head->pages[head->count];
	head->hdr_count = head->count; /* save count of hdr pages */
	head->arg.page_base = 0;
	head->arg.page_len = byte_count;
	head->arg.len = rqstp->rq_arg.len + byte_count;
	head->arg.buflen = rqstp->rq_arg.buflen + byte_count;

	/* Fast register the page list */
	frmr->kva = page_address(rqstp->rq_arg.pages[0]);
	frmr->direction = DMA_FROM_DEVICE;
	frmr->access_flags = (IB_ACCESS_LOCAL_WRITE|IB_ACCESS_REMOTE_WRITE);
	frmr->map_len = byte_count;
	frmr->page_list_len = PAGE_ALIGN(byte_count) >> PAGE_SHIFT;
	for (page_no = 0; page_no < frmr->page_list_len; page_no++) {
		frmr->page_list->page_list[page_no] =
			ib_dma_map_page(xprt->sc_cm_id->device,
					rqstp->rq_arg.pages[page_no], 0,
					PAGE_SIZE, DMA_FROM_DEVICE);
		if (ib_dma_mapping_error(xprt->sc_cm_id->device,
					 frmr->page_list->page_list[page_no]))
			goto fatal_err;
		atomic_inc(&xprt->sc_dma_used);
		head->arg.pages[page_no] = rqstp->rq_arg.pages[page_no];
	}
	head->count += page_no;

	/* rq_respages points one past arg pages */
	rqstp->rq_respages = &rqstp->rq_arg.pages[page_no];

	/* Create the reply and chunk maps */
	offset = 0;
	ch = (struct rpcrdma_read_chunk *)&rmsgp->rm_body.rm_chunks[0];
	for (ch_no = 0; ch_no < ch_count; ch_no++) {
		int len = ntohl(ch->rc_target.rs_length);
		rpl_map->sge[ch_no].iov_base = frmr->kva + offset;
		rpl_map->sge[ch_no].iov_len = len;
		chl_map->ch[ch_no].count = 1;
		chl_map->ch[ch_no].start = ch_no;
		offset += len;
		ch++;
	}

	ret = svc_rdma_fastreg(xprt, frmr);
	if (ret)
		goto fatal_err;

	return ch_no;

 fatal_err:
	printk("svcrdma: error fast registering xdr for xprt %p", xprt);
	svc_rdma_put_frmr(xprt, frmr);
	return -EIO;
}

static int rdma_set_ctxt_sge(struct svcxprt_rdma *xprt,
			     struct svc_rdma_op_ctxt *ctxt,
			     struct svc_rdma_fastreg_mr *frmr,
			     struct kvec *vec,
			     u64 *sgl_offset,
			     int count)
{
	int i;
	unsigned long off;

	ctxt->count = count;
	ctxt->direction = DMA_FROM_DEVICE;
	for (i = 0; i < count; i++) {
		ctxt->sge[i].length = 0; /* in case map fails */
		if (!frmr) {
			BUG_ON(!virt_to_page(vec[i].iov_base));
			off = (unsigned long)vec[i].iov_base & ~PAGE_MASK;
			ctxt->sge[i].addr =
				ib_dma_map_page(xprt->sc_cm_id->device,
						virt_to_page(vec[i].iov_base),
						off,
						vec[i].iov_len,
						DMA_FROM_DEVICE);
			if (ib_dma_mapping_error(xprt->sc_cm_id->device,
						 ctxt->sge[i].addr))
				return -EINVAL;
			ctxt->sge[i].lkey = xprt->sc_dma_lkey;
			atomic_inc(&xprt->sc_dma_used);
		} else {
			ctxt->sge[i].addr = (unsigned long)vec[i].iov_base;
			ctxt->sge[i].lkey = frmr->mr->lkey;
		}
		ctxt->sge[i].length = vec[i].iov_len;
		*sgl_offset = *sgl_offset + vec[i].iov_len;
	}
	return 0;
}

static int rdma_read_max_sge(struct svcxprt_rdma *xprt, int sge_count)
{
	if ((rdma_node_get_transport(xprt->sc_cm_id->device->node_type) ==
	     RDMA_TRANSPORT_IWARP) &&
	    sge_count > 1)
		return 1;
	else
		return min_t(int, sge_count, xprt->sc_max_sge);
}

/*
 * Use RDMA_READ to read data from the advertised client buffer into the
 * XDR stream starting at rq_arg.head[0].iov_base.
 * Each chunk in the array
 * contains the following fields:
 * discrim      - '1', This isn't used for data placement
 * position     - The xdr stream offset (the same for every chunk)
 * handle       - RMR for client memory region
 * length       - data transfer length
 * offset       - 64 bit tagged offset in remote memory region
 *
 * On our side, we need to read into a pagelist. The first page immediately
 * follows the RPC header.
 *
 * This function returns:
 * 0 - No error and no read-list found.
 *
 * 1 - Successful read-list processing. The data is not yet in
 * the pagelist and therefore the RPC request must be deferred. The
 * I/O completion will enqueue the transport again and
 * svc_rdma_recvfrom will complete the request.
 *
 * <0 - Error processing/posting read-list.
 *
 * NOTE: The ctxt must not be touched after the last WR has been posted
 * because the I/O completion processing may occur on another
 * processor and free / modify the context. Ne touche pas!
 */
static int rdma_read_xdr(struct svcxprt_rdma *xprt,
			 struct rpcrdma_msg *rmsgp,
			 struct svc_rqst *rqstp,
			 struct svc_rdma_op_ctxt *hdr_ctxt)
{
	struct ib_send_wr read_wr;
	struct ib_send_wr inv_wr;
	int err = 0;
	int ch_no;
	int ch_count;
	int byte_count;
	int sge_count;
	u64 sgl_offset;
	struct rpcrdma_read_chunk *ch;
	struct svc_rdma_op_ctxt *ctxt = NULL;
	struct svc_rdma_req_map *rpl_map;
	struct svc_rdma_req_map *chl_map;
=======
/* Issue an RDMA_READ using the local lkey to map the data sink */
int rdma_read_chunk_lcl(struct svcxprt_rdma *xprt,
			struct svc_rqst *rqstp,
			struct svc_rdma_op_ctxt *head,
			int *page_no,
			u32 *page_offset,
			u32 rs_handle,
			u32 rs_length,
			u64 rs_offset,
			bool last)
{
	struct ib_rdma_wr read_wr;
	int pages_needed = PAGE_ALIGN(*page_offset + rs_length) >> PAGE_SHIFT;
	struct svc_rdma_op_ctxt *ctxt = svc_rdma_get_context(xprt);
	int ret, read, pno;
	u32 pg_off = *page_offset;
	u32 pg_no = *page_no;

	ctxt->direction = DMA_FROM_DEVICE;
	ctxt->read_hdr = head;
	pages_needed = min_t(int, pages_needed, xprt->sc_max_sge_rd);
	read = min_t(int, (pages_needed << PAGE_SHIFT) - *page_offset,
		     rs_length);

	for (pno = 0; pno < pages_needed; pno++) {
		int len = min_t(int, rs_length, PAGE_SIZE - pg_off);

		head->arg.pages[pg_no] = rqstp->rq_arg.pages[pg_no];
		head->arg.page_len += len;

		head->arg.len += len;
		if (!pg_off)
			head->count++;
		rqstp->rq_respages = &rqstp->rq_arg.pages[pg_no+1];
		rqstp->rq_next_page = rqstp->rq_respages + 1;
		ctxt->sge[pno].addr =
			ib_dma_map_page(xprt->sc_cm_id->device,
					head->arg.pages[pg_no], pg_off,
					PAGE_SIZE - pg_off,
					DMA_FROM_DEVICE);
		ret = ib_dma_mapping_error(xprt->sc_cm_id->device,
					   ctxt->sge[pno].addr);
		if (ret)
			goto err;
		svc_rdma_count_mappings(xprt, ctxt);

		ctxt->sge[pno].lkey = xprt->sc_pd->local_dma_lkey;
		ctxt->sge[pno].length = len;
		ctxt->count++;

		/* adjust offset and wrap to next page if needed */
		pg_off += len;
		if (pg_off == PAGE_SIZE) {
			pg_off = 0;
			pg_no++;
		}
		rs_length -= len;
	}

	if (last && rs_length == 0)
		set_bit(RDMACTXT_F_LAST_CTXT, &ctxt->flags);
	else
		clear_bit(RDMACTXT_F_LAST_CTXT, &ctxt->flags);

	memset(&read_wr, 0, sizeof(read_wr));
	ctxt->cqe.done = svc_rdma_wc_read;
	read_wr.wr.wr_cqe = &ctxt->cqe;
	read_wr.wr.opcode = IB_WR_RDMA_READ;
	read_wr.wr.send_flags = IB_SEND_SIGNALED;
	read_wr.rkey = rs_handle;
	read_wr.remote_addr = rs_offset;
	read_wr.wr.sg_list = ctxt->sge;
	read_wr.wr.num_sge = pages_needed;

	ret = svc_rdma_send(xprt, &read_wr.wr);
	if (ret) {
		pr_err("svcrdma: Error %d posting RDMA_READ\n", ret);
		set_bit(XPT_CLOSE, &xprt->sc_xprt.xpt_flags);
		goto err;
	}

	/* return current location in page array */
	*page_no = pg_no;
	*page_offset = pg_off;
	ret = read;
	atomic_inc(&rdma_stat_read);
	return ret;
 err:
	svc_rdma_unmap_dma(ctxt);
	svc_rdma_put_context(ctxt, 0);
	return ret;
}

/* Issue an RDMA_READ using an FRMR to map the data sink */
int rdma_read_chunk_frmr(struct svcxprt_rdma *xprt,
			 struct svc_rqst *rqstp,
			 struct svc_rdma_op_ctxt *head,
			 int *page_no,
			 u32 *page_offset,
			 u32 rs_handle,
			 u32 rs_length,
			 u64 rs_offset,
			 bool last)
{
	struct ib_rdma_wr read_wr;
	struct ib_send_wr inv_wr;
	struct ib_reg_wr reg_wr;
	u8 key;
	int nents = PAGE_ALIGN(*page_offset + rs_length) >> PAGE_SHIFT;
	struct svc_rdma_op_ctxt *ctxt = svc_rdma_get_context(xprt);
	struct svc_rdma_fastreg_mr *frmr = svc_rdma_get_frmr(xprt);
	int ret, read, pno, dma_nents, n;
	u32 pg_off = *page_offset;
	u32 pg_no = *page_no;

	if (IS_ERR(frmr))
		return -ENOMEM;

	ctxt->direction = DMA_FROM_DEVICE;
	ctxt->frmr = frmr;
	nents = min_t(unsigned int, nents, xprt->sc_frmr_pg_list_len);
	read = min_t(int, (nents << PAGE_SHIFT) - *page_offset, rs_length);

	frmr->direction = DMA_FROM_DEVICE;
	frmr->access_flags = (IB_ACCESS_LOCAL_WRITE|IB_ACCESS_REMOTE_WRITE);
	frmr->sg_nents = nents;

	for (pno = 0; pno < nents; pno++) {
		int len = min_t(int, rs_length, PAGE_SIZE - pg_off);

		head->arg.pages[pg_no] = rqstp->rq_arg.pages[pg_no];
		head->arg.page_len += len;
		head->arg.len += len;
		if (!pg_off)
			head->count++;

		sg_set_page(&frmr->sg[pno], rqstp->rq_arg.pages[pg_no],
			    len, pg_off);

		rqstp->rq_respages = &rqstp->rq_arg.pages[pg_no+1];
		rqstp->rq_next_page = rqstp->rq_respages + 1;

		/* adjust offset and wrap to next page if needed */
		pg_off += len;
		if (pg_off == PAGE_SIZE) {
			pg_off = 0;
			pg_no++;
		}
		rs_length -= len;
	}

	if (last && rs_length == 0)
		set_bit(RDMACTXT_F_LAST_CTXT, &ctxt->flags);
	else
		clear_bit(RDMACTXT_F_LAST_CTXT, &ctxt->flags);

	dma_nents = ib_dma_map_sg(xprt->sc_cm_id->device,
				  frmr->sg, frmr->sg_nents,
				  frmr->direction);
	if (!dma_nents) {
		pr_err("svcrdma: failed to dma map sg %p\n",
		       frmr->sg);
		return -ENOMEM;
	}
	atomic_inc(&xprt->sc_dma_used);

	n = ib_map_mr_sg(frmr->mr, frmr->sg, frmr->sg_nents, NULL, PAGE_SIZE);
	if (unlikely(n != frmr->sg_nents)) {
		pr_err("svcrdma: failed to map mr %p (%d/%d elements)\n",
		       frmr->mr, n, frmr->sg_nents);
		return n < 0 ? n : -EINVAL;
	}

	/* Bump the key */
	key = (u8)(frmr->mr->lkey & 0x000000FF);
	ib_update_fast_reg_key(frmr->mr, ++key);

	ctxt->sge[0].addr = frmr->mr->iova;
	ctxt->sge[0].lkey = frmr->mr->lkey;
	ctxt->sge[0].length = frmr->mr->length;
	ctxt->count = 1;
	ctxt->read_hdr = head;

	/* Prepare REG WR */
	ctxt->reg_cqe.done = svc_rdma_wc_reg;
	reg_wr.wr.wr_cqe = &ctxt->reg_cqe;
	reg_wr.wr.opcode = IB_WR_REG_MR;
	reg_wr.wr.send_flags = IB_SEND_SIGNALED;
	reg_wr.wr.num_sge = 0;
	reg_wr.mr = frmr->mr;
	reg_wr.key = frmr->mr->lkey;
	reg_wr.access = frmr->access_flags;
	reg_wr.wr.next = &read_wr.wr;

	/* Prepare RDMA_READ */
	memset(&read_wr, 0, sizeof(read_wr));
	ctxt->cqe.done = svc_rdma_wc_read;
	read_wr.wr.wr_cqe = &ctxt->cqe;
	read_wr.wr.send_flags = IB_SEND_SIGNALED;
	read_wr.rkey = rs_handle;
	read_wr.remote_addr = rs_offset;
	read_wr.wr.sg_list = ctxt->sge;
	read_wr.wr.num_sge = 1;
	if (xprt->sc_dev_caps & SVCRDMA_DEVCAP_READ_W_INV) {
		read_wr.wr.opcode = IB_WR_RDMA_READ_WITH_INV;
		read_wr.wr.ex.invalidate_rkey = ctxt->frmr->mr->lkey;
	} else {
		read_wr.wr.opcode = IB_WR_RDMA_READ;
		read_wr.wr.next = &inv_wr;
		/* Prepare invalidate */
		memset(&inv_wr, 0, sizeof(inv_wr));
		ctxt->inv_cqe.done = svc_rdma_wc_inv;
		inv_wr.wr_cqe = &ctxt->inv_cqe;
		inv_wr.opcode = IB_WR_LOCAL_INV;
		inv_wr.send_flags = IB_SEND_SIGNALED | IB_SEND_FENCE;
		inv_wr.ex.invalidate_rkey = frmr->mr->lkey;
	}

	/* Post the chain */
	ret = svc_rdma_send(xprt, &reg_wr.wr);
	if (ret) {
		pr_err("svcrdma: Error %d posting RDMA_READ\n", ret);
		set_bit(XPT_CLOSE, &xprt->sc_xprt.xpt_flags);
		goto err;
	}

	/* return current location in page array */
	*page_no = pg_no;
	*page_offset = pg_off;
	ret = read;
	atomic_inc(&rdma_stat_read);
	return ret;
 err:
	svc_rdma_put_context(ctxt, 0);
	svc_rdma_put_frmr(xprt, frmr);
	return ret;
}

static unsigned int
rdma_rcl_chunk_count(struct rpcrdma_read_chunk *ch)
{
	unsigned int count;

	for (count = 0; ch->rc_discrim != xdr_zero; ch++)
		count++;
	return count;
}

/* If there was additional inline content, append it to the end of arg.pages.
 * Tail copy has to be done after the reader function has determined how many
 * pages are needed for RDMA READ.
 */
static int
rdma_copy_tail(struct svc_rqst *rqstp, struct svc_rdma_op_ctxt *head,
	       u32 position, u32 byte_count, u32 page_offset, int page_no)
{
	char *srcp, *destp;
	int ret;

	ret = 0;
	srcp = head->arg.head[0].iov_base + position;
	byte_count = head->arg.head[0].iov_len - position;
	if (byte_count > PAGE_SIZE) {
		dprintk("svcrdma: large tail unsupported\n");
		return 0;
	}

	/* Fit as much of the tail on the current page as possible */
	if (page_offset != PAGE_SIZE) {
		destp = page_address(rqstp->rq_arg.pages[page_no]);
		destp += page_offset;
		while (byte_count--) {
			*destp++ = *srcp++;
			page_offset++;
			if (page_offset == PAGE_SIZE && byte_count)
				goto more;
		}
		goto done;
	}

more:
	/* Fit the rest on the next page */
	page_no++;
	destp = page_address(rqstp->rq_arg.pages[page_no]);
	while (byte_count--)
		*destp++ = *srcp++;

	rqstp->rq_respages = &rqstp->rq_arg.pages[page_no+1];
	rqstp->rq_next_page = rqstp->rq_respages + 1;

done:
	byte_count = head->arg.head[0].iov_len - position;
	head->arg.page_len += byte_count;
	head->arg.len += byte_count;
	head->arg.buflen += byte_count;
	return 1;
}

static int rdma_read_chunks(struct svcxprt_rdma *xprt,
			    struct rpcrdma_msg *rmsgp,
			    struct svc_rqst *rqstp,
			    struct svc_rdma_op_ctxt *head)
{
	int page_no, ret;
	struct rpcrdma_read_chunk *ch;
	u32 handle, page_offset, byte_count;
	u32 position;
	u64 rs_offset;
	bool last;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* If no read list is present, return 0 */
	ch = svc_rdma_get_read_chunk(rmsgp);
	if (!ch)
		return 0;

<<<<<<< HEAD
	svc_rdma_rcl_chunk_counts(ch, &ch_count, &byte_count);
	if (ch_count > RPCSVC_MAXPAGES)
		return -EINVAL;

	/* Allocate temporary reply and chunk maps */
	rpl_map = svc_rdma_get_req_map();
	chl_map = svc_rdma_get_req_map();

	if (!xprt->sc_frmr_pg_list_len)
		sge_count = map_read_chunks(xprt, rqstp, hdr_ctxt, rmsgp,
					    rpl_map, chl_map, ch_count,
					    byte_count);
	else
		sge_count = fast_reg_read_chunks(xprt, rqstp, hdr_ctxt, rmsgp,
						 rpl_map, chl_map, ch_count,
						 byte_count);
	if (sge_count < 0) {
		err = -EIO;
		goto out;
	}

	sgl_offset = 0;
	ch_no = 0;

	for (ch = (struct rpcrdma_read_chunk *)&rmsgp->rm_body.rm_chunks[0];
	     ch->rc_discrim != 0; ch++, ch_no++) {
		u64 rs_offset;
next_sge:
		ctxt = svc_rdma_get_context(xprt);
		ctxt->direction = DMA_FROM_DEVICE;
		ctxt->frmr = hdr_ctxt->frmr;
		ctxt->read_hdr = NULL;
		clear_bit(RDMACTXT_F_LAST_CTXT, &ctxt->flags);
		clear_bit(RDMACTXT_F_FAST_UNREG, &ctxt->flags);

		/* Prepare READ WR */
		memset(&read_wr, 0, sizeof read_wr);
		read_wr.wr_id = (unsigned long)ctxt;
		read_wr.opcode = IB_WR_RDMA_READ;
		ctxt->wr_op = read_wr.opcode;
		read_wr.send_flags = IB_SEND_SIGNALED;
		read_wr.wr.rdma.rkey = ntohl(ch->rc_target.rs_handle);
		xdr_decode_hyper((__be32 *)&ch->rc_target.rs_offset,
				 &rs_offset);
		read_wr.wr.rdma.remote_addr = rs_offset + sgl_offset;
		read_wr.sg_list = ctxt->sge;
		read_wr.num_sge =
			rdma_read_max_sge(xprt, chl_map->ch[ch_no].count);
		err = rdma_set_ctxt_sge(xprt, ctxt, hdr_ctxt->frmr,
					&rpl_map->sge[chl_map->ch[ch_no].start],
					&sgl_offset,
					read_wr.num_sge);
		if (err) {
			svc_rdma_unmap_dma(ctxt);
			svc_rdma_put_context(ctxt, 0);
			goto out;
		}
		if (((ch+1)->rc_discrim == 0) &&
		    (read_wr.num_sge == chl_map->ch[ch_no].count)) {
			/*
			 * Mark the last RDMA_READ with a bit to
			 * indicate all RPC data has been fetched from
			 * the client and the RPC needs to be enqueued.
			 */
			set_bit(RDMACTXT_F_LAST_CTXT, &ctxt->flags);
			if (hdr_ctxt->frmr) {
				set_bit(RDMACTXT_F_FAST_UNREG, &ctxt->flags);
				/*
				 * Invalidate the local MR used to map the data
				 * sink.
				 */
				if (xprt->sc_dev_caps &
				    SVCRDMA_DEVCAP_READ_W_INV) {
					read_wr.opcode =
						IB_WR_RDMA_READ_WITH_INV;
					ctxt->wr_op = read_wr.opcode;
					read_wr.ex.invalidate_rkey =
						ctxt->frmr->mr->lkey;
				} else {
					/* Prepare INVALIDATE WR */
					memset(&inv_wr, 0, sizeof inv_wr);
					inv_wr.opcode = IB_WR_LOCAL_INV;
					inv_wr.send_flags = IB_SEND_SIGNALED;
					inv_wr.ex.invalidate_rkey =
						hdr_ctxt->frmr->mr->lkey;
					read_wr.next = &inv_wr;
				}
			}
			ctxt->read_hdr = hdr_ctxt;
		}
		/* Post the read */
		err = svc_rdma_send(xprt, &read_wr);
		if (err) {
			printk(KERN_ERR "svcrdma: Error %d posting RDMA_READ\n",
			       err);
			set_bit(XPT_CLOSE, &xprt->sc_xprt.xpt_flags);
			svc_rdma_unmap_dma(ctxt);
			svc_rdma_put_context(ctxt, 0);
			goto out;
		}
		atomic_inc(&rdma_stat_read);

		if (read_wr.num_sge < chl_map->ch[ch_no].count) {
			chl_map->ch[ch_no].count -= read_wr.num_sge;
			chl_map->ch[ch_no].start += read_wr.num_sge;
			goto next_sge;
		}
		sgl_offset = 0;
		err = 1;
	}

 out:
	svc_rdma_put_req_map(rpl_map);
	svc_rdma_put_req_map(chl_map);

	/* Detach arg pages. svc_recv will replenish them */
	for (ch_no = 0; &rqstp->rq_pages[ch_no] < rqstp->rq_respages; ch_no++)
		rqstp->rq_pages[ch_no] = NULL;

	/*
	 * Detach res pages. If svc_release sees any it will attempt to
	 * put them.
	 */
	while (rqstp->rq_next_page != rqstp->rq_respages)
		*(--rqstp->rq_next_page) = NULL;

	return err;
}

static int rdma_read_complete(struct svc_rqst *rqstp,
			      struct svc_rdma_op_ctxt *head)
{
	int page_no;
	int ret;

	BUG_ON(!head);
=======
	if (rdma_rcl_chunk_count(ch) > RPCSVC_MAXPAGES)
		return -EINVAL;

	/* The request is completed when the RDMA_READs complete. The
	 * head context keeps all the pages that comprise the
	 * request.
	 */
	head->arg.head[0] = rqstp->rq_arg.head[0];
	head->arg.tail[0] = rqstp->rq_arg.tail[0];
	head->hdr_count = head->count;
	head->arg.page_base = 0;
	head->arg.page_len = 0;
	head->arg.len = rqstp->rq_arg.len;
	head->arg.buflen = rqstp->rq_arg.buflen;

	/* RDMA_NOMSG: RDMA READ data should land just after RDMA RECV data */
	position = be32_to_cpu(ch->rc_position);
	if (position == 0) {
		head->arg.pages = &head->pages[0];
		page_offset = head->byte_len;
	} else {
		head->arg.pages = &head->pages[head->count];
		page_offset = 0;
	}

	ret = 0;
	page_no = 0;
	for (; ch->rc_discrim != xdr_zero; ch++) {
		if (be32_to_cpu(ch->rc_position) != position)
			goto err;

		handle = be32_to_cpu(ch->rc_target.rs_handle),
		byte_count = be32_to_cpu(ch->rc_target.rs_length);
		xdr_decode_hyper((__be32 *)&ch->rc_target.rs_offset,
				 &rs_offset);

		while (byte_count > 0) {
			last = (ch + 1)->rc_discrim == xdr_zero;
			ret = xprt->sc_reader(xprt, rqstp, head,
					      &page_no, &page_offset,
					      handle, byte_count,
					      rs_offset, last);
			if (ret < 0)
				goto err;
			byte_count -= ret;
			rs_offset += ret;
			head->arg.buflen += ret;
		}
	}

	/* Read list may need XDR round-up (see RFC 5666, s. 3.7) */
	if (page_offset & 3) {
		u32 pad = 4 - (page_offset & 3);

		head->arg.tail[0].iov_len += pad;
		head->arg.len += pad;
		head->arg.buflen += pad;
		page_offset += pad;
	}

	ret = 1;
	if (position && position < head->arg.head[0].iov_len)
		ret = rdma_copy_tail(rqstp, head, position,
				     byte_count, page_offset, page_no);
	head->arg.head[0].iov_len = position;
	head->position = position;

 err:
	/* Detach arg pages. svc_recv will replenish them */
	for (page_no = 0;
	     &rqstp->rq_pages[page_no] < rqstp->rq_respages; page_no++)
		rqstp->rq_pages[page_no] = NULL;

	return ret;
}

static void rdma_read_complete(struct svc_rqst *rqstp,
			       struct svc_rdma_op_ctxt *head)
{
	int page_no;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Copy RPC pages */
	for (page_no = 0; page_no < head->count; page_no++) {
		put_page(rqstp->rq_pages[page_no]);
		rqstp->rq_pages[page_no] = head->pages[page_no];
	}
<<<<<<< HEAD
=======

	/* Adjustments made for RDMA_NOMSG type requests */
	if (head->position == 0) {
		if (head->arg.len <= head->sge[0].length) {
			head->arg.head[0].iov_len = head->arg.len -
							head->byte_len;
			head->arg.page_len = 0;
		} else {
			head->arg.head[0].iov_len = head->sge[0].length -
								head->byte_len;
			head->arg.page_len = head->arg.len -
						head->sge[0].length;
		}
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* Point rq_arg.pages past header */
	rqstp->rq_arg.pages = &rqstp->rq_pages[head->hdr_count];
	rqstp->rq_arg.page_len = head->arg.page_len;
	rqstp->rq_arg.page_base = head->arg.page_base;

	/* rq_respages starts after the last arg page */
<<<<<<< HEAD
	rqstp->rq_respages = &rqstp->rq_arg.pages[page_no];
	rqstp->rq_next_page = &rqstp->rq_arg.pages[page_no];
=======
	rqstp->rq_respages = &rqstp->rq_pages[page_no];
	rqstp->rq_next_page = rqstp->rq_respages + 1;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Rebuild rq_arg head and tail. */
	rqstp->rq_arg.head[0] = head->arg.head[0];
	rqstp->rq_arg.tail[0] = head->arg.tail[0];
	rqstp->rq_arg.len = head->arg.len;
	rqstp->rq_arg.buflen = head->arg.buflen;
<<<<<<< HEAD

	/* Free the context */
	svc_rdma_put_context(head, 0);

	/* XXX: What should this be? */
	rqstp->rq_prot = IPPROTO_MAX;
	svc_xprt_copy_addrs(rqstp, rqstp->rq_xprt);

	ret = rqstp->rq_arg.head[0].iov_len
		+ rqstp->rq_arg.page_len
		+ rqstp->rq_arg.tail[0].iov_len;
	dprintk("svcrdma: deferred read ret=%d, rq_arg.len =%d, "
		"rq_arg.head[0].iov_base=%p, rq_arg.head[0].iov_len = %zd\n",
		ret, rqstp->rq_arg.len,	rqstp->rq_arg.head[0].iov_base,
		rqstp->rq_arg.head[0].iov_len);

	return ret;
=======
}

/* By convention, backchannel calls arrive via rdma_msg type
 * messages, and never populate the chunk lists. This makes
 * the RPC/RDMA header small and fixed in size, so it is
 * straightforward to check the RPC header's direction field.
 */
static bool
svc_rdma_is_backchannel_reply(struct svc_xprt *xprt, struct rpcrdma_msg *rmsgp)
{
	__be32 *p = (__be32 *)rmsgp;

	if (!xprt->xpt_bc_xprt)
		return false;

	if (rmsgp->rm_type != rdma_msg)
		return false;
	if (rmsgp->rm_body.rm_chunks[0] != xdr_zero)
		return false;
	if (rmsgp->rm_body.rm_chunks[1] != xdr_zero)
		return false;
	if (rmsgp->rm_body.rm_chunks[2] != xdr_zero)
		return false;

	/* sanity */
	if (p[7] != rmsgp->rm_xid)
		return false;
	/* call direction */
	if (p[8] == cpu_to_be32(RPC_CALL))
		return false;

	return true;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * Set up the rqstp thread context to point to the RQ buffer. If
 * necessary, pull additional data from the client with an RDMA_READ
 * request.
 */
int svc_rdma_recvfrom(struct svc_rqst *rqstp)
{
	struct svc_xprt *xprt = rqstp->rq_xprt;
	struct svcxprt_rdma *rdma_xprt =
		container_of(xprt, struct svcxprt_rdma, sc_xprt);
	struct svc_rdma_op_ctxt *ctxt = NULL;
	struct rpcrdma_msg *rmsgp;
	int ret = 0;
<<<<<<< HEAD
	int len;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	dprintk("svcrdma: rqstp=%p\n", rqstp);

	spin_lock_bh(&rdma_xprt->sc_rq_dto_lock);
	if (!list_empty(&rdma_xprt->sc_read_complete_q)) {
		ctxt = list_entry(rdma_xprt->sc_read_complete_q.next,
				  struct svc_rdma_op_ctxt,
				  dto_q);
		list_del_init(&ctxt->dto_q);
<<<<<<< HEAD
	}
	if (ctxt) {
		spin_unlock_bh(&rdma_xprt->sc_rq_dto_lock);
		return rdma_read_complete(rqstp, ctxt);
	}

	if (!list_empty(&rdma_xprt->sc_rq_dto_q)) {
=======
		spin_unlock_bh(&rdma_xprt->sc_rq_dto_lock);
		rdma_read_complete(rqstp, ctxt);
		goto complete;
	} else if (!list_empty(&rdma_xprt->sc_rq_dto_q)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ctxt = list_entry(rdma_xprt->sc_rq_dto_q.next,
				  struct svc_rdma_op_ctxt,
				  dto_q);
		list_del_init(&ctxt->dto_q);
	} else {
		atomic_inc(&rdma_stat_rq_starve);
		clear_bit(XPT_DATA, &xprt->xpt_flags);
		ctxt = NULL;
	}
	spin_unlock_bh(&rdma_xprt->sc_rq_dto_lock);
	if (!ctxt) {
		/* This is the EAGAIN path. The svc_recv routine will
		 * return -EAGAIN, the nfsd thread will go to call into
		 * svc_recv again and we shouldn't be on the active
		 * transport list
		 */
		if (test_bit(XPT_CLOSE, &xprt->xpt_flags))
<<<<<<< HEAD
			goto close_out;

		BUG_ON(ret);
=======
			goto defer;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		goto out;
	}
	dprintk("svcrdma: processing ctxt=%p on xprt=%p, rqstp=%p, status=%d\n",
		ctxt, rdma_xprt, rqstp, ctxt->wc_status);
<<<<<<< HEAD
	BUG_ON(ctxt->wc_status != IB_WC_SUCCESS);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	atomic_inc(&rdma_stat_recv);

	/* Build up the XDR from the receive buffers. */
	rdma_build_arg_xdr(rqstp, ctxt, ctxt->byte_len);

	/* Decode the RDMA header. */
<<<<<<< HEAD
	len = svc_rdma_xdr_decode_req(&rmsgp, rqstp);
	rqstp->rq_xprt_hlen = len;

	/* If the request is invalid, reply with an error */
	if (len < 0) {
		if (len == -ENOSYS)
			svc_rdma_send_error(rdma_xprt, rmsgp, ERR_VERS);
		goto close_out;
	}

	/* Read read-list data. */
	ret = rdma_read_xdr(rdma_xprt, rmsgp, rqstp, ctxt);
	if (ret > 0) {
		/* read-list posted, defer until data received from client. */
		goto defer;
	}
	if (ret < 0) {
=======
	rmsgp = (struct rpcrdma_msg *)rqstp->rq_arg.head[0].iov_base;
	ret = svc_rdma_xdr_decode_req(&rqstp->rq_arg);
	if (ret < 0)
		goto out_err;
	if (ret == 0)
		goto out_drop;
	rqstp->rq_xprt_hlen = ret;

	if (svc_rdma_is_backchannel_reply(xprt, rmsgp)) {
		ret = svc_rdma_handle_bc_reply(xprt->xpt_bc_xprt, rmsgp,
					       &rqstp->rq_arg);
		svc_rdma_put_context(ctxt, 0);
		if (ret)
			goto repost;
		return ret;
	}

	/* Read read-list data. */
	ret = rdma_read_chunks(rdma_xprt, rmsgp, rqstp, ctxt);
	if (ret > 0) {
		/* read-list posted, defer until data received from client. */
		goto defer;
	} else if (ret < 0) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/* Post of read-list failed, free context. */
		svc_rdma_put_context(ctxt, 1);
		return 0;
	}

<<<<<<< HEAD
=======
complete:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ret = rqstp->rq_arg.head[0].iov_len
		+ rqstp->rq_arg.page_len
		+ rqstp->rq_arg.tail[0].iov_len;
	svc_rdma_put_context(ctxt, 0);
 out:
<<<<<<< HEAD
	dprintk("svcrdma: ret = %d, rq_arg.len =%d, "
		"rq_arg.head[0].iov_base=%p, rq_arg.head[0].iov_len = %zd\n",
=======
	dprintk("svcrdma: ret=%d, rq_arg.len=%u, "
		"rq_arg.head[0].iov_base=%p, rq_arg.head[0].iov_len=%zd\n",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ret, rqstp->rq_arg.len,
		rqstp->rq_arg.head[0].iov_base,
		rqstp->rq_arg.head[0].iov_len);
	rqstp->rq_prot = IPPROTO_MAX;
	svc_xprt_copy_addrs(rqstp, xprt);
	return ret;

<<<<<<< HEAD
 close_out:
	if (ctxt)
		svc_rdma_put_context(ctxt, 1);
	dprintk("svcrdma: transport %p is closing\n", xprt);
	/*
	 * Set the close bit and enqueue it. svc_recv will see the
	 * close bit and call svc_xprt_delete
	 */
	set_bit(XPT_CLOSE, &xprt->xpt_flags);
defer:
	return 0;
=======
out_err:
	svc_rdma_send_error(rdma_xprt, rmsgp, ret);
	svc_rdma_put_context(ctxt, 0);
	return 0;

defer:
	return 0;

out_drop:
	svc_rdma_put_context(ctxt, 1);
repost:
	return svc_rdma_repost_recv(rdma_xprt, GFP_KERNEL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
