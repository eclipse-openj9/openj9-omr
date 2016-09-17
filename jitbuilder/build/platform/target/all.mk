#
# If user didn't pass a target arch, assume same as host
#
ifeq (,$(TARGET_ARCH))
    TARGET_ARCH=$(HOST_ARCH)
    TARGET_SUBARCH=$(HOST_SUBARCH)
    TARGET_BITS=$(HOST_BITS)
endif
