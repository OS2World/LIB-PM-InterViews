# Makefile for creating PM programs

TOP = d:/iv/src
TOPI = $(TOP)/interviews
TOPSTDI = $(TOPI)/Std
CDEBUGFLAGS = -Od -Zi -k
CXXFLAGS = !Q !C +zf127 +zl30000
CFLAGS = $(CXXFLAGS) -c -Alfu -Gt1 -G2s -D__cplusplus
LDDEBUGFLAGS = /CO
LDFLAGS = $(LDDEBUGFLAGS) /SE:256 /align:16
INCLUDES = -I$(TOPI)/pm -I$(TOPSTDI) -I$(TOPI) -I$(TOP)
IVLIB = d:\iv\src\libinterviews\intervw.lib
LIBS = os2 $(IVLIB) lCxxCe llibce
PROGNAME = logo
DEBUGDIR = cvp
OBJS = main.obj logo.obj

RM = del
CP = copy
CC = ccxx
LD = link

all: $(PROGNAME)

$(PROGNAME): $(PROGNAME).exe

$(PROGNAME).exe: $(OBJS) $(IVLIB)
	$(LD) $(LDFLAGS) $(OBJS),$(PROGNAME),$(PROGNAME),$(LIBS),$(PROGNAME)

main.obj: main.cxx
	@echo "compiling main.cxx"
	-$(RM) $@
	$(CC) $(CFLAGS) $(INCLUDES) main.cxx

logo.obj: logo.cxx
	@echo "compiling logo.cxx"
	-$(RM) $@
	$(CC) $(CFLAGS) $(INCLUDES) logo.cxx

clean:
	-$(RM) *.obj
	-$(RM) *.exe
