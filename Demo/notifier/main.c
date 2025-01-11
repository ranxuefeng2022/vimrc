#include <linux/init.h>
#include <linux/notifier.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kern_levels.h>

#include <linux/slab.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/time.h>
#include <linux/jiffies.h>
#include "voter.h"
#include "linux/version.h"

static int log_level = 1;

#define LOG_ERR(fmt, ...)					\
do {								\
	if (log_level >= 0)					\
			printk(KERN_ERR "[ERR] ""%s[%d]: " fmt, __func__, __LINE__, ##__VA_ARGS__);	\
} while (0)

#define LOG_INFO(fmt, ...)					\
do {								\
	if (log_level >= 1)					\
			printk(KERN_ERR "[INFO] ""%s[%d]: " fmt, __func__, __LINE__, ##__VA_ARGS__);	\
} while (0)

#define LOG_DBG(fmt, ...)					\
do {								\
	if (log_level >= 2)					\
			printk(KERN_ERR "[DBG] ""%s[%d]: " fmt, __func__, __LINE__, ##__VA_ARGS__);	\
} while (0)


#define MIN_VOTER1 "MIN_VOTER1"
#define MIN_VOTER2 "MIN_VOTER2"
#define ANY_VOTER1 "ANY_VOTER1"
#define ANY_VOTER2 "ANY_VOTER2"

static char *module_str = "hello";
static int module_int = 10;
module_param(module_str, charp, S_IRUGO);
module_param(module_int, int, S_IRUGO);

static BLOCKING_NOTIFIER_HEAD(demo_notifier);
static struct platform_device g_pdev;

struct demo_chip {
	struct notifier_block demo_nb;
	struct device *dev;
	int debug_level;

	struct work_struct sys_work;
	struct delayed_work dwork;
	struct delayed_work event_work;
	struct workqueue_struct *event_wq;

	struct votable *votable_min;
	struct votable *votable_max;
	struct votable *votable_any;
};
static struct demo_chip *this_chip;

static int demo_notifier_cb(struct notifier_block *nb,
	unsigned long event, void *ptr)
{
	struct demo_chip *chip = container_of(nb, struct demo_chip, demo_nb);
	int *data = ptr;

	LOG_INFO("ev:%d data:%d debug_level:%d\n", event, *data, chip->debug_level);
	return NOTIFY_OK;
}

int demo_reg_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_register(&demo_notifier, nb);
}

void demo_unreg_notifier(struct notifier_block *nb)
{
	blocking_notifier_chain_unregister(&demo_notifier, nb);
}

void demo_notifier_broadcast(unsigned long event, void *data)
{
	blocking_notifier_call_chain(&demo_notifier,
			event, data);
}

static void demo_work(struct work_struct *work)
{
	struct demo_chip *chip = container_of(work, struct demo_chip, sys_work);

	LOG_INFO("demo work execed! log level:%d\n", chip->debug_level);
}

static void demo_delayed_work(struct work_struct *work)
{
	struct demo_chip *chip = container_of(work, struct demo_chip,
			dwork.work);

	LOG_INFO("demo work execed! log level:%d\n", chip->debug_level);
	schedule_delayed_work(&this_chip->dwork, msecs_to_jiffies(1000));
}

static void demo_event_work(struct work_struct *work)
{
	struct demo_chip *chip = container_of(work, struct demo_chip,
			event_work.work);

	LOG_INFO("demo work execed! log level:%d\n", chip->debug_level);
}

static int demo_votable_min(struct votable *votable, void *data, int value, const char *client)
{
	struct demo_chip *chip = data;
	int ret = 0;

	LOG_INFO("demo votable, debug_level:%d client %s vote %d\n", chip->debug_level, client, value);
	return ret;
}

static int demo_votable_max(struct votable *votable, void *data, int value, const char *client)
{
	struct demo_chip *chip = data;
	int ret = 0;

	LOG_INFO("demo votable, debug_level:%d client %s vote %d\n", chip->debug_level, client, value);
	return ret;
}

static int demo_votable_any(struct votable *votable, void *data, int value, const char *client)
{
	struct demo_chip *chip = data;
	int ret = 0;

	LOG_INFO("demo votable, debug_level:%d client %s vote %d\n", chip->debug_level, client, value);
	return ret;
}
/*************************************************************************************/
static int __init demo_init(void) {
	struct platform_device *pdev = &g_pdev; 
	static struct demo_chip *chip;
	int data = 2;

	this_chip = kmalloc(sizeof(*this_chip), GFP_KERNEL);

	if (IS_ERR_OR_NULL(this_chip)) {
		LOG_INFO("alloc mem failed!!!!\n");
		return -ENOMEM;
	}

	LOG_INFO("0x%x\n", LINUX_VERSION_CODE);
	LOG_INFO("module_str:%s module_int:%d\n", module_str, module_int);
	//LOG_INFO("%s 0x%x\n",UTS_RELEASE, LINUX_VERSION_CODE);
	chip = this_chip;
	chip->dev = &pdev->dev;
	chip->demo_nb.notifier_call = demo_notifier_cb;
	chip->debug_level = 3;

	platform_set_drvdata(pdev, chip);

	/*create votable*/
	chip->votable_min = create_votable("DEMO_VOTABLE_MIN", VOTE_MIN, demo_votable_min, chip);
	chip->votable_max = create_votable("DEMO_VOTABLE_MAX", VOTE_MAX, demo_votable_max, chip);
	chip->votable_any = create_votable("DEMO_VOTABLE_ANY", VOTE_SET_ANY, demo_votable_any, chip);
	if (IS_ERR_OR_NULL(chip->votable_min) 
			|| IS_ERR_OR_NULL(chip->votable_max) 
			|| IS_ERR_OR_NULL(chip->votable_any)) {

		LOG_INFO("failed to create votable:%d %d %d\n", 
				IS_ERR_OR_NULL(chip->votable_min),
				IS_ERR_OR_NULL(chip->votable_max),
				IS_ERR_OR_NULL(chip->votable_any));
		goto CREATE_VOTABLE_ERR;
	}

	vote(chip->votable_min, MIN_VOTER1, true, 100);
	vote(chip->votable_min, MIN_VOTER2, true, 200);
	vote(chip->votable_min, MIN_VOTER2, true, 50);
	vote(chip->votable_min, MIN_VOTER1, true, 50);

	vote(chip->votable_any, ANY_VOTER1, true, 0);
	vote(chip->votable_any, ANY_VOTER2, false, 0);

	/*notifier*/
	demo_reg_notifier(&chip->demo_nb);
	demo_notifier_broadcast(1, &data);

	/*work*/
	INIT_WORK(&chip->sys_work, demo_work);
	INIT_DELAYED_WORK(&chip->dwork, demo_delayed_work);
	INIT_DELAYED_WORK(&chip->event_work, demo_event_work);
	chip->event_wq = alloc_ordered_workqueue("demo_event",
			WQ_FREEZABLE);

	queue_delayed_work(chip->event_wq, &chip->event_work,
			msecs_to_jiffies(1000));

	schedule_delayed_work(&chip->dwork, msecs_to_jiffies(20000));
	queue_work(system_long_wq, &chip->sys_work);
	flush_work(&chip->sys_work);

	/*set pri data*/
	platform_set_drvdata(pdev, chip);

	//WARN_ON(1);
	//dump_stack();

	return 0;

CREATE_VOTABLE_ERR:
	kfree(chip);
	return -EINVAL;
}

static void __exit demo_exit(void) {
	struct demo_chip *chip = this_chip;

	LOG_INFO("\n");
	cancel_delayed_work_sync(&chip->dwork);
	flush_work(&chip->sys_work);
	//cancel_delayed_work_sync(&chip->event_work);
	flush_workqueue(chip->event_wq);
	destroy_workqueue(chip->event_wq);
	destroy_votable(chip->votable_any);
	destroy_votable(chip->votable_min);
	destroy_votable(chip->votable_max);
	kfree(chip);
}

module_init(demo_init);
module_exit(demo_exit);

MODULE_DESCRIPTION("demo code");
MODULE_AUTHOR("ranxuefeng");
MODULE_AUTHOR("jack.ran");
MODULE_LICENSE("GPL");
