QMAKE_CXXFLAGS += -fsigned-char

CONFIG += qt \
    resources \
    thread \
    warn_on
CONFIG += rtti
#CONFIG += mobility
MOBILITY =
#DEFINES += NO_SOUND Q_OS_ANDROID
QT += core \
    gui \
    network \
#    opengl \
    xml \
    multimedia \
    script

TARGET = Pockemul
TEMPLATE = app
TRANSLATIONS += pockemul_fr.ts
UI_DIR += build/ui
mac {
    debug:OBJECTS_DIR += build/o/wd
    release:OBJECTS_DIR += build/o/wr
}
unix {
    debug:OBJECTS_DIR += build/o/wd
    release:OBJECTS_DIR += build/o/wr
}
win32 {
    Release:OBJECTS_DIR += build/o/wr
    Debug:OBJECTS_DIR += build/o/wd
}

FORMS += ui/about.ui \
    ui/dialoganalog.ui \
    ui/dialogconnectorlink.ui \
    ui/dialogdump.ui \
    ui/dialogkeylist.ui \
    ui/dialoglog.ui \
    ui/pockemul.ui \
    ui/serialconsole.ui \
    ui/startup.ui \
    ui/dialogpotar.ui \
    ui/simulatorconsole.ui \
    qcodeedit/widgets/searchreplace.ui \
    qcodeedit/widgets/gotolinedialog.ui \
    qcodeedit/widgets/gotoline.ui \
    qcodeedit/widgets/formatconfig.ui \
    qcodeedit/widgets/editconfig.ui \
    qcodeedit/snippets/snippetedit.ui \
    ui/windowide.ui \
    ui/dialogdasm.ui \
    ui/cregssc61860widget.ui \
    ui/cregslh5801widget.ui \
    ui/cregsz80widget.ui \
    ui/uartconsole.ui \
    ui/cregshd61700widget.ui
