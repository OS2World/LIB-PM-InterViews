# Makefile for creating PM programs

TOP = d:/iv/src
TOPI = $(TOP)/interviews
TOPSTDI = $(TOPI)/Std
CDEBUGFLAGS = -Od -Zi -k
CXXFLAGS = !Q !C +zf127 +Zl30000
CFLAGS = $(CXXFLAGS) -c -Alfu -Gt1 -G2s -D__cplusplus
LDDEBUGFLAGS = /CO
LDFLAGS = $(LDDEBUGFLAGS) /align:16, NUL
INCLUDES = -I$(TOPI)/pm -I$(TOPSTDI) -I$(TOPI) -I$(TOP)
IVLIB = d:\iv\src\libinterviews\intervw.lib
LIBS = os2 lCxxCe llibce $(IVLIB)
PROGNAME = iclock

RM = del
CP = copy
CC = ccxx
LD = link

all: $(PROGNAME)

$(PROGNAME): $(PROGNAME).exe

$(PROGNAME).exe: $(PROGNAME).obj $(IVLIB)
	$(LD) $(PROGNAME),$(LDFLAGS),$(LIBS),$(PROGNAME)

$(PROGNAME).obj: $(PROGNAME).cxx
	-$(RM) $@
	$(CC) $(CFLAGS) $(INCLUDES) $(PROGNAME).cxx

clean:
	-$(RM) *.obj
	-$(RM) *.exe

.SUFFIXES: .obj .CXX .h .exe

.CXX.obj:
	$(CC) $(CFLAGS) $(INCLUDES) $<

.obj.exe:
	$(LD) $<,$(LDFLAGS),$(LIBS),$<
