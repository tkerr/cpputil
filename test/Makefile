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
	Queue16Test \
	QueueTest \
	sha256Test \
	SortTest 

.PHONY: $(SUBDIRS) recurse

$(MAKECMDGOALS) recurse : $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

