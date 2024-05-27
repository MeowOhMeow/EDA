1. 學號：b11032006
2. 姓名：周柏宇
3. 使用之程式語言：C++
4. 測試平台平台：
    1. 作業系統	
        - OS Name:          Ubuntu 22.04.4 LTS
        - OS Type:          64-bit
        - GNOME Version:    42.9
        - Windowing System: Wayland
        - Virtualization:   Oracle
    2. 編譯器
        - g++ (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
    3. Make
        - GNU Make 4.3
        - Built for x86_64-pc-linux-gnu
5. 檔案壓縮方式: tar zcvf b11032006-p3.tgz b11032006-p3
    - bsdtar 3.6.2 - libarchive 3.6.2 zlib/1.2.5.f-ipp liblzma/5.2.5 bz2lib/1.0.8 libzstd/1.5.4
6. 各檔案說明：
    b11032006-p3
    │  main.cpp     // 主程式
    │  makefile     // Makefile
    └─ readme.txt   // 說明文件
7. 編譯方式：
    - 解壓縮後，請進入 b11032006-p3，使用 make 指令進行編譯
    - 編譯完成後，將會產生一個名為 Malware 的執行檔
    ```shell
    $ tar zxvf b11032006-p3.tgz
    $ cd b11032006-p3
    $ make
    ```
    注意：makefile 中的變數 CPLEX_PATH 是使用以下指令取得的路徑
    ```shell
    $ which cplex
    ```
    只要是有正確安裝 CPLEX 的 linux 環境且沒有更動過 CPLEX 內的路徑結構，就可以使用上述指令取得正確路徑。
    如果有更動過 CPLEX 內的路徑結構，請自行修改 makefile 中的 CXXFLAGS, LDFLAGS, LDLIBS 變數。
8. 執行方式：
    - 編譯完成後，請使用以下指令執行程式
    ```shell
    $ ./tpl <input file name> <output file name>
    ```
