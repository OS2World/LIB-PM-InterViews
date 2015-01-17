# Makefile for OS/2
# 
# libInterViews - source files for the InterViews library
#

IV = \iv\src
IVH = $(IV)/InterViews
IVHSTD = $(IVH)/Std
IVHPM = $(IVH)/PM
GEN = $(IV)/libInterViews/Generated
INTLIB = InterVW.lib
X = X11
RM = del
LIB = libit
SRC = .
CC = ccxx
CDEBUGFLAGS = -Od -Zi -k -DDEBUG 
CXXFLAGS = !V !C +zf127 +zl30000
CFLAGS = $(CXXFLAGS) -Alfu -Gt1 -G2s -D__cplusplus
DEBUGDIR = cvp
INCLUDEPATH = -I$(IVHPM) -I$(IVHSTD) -I$(IVH) -I$(GEN) -I$(IV)

PMOBJS = \
	pm_bitmap.obj \
	pm_canvas.obj \
	pm_cursor.obj \
	pm_event.obj \
        pm_inter.obj \
        pm_lib.obj   \
	pm_painter.obj \
        pm_paint.obj \
	pm_raster.obj \
        pm_scene.obj \
	pm_sensor.obj \
        pm_world.obj \
	pm_wview.obj 

OBJS = \
	_ctorlist.obj \
	_dtorlist.obj \
	_exit.obj \
	_finish.obj \
	_handler.obj \
	_init.obj \
	adjuster.obj \
	banner.obj \
	bitmap.obj \
	border.obj \
	box.obj \
	button.obj \
	catalog.obj \
	chief.obj \
#	connection.obj \
	control.obj \
	cursor.obj \
	deck.obj \
	deputy.obj \
	dialog.obj \
#	filebrowser.obj \
	filechooser.obj \
	frame.obj \
	glue.obj \
	interactor.obj \
	menu.obj \
	message.obj \
	paint.obj \
	painter.obj \
	panner.obj \
	perspective.obj \
	propsheet.obj \
	raster.obj \
	regexp.obj \
	resource.obj \
	rubband.obj \
	rubcurve.obj \
	rubgroup.obj \
	rubline.obj \
	rubrect.obj \
	rubverts.obj \
	scene.obj \
	scroller.obj \
	sensor.obj \
	shape.obj \
#	space.obj \
#	spaceman.obj \
#	stream.obj \
	strbrowser.obj \
	strchooser.obj \
	streditor.obj \
	strpool.obj \
	strtable.obj \
	stub.obj \
	subject.obj \
	table.obj \
	table2.obj \
	tagtable.obj \
	textbuffer.obj \
	textdisplay.obj \
	texteditor.obj \
	transformer.obj \
	tray.obj \
	viewport.obj \
	world.obj \
	worldview.obj

all:: $(INTLIB)

$(INTLIB):: $(OBJS) 
	@echo "building $@"
	@$(LIB) $@ $(OBJS)

$(INTLIB)::  $(PMOBJS)
        @$(LIB) $@ $(PMOBJS)

_ctorlist.obj: $(SRC)/_ctorlist.cxx
	@echo "compiling $(SRC)/_ctorlist.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/_ctorlist.cxx

_dtorlist.obj: $(SRC)/_dtorlist.cxx
	@echo "compiling $(SRC)/_dtorlist.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/_dtorlist.cxx

_exit.obj: $(SRC)/_exit.cxx
	@echo "compiling $(SRC)/_exit.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/_exit.cxx

_finish.obj: $(SRC)/_finish.cxx
	@echo "compiling $(SRC)/_finish.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/_finish.cxx

_handler.obj: $(SRC)/_handler.cxx
	@echo "compiling $(SRC)/_handler.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/_handler.cxx

_init.obj: $(SRC)/_init.cxx
	@echo "compiling $(SRC)/_init.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/_init.cxx

adjuster.obj: $(SRC)/adjuster.cxx
	@echo "compiling $(SRC)/adjuster.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/adjuster.cxx

banner.obj: $(SRC)/banner.cxx
	@echo "compiling $(SRC)/banner.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/banner.cxx

bitmap.obj: $(SRC)/bitmap.cxx
	@echo "compiling $(SRC)/bitmap.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/bitmap.cxx

border.obj: $(SRC)/border.cxx
	@echo "compiling $(SRC)/border.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/border.cxx

