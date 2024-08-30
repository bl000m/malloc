#!/bin/sh

# Check if the correct number of arguments are provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <source_file> <executable_name>"
    exit 1
fi

SOURCE_FILE=$1
EXECUTABLE_NAME=$2

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.

# Compile the test program
cc "$SOURCE_FILE" -o "$EXECUTABLE_NAME" -L. -lft_malloc -I ./include -I ./libft

# Run the compiled test program
./"$EXECUTABLE_NAME"