HEADERS += src/core/Connect.h \
    src/core/Dasm.h \
    src/core/Debug.h \
    src/core/Inter.h \
    src/core/Keyb.h \
    src/core/Keyb1250.h \
    src/core/Keyb1251.h \
    src/core/Keyb1350.h \
    src/core/Keyb1360.h \
    src/core/Keyb1401.h \
    src/core/Keyb1403.h \
    src/core/Keyb1450.h \
    src/core/Keyb1500.h \
    src/core/Log.h \
    src/core/analog.h \
    src/core/autoupdater.h \
    src/core/clink.h \
    src/core/common.h \
    src/core/cprinter.h \
    src/core/dialogabout.h \
    src/core/dialoganalog.h \
    src/core/dialogconsole.h \
    src/core/dialogdump.h \
    src/core/dialogkeylist.h \
    src/core/dialoglog.h \
    src/core/dialogstartup.h \
    src/core/global.h \
    src/core/hexviewer.h \
    src/core/init.h \
    src/core/keybce150.h \
    src/core/keybce152.h \
    src/core/mainwindowpockemul.h \
    src/core/paperwidget.h \
    src/core/pobject.h \
    src/core/slot.h \
    src/core/xmlwriter.h \
    src/cpu/cpu.h \
    src/cpu/lh5801.h \
    src/cpu/lh5810.h \
    src/cpu/pd1990ac.h \
    src/cpu/sc61860.h \
    src/cpu/sc62015.h \
    src/machine/Ce126.h \
    src/machine/ccable.h \
    src/machine/ce125.h \
    src/machine/ce150.h \
    src/machine/ce152.h \
    src/machine/pc1245.h \
    src/machine/pc1250.h \
    src/machine/pc1251.h \
    src/machine/pc1255.h \
    src/machine/pc1260.h \
    src/machine/pc1350.h \
    src/machine/pc1360.h \
    src/machine/pc1401.h \
    src/machine/pc1402.h \
    src/machine/pc1403.h \
    src/machine/pc1450.h \
    src/machine/pc1475.h \
    src/machine/pc1500.h \
    src/machine/pcxxxx.h \
    src/machine/sio.h \
    src/cpu/z80.h \
    src/cpu/z80memory.h \
    src/cpu/lh5803.h \
    src/machine/pc1600.h \
    src/machine/ce1600p.h \
    src/cpu/hd61102.h \
    src/cpu/z80-2.h \
    src/cpu/lu57813p.h \
    src/core/Keyb1600.h \
    src/machine/potar.h \
    src/core/dialogpotar.h \
    src/machine/cmotor.h \
    src/cpu/tc8576p.h \
    src/core/cextension.h \
    src/machine/pc2500.h \
    src/core/Keyb2500.h \
    src/machine/ce515p.h \
    src/machine/pc1421.h \
    src/machine/pc1280.h \
    src/core/Keyb1280.h \
    src/core/qhexedit.h \
    src/core/qhexedit_p.h \
    src/machine/ce140p.h \
    src/core/weblinksparser.h \
    src/core/wavfile.h \
    src/core/lcc/lcc.h \
    src/core/ide/highlighter.h \
    src/core/lcc/lcpp.h \
    src/core/lcc/codegen.h \
    src/core/lcc/pasm.h \
    resources/lcc/model/PC-1350/stdio.h \
    resources/lcc/model/PC-1350/stdio.h \
    src/core/lcc/cstdlib.h \
    resources/lcc/model/PC-1350/graph.h \
    src/core/lcc/parser/parser.h \
    src/machine/ce140f.h \
    src/machine/cesimu.h \
    src/core/dialogsimulator.h \
    src/core/bineditor/bineditor.h \
    src/core/bineditor/texteditor_global.h \
    src/core/bineditor/colorscheme.h \
    src/core/bineditor/texteditorconstants.h \
    src/machine/ce120p.h \
    qcodeedit/qreliablefilewatch.h \
    qcodeedit/qpanellayout.h \
    qcodeedit/qlinemarksinfocenter.h \
    qcodeedit/qlanguagefactory.h \
    qcodeedit/qlanguagedefinition.h \
    qcodeedit/qformatscheme.h \
    qcodeedit/qformatfactory.h \
    qcodeedit/qformat.h \
    qcodeedit/qeditsession.h \
    qcodeedit/qeditorinputbindinginterface.h \
    qcodeedit/qeditorinputbinding.h \
    qcodeedit/qeditorfactory.h \
    qcodeedit/qeditor.h \
    qcodeedit/qcodeedit.h \
    qcodeedit/qcodecompletionengine.h \
    qcodeedit/qce-config.h \
    qcodeedit/document/qdocumentsearch.h \
    qcodeedit/document/qdocumentline_p.h \
    qcodeedit/document/qdocumentline.h \
    qcodeedit/document/qdocumentcursor_p.h \
    qcodeedit/document/qdocumentcursor.h \
    qcodeedit/document/qdocumentcommand.h \
    qcodeedit/document/qdocumentbuffer.h \
    qcodeedit/document/qdocument_p.h \
    qcodeedit/document/qdocument.h \
    qcodeedit/qnfa/qnfadefinition.h \
    qcodeedit/qnfa/qnfa.h \
    qcodeedit/qnfa/light_vector.h \
    qcodeedit/snippets/qsnippetpatternloader.h \
    qcodeedit/snippets/qsnippetmanager.h \
    qcodeedit/snippets/qsnippetedit.h \
    qcodeedit/snippets/qsnippetbinding.h \
    qcodeedit/snippets/qsnippet_p.h \
    qcodeedit/snippets/qsnippet.h \
    qcodeedit/widgets/qstatuspanel.h \
    qcodeedit/widgets/qsimplecolorpicker.h \
    qcodeedit/widgets/qsearchreplacepanel.h \
    qcodeedit/widgets/qpanel.h \
    qcodeedit/widgets/qlinenumberpanel.h \
    qcodeedit/widgets/qlinemarkpanel.h \
    qcodeedit/widgets/qlinechangepanel.h \
    qcodeedit/widgets/qgotolinepanel.h \
    qcodeedit/widgets/qgotolinedialog.h \
    qcodeedit/widgets/qformatconfig.h \
    qcodeedit/widgets/qfoldpanel.h \
    qcodeedit/widgets/qeditconfig.h \
    qcodeedit/widgets/qcalltip.h \
    src/core/ide/editorwidget.h \
    ui/windowide.h \
    qcodeedit/widgets/qhexpanel.h \
    qcodeedit/widgets/qoutpanel.h \
    resources/lcc/model/PC-1350/sound.h \
    resources/lcc/model/__sound.h \
    resources/lcc/model/PC-1350/internal.h \
    resources/lcc/model/PC-1251/internal.h \
    resources/lcc/model/PC-1251/sound.h \
    resources/lcc/model/__stdio.h \
    resources/lcc/model/PC-1251/stdio.h \
    resources/lcc/model/__internal.h \
    resources/lcc/model/PC-1262/stdio.h \
    resources/lcc/model/PC-1262/sound.h \
    resources/lcc/model/PC-1262/internal.h \
    resources/lcc/model/PC-1360/stdio.h \
    resources/lcc/model/PC-1360/sound.h \
    resources/lcc/model/PC-1360/internal.h \
    resources/lcc/model/PC-1360/graph.h \
    qcodeedit/ccompletion.h \
    qcodeedit/qcodecompletionwidget_p.h \
    qcodeedit/qcodecompletionwidget.h \
    resources/lcc/model/PC-1261/stdio.h \
    resources/lcc/model/PC-1261/sound.h \
    resources/lcc/model/PC-1261/internal.h \
    resources/lcc/model/PC-1250/stdio.h \
    resources/lcc/model/PC-1250/sound.h \
    resources/lcc/model/PC-1250/internal.h \
    resources/lcc/model/PC-1255/stdio.h \
    resources/lcc/model/PC-1255/sound.h \
    resources/lcc/model/PC-1255/internal.h \
    resources/lcc/model/PC-1260/stdio.h \
    resources/lcc/model/PC-1260/sound.h \
    resources/lcc/model/PC-1260/internal.h \
    resources/lcc/model/PC-1475/stdio.h \
    resources/lcc/model/PC-1475/sound.h \
    resources/lcc/model/PC-1475/internal.h \
    qcodemodel2/qcodenode.h \
    ui/dialogdasm.h \
    ui/cregssc61860widget.h \
    ui/cregcpu.h \
    ui/cregslh5801widget.h \
    src/machine/cx07.h \
    src/cpu/ct6834.h \
    src/machine/cx07char.h \
    ui/cregsz80widget.h \
    src/cpu/hd61700.h \
    src/machine/e500.h \
    src/machine/cx710.h \
    src/cpu/uart.h \
    ui/uartconsole.h \
    src/machine/pb1000.h \
    src/cpu/hd44352.h \
    ui/cregshd61700widget.h \
    src/lcd/Lcdc_x07.h \
    src/lcd/Lcdc_symb.h \
    src/lcd/Lcdc_pc1600.h \
    src/lcd/Lcdc_pb1000.h \
    src/lcd/Lcdc_e500.h \
    src/lcd/Lcdc.h \
    pictureflow/pictureflow.h \
    pictureflow/materialflow.h \
    pictureflow/fluidlauncher.h \
    pictureflow/launcher.h \
    pictureflow/slideshow.h \
    src/core/launchbuttonwidget.h \
    src/core/tapandholdgesture.h \
    src/machine/pb2000.h \
    src/machine/md100.h \
    src/machine/casiodisk.h \
    src/machine/fp100.h \
    src/machine/g850v.h \
    src/cpu/sed1560.h \
    src/lcd/Lcdc_g850.h \
    src/cpu/i80x86.h \
    src/machine/z1.h \
    src/cpu/hd66108.h \
    src/lcd/Lcdc_z1.h \
    src/cpu/cf79107pj.h \
    src/cpu/i8085.h \
    src/cpu/i8085cpu.h \
    src/cpu/i8085daa.h \
    src/machine/fp200.h \
    src/lcd/Lcdc_fp200.h