box.obj: $(SRC)/box.cxx
	@echo "compiling $(SRC)/box.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/box.cxx

button.obj: $(SRC)/button.cxx
	@echo "compiling $(SRC)/button.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/button.cxx

catalog.obj: $(SRC)/catalog.cxx
	@echo "compiling $(SRC)/catalog.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/catalog.cxx

chief.obj: $(SRC)/chief.cxx
	@echo "compiling $(SRC)/chief.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/chief.cxx

connection.obj: $(SRC)/connection.cxx
	@echo "compiling $(SRC)/connection.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/connection.cxx

control.obj: $(SRC)/control.cxx
	@echo "compiling $(SRC)/control.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/control.cxx

cursor.obj: $(SRC)/cursor.cxx
	@echo "compiling $(SRC)/cursor.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/cursor.cxx

deck.obj: $(SRC)/deck.cxx
	@echo "compiling $(SRC)/deck.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/deck.cxx

deputy.obj: $(SRC)/deputy.cxx
	@echo "compiling $(SRC)/deputy.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/deputy.cxx

dialog.obj: $(SRC)/dialog.cxx
	@echo "compiling $(SRC)/dialog.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/dialog.cxx

filebrowser.obj: $(SRC)/filebrowser.cxx
	@echo "compiling $(SRC)/filebrowser.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/filebrowser.cxx

filechooser.obj: $(SRC)/filechooser.cxx
	@echo "compiling $(SRC)/filechooser.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/filechooser.cxx

frame.obj: $(SRC)/frame.cxx
	@echo "compiling $(SRC)/frame.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/frame.cxx

glue.obj: $(SRC)/glue.cxx
	@echo "compiling $(SRC)/glue.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/glue.cxx

interactor.obj: $(SRC)/interactor.cxx
	@echo "compiling $(SRC)/interactor.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/interactor.cxx

menu.obj: $(SRC)/menu.cxx
	@echo "compiling $(SRC)/menu.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/menu.cxx

message.obj: $(SRC)/message.cxx
	@echo "compiling $(SRC)/message.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/message.cxx

paint.obj: $(SRC)/paint.cxx
	@echo "compiling $(SRC)/paint.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/paint.cxx

painter.obj: $(SRC)/painter.cxx
	@echo "compiling $(SRC)/painter.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/painter.cxx

panner.obj: $(SRC)/panner.cxx
	@echo "compiling $(SRC)/panner.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/panner.cxx

perspective.obj: $(SRC)/perspective.cxx
	@echo "compiling $(SRC)/perspective.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/perspective.cxx

propsheet.obj: $(SRC)/propsheet.cxx
	@echo "compiling $(SRC)/propsheet.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/propsheet.cxx

raster.obj: $(SRC)/raster.cxx
	@echo "compiling $(SRC)/raster.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/raster.cxx

regexp.obj: $(SRC)/regexp.cxx
	@echo "compiling $(SRC)/regexp.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/regexp.cxx

resource.obj: $(SRC)/resource.cxx
	@echo "compiling $(SRC)/resource.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/resource.cxx

rubband.obj: $(SRC)/rubband.cxx
	@echo "compiling $(SRC)/rubband.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/rubband.cxx

rubcurve.obj: $(SRC)/rubcurve.cxx
	@echo "compiling $(SRC)/rubcurve.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/rubcurve.cxx

rubgroup.obj: $(SRC)/rubgroup.cxx
	@echo "compiling $(SRC)/rubgroup.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/rubgroup.cxx

rubline.obj: $(SRC)/rubline.cxx
	@echo "compiling $(SRC)/rubline.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/rubline.cxx

rubrect.obj: $(SRC)/rubrect.cxx
	@echo "compiling $(SRC)/rubrect.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/rubrect.cxx

rubverts.obj: $(SRC)/rubverts.cxx
	@echo "compiling $(SRC)/rubverts.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/rubverts.cxx

scene.obj: $(SRC)/scene.cxx
	@echo "compiling $(SRC)/scene.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/scene.cxx

scroller.obj: $(SRC)/scroller.cxx
	@echo "compiling $(SRC)/scroller.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/scroller.cxx

sensor.obj: $(SRC)/sensor.cxx
	@echo "compiling $(SRC)/sensor.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/sensor.cxx

