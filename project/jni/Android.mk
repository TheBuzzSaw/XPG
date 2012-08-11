LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := ../../source/TimeSpan.cpp \
    ../../source/Stopwatch.cpp \
    ../../source/Event.cpp \
    ../../source/DateTime.cpp \
    ../../source/linux/Clock.cpp \
    ../../source/unix/Thread.cpp \
    ../../source/unix/Mutex.cpp
LOCAL_MODULE := xpgmobile

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := dummy
LOCAL_STATIC_LIBRARIES := xpgmobile
include $(BUILD_SHARED_LIBRARY)

