# Copyright (C) 2017 The LineageOS Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


LOCAL_PATH := $(call my-dir)
VENDOR_PATH := /vendor/
include $(CLEAR_VARS)

LOCAL_C_INCLUDES       	:= $(LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE 			:= vendor.samsung.hardware.radio@1.0
LOCAL_SRC_FILES 		:= lib/$(TARGET_ARCH_ABI)/vendor.samsung.hardware.radio@1.0.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES       	:= $(LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE 			:= backtrace  
LOCAL_SRC_FILES			:= lib/$(TARGET_ARCH_ABI)/libbacktrace.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES       	:= $(LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE 			:= base  
LOCAL_SRC_FILES			:= lib/$(TARGET_ARCH_ABI)/libbase.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES       	:= $(LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE 			:= binder
LOCAL_SRC_FILES 		:= lib/$(TARGET_ARCH_ABI)/libbinder.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES       	:= $(LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE 			:= cutils
LOCAL_SRC_FILES 		:= lib/$(TARGET_ARCH_ABI)/libcutils.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES       	:= $(LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE 			:= c
LOCAL_SRC_FILES 		:= lib/$(TARGET_ARCH_ABI)/libc.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES       	:= $(LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE 			:= c++
LOCAL_SRC_FILES 		:= lib/$(TARGET_ARCH_ABI)/libc++.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES       	:= $(LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE 			:= dl
LOCAL_SRC_FILES 		:= lib/$(TARGET_ARCH_ABI)/libdl.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES       	:= $(LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE 			:= expat 
LOCAL_SRC_FILES 		:= lib/$(TARGET_ARCH_ABI)/libexpat.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES       	:= $(LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE 			:= hardware_legacy
LOCAL_SRC_FILES 		:= lib/$(TARGET_ARCH_ABI)/libhardware_legacy.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES       	:= $(LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE 			:= log
LOCAL_SRC_FILES 		:= lib/$(TARGET_ARCH_ABI)/liblog.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES       	:= $(LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE 			:= m 
LOCAL_SRC_FILES 		:= lib/$(TARGET_ARCH_ABI)/libm.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES       	:= $(LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE 			:= netutils  
LOCAL_SRC_FILES			:= lib/$(TARGET_ARCH_ABI)/libnetutils.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES       	:= $(LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE 			:= secnativefeature
LOCAL_SRC_FILES 		:= lib/$(TARGET_ARCH_ABI)/libsecnativefeature.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES       	:= $(LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE 			:= utils    
LOCAL_SRC_FILES			:= lib/$(TARGET_ARCH_ABI)/libutils.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES       	:= $(LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE 			:= unwind  
LOCAL_SRC_FILES			:= lib/$(TARGET_ARCH_ABI)/libunwind.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES       	:= $(LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE 			:= wpa_client  
LOCAL_SRC_FILES			:= lib/$(TARGET_ARCH_ABI)/libwpa_client.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES       	:= $(LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE 			:= rilutils 
LOCAL_SRC_FILES 		:= lib/$(TARGET_ARCH_ABI)/librilutils.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES       	:= $(LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE 			:= ril
LOCAL_SRC_FILES 		:= lib/$(TARGET_ARCH_ABI)/libril.so
LOCAL_SHARED_LIBRARIES	:= \
					lib/$(TARGET_ARCH_ABI)/android.hardware.radio@1.0.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES       	:= $(LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE 			:= android.hardware.radio@1.0
LOCAL_SRC_FILES 		:= lib/$(TARGET_ARCH_ABI)/android.hardware.radio@1.0.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

LOCAL_CFLAGS += -DANDROID_MULTI_SIM
LOCAL_CFLAGS += -DANDROID_SIM_COUNT_2

LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_PROPRIETARY_MODULE := true

LOCAL_C_INCLUDES 		:= $(LOCAL_PATH)/include 
LOCAL_MODULE:= rild
LOCAL_INIT_RC := rild.rc
LOCAL_INIT_RC := rild.legacy.rc
LOCAL_SHARED_LIBRARIES	:= 	\
						libcutils \
						libc \
						libm\
						libdl \
						liblog \
						libril  
						
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
							
LOCAL_CFLAGS := -DRIL_SHLIB
LOCAL_CFLAGS += -Wall -Wextra -Werror -static-libstdc++ -fvisibility=default

LOCAL_SRC_FILES        	:= rild.c


include $(BUILD_EXECUTABLE)