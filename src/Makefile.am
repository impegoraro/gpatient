bin_PROGRAMS = herbanaria

AM_CPPFLAGS=$(DEPS_CFLAGS) -std=c++0x -O0 -g 
herbanaria_LDADD=$(DEPS_LIBS)

herbanaria_SOURCES = util.h util.cpp \
                     configuration/key.h configuration/key.cpp \
                     configuration/section.h configuration/section.cpp \
                     configuration/configuration.h configuration/configuration.cpp \
                     exceptions/sql-connection.h exceptions/sql-connection.cpp \
                     ui/widgets/widgets.h ui/widgets/numeric-entry.h ui/widgets/numeric-entry.cpp \
                     ui/widgets/date-entry.h ui/widgets/date-entry.cpp \
                     ui/widgets/calendar-window.h ui/widgets/calendar-window.cpp \
                     ui/patient-window.h ui/patient-window.cpp \
                     ui/main-window.h ui/main-window.cpp \
                     ui/view-patient.cpp ui/view-patient.h \
                     db/dbhandler.h db/dbhandler.cpp \
                     person.h person.cpp \
                     main.cpp main.h visitswindow.cc visitswindow.h 