INCLUDEPATH += . \
    src/core \
    src/cpu \
    src/machine \
    src/lcd \
    src/qmdilib \
    pictureflow

INCLUDEPATH += qcodeedit qcodeedit/document qcodeedit/language qcodeedit/widgets qcodeedit/snippets qcodeedit/qnfa
LANGUAGE += C++
LICENSE += GPL
MOC_DIR += build/moc
OPENEDFILES += 

RCC_DIR += build/rcc
RESOURCES += resources/ext.qrc \
    resources/pc1245.qrc \
    resources/pc1250.qrc \
    resources/pc1251.qrc \
    resources/pc1255.qrc \
    resources/pc1260.qrc \
    resources/pc1261.qrc \
    resources/pc1262.qrc \
    resources/pc1350.qrc \
    resources/pc1360.qrc \
    resources/pc1401.qrc \
    resources/pc1402.qrc \
    resources/pc1403.qrc \
    resources/pc1450.qrc \
    resources/pc1475.qrc \
    resources/pc1500.qrc \
    resources/pockemul.qrc \
    src/core/lfhex/lfhex.qrc \
    resources/pc1600.qrc \
    resources/pc2500.qrc \
    resources/pc1421.qrc \
    resources/pc1425.qrc \
    resources/pc1460.qrc \
    resources/pc1280.qrc \
    resources/keymap.qrc \
    resources/stdlibs.qrc \
    qcodeedit/qxs/qxs.qrc \
    resources/asmlibs.qrc \
    resources/x07.qrc \
    resources/pb1000.qrc \
    resources/e500.qrc \
    resources/pb2000.qrc \
    resources/g850v.qrc \
    resources/ext2.qrc \
    resources/z1.qrc \
    resources/fp200.qrc

