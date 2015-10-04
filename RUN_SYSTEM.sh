#This Executable should contain all the necessary commands for running the system

#Executable Names
export BCI_EXECUTABLE="BCI/BCI"
export TM_EXTRACT_TOOL="TOOLS/TM_EXTRACT/TelemetryExtractTool"
export FILE_UNLOCK_TOOL="TOOLS/unlockDevices"

#Shared Libraries are included in the Build, Not usually a good idea but hey whatever man
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/lib:/usr/local/lib:$HOME/SENIOR_DESIGN/SMART_Software/LIB_DESKTOP:$HOME/SENIOR_DESIGN/SMART_Software/LIB_PI"

#Gotta change permissions for those stupid serial ports
chmod +x $FILE_UNLOCK_TOOL
$FILE_UNLOCK_TOOL

#Compile Source Using Top Level MakeFile
make

#Run the Program
sudo chmod +x $BCI_EXECUTABLE
$BCI_EXECUTABLE 

#Extract Telemetry 
sudo chmod +x $TM_EXTRACT_TOOL
$TM_EXTRACT_TOOL debug_files/tm_output_data.bin debug_files/extracted_telemetry.csv
