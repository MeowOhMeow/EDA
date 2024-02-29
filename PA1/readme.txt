1. 學號：b11032006
2. 姓名：周柏宇
3. 使用之程式語言：C++
4. 使用之編譯平台：Linux GNU g++
5. 檔案壓縮方式: tar zcvf b11032006-p1.tgz b11032006-p1
6. 各檔案說明：
	b11032006-p1/main.cpp		: 主程式
	b11032006-p1/debug.h  		: define error messages
	b11032006-p1/util.h  		: utility function header file
	b11032006-p1/util.cpp  		: implementation
	b11032006-p1/Generator.h  	: header file
	b11032006-p1/Generator.cpp  : cpp file
	b11032006-p1/Makefile  		: Makefile
	b11032006-p1/readme.txt		: 本檔案
	b11032006-p1/prefix.txt     : prefix file
	b11032006-p1/suffix.txt     : suffix file
7. 編譯方式說明：        	
   主程式：
	   在 b11032006-p1/ 這個資料夾下指令 : make
	   即可在 b11032006-p1 產生 ./genPlot 的執行檔
	
8. 執行、使用方式說明：
   主程式：
   	   compile 完成後，在 b11032006-p1/ 目錄下會產生一個 genPlot 的執行檔
   	   執行檔的命令格式為 :
   	   ./genPlot [input file name] [output file name]

	   ex: ./genPlot input.txt output.gp
	   