SOURCES += src/core/Connect.cpp \
    src/core/Dasm.cpp \
    src/core/Debug.cpp \
    src/core/Inter.cpp \
    src/core/Keyb.cpp \
    src/core/Log.cpp \
    src/core/analog.cpp \
    src/core/autoupdater.cpp \
    src/core/clink.cpp \
    src/core/cprinter.cpp \
    src/core/dialogabout.cpp \
    src/core/dialoganalog.cpp \
    src/core/dialogconsole.cpp \
    src/core/dialogdump.cpp \
    src/core/dialogkeylist.cpp \
    src/core/dialoglog.cpp \
    src/core/dialogstartup.cpp \
    src/core/hexviewer.cpp \
    src/core/init.cpp \
    src/core/mainwindowpockemul.cpp \
    src/core/paperwidget.cpp \
    src/core/pcxxxx_ext.cpp \
    src/core/pobject.cpp \
    src/core/pockemul.cpp \
    src/core/slot.cpp \
    src/core/xmlwriter.cpp \
    src/cpu/cpu.cpp \
    src/cpu/lh5801.cpp \
    src/cpu/lh5810.cpp \
    src/cpu/pd1990ac.cpp \
    src/cpu/sc61860.cpp \
    src/cpu/sc62015.cpp \
    src/machine/Ce126.cpp \
    src/machine/ccable.cpp \
    src/machine/ce125.cpp \
    src/machine/ce150.cpp \
    src/machine/ce152.cpp \
    src/machine/pc1245.cpp \
    src/machine/pc1250.cpp \
    src/machine/pc1251.cpp \
    src/machine/pc1255.cpp \
    src/machine/pc1260.cpp \
    src/machine/pc1350.cpp \
    src/machine/pc1360.cpp \
    src/machine/pc1401.cpp \
    src/machine/pc1402.cpp \
    src/machine/pc1403.cpp \
    src/machine/pc1450.cpp \
    src/machine/pc1475.cpp \
    src/machine/pc1500.cpp \
    src/machine/pcxxxx.cpp \
    src/machine/sio.cpp \
    src/cpu/z80.cpp \
    src/cpu/lh5803.cpp \
    src/machine/pc1600.cpp \
    src/machine/ce1600p.cpp \
    src/cpu/z80_dasm.cpp \
    src/cpu/hd61102.cpp \
    src/cpu/lu57813p.cpp \
    src/machine/potar.cpp \
    src/core/dialogpotar.cpp \
    src/machine/cmotor.cpp \
    src/cpu/tc8576p.cpp \
    src/core/cextension.cpp \
    src/machine/pc2500.cpp \
    src/machine/ce515p.cpp \
    src/machine/pc1421.cpp \
    src/machine/pc1280.cpp \
    src/core/qhexedit.cpp \
    src/core/qhexedit_p.cpp \
    src/machine/ce140p.cpp \
    src/core/weblinksparser.cpp \
    src/core/wavfile.cpp \
    src/core/lcc/lcc.cpp \
    src/core/lcc/calcunit.cpp \
    src/core/ide/highlighter.cpp \
    src/core/lcc/lcpp.cpp \
    src/core/lcc/codegen.cpp \
    src/core/lcc/pasm.cpp \
    src/core/lcc/cstdlib.cpp \
    src/core/lcc/parser/parser.cpp \
    src/machine/ce140f.cpp \
    src/machine/cesimu.cpp \
    src/core/dialogsimulator.cpp \
    src/core/bineditor/bineditor.cpp \
    src/core/bineditor/colorscheme.cpp \
    src/machine/ce120p.cpp \
    qcodeedit/qreliablefilewatch.cpp \
    qcodeedit/qpanellayout.cpp \
    qcodeedit/qlinemarksinfocenter.cpp \
    qcodeedit/qlanguagefactory.cpp \
    qcodeedit/qlanguagedefinition.cpp \
    qcodeedit/qformatscheme.cpp \
    qcodeedit/qeditsession.cpp \
    qcodeedit/qeditorinputbinding.cpp \
    qcodeedit/qeditorfactory.cpp \
    qcodeedit/qeditor.cpp \
    qcodeedit/qcodeedit.cpp \
    qcodeedit/qcodecompletionengine.cpp \
    qcodeedit/document/qdocumentsearch.cpp \
    qcodeedit/document/qdocumentline.cpp \
    qcodeedit/document/qdocumentcursor.cpp \
    qcodeedit/document/qdocumentcommand.cpp \
    qcodeedit/document/qdocumentbuffer.cpp \
    qcodeedit/document/qdocument.cpp \
    qcodeedit/qnfa/xml2qnfa.cpp \
    qcodeedit/qnfa/qnfadefinition.cpp \
    qcodeedit/qnfa/qnfa.cpp \
    qcodeedit/snippets/qsnippetmanager.cpp \
    qcodeedit/snippets/qsnippetedit.cpp \
    qcodeedit/snippets/qsnippetbinding.cpp \
    qcodeedit/snippets/qsnippet.cpp \
    qcodeedit/widgets/qstatuspanel.cpp \
    qcodeedit/widgets/qsimplecolorpicker.cpp \
    qcodeedit/widgets/qsearchreplacepanel.cpp \
    qcodeedit/widgets/qpanel.cpp \
    qcodeedit/widgets/qlinenumberpanel.cpp \
    qcodeedit/widgets/qlinemarkpanel.cpp \
    qcodeedit/widgets/qlinechangepanel.cpp \
    qcodeedit/widgets/qgotolinepanel.cpp \
    qcodeedit/widgets/qgotolinedialog.cpp \
    qcodeedit/widgets/qformatconfig.cpp \
    qcodeedit/widgets/qfoldpanel.cpp \
    qcodeedit/widgets/qeditconfig.cpp \
    qcodeedit/widgets/qcalltip.cpp \
    src/core/ide/editorwidget.cpp \
    ui/windowide.cpp \
    qcodeedit/widgets/qhexpanel.cpp \
    qcodeedit/widgets/qoutpanel.cpp \
    qcodeedit/ccompletion.cpp \
    qcodeedit/qcodecompletionwidget.cpp \
    qcodemodel2/qcodenode.cpp \
    ui/dialogdasm.cpp \
    ui/cregssc61860widget.cpp \
    ui/cregcpu.cpp \
    ui/cregslh5801widget.cpp \
    src/machine/cx07.cpp \
    src/cpu/ct6834.cpp \
    ui/cregsz80widget.cpp \
    src/cpu/hd61700d.cpp \
    src/cpu/hd61700.cpp \
    src/machine/e500.cpp \
    src/machine/cx710.cpp \
    src/cpu/uart.cpp \
    ui/uartconsole.cpp \
    src/machine/pb1000.cpp \
    src/cpu/hd44352.cpp \
    ui/cregshd61700widget.cpp \
    src/lcd/Lcdc_x07.cpp \
    src/lcd/Lcdc_pc1600.cpp \
    src/lcd/Lcdc_pb1000.cpp \
    src/lcd/Lcdc_e500.cpp \
    src/lcd/Lcdc.cpp \
    pictureflow/pictureflow.cpp \
    pictureflow/materialflow.cpp \
    pictureflow/fluidlauncher.cpp \
    pictureflow/launcher.cpp \
    pictureflow/slideshow.cpp \
    src/core/launchbuttonwidget.cpp \
    src/core/tapandholdgesture.cpp \
    src/machine/pb2000.cpp \
    src/machine/md100.cpp \
    src/machine/casiodisk.cpp \
    src/machine/fp100.cpp \
    src/machine/g850v.cpp \
    src/cpu/sed1560.cpp \
    src/lcd/Lcdc_g850.cpp \
    src/cpu/i80x86.cpp \
    src/machine/z1.cpp \
    src/cpu/hd66108.cpp \
    src/lcd/Lcdc_z1.cpp \
    src/cpu/i80x86_dasm.cpp \
    src/cpu/cf79107pj.cpp \
    src/cpu/i8085.cpp \
    src/machine/fp200.cpp \
    src/cpu/i8085_dasm.cpp \
    src/lcd/Lcdc_fp200.cpp




