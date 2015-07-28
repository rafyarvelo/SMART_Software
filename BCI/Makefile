# Project: BCI
# Makefile created by Rafy

CPP      = g++.exe
CC       = gcc.exe
OBJ      = bci_c_bci_package.o bci_main.o bci_c_signal_processing.o bci_c_judgment_algorithm.o bci_c_rvs.o bci_c_flasher_io.o bci_c_eeg_io_debug.o bci_c_serial_comm.o 
LINKOBJ  = bci_c_bci_package.o bci_main.o bci_c_signal_processing.o bci_c_judgment_algorithm.o bci_c_rvs.o bci_c_flasher_io.o bci_c_eeg_io_debug.o bci_c_serial_comm.o 
BOOST_LIB = -I"../lib/boost_1_58_0"
#INCS     = -I"C:/Program Files (x86)/Dev-Cpp/MinGW64/x86_64-w64-mingw32/include" -I"C:/Users/Rafy/SENIOR_DESIGN/SMART_Software/lib/boost_1_58_0"
#CXXINCS  = -I"C:/Program Files (x86)/Dev-Cpp/MinGW64/x86_64-w64-mingw32/include" -I"C:/Users/Rafy/SENIOR_DESIGN/SMART_Software/lib/boost_1_58_0" -I"C:/Users/Rafy/SENIOR_DESIGN/SMART_Software/lib/boost_1_58_0"
BIN      = BCI.exe
CXXFLAGS = $(CXXINCS) -g3
CFLAGS   = $(INCS) -g3
RM       = rm -f *~

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) #$(LIBS)

bci_c_bci_package.o: bci_c_bci_package.cpp
	$(CPP) -c bci_c_bci_package.cpp -o bci_c_bci_package.o $(CXXFLAGS)

bci_main.o: bci_main.cpp
	$(CPP) -c bci_main.cpp -o bci_main.o $(CXXFLAGS)

bci_c_signal_processing.o: bci_c_signal_processing.cpp
	$(CPP) -c bci_c_signal_processing.cpp -o bci_c_signal_processing.o $(CXXFLAGS)

bci_c_judgment_algorithm.o: bci_c_judgment_algorithm.cpp
	$(CPP) -c bci_c_judgment_algorithm.cpp -o bci_c_judgment_algorithm.o $(CXXFLAGS)

bci_c_rvs.o: bci_c_rvs.cpp
	$(CPP) -c bci_c_rvs.cpp -o bci_c_rvs.o $(CXXFLAGS)

bci_c_flasher_io.o: bci_c_flasher_io.cpp
	$(CPP) -c bci_c_flasher_io.cpp -o bci_c_flasher_io.o $(CXXFLAGS)

bci_c_eeg_io_debug.o: bci_c_eeg_io_debug.cpp
	$(CPP) -c bci_c_eeg_io_debug.cpp -o bci_c_eeg_io_debug.o $(CXXFLAGS)

bci_c_serial_comm.o: bci_c_serial_comm.cpp
	$(CPP) -c bci_c_serial_comm.cpp -o bci_c_serial_comm.o $(CXXFLAGS) $(BOOST_LIB)

move_files:
	mkdir obj
	move *.o obj
