#include <system.h>

typedef struct {
	char *name;
	uint16_t id;
} vendor_t;

static vendor_t **vendor_list;

static inline uint32_t inl(uint16_t port) {
	uint32_t data;
	asm volatile("inl %1, %0" : "=a"(data) : "Nd"(port));
	return data;
}

static inline void outl(uint16_t port, uint32_t data) {
	asm volatile("outl %0, %1" : : "a"(data), "Nd"(port));
}

static inline uint16_t pci_read_word(uint16_t bus, uint16_t slot, uint16_t func,
                                     uint16_t offset) {
	uint64_t address;
	uint64_t lbus = (uint64_t)bus;
	uint64_t lslot = (uint64_t)slot;
	uint64_t lfunc = (uint64_t)func;
	uint16_t tmp = 0;
	address = (uint64_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) |
	                     (offset & 0xFC) | ((uint32_t)0x80000000));
	outl(0xCF8, address);
	tmp = (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
	return (tmp);
}

static inline uint16_t get_vendor_id(uint16_t bus, uint16_t device,
                                     uint16_t function) {
	uint32_t r0 = pci_read_word(bus, device, function, 0);
	return r0;
}

static inline uint16_t get_device_id(uint16_t bus, uint16_t device,
                                     uint16_t function) {
	uint32_t r0 = pci_read_word(bus, device, function, 2);
	return r0;
}

static inline uint16_t get_class_id(uint16_t bus, uint16_t device,
                                    uint16_t function) {
	uint32_t r0 = pci_read_word(bus, device, function, 0xA);
	return (r0 & ~0x00FF) >> 8;
}

static inline uint16_t get_sub_class_id(uint16_t bus, uint16_t device,
                                        uint16_t function) {
	uint32_t r0 = pci_read_word(bus, device, function, 0xA);
	return (r0 & ~0xFF00);
}

static inline void scan( ) {
	uint64_t devices = 0;
	for (uint32_t bus = 0; bus < 256; bus++) {
		for (uint32_t slot = 0; slot < 32; slot++) {
			for (uint32_t function = 0; function < 8; function++) {
				uint16_t vendor = get_vendor_id(bus, slot, function);

				if (vendor == 0xFFFF) { continue; }

				uint16_t device_id = get_device_id(bus, slot, function);
				uint16_t class_id = get_class_id(bus, slot, function);

				fb_printf("[%u] vendor: 0x%x, device: 0x%x, class: %u\n",
				          devices, vendor, device_id, class_id);

				devices++;
			}
		}
	}
}

module_info_t __attribute__((section(".minit"))) init(env_t *env) {
	init_env(env);

	module_info_t *pci_data = get_module("[PCI][DATA]");

	if (pci_data == NULL) {
		fb_printf("Модуль PCI данных не найден!\n");
	} else {
		fb_printf("Записей в базе PCI: %u\n", pci_data->data_size);
		vendor_list = (vendor_t **)pci_data->data;
	}

	scan( );
	return (module_info_t){
		.name = (char *)"[PCI]",
		.message = (char *)"PCI драйвер",
		.type = 0,
		.data_size = 0,
		.data = (void *)0,
		.err_code = 0,
		.module_id = 0,
	};
}