OTHER_FILES += \
    resources/keymap/trspc2.map \
    resources/keymap/pc2500.map \
    resources/keymap/pc1600.map \
    resources/keymap/pc1500.map \
    resources/keymap/pc1450.map \
    resources/keymap/pc1403.map \
    resources/keymap/pc1401.map \
    resources/keymap/pc1360.map \
    resources/keymap/pc1350.map \
    resources/keymap/pc1280.map \
    resources/keymap/pc1250.map \
    resources/keymap/ce1600p.map \
    resources/keymap/ce150.map \
    resources/pockemul/release_notes.html \
    resources/keymap/ce125tape.map \
    resources/keymap/ce126.map \
    resources/keymap/ce125.map \
    resources/keymap/ce129.map \
    resources/keymap/ce123.map \
    resources/keymap/ce120.map \
    resources/keymap/263591.map \
    resources/keymap/ce140p.map \
    resources/weblinks.xml \
    resources/notes.txt \
    resources/pockemul/script_help.html \
    logitheque/serial1251.qs \
    qcodeedit/qxs/marks.qxm \
    qcodeedit/qxs/xml.qxf \
    qcodeedit/qxs/tex.qxf \
    qcodeedit/qxs/qtscript.qxf \
    qcodeedit/qxs/python.qxf \
    qcodeedit/qxs/pro.qxf \
    qcodeedit/qxs/php.qxf \
    qcodeedit/qxs/formats.qxf \
    qcodeedit/qxs/dox.qxf \
    qcodeedit/qxs/css.qxf \
    qcodeedit/qxs/csharp.qxf \
    qcodeedit/qxs/cpp.qxf \
    qcodeedit/qxs/bibtex.qxf \
    qcodeedit/qxs/xml.qnfa \
    qcodeedit/qxs/tex.qnfa \
    qcodeedit/qxs/snippet.qnfa \
    qcodeedit/qxs/qtscript.qnfa \
    qcodeedit/qxs/python.qnfa \
    qcodeedit/qxs/pro.qnfa \
    qcodeedit/qxs/php.qnfa \
    qcodeedit/qxs/dox.qnfa \
    qcodeedit/qxs/css.qnfa \
    qcodeedit/qxs/csharp.qnfa \
    qcodeedit/qxs/cpp.qnfa \
    qcodeedit/qxs/coldfusion.qnfa \
    qcodeedit/qxs/bibtex.qnfa \
    qcodeedit/qxs/asm.qnfa \
    qcodeedit/qxs/asm.qxf \
    resources/asmlibs/sc61860/xor16.lib \
    resources/asmlibs/sc61860/xor8.lib \
    resources/asmlibs/sc61860/sr16.lib \
    resources/asmlibs/sc61860/sr8.lib \
    resources/asmlibs/sc61860/sl16.lib \
    resources/asmlibs/sc61860/sl8.lib \
    resources/asmlibs/sc61860/mul16.lib \
    resources/asmlibs/sc61860/mul8.lib \
    resources/asmlibs/sc61860/divmod8.lib \
    resources/asmlibs/sc61860/cpse16.lib \
    resources/asmlibs/sc61860/cps16.lib \
    resources/asmlibs/sc61860/cpne16.lib \
    resources/asmlibs/sc61860/cpge16.lib \
    resources/asmlibs/sc61860/cpg16.lib \
    resources/asmlibs/sc61860/cpeq16.lib \
    resources/asmlibs/sc61860/cpe16.lib \
    resources/asmlibs/sc61860/divmod16.lib \
    resources/keymap/x07.map \
    resources/keymap/e500.map \
    resources/keymap/pb1000.map \
    resources/keymap/x710.map \
    resources/pockemul/config.xml \
    resources/pockemul/configExt.xml \
    resources/keymap/pb2000.map \
    resources/keymap/g850v.map \
    resources/keymap/z1.map \
    resources/keymap/fp200.map

















