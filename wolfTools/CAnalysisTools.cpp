#include "pch.h"
#include "CAnalysisTools.h"
#include "string"
#include "CTools.h"
#include <map>
#include <utility>
#include "CDataMgr.h"

CAnalysisTools::CAnalysisTools()
{
}

CAnalysisTools::~CAnalysisTools()
{
}

void CAnalysisTools::InitLoad(const char * path)
{
    m_wb.load(path);
    InitRightData();
    AnalysisData();
}

void CAnalysisTools::InitRightData()
{
    m_completeSheet.clear();
    CDataMgrInst::singleton()->clearData();

    for (auto ws : m_wb) {
        std::string title = ws.title();
        std::string str = UTF8ToANSI(title);

        // 以数字年作为标记
        std::string::size_type idx = str.find("2019");
        if (idx != std::string::npos)
        {
            std::string e = ws.cell(13, 2).to_string();
            if (!e.empty()) {
                m_completeSheet.push_back(ws);
            }
        }
    }
}

void CAnalysisTools::AnalysisData()
{
    for (auto sheet : m_completeSheet) {
        std::string title = sheet.title();
        std::string dayStr = UTF8ToANSI(title);

        int line = 0;
        std::vector<int> roundVec;        
        for (auto row : sheet.rows(false))
        {
            line++;
            bool bNextRow = false;
            xlnt::cell cell = (*row.begin());
            std::string firstColumn = UTF8ToANSI(cell.to_string());

            if (firstColumn != NameTagStrs[eProPData_Round]) {
                continue;
            }

            bool bFindTag = false;

            if (!bFindTag) {
                AnalysisTag(row);
                bFindTag = true;
            }

            // 说明没比晒数据
            xlnt::cell fpScore = sheet.cell(eProPData_TotalScore + 1, line + 1);
            if (fpScore.to_string().empty()) {
                continue;
            }
            // auto e3 =row.length();
            roundVec.push_back(line);
        }
        //把每天比赛进行分析
        for (unsigned int i = 0; i < roundVec.size(); i++) {
            AnalysisRoundData(roundVec[i] + 1, roundVec[i] + ROUND_PlayNum, sheet);
        }
    }
    CDataMgrInst::singleton()->calWinRate();
    CDataMgrInst::singleton()->print();
}

void CAnalysisTools::AnalysisTag(xlnt::cell_vector row)
{
    for (auto cell : row) {
        std::string content = cell.to_string();
        if (content.empty()) {
            continue;
        }
        std::string contentStr = UTF8ToANSI(content);     
        // 记住这些属性顺序        
        for (unsigned int i = eProPData_Round; i < eProPData_Count; i++) {
            if (contentStr == NameTagStrs[i]) {
                proDataTag[i] = cell.column_index();
            }
        }
    }

    mapGameCardTags.clear();
    mapGameCardTags.insert(std::make_pair("",eGameCard_None));
    for (unsigned int i = eGameCard_Langr; i < eGameCard_Count; i++) {
        mapGameCardTags[GameCardTagStrs[i]] = eGameCard(i);
    }
}

void CAnalysisTools::AnalysisRoundData(int starTag, int endTag, const xlnt::worksheet &sheet)
{
    int day = atoi(UTF8ToANSI(sheet.title()).c_str());
    std::string strRound = "";

    for (int i = starTag;i < endTag + 1; i++)
    {
        if (i == starTag) {
            strRound = UTF8ToANSI(sheet.cell(eProPData_Round + 1, i).to_string());
        }

        PlayerData playData;        

        for (int j = eProPData_Round; j < eProPData_Count; j++) {
            xlnt::cell cell = sheet.cell(j + 1, i);
            std::string content = UTF8ToANSI(cell.to_string());
            if (j == eProPData_Round) {
                content = strRound;
            }
            playData.sdata[j] = content;
        }
        playData.AnalysisData(day);

        CDataMgrInst::singleton()->addData(playData);
    }
}

eGameCard CAnalysisTools::getGameCard(std::string str)
{
    if (mapGameCardTags.find(str) != mapGameCardTags.end()) {
        return mapGameCardTags[str];
    }

    return eGameCard_None;
}
