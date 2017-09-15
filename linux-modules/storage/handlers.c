#include "storage.h"
#include "common.h"
#include <linux/fs.h>
#include <linux/printk.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/param.h>
#include <linux/kthread.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/dcache.h>
#include <linux/mutex.h>
#include <linux/mm.h>

//Send retval+buf
ssize_t handle_read_request(void *payload, uintptr_t desc){

	//pr_info("calling handle_read_request\n");

	struct m2s_read_write_payload *m2s_rq;
	m2s_rq = (struct m2s_read_write_payload *) payload;
	request rq = constuct_request(m2s_rq->uid, m2s_rq->filename, 0, m2s_rq->len, 
			m2s_rq->offset, m2s_rq->flags);

	int metadata_entry, user_entry;
	ssize_t ret;
	ssize_t *retval;
	char *readbuf;
	void *retbuf;

	int len_retbuf = m2s_rq->len + sizeof(ssize_t);

	if (unlikely(m2s_rq->len > 5*BLK_SIZE)){
		pr_info("read request is too large, request [%u].\n", m2s_rq->len);
		ret = -ENOMEM;
		goto err;
	}

	retbuf = kmalloc(len_retbuf, GFP_KERNEL);
	if (unlikely(!retbuf)){
		pr_info("No memory for read retbuf, request [%u].\n", m2s_rq->len);
		ret = -ENOMEM;
		goto err;
	}

	retval = (ssize_t *) retbuf;
	readbuf = (char *) (retbuf + sizeof(ssize_t));

#ifdef DEBUG_STORAGE
	pr_info("uid -> [%d].\n", m2s_rq->uid);
	pr_info("filename -> [%s].\n", m2s_rq->filename);
	pr_info("len -> [%u].\n", m2s_rq->len);
	pr_info("offset -> [%lu].\n", m2s_rq->offset);
	pr_info("flags -> [%o].\n", m2s_rq->flags);
#endif

	/* *retval = grant_access(&rq, &metadata_entry, &user_entry);
	if (*retval){
		goto out_reply;
	} */ /*enable in future*/
	*retval = 0;

	struct file *filp;
	filp = local_file_open(&rq);
	if (IS_ERR(filp)){
		*retval = PTR_ERR(filp);
		goto out_reply;
	}
	*retval = local_file_read(filp, (const char __user *)readbuf, rq.len, &rq.offset);
	local_file_close(filp);
	//yield_access(metadata_entry, user_entry); //enable in future
	//pr_info("Content in readbuf is [%s]\n", readbuf);

out_reply:
	ret = *retval;
	ibapi_reply_message(retbuf, len_retbuf, desc);
	kfree(retbuf);
	return ret;

err:
	ibapi_reply_message(ret, sizeof(ret), desc);
	return ret;
	
}

//send retval
ssize_t handle_write_request(void *payload, uintptr_t desc){

	//pr_info("calling handle_write_request\n");

	struct m2s_read_write_payload *m2s_wq;
	m2s_wq = (struct m2s_read_write_payload *) payload;
	request rq = constuct_request(m2s_wq->uid, m2s_wq->filename, 0, m2s_wq->len, 
			m2s_wq->offset, m2s_wq->flags);

	int metadata_entry, user_entry;
	ssize_t retval;
	char *writebuf;

	writebuf = (char *) (payload + sizeof(struct m2s_read_write_payload));

#ifdef DEBUG_STORAGE
	pr_info("uid -> [%d].\n", m2s_wq->uid);
	pr_info("filename -> [%s].\n", m2s_wq->filename);
	pr_info("len -> [%u].\n", m2s_wq->len);
	pr_info("offset -> [%lu].\n", m2s_wq->offset);
	pr_info("flags -> [%o].\n", m2s_wq->flags);
	pr_info("Content in writebuf is [%s]\n", writebuf);
#endif
	//pr_info("%c %c %c %c %c\n", writebuf[0], writebuf[1], writebuf[2], writebuf[3], writebuf[4]);
	//pr_info("%d %d %d %d %d\n", (int)writebuf[0], (int)writebuf[1], (int)writebuf[2], (int)writebuf[3], (int)writebuf[4]);

	/*retval = grant_access(&rq, &metadata_entry, &user_entry);
	if (retval){
		goto out_reply;
	}*/ //enable in future
	retval = 0;

	struct file *filp;
	filp = local_file_open(&rq);
	if (IS_ERR(filp)){
		retval = PTR_ERR(filp);
		goto out_reply;
	}
	retval = local_file_write(filp, (const char __user *)writebuf, rq.len, &rq.offset);
	local_file_close(filp);
	//yield_access(metadata_entry, user_entry); //enable in future

out_reply:
	ibapi_reply_message(&retval, sizeof(retval), desc);
	return retval;
	
}

/* Open request from processor directly */
int handle_open_request(void *payload, uintptr_t desc)
{
	struct p2s_open_struct *m2s_op = payload;
	int metadata_entry, user_entry;
	int ret;
	request rq;
	rq = constuct_request(m2s_op->uid, m2s_op->filename, m2s_op->permission, 0, 0, m2s_op->flags);
#ifdef DEBUG_STORAGE
	pr_info("handle_open_request : check pointer address : \n");
	pr_info("payload : %lu\n", m2s_op);
	pr_info("payload->uid : %lu, payload->filename : %lu\n", &m2s_op->uid, m2s_op->filename);
	pr_info("payload->permission : %lu, payload->flags : %lu\n", &m2s_op->permission, &m2s_op->flags);
	pr_info("handle_open_request : [%s]\n", m2s_op->filename);
	pr_info("handle_open_request : permission -> [0%o]\n", m2s_op->permission);
	pr_info("handle_open_request : flags -> [0x%x]\n", m2s_op->flags);
#endif
	ret = 0;

	struct file *filp;

	filp = local_file_open(&rq);
	if (IS_ERR(filp)){
		ret = PTR_ERR(filp);
		goto out_reply;
	}

	local_file_close(filp);

out_reply:
	ibapi_reply_message(&ret, sizeof(ret), desc);
}
