LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := ../../source/TimeSpan.cpp \
    ../../source/Stopwatch.cpp \
    ../../source/Event.cpp \
    ../../source/DateTime.cpp \
    ../../source/linux/Clock.cpp \
    ../../source/unix/Thread.cpp \
    ../../source/unix/Mutex.cpp
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2
LOCAL_STATIC_LIBRARIES := android_native_app_glue
LOCAL_MODULE    := libxpgmobile

include $(BUILD_SHARED_LIBRARY)

