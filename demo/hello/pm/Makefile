# Makefile for creating PM programs

TOP = d:\iv\src
TOPI = $(TOP)\interviews
TOPSTDI = $(TOPI)\Std
CDEBUGFLAGS = -Od -Zi -k
CXXFLAGS = !Q !C +zf127 +zl30000
CFLAGS = -c -Alfu -Gt1 -G2s -D__cplusplus
LDDEBUGFLAGS = /CO
LDFLAGS = $(LDDEBUGFLAGS) /align:16, NUL
INCLUDES = -I$(TOPI)/pm -I$(TOPSTDI) -I$(TOPI) -I$(TOP)
IVLIB = d:\iv\src\libinterviews\intervw.lib
LIBS = os2 lCxxCe llibce $(IVLIB)
PROGNAME = hello

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
	$(CC) $(CXXFLAGS) $(CFLAGS) $(INCLUDES) $(PROGNAME).cxx

clean:
	-$(RM) *.obj
	-$(RM) *.exe

.SUFFIXES: .obj .CXX .h .exe

.CXX.obj:
	$(CC) $(CXXFLAGS) $(CFLAGS) $(INCLUDES) $<

.obj.exe:
	$(LD) $<,$(LDFLAGS),$(LIBS),$<
