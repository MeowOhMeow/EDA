1. 學號：b11032006
2. 姓名：周柏宇
3. 使用之程式語言：C++
4. 使用之編譯平台：Linux GNU g++
5. 檔案壓縮方式: tar zcvf b11032006-p2.tgz b11032006-p2
6. 各檔案說明：
	b11032006-p2/main.cpp		: 主程式source code
	b11032006-p2/debug.h  		: define error messages
	b11032006-p2/util.h  		: utility function header file
	b11032006-p2/util.cpp  		: implementation
    b11032006-p2/Dijkstra.h  	: Dijkstra algorithm header file(parent class)
    b11032006-p2/Dijkstra.cpp  	: implementation
	b11032006-p2/solution.h  	: header file(sub class)
	b11032006-p2/solution.cpp  	: cpp file
    b11032006-p2/Makefile  		: Makefile
	b11032006-p2/readme.txt		: 本檔案
7. 編譯方式說明：        	
   主程式：
	   在 b11032006-p2/ 這個資料夾下指令 : make
	   即可在 b11032006-p2 產生 ./solution 的執行檔
	
8. 執行、使用方式說明：
   主程式：
   	   compile 完成後，在 b11032006-p2/ 目錄下會產生一個 solution 的執行檔
   	   執行檔的命令格式為 :
   	   ./solution [input file name] [output file name]

	   ex: ./solution ./io/in/5x5.in ./io/out/5x5.out
	   
