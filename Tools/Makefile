MODULES = DumpRenderTree CyberKitTestRunner MiniBrowser ../Source/ThirdParty/gtest/xcode TestCyberKitAPI

ifneq (,$(SDKROOT))
	ifeq (,$(findstring macosx,$(SDKROOT)))
		# Embedded OS
		MODULES = DumpRenderTree CyberKitTestRunner ../Source/ThirdParty/gtest/xcode TestCyberKitAPI
	endif
	ifneq (,$(findstring macosx,$(SDKROOT)))
		MODULES += lldb/lldbCyberKitTester
	endif
	ifneq (,$(findstring iphone,$(SDKROOT)))
		MODULES += MobileMiniBrowser
	endif
endif
ifeq (,$(DO_NOT_BUILD_IMAGE_DIFF))
	MODULES += ImageDiff
endif

all:
	@for dir in $(MODULES); do ${MAKE} $@ -C $$dir; exit_status=$$?; \
	if [ $$exit_status -ne 0 ]; then exit $$exit_status; fi; done

debug d development dev develop:
	@for dir in $(MODULES); do ${MAKE} $@ -C $$dir; exit_status=$$?; \
	if [ $$exit_status -ne 0 ]; then exit $$exit_status; fi; done

release r deployment dep deploy:
	@for dir in $(MODULES); do ${MAKE} $@ -C $$dir; exit_status=$$?; \
	if [ $$exit_status -ne 0 ]; then exit $$exit_status; fi; done

analyze:
	@for dir in $(MODULES); do ${MAKE} $@ -C $$dir; exit_status=$$?; \
	if [ $$exit_status -ne 0 ]; then exit $$exit_status; fi; done

clean:
	@for dir in $(MODULES); do ${MAKE} $@ -C $$dir; exit_status=$$?; \
	if [ $$exit_status -ne 0 ]; then exit $$exit_status; fi; done
