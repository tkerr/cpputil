##############################################################################
# GNU Makefile for Arduino unit test applications.
#
# Designed for use by the WiTAQ build environment.
#
# Recursively executes make in each unit test directory.
#
# Targets:
#    Targets are passed through to the unit test makefiles.
#
#######################
# Modification History:
#
# 10/07/2015 - Tom Kerr
# Added pcgTest.
#
# 09/24/2015 - Tom Kerr
# Created.
##############################################################################

SUBDIRS = \
	AdcSpeedTest \
	aunitTest \
	BcdTest \
	ChecksumTest \
	crcTest \
	FifoTest \
	hexTest \
    MedianTest \
    pcgTest \
	Queue16Test \
	QueueTest \
	sha256Test \
	SortTest \
    uECCTest

.PHONY: $(SUBDIRS) recurse

$(MAKECMDGOALS) recurse : $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

