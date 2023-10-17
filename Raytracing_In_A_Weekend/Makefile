src_files = *.cpp
out_file = rt_in_a_weekend

debug:
	g++ $(src_files) -Og -std=c++14 -o $(out_file)

release:
	g++ $(src_files) -O3 -flto -march=native -ffunction-sections -fdata-sections -std=c++14 -o $(out_file) -Wl,--gc-sections
clean:
	-rm rt_weekend_debug
	-rm rt_weekend_release
	-rm $(out_file)
	-rm *.ppm
	-rm *.bmp