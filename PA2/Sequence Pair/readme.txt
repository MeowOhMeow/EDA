1. 學號：b11032006
2. 姓名：周柏宇
3. 使用之程式語言：C++
4. 測試平台平台：
    1.  作業系統	
        - 版本    Windows 11 家用版
        - 版本	22H2
        - OS 組建	22621.3527
        - 體驗	Windows Feature Experience Pack 1000.22700.1003.0
    2.  編譯器
        - g++.exe (Rev6, Built by MSYS2 project) 13.2.0
    3. Make
        - GNU Make 4.4.1
        - Built for Windows32
5. 檔案壓縮方式: tar zcvf b11032006-p2.tgz b11032006-p2
    - bsdtar 3.6.2 - libarchive 3.6.2 zlib/1.2.5.f-ipp liblzma/5.2.5 bz2lib/1.0.8 libzstd/1.5.4
6. 各檔案說明：
    b11032006-p2
    │  Coordinates.hpp              // 儲存座標和一個template class，用來儲存更多的資訊
    │  Macro.hpp                    // 儲存 macro 的資訊，僅在IO中使用
    │  main.cpp                     // 主程式
    │  Makefile                     // Makefile
    │  readme.txt                   // 說明文件
    │  Scheduler.hpp                // 定義SA運作方式，用來排程
    │  SEQPairGraph.hpp             // 實作Horizontal、Vertical Graph，繼承自Graph/Graph.hpp中的Graph，使用Coordinates<int> *作為vertex的property，NoProperty作為edge的property
    │  SimulatedAnnealing.hpp       // 定義SA的流程，使用Scheduler來排程
    │
    ├─Algorithms
    │      LongestPath.hpp          // 實作Longest Path演算法輸入一個Graph，輸出一個vector<float>，代表每個vertex的最長路徑(weight)。使用TopologicalSort來排序vertex
    │      TopologicalSort.hpp      // 實作Topological Sort演算法輸入一個Graph，輸出一個vector<int>，代表每個vertex的排序(index)
    │
    ├─Graph
    │      EdgeProperty.hpp         // Edge property base class
    │      Graph.hpp                // Graph base class
    │      NoProperty.hpp           // No property，就是一個空class
    │      Vertex.hpp               // Vertex base class
    │      VertexProperty.hpp       // Vertex property base class
    │
    └─logs                          // 用來存放 log 檔

7. 編譯方式說明：
    - 解壓縮後，請進入 b11032006-p2/ 目錄，執行 make 來編譯程式
    ```bash       	
        cd b11032006-p2
        mkdir -p logs
        make
    ```

8. 執行、使用方式說明：
    - compile 完成後，在 b11032006-p2/ 目錄下會產生一個 floorplanner 的執行檔
    - 執行檔的命令格式為 :
    ```bash
        ./floorplanner <input file name> <output file name>
    ```
    - 例如 : ./floorplanner floorplan_100.txt output.txt