shape.obj: $(SRC)/shape.cxx
	@echo "compiling $(SRC)/shape.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/shape.cxx

space.obj: $(SRC)/space.cxx
	@echo "compiling $(SRC)/space.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/space.cxx

spaceman.obj: $(SRC)/spaceman.cxx
	@echo "compiling $(SRC)/spaceman.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/spaceman.cxx

stream.obj: $(SRC)/stream.cxx
	@echo "compiling $(SRC)/stream.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/stream.cxx

strbrowser.obj: $(SRC)/strbrowser.cxx
	@echo "compiling $(SRC)/strbrowser.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/strbrowser.cxx

strchooser.obj: $(SRC)/strchooser.cxx
	@echo "compiling $(SRC)/strchooser.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/strchooser.cxx

streditor.obj: $(SRC)/streditor.cxx
	@echo "compiling $(SRC)/streditor.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/streditor.cxx

strpool.obj: $(SRC)/strpool.cxx
	@echo "compiling $(SRC)/strpool.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/strpool.cxx

strtable.obj: $(SRC)/strtable.cxx
	@echo "compiling $(SRC)/strtable.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/strtable.cxx

stub.obj: $(SRC)/stub.cxx
	@echo "compiling $(SRC)/stub.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/stub.cxx

subject.obj: $(SRC)/subject.cxx
	@echo "compiling $(SRC)/subject.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/subject.cxx

table.obj: $(GEN)/table.cxx
	@echo "compiling $(GEN)/table.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(GEN)/table.cxx

table2.obj: $(GEN)/table2.cxx
	@echo "compiling $(GEN)/table2.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(GEN)/table2.cxx

tagtable.obj: $(SRC)/tagtable.cxx
	@echo "compiling $(SRC)/tagtable.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/tagtable.cxx

textbuffer.obj: $(SRC)/textbuffer.cxx
	@echo "compiling $(SRC)/textbuffer.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/textbuffer.cxx

textdisplay.obj: $(SRC)/textdisplay.cxx
	@echo "compiling $(SRC)/textdisplay.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/textdisplay.cxx

texteditor.obj: $(SRC)/texteditor.cxx
	@echo "compiling $(SRC)/texteditor.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/texteditor.cxx

transformer.obj: $(SRC)/transformer.cxx
	@echo "compiling $(SRC)/transformer.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/transformer.cxx

tray.obj: $(SRC)/tray.cxx
	@echo "compiling $(SRC)/tray.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/tray.cxx

viewport.obj: $(SRC)/viewport.cxx
	@echo "compiling $(SRC)/viewport.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/viewport.cxx

world.obj: $(SRC)/world.cxx
	@echo "compiling $(SRC)/world.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/world.cxx

worldview.obj: $(SRC)/worldview.cxx
	@echo "compiling $(SRC)/worldview.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/worldview.cxx

pm_bitmap.obj: $(SRC)/pm_bitmap.cxx
	@echo "compiling $(SRC)/pm_bitmap.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/pm_bitmap.cxx

pm_canvas.obj: $(SRC)/pm_canvas.cxx
	@echo "compiling $(SRC)/pm_canvas.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/pm_canvas.cxx

pm_cursor.obj: $(SRC)/pm_cursor.cxx
	@echo "compiling $(SRC)/pm_cursor.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/pm_cursor.cxx

pm_event.obj: $(SRC)/pm_event.cxx
	@echo "compiling $(SRC)/pm_event.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/pm_event.cxx

pm_inter.obj: $(SRC)/pm_inter.cxx
	@echo "compiling $(SRC)/pm_inter.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/pm_inter.cxx

pm_lib.obj: $(SRC)/pm_lib.cxx
	@echo "compiling $(SRC)/pm_lib.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/pm_lib.cxx

pm_painter.obj: $(SRC)/pm_painter.cxx
	@echo "compiling $(SRC)/pm_painter.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/pm_painter.cxx

pm_paint.obj: $(SRC)/pm_paint.cxx
	@echo "compiling $(SRC)/pm_paint.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/pm_paint.cxx

pm_raster.obj: $(SRC)/pm_raster.cxx
	@echo "compiling $(SRC)/pm_raster.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/pm_raster.cxx

pm_scene.obj: $(SRC)/pm_scene.cxx
	@echo "compiling $(SRC)/pm_scene.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/pm_scene.cxx

