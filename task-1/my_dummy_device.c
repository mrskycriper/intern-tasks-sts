#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>

// Константы для вывода из устройства
#define DEVICE_NAME "my_dummy_device"
#define EXAMPLE_MSG "Uninteresting generic peace of text.\n"
#define MSG_BUFFER_LEN 38

// Объявляем функции устройства
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
static int major_num;
static int device_open_count = 0;
static char msg_buffer[MSG_BUFFER_LEN];
static char *msg_ptr;
static struct class *cls;
struct device* dev;

// Структура описывающия функции устройства
static struct file_operations file_ops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

// Вызывается при попытке чтения из устройства
static ssize_t device_read(struct file *flip, char *buffer, size_t len, loff_t *offset) {
    int bytes_read = 0;
    if (*msg_ptr == 0) {
        msg_ptr = msg_buffer;
    }
    while (len && *msg_ptr) {
        // Буффер находится в адресном пространстве юзера, потому обращаемя через специальную функцию
        put_user(*(msg_ptr++), buffer++);
        len--;
        bytes_read++;
    }
    return bytes_read;
}

// Вызывается при попытке записи в устройство
static ssize_t device_write(struct file *flip, const char *buffer, size_t len, loff_t *offset) {
    printk(KERN_ALERT "Unsupported operation. This is a readonly device.\n");
    return -EINVAL;
}

// Вызывается при открытии устройства процессом
static int device_open(struct inode *inode, struct file *file) {
    if (device_open_count) {
        return -EBUSY;
    }
    device_open_count++;
    try_module_get(THIS_MODULE);
    return 0;
}

// Вызывается при закрытии устройства процессом
static int device_release(struct inode *inode, struct file *file) {
    device_open_count--;
    module_put(THIS_MODULE);
    return 0;
}

// Вызывается при подключении модуля
static int my_dummy_device_init(void) {
    strncpy(msg_buffer, EXAMPLE_MSG, MSG_BUFFER_LEN);
    msg_ptr = msg_buffer;
    // Регистрируем устройство
    major_num = register_chrdev(0, "my_dummy_device", &file_ops);
    if (major_num < 0) {
        printk(KERN_ALERT "Could not register device on major number: %d\n", major_num);
        return -EINVAL;
    } else {
        printk(KERN_INFO "my_dummy_device module loaded with major device number %d\n", major_num);
    }

    // Создаем псевдофайл устройства для обращения к нему
    cls = class_create(THIS_MODULE, DEVICE_NAME);
    if(IS_ERR(cls)) {
        printk(KERN_ALERT "class_create() failed: %ld\n", PTR_ERR(cls));
        return -EINVAL;
    }

    dev = device_create(cls, NULL, MKDEV(major_num, 0), NULL, DEVICE_NAME);
    if(IS_ERR(dev)) {
        printk(KERN_ALERT "device_create() failed: %ld\n", PTR_ERR(dev));
        return -EINVAL;
    }

    printk(KERN_INFO "/dev/%s created\n", DEVICE_NAME);
    return 0;
}

// Вызывается при отключении модуля
static void my_dummy_device_remove(void) {
    device_destroy(cls, MKDEV(major_num, 0));
    printk(KERN_INFO "/dev/%s removed\n", DEVICE_NAME);
    class_destroy(cls);
    unregister_chrdev(major_num, DEVICE_NAME);
    printk(KERN_INFO "my_dummy_device module unloaded\n");
}

// Регистрация функций модуля
module_init(my_dummy_device_init);
module_exit(my_dummy_device_remove);

MODULE_LICENSE("GPL");