TEMPLATE = app
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
HEADERS += \
   hmac.h \
   otp.h \
   softtoken.h
SOURCES += \
   hmac.cpp \
   main.cpp \
   otp.cpp \
   softtoken.cpp
FORMS += \
   softtoken.ui
RC_FILE = softtoken.rc
