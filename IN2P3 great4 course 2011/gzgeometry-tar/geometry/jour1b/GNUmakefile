# $Id: GNUmakefile 54 2008-05-13 16:17:48Z maire $
#
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# -----------------------------------------------------------

name := jour1b
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/architecture.gmk

include $(G4INSTALL)/config/binmake.gmk

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*
