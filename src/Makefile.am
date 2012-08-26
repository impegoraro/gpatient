bin_PROGRAMS = herbanaria

AM_CPPFLAGS = $(DEPS_CFLAGS) -std=c++11
herbanaria_LDADD = $(DEPS_LIBS)

herbanaria_SOURCES = configuration/key.h configuration/key.cpp \
                     configuration/section.h configuration/section.cpp \
                     configuration/configuration.h configuration/configuration.cpp \
                     patient-window.h patient-window.cpp \
                     db/dbhandler.h db/dbhandler.cpp \
                     main-window.h main-window.cpp \
                     login-window.h login-window.cpp \
                     main.cpp
