LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := ../../source/TimeSpan.cpp \
    ../../source/Stopwatch.cpp \
    ../../source/Event.cpp \
    ../../source/DateTime.cpp \
    ../../source/linux/Clock.cpp \
    ../../source/unix/Thread.cpp \
    ../../source/unix/Mutex.cpp \
    ../../source/android/Main.cpp \
		../../source/android/Screen.cpp
LOCAL_MODULE := xpgmobile
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_STATIC_LIBRARY)

$(call import-module,android/native_app_glue)

include $(CLEAR_VARS)
LOCAL_MODULE := dummy
LOCAL_STATIC_LIBRARIES := xpgmobile
include $(BUILD_SHARED_LIBRARY)

