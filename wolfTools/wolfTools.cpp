// wolfTools.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "third-party/xlnt/include/xlnt/xlnt.hpp"

#include "CTools.h"
#include "CAnalysisTools.h"

#include<io.h>
#include<string>
//#include<iostream>
#include<vector>

using namespace std;

void test2(xlnt::workbook &wbOut) {
    std::vector< std::vector<std::string> > wholeWorksheet;
    //Looping through each row (100 rows as per the second argument in the for loop)
    for (int outer = 0; outer < 3; outer++)
    {
        //Creating a fresh vector for a fresh row
        std::vector<std::string> singleRow;
        //Looping through each of the columns (100 as per the second argument in the for loop) in this particular row
        for (int inner = 0; inner < 4; inner++)
        {
            //Adding a single value in each cell of the row 
            std::string val = std::to_string(inner + 1);
            singleRow.push_back(val);
        }
        //Adding the single row to the 2 dimensional vector
        wholeWorksheet.push_back(singleRow);
        std::clog << "Writing to row " << outer << " in the vector " << std::endl;
    }
    //Writing to the spread sheet
    //Creating the output workbook 
    std::clog << "Creating workbook" << std::endl;
    xlnt::worksheet wsOut = wbOut.create_sheet(); // copy a sheet before doing anything with "Sheet1"
    wsOut.title("data2");

    //We will now be looping through the 2 dimensional vector which we created above
    //In this case we have two iterators one for the outer loop (row) and one for the inner loop (column)
    std::clog << "Looping through vector and writing to spread sheet" << std::endl;
    for (int fOut = 0; fOut < wholeWorksheet.size(); fOut++)
    {
        std::clog << "Row" << fOut << std::endl;
        for (int fIn = 0; fIn < wholeWorksheet.at(fOut).size(); fIn++)
        {
            //Take notice of the difference between accessing the vector and accessing the work sheet
        //As you may already know Excel spread sheets start at row 1 and column 1 (not row 0 and column 0 like you would expect from a C++ vector) 
        //In short the xlnt cell reference starts at column 1 row 1 (hence the + 1s below) and the vector reference starts at row 0 and column 0
            wsOut.cell(xlnt::cell_reference(fIn + 1, fOut + 1)).value(wholeWorksheet.at(fOut).at(fIn));
            //Further clarification to avoid confusion
            //Cell reference arguments are (column number, row number); e.g. cell_reference(fIn + 1, fOut + 1)
            //Vector arguments are (row number, column number); e.g. wholeWorksheet.at(fOut).at(fIn)
        }
    }

}

void testWrite() 
{
    //Creating a 2 dimensional vector which we will write values to
    std::vector< std::vector<std::string> > wholeWorksheet;
    //Looping through each row (100 rows as per the second argument in the for loop)
    for (int outer = 0; outer < 22; outer++)
    {
        //Creating a fresh vector for a fresh row
        std::vector<std::string> singleRow;
        //Looping through each of the columns (100 as per the second argument in the for loop) in this particular row
        for (int inner = 0; inner < 11; inner++)
        {
            //Adding a single value in each cell of the row 
            std::string val = std::to_string(inner + 1);
            singleRow.push_back(val);
        }
        //Adding the single row to the 2 dimensional vector
        wholeWorksheet.push_back(singleRow);
        std::clog << "Writing to row " << outer << " in the vector " << std::endl;
    }
    //Writing to the spread sheet
    //Creating the output workbook 
    std::clog << "Creating workbook" << std::endl;
    xlnt::workbook wbOut;
    //Setting the destination output file name
    std::string dest_filename = "output.xlsx";
    //Creating the output worksheet
    xlnt::worksheet wsOut = wbOut.active_sheet();
    //Giving the output worksheet a title/name
    wsOut.title("data");    
    //We will now be looping through the 2 dimensional vector which we created above
    //In this case we have two iterators one for the outer loop (row) and one for the inner loop (column)
    std::clog << "Looping through vector and writing to spread sheet" << std::endl;
    for (int fOut = 0; fOut < wholeWorksheet.size(); fOut++)
    {
        std::clog << "Row" << fOut << std::endl;
        for (int fIn = 0; fIn < wholeWorksheet.at(fOut).size(); fIn++)
        {
            //Take notice of the difference between accessing the vector and accessing the work sheet
        //As you may already know Excel spread sheets start at row 1 and column 1 (not row 0 and column 0 like you would expect from a C++ vector) 
        //In short the xlnt cell reference starts at column 1 row 1 (hence the + 1s below) and the vector reference starts at row 0 and column 0
            wsOut.cell(xlnt::cell_reference(fIn + 1, fOut + 1)).value(wholeWorksheet.at(fOut).at(fIn));
            //Further clarification to avoid confusion
            //Cell reference arguments are (column number, row number); e.g. cell_reference(fIn + 1, fOut + 1)
            //Vector arguments are (row number, column number); e.g. wholeWorksheet.at(fOut).at(fIn)
        }
    }
    std::clog << "Finished writing spread sheet" << std::endl;
    test2(wbOut);


    wbOut.save(dest_filename);
    //return 0;
}


