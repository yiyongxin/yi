#!/bin/bash

for file in $(find ../protocol -name *.proto | grep -v discard); do
    echo $file
	protoc --cpp_out=../src/protobuf  --proto_path=../protocol $file
done
