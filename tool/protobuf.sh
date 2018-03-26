#!/bin/bash

for file in $(find ../protocol -name *.proto); do
    echo $file
	protoc --cpp_out=../src/protobuf  --proto_path=../protocol $file
done