void testRead() {
    xlnt::workbook wb;
    wb.load("d:/Users/IGG/Downloads/1.xlsx");
    auto ws = wb.active_sheet();
    std::clog << "Processing spread sheet" << std::endl;
    for (auto row : ws.rows(false))
    {
        for (auto cell : row)
        {
            std::clog << cell.to_string() << std::endl;
        }
    }
    std::clog << "Processing complete" << std::endl;    
}

void getFiles(string path, vector<string>& files)
{
    //文件句柄  
    long   hFile = 0;
    //文件信息  
    struct _finddata_t fileinfo;
    string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
    {
        do
        {
            //如果是目录,迭代之  
            //如果不是,加入列表  
            if ((fileinfo.attrib &  _A_SUBDIR))
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}

double round(double d)
{
    return floor(d + 0.5);
}

void LXQ() 
{
    std::cout << "欢迎李皮皮" << std::endl;
    std::cout << "1 将文档拷贝到Cal文件夹下 2 文档名需要为非中文 3 该Excel必须关闭" << std::endl;
    vector<string> files;
    ////获取该路径下的所有文件  
    getFiles("./Cal", files);
    
    struct sLXQParm {
        std::string name;
        int score;
    };

    struct sLXQPersonData {
        std::string name;
        std::vector<sLXQParm> m_vecData;
        int nAverageScore;
        void CalAvage() {
            unsigned int nSize = m_vecData.size();
            float nSCore = 0.0f;
            nAverageScore = 0;
            for (unsigned int i = 0; i < nSize; i++) {
                nSCore += m_vecData[i].score;
            }
            nAverageScore = round(nSCore / nSize);
        }
    };


    int nNameLine = 4;
    int nStarTag = 'I' - 'A' + 1;
    int nEndTag = 'S' - 'A' + 1;
    int nScoreLine = 24;
    bool bChange = false;

    char c = 'A' + 2;

    std::cout << "名字行为" << nNameLine << "行，列为" << (char)('A' + nStarTag-1) << " ~ " << (char)('A' + nEndTag - 1) << "，总分在" << nScoreLine << "行" << std::endl;
    std::cout << "是否要修改 Y/N ?" << std::endl;
    char tempChar;
    std::cin >> tempChar;
    if (tempChar == 'Y' || tempChar == 'y') {
        bChange = true;
    }
    if (bChange) {
        std::cout << "输入名字所在行" << std::endl;
        std::cin >> nNameLine;
        std::cout << "输入名字起始列(A～Z)" << std::endl;

        std::cin >> tempChar;
        nStarTag = tempChar - 'A' + 1;
        std::cout << "输入名字结束列(A～Z)" << std::endl;
        std::cin >> tempChar;
        nEndTag = tempChar - 'A' + 1;
        std::cout << "输入分数行" << std::endl;
        std::cin >> nScoreLine;
    }
    
    int nNum = nEndTag - nStarTag + 1;
    std::map<std::string, sLXQPersonData> mapData;

    for (unsigned int i = 0; i < files.size(); i++) {
        xlnt::workbook wb;
        wb.load(files[i].c_str());
        auto ws = wb.active_sheet();

        int tag = nStarTag;
        for (unsigned int j = tag; j <= nEndTag; j++) {
            std::string nameTemp = ws.cell(j,nNameLine).to_string();
            std::string name = UTF8ToANSI(nameTemp);
            int score = atoi(ws.cell(j, nScoreLine).to_string().c_str());

            sLXQParm parm;
            parm.name = name;
            parm.score = score;

            if (mapData.find(parm.name) == mapData.end()) {
                sLXQPersonData data;
                data.name = parm.name;
                mapData.insert(std::make_pair(parm.name,data));
            }
            
            mapData[parm.name].m_vecData.push_back(parm);
        }
    }
    std::cout << "一共" << files.size() << "份文档" << std::endl;
    std::vector<sLXQPersonData> erorVec;
    for (std::map<std::string, sLXQPersonData>::iterator iter = mapData.begin();
        iter != mapData.end();iter++) {
        iter->second.CalAvage();
        if (iter->second.m_vecData.size() != files.size()) {
            erorVec.push_back(iter->second);
        }
        std::cout << iter->first << " " << iter->second.nAverageScore << std::endl;
    }
    std::cout << "" << std::endl;
    std::cout << "可能有错别字的人有" << std::endl;
    for (unsigned int i = 0; i < erorVec.size(); i++) {
        std::cout << erorVec[i].name << "(" << erorVec[i].m_vecData.size() << "份)" << std::endl;
    }

    std::cout << "输入任意字符回车结束程序" << std::endl;
    std::cin >> tempChar;


}

int main()
{    
    LXQ();
    //CAnalysisToolsInst::singleton()->InitLoad("./read.xlsx");
    //testWrite();
    
    //std::cout << "Hello World!\n"; 
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
