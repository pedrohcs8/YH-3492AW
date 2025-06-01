rm -rf build/*
cd build
cmake ..
bear -- make
rm -rf ../compile_commands.json
cp compile_commands.json ..
cp microwave-display.uf2 /media/pedrohcs8/RPI-RP2
$shell
