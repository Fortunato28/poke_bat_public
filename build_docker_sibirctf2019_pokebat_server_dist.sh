#!/bin/bash

rm -rf _result

docker build --file "Dockerfile.dist" --tag "sibirctf2019/poke_bat_server:0.0.2" .