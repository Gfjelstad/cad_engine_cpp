#!/bin/bash

# OpenCASCADE Download and Extract Script
# Downloads OCCT v7.9.1 and extracts to opencascade folder

set -e  # Exit on any error

# Configuration
DOWNLOAD_URL="https://github.com/Open-Cascade-SAS/OCCT/releases/download/V7_9_1/occt_vc14-64-combined-with-debug.zip"
ZIP_FILE="occt_vc14-64-combined-with-debug.zip"
TARGET_DIR="opencascade"

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "OpenCASCADE Download Script"
echo "=========================="
echo "Script directory: $SCRIPT_DIR"
echo "Target directory: $SCRIPT_DIR/$TARGET_DIR"
echo

# Check if target directory already exists
if [ -d "$SCRIPT_DIR/$TARGET_DIR" ]; then
    echo "Warning: Directory '$TARGET_DIR' already exists."
    read -p "Do you want to remove it and continue? (y/N): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo "Removing existing directory..."
        rm -rf "$SCRIPT_DIR/$TARGET_DIR"
    else
        echo "Aborted."
        exit 1
    fi
fi

# Create target directory
echo "Creating directory: $TARGET_DIR"
mkdir -p "$SCRIPT_DIR/$TARGET_DIR"

# Change to script directory
cd "$SCRIPT_DIR"

# Download the zip file
echo "Downloading OpenCASCADE..."
echo "URL: $DOWNLOAD_URL"

if command -v curl >/dev/null 2>&1; then
    curl -L -o "$ZIP_FILE" "$DOWNLOAD_URL"
elif command -v wget >/dev/null 2>&1; then
    wget -O "$ZIP_FILE" "$DOWNLOAD_URL"
else
    echo "Error: Neither curl nor wget is available. Please install one of them."
    exit 1
fi

# Check if download was successful
if [ ! -f "$ZIP_FILE" ]; then
    echo "Error: Download failed. File $ZIP_FILE not found."
    exit 1
fi

echo "Download completed: $(ls -lh "$ZIP_FILE" | awk '{print $5}')"

# Extract the zip file
echo "Extracting to $TARGET_DIR..."

if command -v unzip >/dev/null 2>&1; then
    unzip -q "$ZIP_FILE" -d "$TARGET_DIR"
elif command -v 7z >/dev/null 2>&1; then
    7z x "$ZIP_FILE" -o"$TARGET_DIR" -y >/dev/null
else
    echo "Error: Neither unzip nor 7z is available. Please install unzip."
    exit 1
fi

# Clean up zip file
echo "Cleaning up..."
rm "$ZIP_FILE"

# Verify extraction
if [ -d "$TARGET_DIR" ] && [ "$(ls -A "$TARGET_DIR")" ]; then
    echo
    echo "✅ Success! OpenCASCADE extracted to: $TARGET_DIR"
    echo "Contents:"
    ls -la "$TARGET_DIR"
else
    echo "❌ Error: Extraction may have failed. Directory is empty or doesn't exist."
    exit 1
fi

echo
echo "Setup complete! You can now use OpenCASCADE in your CMake project."