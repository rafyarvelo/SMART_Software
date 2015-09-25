#This Executable should contain all the necessary commands for running the system

#Executable Names
export BCI_EXECUTABLE="BCI/BCI"
export TM_EXTRACT_TOOL="BCI/TM_EXTRACT/TelemetryExtractTool"

#Shared Libraries are included in the Build, Not usually a good idea but hey whatever man
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/lib:/usr/local/lib:$HOME/SENIOR_DESIGN/SMART_Software/LIB_DESKTOP:$HOME/SENIOR_DESIGN/SMART_Software/LIB_PI"

#Gotta change TOOLpermissions for those stupid serial ports
deviceName=`dmesg | grep tty | grep USB | cut -d " " -f 4`
deviceName=${deviceName/:/}

echo "Unlocking Permissions for $deviceName..."
sudo chmod 777 /dev/$deviceName

#Compile Source Using Top Level MakeFile
make

#Run the Program
sudo chmod +x $BCI_EXECUTABLE
./$BCI_EXECUTABLE

#Extract Telemetry when Program Finishes (Usually via Ctrl-C)
sudo chmod +x $TM_EXTRACT_TOOL
./$TM_EXTRACT_TOOL
