#pragma once
#include "pch.h"
#include "third-party/xlnt/include/xlnt/xlnt.hpp"
#include <xlnt/worksheet/worksheet.hpp>
#include "CDataMgr.h"
#include "SingletonTemplate.h"

class CAnalysisTools
{
public:    
    CAnalysisTools();
    ~CAnalysisTools();

    void InitLoad(const char * path);
    void InitRightData();
    void AnalysisData();
    void AnalysisTag(xlnt::cell_vector row);
    void AnalysisRoundData(int starTag, int endTag,const xlnt::worksheet &sheet);
    eGameCard getGameCard(std::string str);

    void SaveExcel(const char* path,const char* excelName);
    void SaveWinRateExcel(xlnt::workbook &outWb,const char* pTitle);
    void SaveScoreExcel(xlnt::workbook &outWb,const char* pTitle);

public:
    void setLuckToDetailTotalScore(bool bVal);

private:
    xlnt::workbook m_wb;
    std::vector< xlnt::worksheet> m_completeSheet;
    int proDataTag[eProPData_Count];
    std::map<std::string, eGameCard> mapGameCardTags;
    bool m_bLuckToDetialScore;
};

typedef ChaSingleton<CAnalysisTools>  CAnalysisToolsInst;