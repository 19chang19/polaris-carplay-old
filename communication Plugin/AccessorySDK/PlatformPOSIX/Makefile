#
#	File:    	Makefile
#	Package: 	Apple CarPlay Communication Plug-in.
#	Abstract: 	n/a 
#	Version: 	410.12
#	
#	Disclaimer: IMPORTANT: This Apple software is supplied to you, by Apple Inc. ("Apple"), in your
#	capacity as a current, and in good standing, Licensee in the MFi Licensing Program. Use of this
#	Apple software is governed by and subject to the terms and conditions of your MFi License,
#	including, but not limited to, the restrictions specified in the provision entitled ”Public 
#	Software”, and is further subject to your agreement to the following additional terms, and your 
#	agreement that the use, installation, modification or redistribution of this Apple software
#	constitutes acceptance of these additional terms. If you do not agree with these additional terms,
#	please do not use, install, modify or redistribute this Apple software.
#	
#	Subject to all of these terms and in consideration of your agreement to abide by them, Apple grants
#	you, for as long as you are a current and in good-standing MFi Licensee, a personal, non-exclusive 
#	license, under Apple's copyrights in this original Apple software (the "Apple Software"), to use, 
#	reproduce, and modify the Apple Software in source form, and to use, reproduce, modify, and 
#	redistribute the Apple Software, with or without modifications, in binary form. While you may not 
#	redistribute the Apple Software in source form, should you redistribute the Apple Software in binary
#	form, you must retain this notice and the following text and disclaimers in all such redistributions
#	of the Apple Software. Neither the name, trademarks, service marks, or logos of Apple Inc. may be
#	used to endorse or promote products derived from the Apple Software without specific prior written
#	permission from Apple. Except as expressly stated in this notice, no other rights or licenses, 
#	express or implied, are granted by Apple herein, including but not limited to any patent rights that
#	may be infringed by your derivative works or by other works in which the Apple Software may be 
#	incorporated.  
#	
#	Unless you explicitly state otherwise, if you provide any ideas, suggestions, recommendations, bug 
#	fixes or enhancements to Apple in connection with this software (“Feedback”), you hereby grant to
#	Apple a non-exclusive, fully paid-up, perpetual, irrevocable, worldwide license to make, use, 
#	reproduce, incorporate, modify, display, perform, sell, make or have made derivative works of,
#	distribute (directly or indirectly) and sublicense, such Feedback in connection with Apple products 
#	and services. Providing this Feedback is voluntary, but if you do provide Feedback to Apple, you 
#	acknowledge and agree that Apple may exercise the license granted above without the payment of 
#	royalties or further consideration to Participant.
#	
#	The Apple Software is provided by Apple on an "AS IS" basis. APPLE MAKES NO WARRANTIES, EXPRESS OR 
#	IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY 
#	AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR
#	IN COMBINATION WITH YOUR PRODUCTS.
#	
#	IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
#	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#	PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION 
#	AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT
#	(INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE 
#	POSSIBILITY OF SUCH DAMAGE.
#	
#	Copyright (C) 2013-2015 Apple Inc. All Rights Reserved. Not to be used or disclosed without permission from Apple.
#
#	Build options
#	-------------
#	aes_gcm		-- 1=Include AES-GCM support.
#	arch		-- "arm" for ARMv7 or "x86" for x86.
#	debug		-- 1=Compile in debug code, asserts, etc. 0=Strip out debug code for a release build.
#	hidbrowser  -- 1=Use legacy HIDBrowser & Screen Registration
#	libtommath	-- 1=Use LibTomMath for big number math.
#	mfi			-- 1=Enable support for MFi auth IC's.
#	nobonjour	-- 1=Don't include Bonjour support.
#	nostrip		-- 1=Don't strip binaries.
#	nvtools		-- 1=Build using NVIDIA's Vibrante toolchain (only works on a Linux host).
#	openssl		-- 1=Use OpenSSL for AES, SHA*, etc. 0=Compile in AES, SHA*, etc. code directly.
#	os			-- "linux" or "qnx".
#

