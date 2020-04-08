# Samsung-hardware-ril
 Precompile binary with standalone NDK
 
 Prebuilt RILD will allow any library name to working it. current build support for android OREO with architecture arm64-v8a, armeabi-v7a and armeabi

 ## NOTED:
 ----------------------------
 Choose toolchains stable version and follow below command
 
 ## COMMAND BUILD on windows
 ----------------------------
 ..\ndk-build.cmd APP_BUILD_SCRIPT=..\rild\Android.mk APP_ABI=arm64-v8a NDK_ALL_ABIS=arm64-v8a APP_PLATFORM=android-19 NDK_PROJECT_PATH=..\rild
 
  ## INSTALL ON DEVICE
  ----------------------------
  "Set permission"
  set_metadata("/system/vendor/bin/hw/rild","uid",0,"gid",2000,"mode",0755,"capabilities",0x0,"selabel","u:object_r:rild_exec:s0");