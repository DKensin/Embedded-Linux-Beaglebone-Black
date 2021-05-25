#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x24a52589, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xfb37adb7, __VMLINUX_SYMBOL_STR(param_ops_bool) },
	{ 0x8f454b97, __VMLINUX_SYMBOL_STR(param_ops_uint) },
	{ 0xfe990052, __VMLINUX_SYMBOL_STR(gpio_free) },
	{ 0xc1514a3b, __VMLINUX_SYMBOL_STR(free_irq) },
	{ 0x1043da48, __VMLINUX_SYMBOL_STR(gpiod_unexport) },
	{ 0xd6b8e852, __VMLINUX_SYMBOL_STR(request_threaded_irq) },
	{ 0x901bb8a, __VMLINUX_SYMBOL_STR(gpiod_to_irq) },
	{ 0x78f2aa74, __VMLINUX_SYMBOL_STR(gpiod_direction_input) },
	{ 0x2cd41dda, __VMLINUX_SYMBOL_STR(gpiod_export) },
	{ 0x3de99f27, __VMLINUX_SYMBOL_STR(gpiod_direction_output_raw) },
	{ 0x47229b5c, __VMLINUX_SYMBOL_STR(gpio_request) },
	{ 0xe0335164, __VMLINUX_SYMBOL_STR(kobject_put) },
	{ 0x21db5ec1, __VMLINUX_SYMBOL_STR(sysfs_create_group) },
	{ 0xc1da89c4, __VMLINUX_SYMBOL_STR(kobject_create_and_add) },
	{ 0x14076839, __VMLINUX_SYMBOL_STR(kernel_kobj) },
	{ 0x7b895137, __VMLINUX_SYMBOL_STR(gpiod_get_raw_value) },
	{ 0xe6da44a, __VMLINUX_SYMBOL_STR(set_normalized_timespec) },
	{ 0x94eea794, __VMLINUX_SYMBOL_STR(getnstimeofday64) },
	{ 0xb2ad9fd5, __VMLINUX_SYMBOL_STR(gpiod_set_raw_value) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x6d708b3a, __VMLINUX_SYMBOL_STR(gpiod_set_debounce) },
	{ 0xa4375714, __VMLINUX_SYMBOL_STR(gpio_to_desc) },
	{ 0x8f678b07, __VMLINUX_SYMBOL_STR(__stack_chk_guard) },
	{ 0x20c55ae0, __VMLINUX_SYMBOL_STR(sscanf) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xb1ad28e0, __VMLINUX_SYMBOL_STR(__gnu_mcount_nc) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "32247FBD678C22DB2499917");