pm_sensor.obj: $(SRC)/pm_sensor.cxx
	@echo "compiling $(SRC)/pm_sensor.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/pm_sensor.cxx

pm_world.obj: $(SRC)/pm_world.cxx
	@echo "compiling $(SRC)/pm_world.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/pm_world.cxx

pm_wview.obj: $(SRC)/pm_wview.cxx
	@echo "compiling $(SRC)/pm_wview.cxx"
	-$(RM) $@
	$(CC) -c  $(CFLAGS) $(INCLUDEPATH) $(SRC)/pm_wview.cxx

#
# common rules for all Makefiles
#
clean::
	-$(RM) *.map
	-$(RM) *.i

clobber:: clean
	-$(RM) *.obj
	-$(RM) $(HT)
	-$(RM) $(INTLIB)

install::

#
# dependencies generated by makedepend
#

# DO NOT DELETE
pm_bitmap.obj: $(IVH)/defs.h $(IVH)/bitmap.h $(IVH)/font.h $(IVH)/transformer.h $(IVH)/pm/painterrep.h $(IVH)/pm/pm_lib.h $(IVH)/pm/worldrep.h $(IVH)/Std/bstring.h $(IVH)/Std/math.h $(IVH)/Std/string.h
pm_canvas.obj: $(GEN)/itable.h $(IVH)/table.h $(IVH)/defs.h $(IVH)/canvas.h $(IVH)/color.h $(IVH)/resource.h $(IVH)/world.h $(IVH)/scene.h $(IVH)/interactor.h $(IVH)/pm/worldrep.h $(IVH)/pm/pm_lib.h
pm_cursor.obj: $(IVH)/defs.h $(IVH)/bitmap.h $(IVH)/resource.h $(IVH)/color.h $(IVH)/cursor.h $(IVH)/font.h $(IVH)/pm/worldrep.h $(IVH)/pm/pm_lib.h $(IVH)/Std/malloc.h
pm_event.obj: $(GEN)/itable.h $(IVH)/table.h $(IVH)/defs.h $(IVH)/event.h $(IVH)/pm/eventrep.h $(IVH)/pm/pm_lib.h $(IVH)/pm/worldrep.h $(IVH)/Std/string.h
pm_inter.obj: $(GEN)/itable.h $(IVH)/table.h $(IVH)/defs.h $(IVH)/bitmap.h $(IVH)/resource.h $(IVH)/canvas.h $(IVH)/cursor.h $(IVH)/interactor.h $(IVH)/sensor.h $(IVH)/event.h $(IVH)/shape.h $(IVH)/world.h $(IVH)/scene.h $(IVH)/worldview.h $(IVH)/pm/eventrep.h
pm_paint.obj: $(IVH)/bitmap.h $(IVH)/defs.h $(IVH)/resource.h $(IVH)/brush.h $(IVH)/color.h $(IVH)/font.h $(IVH)/pattern.h $(IVH)/pm/worldrep.h $(IVH)/pm/pm_lib.h $(IVH)/Std/string.h $(IVH)/Std/malloc.h $(GEN)/btable.h $(GEN)/table2.h
pm_painter.obj: $(IVH)/defs.h $(IVH)/bitmap.h $(IVH)/resource.h $(IVH)/brush.h $(IVH)/canvas.h $(IVH)/color.h $(IVH)/font.h $(IVH)/painter.h $(IVH)/pattern.h $(IVH)/raster.h $(IVH)/transformer.h $(IVH)/pm/painterrep.h $(IVH)/pm/pm_lib.h $(IVH)/pm/worldrep.h
pm_raster.obj: $(IVH)/canvas.h $(IVH)/defs.h $(IVH)/raster.h $(IVH)/resource.h $(IVH)/pm/worldrep.h $(IVH)/pm/pm_lib.h
pm_scene.obj: $(GEN)/itable.h $(IVH)/table.h $(IVH)/defs.h $(IVH)/bitmap.h $(IVH)/resource.h $(IVH)/canvas.h $(IVH)/cursor.h $(IVH)/interactor.h $(IVH)/scene.h $(IVH)/shape.h $(IVH)/world.h $(IVH)/pm/pm_lib.h $(IVH)/pm/worldrep.h $(IVH)/Std/string.h 
pm_world.obj: $(GEN)/btable.h $(GEN)/table2.h $(IVH)/defs.h $(GEN)/itable.h $(IVH)/table.h $(IVH)/bitmap.h $(IVH)/resource.h $(IVH)/canvas.h $(IVH)/color.h $(IVH)/font.h $(IVH)/painter.h $(IVH)/interactor.h $(IVH)/shape.h $(IVH)/world.h $(IVH)/pm/worldrep.h $(IVH)/pm/pm_lib.h $(IVH)/pm/pm_world.h $(IVH)/pm/canvasrep.h
pm_wview.obj: $(IVH)/canvas.h $(IVH)/defs.h $(IVH)/cursor.h $(IVH)/painter.h $(IVH)/resource.h $(IVH)/shape.h $(IVH)/world.h $(IVH)/scene.h $(IVH)/interactor.h $(IVH)/worldview.h $(IVH)/pm/worldrep.h $(IVH)/pm/pm_lib.h
pm_lib.obj: $(IVH)/pm/pm_lib.h $(IVH)/std/string.h $(IVH)/std/malloc.h $(IVH)/std/ctype.h
adjuster.obj: $(IVH)/adjuster.h $(IVH)/interactor.h $(IVH)/defs.h $(IVH)/bitmap.h $(IVH)/resource.h $(IVH)/painter.h $(IVH)/perspective.h $(IVH)/sensor.h $(IVH)/event.h $(IVH)/shape.h $(IVH)/Bitmaps/enlargeHit.bm $(IVH)/Bitmaps/enlargeMask.bm $(IVH)/Bitmaps/enlarge.bm $(IVH)/Bitmaps/reducerHit.bm $(IVH)/Bitmaps/reducerMask.bm $(IVH)/Bitmaps/reducer.bm $(IVH)/Bitmaps/lmoverHit.bm $(IVH)/Bitmaps/lmoverMask.bm $(IVH)/Bitmaps/lmover.bm $(IVH)/Bitmaps/rmoverHit.bm $(IVH)/Bitmaps/rmoverMask.bm $(IVH)/Bitmaps/rmover.bm $(IVH)/Bitmaps/umoverHit.bm $(IVH)/Bitmaps/umoverMask.bm $(IVH)/Bitmaps/umover.bm $(IVH)/Bitmaps/dmoverHit.bm $(IVH)/Bitmaps/dmoverMask.bm $(IVH)/Bitmaps/dmover.bm 
banner.obj: $(IVH)/banner.h $(IVH)/interactor.h $(IVH)/defs.h $(IVH)/font.h $(IVH)/resource.h $(IVH)/painter.h $(IVH)/shape.h 
bitmap.obj: $(IVH)/bitmap.h $(IVH)/defs.h $(IVH)/resource.h $(IVH)/transformer.h 
border.obj: $(IVH)/border.h $(IVH)/interactor.h $(IVH)/defs.h $(IVH)/painter.h $(IVH)/resource.h $(IVH)/shape.h
box.obj: $(IVH)/box.h $(IVH)/scene.h $(IVH)/interactor.h $(IVH)/defs.h $(IVH)/shape.h 
button.obj: $(IVH)/bitmap.h $(IVH)/defs.h $(IVH)/resource.h $(IVH)/button.h $(IVH)/interactor.h $(IVH)/subject.h $(IVH)/font.h $(IVH)/painter.h $(IVH)/pattern.h $(IVH)/sensor.h $(IVH)/event.h $(IVH)/shape.h $(IVH)/Bitmaps/radioBoth.bm $(IVH)/Bitmaps/radioChosen.bm $(IVH)/Bitmaps/radioHit.bm $(IVH)/Bitmaps/radioMask.bm $(IVH)/Bitmaps/radio.bm 
catalog.obj: $(IVH)/catalog.h $(IVH)/stub.h $(IVH)/resource.h $(IVH)/defs.h $(IVH)/tag.h 
chief.obj: $(IVH)/chief.h $(IVH)/defs.h $(IVH)/tag.h $(IVH)/packet.h $(IVH)/connection.h 
connection.obj: $(IVH)/connection.h $(IVH)/defs.h $(IVH)/Std/sys/socket.h $(IVH)/Std/netinet/in.h $(IVH)/Std/netdb.h $(IVH)/Std/bstring.h $(IVH)/Std/errno.h $(IVH)/Std/fcntl.h $(IVH)/Std/string.h $(IVH)/Std/os/fs.h $(IVH)/Std/os/ipc.h $(IVH)/Std/os/proc.h
control.obj: $(IVH)/control.h $(IVH)/scene.h $(IVH)/interactor.h $(IVH)/defs.h $(IVH)/subject.h $(IVH)/resource.h $(IVH)/message.h $(IVH)/sensor.h $(IVH)/event.h 
cursor.obj: $(IVH)/color.h $(IVH)/resource.h $(IVH)/defs.h $(IVH)/cursor.h 
deck.obj: $(IVH)/deck.h $(IVH)/scene.h $(IVH)/interactor.h $(IVH)/defs.h $(IVH)/perspective.h $(IVH)/resource.h $(IVH)/glue.h $(IVH)/shape.h 
deputy.obj: $(IVH)/deputy.h $(IVH)/tag.h $(IVH)/chief.h $(IVH)/defs.h 
dialog.obj: $(IVH)/button.h $(IVH)/interactor.h $(IVH)/defs.h $(IVH)/subject.h $(IVH)/resource.h $(IVH)/dialog.h $(IVH)/scene.h $(IVH)/event.h $(IVH)/world.h 
filebrowser.obj: $(IVH)/filebrowser.h $(IVH)/strbrowser.h $(IVH)/interactor.h $(IVH)/defs.h 
filechooser.obj: $(IVH)/adjuster.h $(IVH)/interactor.h $(IVH)/defs.h $(IVH)/border.h $(IVH)/box.h $(IVH)/scene.h $(IVH)/button.h $(IVH)/subject.h $(IVH)/resource.h $(IVH)/event.h $(IVH)/filebrowser.h $(IVH)/strbrowser.h $(IVH)/filechooser.h $(IVH)/strchooser.h $(IVH)/dialog.h $(IVH)/frame.h $(IVH)/glue.h $(IVH)/shape.h $(IVH)/message.h $(IVH)/scroller.h $(IVH)/sensor.h $(IVH)/streditor.h 
frame.obj: $(IVH)/banner.h $(IVH)/interactor.h $(IVH)/defs.h $(IVH)/border.h $(IVH)/box.h $(IVH)/scene.h $(IVH)/canvas.h $(IVH)/frame.h $(IVH)/painter.h $(IVH)/resource.h $(IVH)/pattern.h $(IVH)/shape.h $(IVH)/sensor.h $(IVH)/event.h 
glue.obj: $(IVH)/glue.h $(IVH)/interactor.h $(IVH)/defs.h $(IVH)/shape.h 
interactor.obj: $(IVH)/canvas.h $(IVH)/defs.h $(IVH)/interactor.h $(IVH)/paint.h $(IVH)/brush.h $(IVH)/resource.h $(IVH)/color.h $(IVH)/font.h $(IVH)/pattern.h $(IVH)/painter.h $(IVH)/propsheet.h $(IVH)/scene.h $(IVH)/shape.h $(IVH)/sensor.h $(IVH)/event.h $(IVH)/strtable.h $(IVH)/world.h 
menu.obj: $(IVH)/box.h $(IVH)/scene.h $(IVH)/interactor.h $(IVH)/defs.h $(IVH)/event.h $(IVH)/frame.h $(IVH)/glue.h $(IVH)/shape.h $(IVH)/menu.h $(IVH)/control.h $(IVH)/subject.h $(IVH)/resource.h $(IVH)/message.h $(IVH)/painter.h $(IVH)/pattern.h $(IVH)/world.h 
message.obj: $(IVH)/message.h $(IVH)/interactor.h $(IVH)/defs.h $(IVH)/font.h $(IVH)/resource.h $(IVH)/painter.h $(IVH)/shape.h 
paint.obj: $(GEN)/ftable.h $(IVH)/table.h $(IVH)/defs.h $(IVH)/paint.h $(IVH)/brush.h $(IVH)/resource.h $(IVH)/color.h $(IVH)/font.h $(IVH)/pattern.h $(IVH)/strtable.h 
painter.obj: $(IVH)/brush.h $(IVH)/resource.h $(IVH)/defs.h $(IVH)/color.h $(IVH)/font.h $(IVH)/painter.h $(IVH)/pattern.h $(IVH)/transformer.h  
panner.obj: $(IVH)/adjuster.h $(IVH)/interactor.h $(IVH)/defs.h $(IVH)/border.h $(IVH)/box.h $(IVH)/scene.h $(IVH)/glue.h $(IVH)/shape.h $(IVH)/painter.h $(IVH)/resource.h $(IVH)/panner.h $(IVH)/pattern.h $(IVH)/perspective.h $(IVH)/rubrect.h $(IVH)/rubband.h $(IVH)/sensor.h $(IVH)/event.h 
perspective.obj: $(IVH)/perspective.h $(IVH)/resource.h $(IVH)/defs.h $(IVH)/interactor.h 
propsheet.obj: $(IVH)/propsheet.h $(IVH)/defs.h $(IVH)/strpool.h $(IVH)/strtable.h 
raster.obj: $(IVH)/raster.h $(IVH)/defs.h $(IVH)/resource.h $(IVH)/color.h 
regexp.obj: $(IVH)/defs.h $(IVH)/regexp.h 
resource.obj: $(IVH)/resource.h $(IVH)/defs.h 
rubband.obj: $(IVH)/rubband.h $(IVH)/defs.h $(IVH)/resource.h $(IVH)/painter.h 
rubcurve.obj: $(IVH)/painter.h $(IVH)/defs.h $(IVH)/resource.h $(IVH)/rubcurve.h $(IVH)/rubband.h 
rubgroup.obj: $(IVH)/rubgroup.h $(IVH)/rubband.h $(IVH)/defs.h $(IVH)/resource.h 
rubline.obj: $(IVH)/rubline.h $(IVH)/rubband.h $(IVH)/defs.h $(IVH)/resource.h $(IVH)/painter.h 
rubrect.obj: $(IVH)/painter.h $(IVH)/defs.h $(IVH)/resource.h $(IVH)/rubrect.h $(IVH)/rubband.h 
rubverts.obj: $(IVH)/painter.h $(IVH)/defs.h $(IVH)/resource.h $(IVH)/rubverts.h $(IVH)/rubband.h 
scene.obj: $(IVH)/scene.h $(IVH)/interactor.h $(IVH)/defs.h $(IVH)/shape.h $(IVH)/canvas.h $(IVH)/painter.h $(IVH)/resource.h $(IVH)/strtable.h $(IVH)/painter.h 
scroller.obj: $(IVH)/defs.h $(IVH)/resource.h $(IVH)/pattern.h $(IVH)/perspective.h $(IVH)/rubrect.h $(IVH)/rubband.h $(IVH)/scroller.h $(IVH)/interactor.h $(IVH)/sensor.h $(IVH)/event.h $(IVH)/shape.h 
sensor.obj: $(IVH)/interactor.h $(IVH)/defs.h $(IVH)/sensor.h $(IVH)/event.h $(IVH)/resource.h $(IVH)/world.h $(IVH)/scene.h 
shape.obj: $(IVH)/shape.h $(IVH)/defs.h $(IVH)/catalog.h $(IVH)/stub.h $(IVH)/resource.h $(IVH)/defs.h $(IVH)/tag.h $(IVH)/connection.h $(IVH)/space.h $(IVH)/packet.h $(IVH)/spaceman.h $(IVH)/deputy.h $(IVH)/tagtable.h 
spaceman.obj: $(IVH)/spaceman.h $(IVH)/defs.h $(IVH)/deputy.h $(IVH)/tag.h $(IVH)/chief.h $(IVH)/connection.h 
strbrowser.obj: $(IVH)/bitmap.h $(IVH)/defs.h $(IVH)/resource.h $(IVH)/button.h $(IVH)/interactor.h $(IVH)/subject.h $(IVH)/cursor.h $(IVH)/font.h $(IVH)/painter.h $(IVH)/perspective.h $(IVH)/sensor.h $(IVH)/event.h $(IVH)/shape.h $(IVH)/strbrowser.h $(IVH)/textdisplay.h $(IVH)/Bitmaps/hand.bm $(IVH)/Bitmaps/handMask.bm $(IVH)/Bitmaps/dfast.bm $(IVH)/Bitmaps/dfastMask.bm $(IVH)/Bitmaps/ufast.bm $(IVH)/Bitmaps/ufastMask.bm 
strchooser.obj: $(IVH)/button.h $(IVH)/interactor.h $(IVH)/defs.h $(IVH)/subject.h $(IVH)/resource.h $(IVH)/event.h $(IVH)/sensor.h $(IVH)/strbrowser.h $(IVH)/strchooser.h $(IVH)/dialog.h $(IVH)/scene.h $(IVH)/streditor.h 
stream.obj: $(IVH)/defs.h 
streditor.obj: $(IVH)/bitmap.h $(IVH)/defs.h $(IVH)/resource.h $(IVH)/button.h $(IVH)/interactor.h $(IVH)/subject.h $(IVH)/cursor.h $(IVH)/font.h $(IVH)/painter.h $(IVH)/sensor.h $(IVH)/event.h $(IVH)/shape.h $(IVH)/streditor.h $(IVH)/textbuffer.h $(IVH)/textdisplay.h $(IVH)/Bitmaps/hand.bm $(IVH)/Bitmaps/handMask.bm $(IVH)/Bitmaps/lfast.bm $(IVH)/Bitmaps/lfastMask.bm $(IVH)/Bitmaps/rfast.bm $(IVH)/Bitmaps/rfastMask.bm 
strpool.obj: $(IVH)/strpool.h $(IVH)/defs.h 
strtable.obj: $(IVH)/strpool.h $(IVH)/defs.h $(IVH)/strtable.h 
stub.obj: $(IVH)/stub.h $(IVH)/resource.h $(IVH)/defs.h $(IVH)/tag.h 
subject.obj: $(IVH)/interactor.h $(IVH)/defs.h $(IVH)/subject.h $(IVH)/resource.h 
tagtable.obj: $(IVH)/tagtable.h $(IVH)/stub.h $(IVH)/resource.h $(IVH)/defs.h $(IVH)/tag.h 
textbuffer.obj: $(IVH)/regexp.h $(IVH)/textbuffer.h $(IVH)/defs.h 
textdisplay.obj: $(IVH)/font.h $(IVH)/resource.h $(IVH)/defs.h $(IVH)/painter.h $(IVH)/shape.h $(IVH)/textdisplay.h 
texteditor.obj: $(IVH)/bitmap.h $(IVH)/defs.h $(IVH)/resource.h $(IVH)/cursor.h $(IVH)/event.h $(IVH)/font.h $(IVH)/painter.h $(IVH)/perspective.h $(IVH)/shape.h $(IVH)/textbuffer.h $(IVH)/textdisplay.h $(IVH)/texteditor.h $(IVH)/interactor.h $(IVH)/Bitmaps/hand.bm $(IVH)/Bitmaps/handMask.bm $(IVH)/Bitmaps/dfast.bm $(IVH)/Bitmaps/dfastMask.bm $(IVH)/Bitmaps/ufast.bm $(IVH)/Bitmaps/ufastMask.bm 
transformer.obj: $(IVH)/transformer.h $(IVH)/defs.h $(IVH)/resource.h 
tray.obj: $(IVH)/canvas.h $(IVH)/defs.h $(IVH)/painter.h $(IVH)/resource.h $(IVH)/shape.h $(IVH)/tray.h $(IVH)/scene.h $(IVH)/interactor.h 
viewport.obj: $(IVH)/canvas.h $(IVH)/defs.h $(IVH)/painter.h $(IVH)/resource.h $(IVH)/pattern.h $(IVH)/perspective.h $(IVH)/shape.h $(IVH)/viewport.h $(IVH)/scene.h $(IVH)/interactor.h 
world.obj: $(IVH)/canvas.h $(IVH)/defs.h $(IVH)/event.h $(IVH)/painter.h $(IVH)/resource.h $(IVH)/propsheet.h $(IVH)/shape.h $(IVH)/strtable.h $(IVH)/world.h $(IVH)/scene.h $(IVH)/interactor.h 
worldview.obj: $(IVH)/sensor.h $(IVH)/event.h $(IVH)/defs.h $(IVH)/resource.h $(IVH)/shape.h $(IVH)/world.h $(IVH)/scene.h $(IVH)/interactor.h $(IVH)/worldview.h 
table.obj: $(IVH)/table.h $(IVH)/defs.h
table2.obj: $(GEN)/table2.h $(IVH)/defs.h
