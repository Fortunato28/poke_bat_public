#you can specify your source dir path with this variable
#SOURCE_DIR="/home/fort/Programming/cpp/poke_bat"

docker run --rm -it -v "$PWD:/usr/src/poke_bat" keimashikai/poke_test ./build_all.sh
