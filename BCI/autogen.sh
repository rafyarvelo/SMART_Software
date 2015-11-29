#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
protoc -I=$DIR/../py/ --cpp_out=$DIR $DIR/../py/seniordesign.proto
