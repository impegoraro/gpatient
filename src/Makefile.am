bin_PROGRAMS = herbanaria

AM_CPPFLAGS=$(DEPS_CFLAGS) -std=c++0x -O3 
herbanaria_LDADD=$(DEPS_LIBS)

herbanaria_SOURCES = util.h util.cpp \
                     exceptions/sql-connection.h exceptions/sql-connection.cpp \
                     ui/widgets/widgets.h ui/widgets/numeric-entry.h ui/widgets/numeric-entry.cpp \
                     ui/widgets/date-entry.h ui/widgets/date-entry.cpp \
                     ui/widgets/calendar-window.h ui/widgets/calendar-window.cpp \
                     ui/visitswindow.cpp ui/visitswindow.h \
                     ui/patient-window.h ui/patient-window.cpp \
                     ui/main-window.h ui/main-window.cpp \
                     ui/view-patient.cpp ui/view-patient.h \
                     db/dbhandler.h db/dbhandler.cpp \
                     db/allergy.h db/allergy.cpp \
                     person.h person.cpp \
                     main.cpp main.h \
                     configuration.h configuration.cpp
#                    visit.h #visit.cpp 