INTERACTIVE := $(shell [ -t 0 ] && echo 1)
ifdef INTERACTIVE
	ColorMagenta=\033[0;35m
	ColorCyan=\033[0;36m
	ColorEnd=\033[0m
endif

ifeq ($(debug),1)
	config	= Debug
else
	config	= Release
endif
ifeq ($(aes_gcm),1)
	gladman_aes = 1
endif
ifneq ($(openssl),1)
	gladman_aes = 1
endif
ifneq ($(verbose),1)
	quiet	= @
endif

# Setup toolchain.
SRCROOT = $(CURDIR)/..
ifeq ($(os),linux)
	LINUX_TOOLS_DIR	?= $(SRCROOT)/../x-tools
endif
ifeq ($(os),qnx)
	QNX_TOOLS_DIR	?= $(CURDIR)/../../x-qnx
	ifneq ("$(wildcard $(QNX_TOOLS_DIR)/env.mk)","")
		include $(QNX_TOOLS_DIR)/env.mk
	else
		qnxold = 1
	endif
endif
ifeq ($(nvtools),1)
	# NVIDIA's Vibrante toolchain (Linux host only).
	NVTOOLSPATH	= ../../..
	include $(NVTOOLSPATH)/make/nvdefs.mk
	CFLAGS		= $(NV_PLATFORM_OPT) $(NV_PLATFORM_CFLAGS)
	CPPFLAGS	= $(NV_PLATFORM_SDK_INC) $(NV_PLATFORM_CPPFLAGS)
	LINKFLAGS	= $(NV_PLATFORM_SDK_LIB) $(NV_PLATFORM_LDFLAGS)
endif
ifeq ($(os),qnx)
	# QNX toolchain.
	ifeq ($(arch),arm)
		qnx_arch		= gcc_ntoarmv7le
		qnx_platform	= armle-v7
		qnx_prefix		= ntoarmv7
	else
		ifeq ($(arch),x86)
			qnx_arch		= gcc_ntox86
			qnx_platform	= x86
			qnx_prefix		= ntox86
		endif
	endif
	
	CROSSROOT			= $(QNX_TARGET)
	CROSSLIB			= $(CROSSROOT)/$(qnx_platform)/usr/lib
	CROSSPREFIX			= $(QNX_HOST)/usr/bin/$(qnx_prefix)-
	CROSS_RROOT			= target/qnx6
	CROSS_RLIB			= $(CROSS_RROOT)/$(qnx_platform)/usr/lib
	
	AR					= $(CROSSPREFIX)ar
	CC					= $(QNX_HOST)/usr/bin/qcc
	CXX					= $(QNX_HOST)/usr/bin/qcc
	LD					= $(CROSSPREFIX)ld
	NM					= $(CROSSPREFIX)nm
	RANLIB				= $(CROSSPREFIX)ranlib
	STRIP				= $(CROSSPREFIX)strip
	
	CFLAGS				+= -V$(qnx_arch)_cpp -w1
	CXXFLAGS			+= -V$(qnx_arch)_cpp -w1
	LINKFLAGS			+= -V$(qnx_arch)_cpp -w1 -lang-c++
else
	ifeq ($(arch),arm)
		# Generic ARM-based Linux toolchain.
		CROSSBASE		= $(LINUX_TOOLS_DIR)/arm-unknown-linux-gnueabi
		CROSSBASEREAL	= $(realpath $(CROSSBASE))
		CROSSROOT		= $(CROSSBASEREAL)/arm-unknown-linux-gnueabi/sysroot
		CROSSLIB		= $(CROSSROOT)/usr/lib
		CROSSPREFIX		= $(CROSSBASEREAL)/bin/arm-unknown-linux-gnueabi-
		CROSS_RROOT		= arm-unknown-linux-gnueabi/arm-unknown-linux-gnueabi/sysroot
		CROSS_RLIB		= $(CROSS_RROOT)/usr/lib
		
		AR				= $(CROSSPREFIX)ar
		CC				= $(CROSSPREFIX)gcc
		CXX				= $(CROSSPREFIX)g++
		LD				= $(CROSSPREFIX)gcc
		NM				= $(CROSSPREFIX)nm
		RANLIB			= $(CROSSPREFIX)ranlib
		STRIP			= $(CROSSPREFIX)strip
	else
		CROSSLIB		?= /usr/lib
		CROSS_RROOT		= ""
		CROSS_RLIB		= usr/lib
		
		AR				?= ar
		CC				?= gcc
		CXX				?= g++
		LD				?= gcc
		NM				?= nm
		RANLIB			?= ranlib
		STRIP			?= strip
	endif
endif

BUILDROOT		= $(SRCROOT)/build/$(config)-$(os)-$(arch)
DSTROOTDIR		= $(BUILDROOT)/root.dst
OBJDIR			= $(BUILDROOT)/obj
MDNSROOT		= $(SRCROOT)/../mDNSResponder

TARGETS			+= $(BUILDROOT)/aktest
TARGETS			+= $(BUILDROOT)/libCoreUtils.so

# Paths
VPATH			+= $(SRCROOT)/External/Curve25519
VPATH			+= $(SRCROOT)/External/Ed25519
ifeq ($(gladman_aes),1)
	VPATH		+= $(SRCROOT)/External/GladmanAES
endif
ifeq ($(libtommath),1)
	VPATH		+= $(SRCROOT)/External/LibTomMath
endif
VPATH			+= $(SRCROOT)/External/Small25519
VPATH			+= $(SRCROOT)/External/SRP
VPATH			+= $(SRCROOT)/PlatformPOSIX
VPATH			+= $(SRCROOT)/Sources
VPATH			+= $(SRCROOT)/Support

# Includes
INCLUDES		+= -I$(SRCROOT)/External/Curve25519
INCLUDES		+= -I$(SRCROOT)/External/Ed25519
ifeq ($(gladman_aes),1)
	INCLUDES	+= -I$(SRCROOT)/External/GladmanAES
endif
ifeq ($(libtommath),1)
	INCLUDES	+= -I$(SRCROOT)/External/LibTomMath
endif
INCLUDES		+= -I$(SRCROOT)/External/Small25519
INCLUDES		+= -I$(SRCROOT)/External/SRP
INCLUDES		+= -I$(SRCROOT)/Sources
INCLUDES		+= -I$(SRCROOT)/Support


# aktest
aktest_SRCS		+= aktest.c

# Support
CoreUtils_SRCS	+= AESUtils.c
CoreUtils_SRCS	+= AsyncConnection.c
aktest_SRCS		+= AtomicUtils.c
ifneq ($(nobonjour),1)
	CoreUtils_SRCS	+= BonjourBrowser.c
endif
CoreUtils_SRCS	+= CFCompat.c
CoreUtils_SRCS	+= CFLite.c
CoreUtils_SRCS	+= CFLiteBinaryPlist.c
aktest_SRCS		+= CFLiteNotifications.c
CoreUtils_SRCS	+= CFLitePreferencesFile.c
CoreUtils_SRCS	+= CFLiteRunLoopSelect.c
CoreUtils_SRCS	+= CFUtils.c
CoreUtils_SRCS	+= ChaCha20Poly1305.c
CoreUtils_SRCS	+= CommandLineUtils.c
CoreUtils_SRCS	+= curve25519-donna.c
aktest_SRCS		+= curve25519-test.c
CoreUtils_SRCS	+= DataBufferUtils.c
CoreUtils_SRCS	+= DebugIPCUtils.c
CoreUtils_SRCS	+= DebugServices.c
CoreUtils_SRCS	+= DispatchLite.c
CoreUtils_SRCS	+= ed25519.c
aktest_SRCS		+= ed25519-test.c
ifeq ($(hidbrowser),1)
CoreUtils_SRCS	+= HIDUtils.c
endif
CoreUtils_SRCS	+= HTTPClient.c
CoreUtils_SRCS	+= HTTPMessage.c
CoreUtils_SRCS	+= HTTPServer.c
aktest_SRCS		+= HTTPTest.c
CoreUtils_SRCS	+= HTTPUtils.c
CoreUtils_SRCS	+= KeychainLiteFile.c
CoreUtils_SRCS	+= KeychainUtils.c
CoreUtils_SRCS	+= LogUtils.c
CoreUtils_SRCS	+= MathUtils.c
ifeq ($(mfi),1)
	CoreUtils_SRCS += MFiSAP.c
	ifeq ($(os),linux)
		CoreUtils_SRCS += MFiServerPlatformLinux.c
	endif
	ifeq ($(os),qnx)
		CoreUtils_SRCS += MFiServerPlatformQNX.c
	endif
endif
CoreUtils_SRCS	+= MiscUtils.c
CoreUtils_SRCS	+= NetTransportChaCha20Poly1305.c
CoreUtils_SRCS	+= NetUtils.c
CoreUtils_SRCS	+= NetworkChangeListener.c
CoreUtils_SRCS	+= PairingUtils.c
CoreUtils_SRCS	+= PrintFUtils.c
CoreUtils_SRCS	+= RandomNumberUtils.c
CoreUtils_SRCS	+= SHAUtils.c
aktest_SRCS		+= Small25519.c
CoreUtils_SRCS	+= SRPUtils.c
CoreUtils_SRCS	+= StringUtils.c
CoreUtils_SRCS	+= ThreadUtils.c
CoreUtils_SRCS	+= TickUtils.c
CoreUtils_SRCS	+= TimeUtils.c
CoreUtils_SRCS	+= TLVUtils.c
CoreUtils_SRCS	+= URLUtils.c
CoreUtils_SRCS	+= utfconv.c
CoreUtils_SRCS	+= UUIDUtils.c

ifeq ($(gladman_aes),1)
	# GladmanAES
	CoreUtils_SRCS	+= aes_modes.c
	CoreUtils_SRCS	+= aescrypt.c
	CoreUtils_SRCS	+= aeskey.c
	CoreUtils_SRCS	+= aestab.c
endif
ifeq ($(aes_gcm),1)
	CoreUtils_SRCS	+= gcm.c
	CoreUtils_SRCS	+= gf128mul.c
endif

ifeq ($(libtommath),1)
	# LibTomMath
	CoreUtils_SRCS	+= bn_fast_mp_invmod.c
	CoreUtils_SRCS	+= bn_fast_mp_montgomery_reduce.c
	CoreUtils_SRCS	+= bn_fast_s_mp_mul_digs.c
	CoreUtils_SRCS	+= bn_fast_s_mp_mul_high_digs.c
	CoreUtils_SRCS	+= bn_fast_s_mp_sqr.c
	CoreUtils_SRCS	+= bn_mp_2expt.c
	CoreUtils_SRCS	+= bn_mp_abs.c
	CoreUtils_SRCS	+= bn_mp_add_d.c
	CoreUtils_SRCS	+= bn_mp_add.c
	CoreUtils_SRCS	+= bn_mp_clamp.c
	CoreUtils_SRCS	+= bn_mp_clear_multi.c
	CoreUtils_SRCS	+= bn_mp_clear.c
	CoreUtils_SRCS	+= bn_mp_cmp_d.c
	CoreUtils_SRCS	+= bn_mp_cmp_mag.c
	CoreUtils_SRCS	+= bn_mp_cmp.c
	CoreUtils_SRCS	+= bn_mp_cnt_lsb.c
	CoreUtils_SRCS	+= bn_mp_copy.c
	CoreUtils_SRCS	+= bn_mp_count_bits.c
	CoreUtils_SRCS	+= bn_mp_div_2.c
	CoreUtils_SRCS	+= bn_mp_div_2d.c
	CoreUtils_SRCS	+= bn_mp_div_3.c
	CoreUtils_SRCS	+= bn_mp_div_d.c
	CoreUtils_SRCS	+= bn_mp_div.c
	CoreUtils_SRCS	+= bn_mp_dr_is_modulus.c
	CoreUtils_SRCS	+= bn_mp_dr_reduce.c
	CoreUtils_SRCS	+= bn_mp_dr_setup.c
	CoreUtils_SRCS	+= bn_mp_exch.c
	CoreUtils_SRCS	+= bn_mp_exptmod_fast.c
	CoreUtils_SRCS	+= bn_mp_exptmod.c
	CoreUtils_SRCS	+= bn_mp_grow.c
	CoreUtils_SRCS	+= bn_mp_init_copy.c
	CoreUtils_SRCS	+= bn_mp_init_multi.c
	CoreUtils_SRCS	+= bn_mp_init_size.c
	CoreUtils_SRCS	+= bn_mp_init.c
	CoreUtils_SRCS	+= bn_mp_invmod_slow.c
	CoreUtils_SRCS	+= bn_mp_invmod.c
	CoreUtils_SRCS	+= bn_mp_karatsuba_mul.c
	CoreUtils_SRCS	+= bn_mp_karatsuba_sqr.c
	CoreUtils_SRCS	+= bn_mp_lshd.c
	CoreUtils_SRCS	+= bn_mp_mod_2d.c
	CoreUtils_SRCS	+= bn_mp_mod_d.c
	CoreUtils_SRCS	+= bn_mp_mod.c
	CoreUtils_SRCS	+= bn_mp_montgomery_calc_normalization.c
	CoreUtils_SRCS	+= bn_mp_montgomery_reduce.c
	CoreUtils_SRCS	+= bn_mp_montgomery_setup.c
	CoreUtils_SRCS	+= bn_mp_mul_2.c
	CoreUtils_SRCS	+= bn_mp_mul_2.c
	CoreUtils_SRCS	+= bn_mp_mul_2d.c
	CoreUtils_SRCS	+= bn_mp_mul_d.c
	CoreUtils_SRCS	+= bn_mp_mul.c
	CoreUtils_SRCS	+= bn_mp_mulmod.c
	CoreUtils_SRCS	+= bn_mp_prime_is_divisible.c
	CoreUtils_SRCS	+= bn_mp_prime_is_prime.c
	CoreUtils_SRCS	+= bn_mp_prime_miller_rabin.c
	CoreUtils_SRCS	+= bn_mp_prime_rabin_miller_trials.c
	CoreUtils_SRCS	+= bn_mp_read_unsigned_bin.c
	CoreUtils_SRCS	+= bn_mp_reduce_2k_l.c
	CoreUtils_SRCS	+= bn_mp_reduce_2k_setup_l.c
	CoreUtils_SRCS	+= bn_mp_reduce_2k_setup.c
	CoreUtils_SRCS	+= bn_mp_reduce_2k.c
	CoreUtils_SRCS	+= bn_mp_reduce_is_2k_l.c
	CoreUtils_SRCS	+= bn_mp_reduce_is_2k.c
	CoreUtils_SRCS	+= bn_mp_reduce_setup.c
	CoreUtils_SRCS	+= bn_mp_reduce.c
	CoreUtils_SRCS	+= bn_mp_rshd.c
	CoreUtils_SRCS	+= bn_mp_set_int.c
	CoreUtils_SRCS	+= bn_mp_set.c
	CoreUtils_SRCS	+= bn_mp_sqr.c
	CoreUtils_SRCS	+= bn_mp_sqrmod.c
	CoreUtils_SRCS	+= bn_mp_sub_d.c
	CoreUtils_SRCS	+= bn_mp_sub.c
	CoreUtils_SRCS	+= bn_mp_to_unsigned_bin.c
	CoreUtils_SRCS	+= bn_mp_toom_mul.c
	CoreUtils_SRCS	+= bn_mp_toom_sqr.c
	CoreUtils_SRCS	+= bn_mp_unsigned_bin_size.c
	CoreUtils_SRCS	+= bn_mp_zero.c
	CoreUtils_SRCS	+= bn_prime_tab.c
	CoreUtils_SRCS	+= bn_reverse.c
	CoreUtils_SRCS	+= bn_s_mp_add.c
	CoreUtils_SRCS	+= bn_s_mp_exptmod.c
	CoreUtils_SRCS	+= bn_s_mp_mul_digs.c
	CoreUtils_SRCS	+= bn_s_mp_mul_high_digs.c
	CoreUtils_SRCS	+= bn_s_mp_sqr.c
	CoreUtils_SRCS	+= bn_s_mp_sub.c
	CoreUtils_SRCS	+= bncore.c
endif

# SRP
CoreUtils_SRCS	+= cstr.c
CoreUtils_SRCS	+= srp.c
CoreUtils_SRCS	+= srp6_client.c
CoreUtils_SRCS	+= srp6_server.c
CoreUtils_SRCS	+= t_math.c

aktest_SRCS		+= $(CoreUtils_SRCS)

# Map sources to built objects in the obj folder.
aktest_OBJS				= $(patsubst %.c,$(OBJDIR)/%.o,$(aktest_SRCS))
CoreUtils_OBJS_direct	= $(patsubst %.c,$(OBJDIR)/%.o,$(CoreUtils_SRCS))
CoreUtils_OBJS_shared	= $(patsubst %.c,$(OBJDIR)/%.so.o,$(CoreUtils_SRCS))

# Libraries
LINKFLAGS		+= -L$(BUILDROOT)

ifeq ($(os),qnx)
	LIBS		+= -lbacktrace
endif
LIBS			+= -lc
ifeq ($(openssl),1)
	LIBS		+= -lcrypto
endif
ifneq ($(os),qnx)
	LIBS		+= -ldl
endif
ifneq ($(nobonjour),1)
	LIBS		+= -ldns_sd
endif
ifeq ($(os),qnx)
	LIBS		+= -lhiddi
endif
LIBS			+= -lm
ifeq ($(os),qnx)
	LIBS		+= -lsocket
else
	LIBS		+= -lpthread
	LIBS		+= -lrt
endif

# Bonjour additions for projects building it directly.
INCLUDES		+= -I$(MDNSROOT)/mDNSShared
LINKFLAGS		+= -L$(MDNSROOT)/mDNSPosix/build/prod

# Flags
COMMONFLAGS		+= -D_GNU_SOURCE
COMMONFLAGS		+= -DDEBUG_EXPORT_ERROR_STRINGS=1
COMMONFLAGS		+= -DFRAMEWORK_STYLE_INCLUDES=0
COMMONFLAGS		+= -DTARGET_HAS_NATIVE_INT128=0
COMMONFLAGS		+= -DTARGET_NEEDS_NATURAL_ALIGNMENT=1

ifeq ($(debug),1)
	COMMONFLAGS	+= -DDEBUG=1
else
	COMMONFLAGS	+= -DNDEBUG=1
endif
ifeq ($(mfi),1)
endif
ifeq ($(nobonjour),1)
	COMMONFLAGS	+= -D_DNS_SD_LIBDISPATCH=0
	COMMONFLAGS	+= -DASYNC_CONNECTION_BONJOUR=0
	COMMONFLAGS	+= -DBONJOUR_ENABLED=0
endif
ifeq ($(libtommath),1)
	COMMONFLAGS	+= -DTARGET_HAS_LIBTOMMATH=1
endif
ifeq ($(aes_gcm),1)
	COMMONFLAGS	+= -DAES_UTILS_HAS_GLADMAN_GCM=1
else
	COMMONFLAGS	+= -DAES_UTILS_HAS_GLADMAN_GCM=0
endif
ifeq ($(gladman_aes),1)
	COMMONFLAGS	+= -DAES_UTILS_USE_GLADMAN_AES=1
	COMMONFLAGS	+= -DUSE_VIA_ACE_IF_PRESENT=0
endif
ifneq ($(openssl),1)
	COMMONFLAGS	+= -DTARGET_HAS_MD5_UTILS=1
	COMMONFLAGS	+= -DTARGET_HAS_SHA_UTILS=1
	COMMONFLAGS	+= -DTARGET_NO_OPENSSL=1
endif

ifneq ($(qnxold),1)
	COMMON_WARNINGS	+= -pedantic
	COMMON_WARNINGS	+= -std=gnu99
endif
COMMON_WARNINGS	+= -W
COMMON_WARNINGS	+= -Wall
COMMON_WARNINGS	+= -Wextra

COMMON_WARNINGS	+= -Wempty-body
COMMON_WARNINGS	+= -Werror
COMMON_WARNINGS	+= -Wformat
COMMON_WARNINGS	+= -Wmissing-braces
COMMON_WARNINGS	+= -Wmissing-field-initializers
COMMON_WARNINGS	+= -Wparentheses
COMMON_WARNINGS	+= -Wpointer-sign
COMMON_WARNINGS	+= -Wshadow
COMMON_WARNINGS	+= -Wsign-compare
COMMON_WARNINGS	+= -Wswitch
COMMON_WARNINGS	+= -Wundef
COMMON_WARNINGS	+= -Wuninitialized
COMMON_WARNINGS	+= -Wunknown-pragmas
COMMON_WARNINGS	+= -Wunused-function
COMMON_WARNINGS	+= -Wunused-label
COMMON_WARNINGS	+= -Wunused-parameter
COMMON_WARNINGS	+= -Wunused-value
COMMON_WARNINGS	+= -Wunused-variable

COMMON_WARNINGS	+= -Wno-cast-align
COMMON_WARNINGS	+= -Wno-unused-result

CWARNS			+= $(COMMON_WARNINGS)
CWARNS			+= -Wmissing-prototypes
CXXWARNS		+= $(COMMON_WARNINGS)
CXXWARNS		+= -Wnon-virtual-dtor
CXXWARNS		+= -Woverloaded-virtual

CFLAGS			+= $(INCLUDES) $(COMMONFLAGS) $(CWARNS) -Os
CFLAGS_direct	+= -DSRP_TESTS=1 -DSNPRINTF_USE_ASSERTS=0
CXXFLAGS		+= $(INCLUDES) $(COMMONFLAGS) $(CXXWARNS) -Os

# Rules
.PHONY : all
all: $(TARGETS)


# aktest
.PHONY : aktest
aktest : $(BUILDROOT)/aktest

$(BUILDROOT)/aktest: $(aktest_OBJS)
	@echo "Linking ($(os)-$(arch)-$(config)) $(ColorMagenta)$(notdir $@)$(ColorEnd)"
	$(quiet)$(CC) $(LINKFLAGS) -o $@ $^ $(LIBS)
	@echo "$(ColorCyan)=== BUILD COMPLETE: $(notdir $@) ($(os)-$(arch)-$(config))$(ColorEnd)"

.PHONY : install
install: $(TARGETS)
	$(quiet)mkdir -p "$(CROSSROOT)/usr/include/CoreUtils"
	$(quiet)rsync -avp $(SRCROOT)/External/Curve25519/curve25519-donna.h "$(CROSSROOT)/usr/include/CoreUtils/"
	$(quiet)rsync -avp $(SRCROOT)/External/Ed25519/ed25519.h "$(CROSSROOT)/usr/include/CoreUtils/"
ifeq ($(aes_gcm),1)
	$(quiet)rsync -avp $(SRCROOT)/External/GladmanAES/gcm.h "$(CROSSROOT)/usr/include/"
	$(quiet)rsync -avp $(SRCROOT)/External/GladmanAES/gf128mul.h "$(CROSSROOT)/usr/include/"
endif
ifeq ($(gladman_aes),1)
	$(quiet)rsync -avp $(SRCROOT)/External/GladmanAES/aes.h "$(CROSSROOT)/usr/include/"
	$(quiet)rsync -avp $(SRCROOT)/External/GladmanAES/brg_endian.h "$(CROSSROOT)/usr/include/"
	$(quiet)rsync -avp $(SRCROOT)/External/GladmanAES/brg_types.h "$(CROSSROOT)/usr/include/"
endif
	$(quiet)rsync -avp $(SRCROOT)/External/Small25519/Small25519.h "$(CROSSROOT)/usr/include/CoreUtils/"
	$(quiet)rsync -avp $(SRCROOT)/Support/*.h "$(CROSSROOT)/usr/include/CoreUtils/"
	$(quiet)cp "$(BUILDROOT)/libCoreUtils.so" "$(CROSSLIB)/"

.PHONY : makeroot
makeroot: $(TARGETS)
	$(quiet)rm -rf "$(DSTROOTDIR)"
	$(quiet)mkdir -p "$(DSTROOTDIR)/$(CROSS_RROOT)/usr/include/CoreUtils"
	$(quiet)rsync -avp $(SRCROOT)/External/Curve25519/curve25519-donna.h "$(DSTROOTDIR)/$(CROSS_RROOT)/usr/include/CoreUtils/"
	$(quiet)rsync -avp $(SRCROOT)/External/Ed25519/ed25519.h "$(DSTROOTDIR)/$(CROSS_RROOT)/usr/include/CoreUtils/"
ifeq ($(aes_gcm),1)
	$(quiet)rsync -avp $(SRCROOT)/External/GladmanAES/gcm.h "$(DSTROOTDIR)/$(CROSS_RROOT)/usr/include/"
	$(quiet)rsync -avp $(SRCROOT)/External/GladmanAES/gf128mul.h "$(DSTROOTDIR)/$(CROSS_RROOT)/usr/include/"
endif
ifeq ($(gladman_aes),1)
	$(quiet)rsync -avp $(SRCROOT)/External/GladmanAES/aes.h "$(DSTROOTDIR)/$(CROSS_RROOT)/usr/include/"
	$(quiet)rsync -avp $(SRCROOT)/External/GladmanAES/brg_endian.h "$(DSTROOTDIR)/$(CROSS_RROOT)/usr/include/"
	$(quiet)rsync -avp $(SRCROOT)/External/GladmanAES/brg_types.h "$(DSTROOTDIR)/$(CROSS_RROOT)/usr/include/"
endif
	$(quiet)rsync -avp $(SRCROOT)/External/Small25519/Small25519.h "$(DSTROOTDIR)/$(CROSS_RROOT)/usr/include/CoreUtils/"
	$(quiet)rsync -avp $(SRCROOT)/Support/*.h "$(DSTROOTDIR)/$(CROSS_RROOT)/usr/include/CoreUtils/"
	$(quiet)mkdir -p "$(DSTROOTDIR)/$(CROSS_RLIB)/"
	$(quiet)cp "$(BUILDROOT)/libCoreUtils.so" "$(DSTROOTDIR)/$(CROSS_RLIB)/"
	ditto -c -j "$(DSTROOTDIR)" "$(DSTROOTDIR).cpio.bz2"

# Library rules.
.PHONY : CoreUtils
CoreUtils: $(BUILDROOT)/libCoreUtils.so

$(BUILDROOT)/libCoreUtils.so: $(CoreUtils_OBJS_shared)
	@echo "Linking ($(os)-$(arch)-$(config)) $(ColorMagenta)$(notdir $@)$(ColorEnd)"
	$(quiet)$(CC) -shared -Wl,-soname,libCoreUtils.so -o $@ $(LINKFLAGS) $^ $(LIBS)
	@if [ "$(nostrip)" != "1" ]; then \
		echo "Stripping $(notdir $@)"; \
		$(STRIP) $@; \
	fi
	@echo "$(ColorCyan)=== BUILD COMPLETE: $(notdir $@) ($(os)-$(arch)-$(config))$(ColorEnd)"

.PHONY : clean
clean:
	-rm -rf $(BUILDROOT)

$(aktest_OBJS):				| $(BUILDROOT)/obj
$(CoreUtils_OBJS_direct):	| $(BUILDROOT)/obj
$(CoreUtils_OBJS_shared):	| $(BUILDROOT)/obj

$(BUILDROOT)/obj:
	$(quiet)mkdir -p $@

# General rules
$(OBJDIR)/%.o: %.c
	@echo "Compiling ($(os)-$(arch)-$(config)-direct) $(ColorMagenta)$(notdir $<)$(ColorEnd)"
	$(quiet)$(CC) -c $(CPPFLAGS) $(CFLAGS) $(CFLAGS_direct) -o $@ $<

$(OBJDIR)/%.o: %.cpp
	@echo "Compiling ($(os)-$(arch)-$(config)-direct) $(ColorMagenta)$(notdir $<)$(ColorEnd)"
	$(quiet)$(CXX) -c $(CXXFLAGS) $(CFLAGS_direct) -o $@ $<

$(OBJDIR)/%.so.o: %.c
	@echo "Compiling ($(os)-$(arch)-$(config)-shared) $(ColorMagenta)$(notdir $<)$(ColorEnd)"
	$(quiet)$(CC) -c -fPIC $(CPPFLAGS) $(CFLAGS) -o $@ $<

$(OBJDIR)/%.so.o: %.cpp
	@echo "Compiling ($(os)-$(arch)-$(config)-shared) $(ColorMagenta)$(notdir $<)$(ColorEnd)"
	$(quiet)$(CXX) -c -fPIC $(CXXFLAGS) -o $@ $<

# Bonjour rules
.PHONY : bonjour
bonjour:
ifeq ($(os),qnx)
	make os=$(os) arch="$(qnx_arch)" -C $(MDNSROOT)/mDNSPosix setup Daemon libdns_sd Clients
else
ifeq ($(hw),generic)
	make os=$(os) -C $(MDNSROOT)/mDNSPosix setup Daemon libdns_sd Clients
else
	make CC="$(CC)" LD="$(LD)" STRIP="$(STRIP)" os=$(os) -C $(MDNSROOT)/mDNSPosix setup Daemon libdns_sd Clients
endif
endif

.PHONY : clean_bonjour
clean_bonjour:
	make os=$(os) -C $(MDNSROOT)/mDNSPosix clean
