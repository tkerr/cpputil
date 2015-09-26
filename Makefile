##############################################################################
# GNU Makefile for cpputil repository.
#
# Designed for use by the WiTAQ build environment.
#
# Recursively builds all necessary components in the repository.
#
# Targets:
#    all :       Recursively runs 'make all' in each subdirectory.
#    clean :     Recursively runs 'make clean' in each subdirectory.
#    clobber :   Recursively runs 'make clobber' in each subdirectory.
#    doxygen :   Makes Doxygen documentation.
#    doxyclean : Deletes generated doxygen files.
#
#######################
# Modification History:
#
# 09/24/2015 - Tom Kerr
# Created.
##############################################################################

# Force doxygen and doxyclean to be remade each time the target is specified.
.PHONY: doxygen doxyclean
	
all clean clobber:
	cd test && make $@

doxygen:
	doxygen
	
doxyclean:
	rm -fr doxygen/html/*
