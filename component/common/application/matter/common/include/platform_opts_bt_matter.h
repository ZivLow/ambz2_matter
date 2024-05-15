#ifndef __PLATFORM_OPTS_BT_MATTER_H__
#define __PLATFORM_OPTS_BT_MATTER_H__

#if defined(CONFIG_PLATFORM_8710C)
#if CONFIG_BT

/* For Matter Application */
#ifdef CONFIG_BT_MESH_WITH_MATTER 		// this is defined in application.is.matter.mk
#undef CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE
#undef CONFIG_BT_MESH_DEVICE_MATTER
#define CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE		1
#define CONFIG_BT_MESH_DEVICE_MATTER				1
#elif CONFIG_BLE_MATTER_ADAPTER 		// this is defined in application.is.matter.mk
#undef CONFIG_BLE_MATTER_MULTI_ADV
#define CONFIG_BLE_MATTER_MULTI_ADV					1
#else
#define CONFIG_BT_MATTER_ADAPTER					1
#endif

/* For Matter Mesh Application */
#if (CONFIG_BT_MESH_DEVICE_MATTER && !CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE) || \
	(!CONFIG_BT_MESH_DEVICE_MATTER && CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE)
#error "Please enable both CONFIG_BT_MESH_DEVICE_MATTER & CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE"
#endif

#if (!CONFIG_BT_MATTER_ADAPTER && !CONFIG_BLE_MATTER_ADAPTER && !CONFIG_BT_MESH_DEVICE_MATTER)
#error "Please enable either CONFIG_BT_MATTER_ADAPTER or CONFIG_BLE_MATTER_ADAPTER or CONFIG_BT_MESH_DEVICE_MATTER"
#endif

#endif /* CONFIG_BT */
#endif /* CONFIG_PLATFORM_8710C */

#endif /* __PLATFORM_OPTS_BT_H__ */
