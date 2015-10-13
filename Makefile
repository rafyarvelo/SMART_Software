#Top Level Makefile for SMART Software

BCI_EXECUTABLE="BCI/BCI"
TM_EXTRACT_TOOL="TOOLS/TM_EXTRACT/TelemetryExtractTool"

all: $(BCI_EXECUTABLE) $(TM_EXTRACT_TOOL)
	$(BCI)
	$(TM_EXTRACT)

$(BCI_EXECUTABLE):
	cd BCI && qmake && make

$(TM_EXTRACT_TOOL):
	cd TOOLS/TM_EXTRACT && qmake && make
	cp TOOLS/TM_EXTRACT/TelemetryExtractTool debug_files/

clean:
	cd BCI && make clean
	cd TOOLS/TM_EXTRACT && make clean
	rm -f $(BCI_EXECUTABLE)
	rm -f $(TM_EXTRACT_TOOL)
