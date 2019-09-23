#!/bin/sh

BUILD_DIR="_result"
if [ ! -d $BUILD_DIR  ]; then
    mkdir $BUILD_DIR
fi
#docker run --rm -it -v "$PWD:/usr/src/poke_bat" keimashikai/poke_test _result/work_with_database/db_test
docker run --rm -it -v "$PWD:/usr/src/poke_bat" keimashikai/poke_test ./build_all.sh
#docker run --rm -it -v "$PWD:/usr/src/poke_bat" keimashikai/poke_test bash
