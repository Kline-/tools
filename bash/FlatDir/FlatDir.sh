#!/bin/sh

###########################################
### NZBGET POST-PROCESSING SCRIPT       ###

# Flatten directories.
#
# This script looks for files based on a list of approved extensions.
#
# Any files found matching the approved extensions are then moved to a
# specified location. This is useful any time you want to break specific
# files out of their own sub-dirs.
#
# Example: When Extensions is set to "mp4 mkv" all files matching *.mp4 or *.mkv
#          after extraction is complete will be moved to DestDir. If DelSrcDir is true
#          then the original source folder for the download and any remaining contents
#          will be deleted.

################################
### OPTIONS                  ###

# File extensions.
#Extensions=mp4 mkv

# Destination directory.
#DestDir=/home/sabnzbd/Downloads/processed/XXX

# Delete source directory and all other files when done.
#DelSrcDir=true

### NZBGET POST-PROCESSING SCRIPT       ###
###########################################

FOUND=false
PP_SUCCESS=93
PP_FAIL=94
PP_SKIP=95

if [ "$NZBPP_TOTALSTATUS" != "SUCCESS" ]; then
	echo "[INFO] Download was not processed correctly, skipping."
	exit $PP_SKIP
fi

for i in $NZBPO_EXTENSIONS; do
	if [ -f "$NZBPP_DIRECTORY/"*.$i ]; then
		FILES=$(ls "$NZBPP_DIRECTORY/"*.$i)
		mv "$FILES" "$NZBPO_DESTDIR"
		echo "[DETAIL] Successfully moved "$(basename "$FILES")
		FOUND=true
	fi
done

if [ $FOUND = false ]; then
	echo "[DETAIL] No files matching approved extensions were found."
	exit $PP_FAIL
elif [ $FOUND = true ]; then
	echo "[NZB] FINALDIR="$NZBPO_DESTDIR
	if [ $NZBPO_DELSRCDIR = true ]; then
		rm -rf "$NZBPP_DIRECTORY"
		echo "[DETAIL] Deleted $NZBPP_DIRECTORY"
	fi
	exit $PP_SUCCESS
else
	echo "[ERROR] Unknown condition, this shouldn't happen."
	exit $PP_FAIL
fi
