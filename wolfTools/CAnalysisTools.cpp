#include "pch.h"
#include "CAnalysisTools.h"
#include "string"
#include "CTools.h"
#include <map>
#include <utility>
#include "CDataMgr.h"

CAnalysisTools::CAnalysisTools()
    :m_bLuckToDetialScore(false)
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
            std::string e = ws.cell(14, 2).to_string();
            std::string eTemp = UTF8ToANSI(e);

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
    this->setLuckToDetailTotalScore(true);
    CDataMgrInst::singleton()->calAllData();
    CDataMgrInst::singleton()->print();
    this->SaveExcel("","OutRank.xlsx");
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

void CAnalysisTools::SaveExcel(const char* path, const char* excelName)
{    
    xlnt::workbook wbOut;
    
    this->SaveWinRateExcel(wbOut,"胜率数据");
    this->SaveScoreExcel(wbOut,"分数数据");
    this->SaveRankExcel(wbOut,"奖励排行版");

    wbOut.save(excelName);
}


void CAnalysisTools::SaveWinRateExcel(xlnt::workbook &outWb, const char* pTitle)
{
    xlnt::worksheet ws = outWb.active_sheet();
    ws.title("Sheet1");
    //xlnt::worksheet wsOut = outWb.create_sheet("Sheet1"); // copy a sheet before doing anything with "Sheet1"
    xlnt::worksheet wsOut = outWb.copy_sheet(ws);
    wsOut.title(ANSIToUTF8(pTitle));
    std::vector< std::vector<std::string> > wholeWorksheet;
    
    
    std::vector<std::string> firstRow;
    firstRow.push_back(ANSIToUTF8("工号"));
    firstRow.push_back(ANSIToUTF8("名字"));
    firstRow.push_back(ANSIToUTF8("总场数"));
    firstRow.push_back(ANSIToUTF8("总场数胜率"));
    firstRow.push_back(ANSIToUTF8("好人阵营场数"));
    firstRow.push_back(ANSIToUTF8("好人阵营胜率"));
    firstRow.push_back(ANSIToUTF8("狼人阵营场数"));
    firstRow.push_back(ANSIToUTF8("狼人阵营胜率"));

    for (int i = eGameCard_Langr; i < eGameCard_Count; i++ ) {
        std::string str = GameCardTagStrs[i] + "场数";
        std::string str2 = GameCardTagStrs[i] + "胜率";
        firstRow.push_back(ANSIToUTF8(str));
        firstRow.push_back(ANSIToUTF8(str2));
    }

    wholeWorksheet.push_back(firstRow);

    const std::map<int, PlayVecData> & mapPlayVecData = CDataMgrInst::singleton()->getPlayVecData();
    for (std::map<int, PlayVecData>::const_iterator iter = mapPlayVecData.begin();
        iter != mapPlayVecData.end();
        iter++) {
        std::vector<std::string> singleRow;
        const PlayVecData &vecDate = iter->second;
        singleRow.push_back(std::to_string(vecDate.vecData[0].jobNum));        
        singleRow.push_back(ANSIToUTF8(vecDate.vecData[0].name));


        std::map<eGameCard, WinRate>::const_iterator iterWin = vecDate.mapWinRate.find(eGameCard_All);        
        if (iterWin != vecDate.mapWinRate.end()) {
            singleRow.push_back(std::to_string(iterWin->second.allRound));
            singleRow.push_back(std::to_string(int(iterWin->second.rate * 100)) + "%");
        }
        iterWin = vecDate.mapWinRate.find(eGameCard_Good);
        if (iterWin != vecDate.mapWinRate.end()) {
            singleRow.push_back(std::to_string(iterWin->second.allRound));
            singleRow.push_back(std::to_string(int(iterWin->second.rate * 100)) + "%");
        }
        iterWin = vecDate.mapWinRate.find(eGameCard_Bad);
        if (iterWin != vecDate.mapWinRate.end()) {
            singleRow.push_back(std::to_string(iterWin->second.allRound));
            singleRow.push_back(std::to_string(int(iterWin->second.rate * 100)) + "%");
        }
        

        for (int i = eGameCard_Langr; i < eGameCard_Count; i++) {
            std::string str = "0";
            std::string str2 = "0%";
            iterWin = vecDate.mapWinRate.find((eGameCard)i);
            if (iterWin != vecDate.mapWinRate.end()) {
                str = std::to_string(iterWin->second.allRound);
                str2 = std::to_string(int(iterWin->second.rate * 100)) + "%";

            }

            singleRow.push_back(str);
            singleRow.push_back(str2);
        }

        wholeWorksheet.push_back(singleRow);
    }


   /* for (int outer = 0; outer < mapPlayVecData.size(); outer++)
    {    
        std::vector<std::string> singleRow;   
        for (int inner = 0; inner < 4; inner++)
        {
            std::string val = std::to_string(inner + 1);
            singleRow.push_back(val);
        }    
        wholeWorksheet.push_back(singleRow);        
    }*/


    for (int fOut = 0; fOut < wholeWorksheet.size(); fOut++)
    {        
        for (int fIn = 0; fIn < wholeWorksheet.at(fOut).size(); fIn++)
        { 
            wsOut.cell(xlnt::cell_reference(fIn + 1, fOut + 1)).value(wholeWorksheet.at(fOut).at(fIn));         
        }
    }
}

void CAnalysisTools::SaveScoreExcel(xlnt::workbook &outWb, const char* pTitle)
{
    xlnt::worksheet ws = outWb.active_sheet();
    ws.title("Sheet1");
    //xlnt::worksheet wsOut = outWb.create_sheet("Sheet1"); // copy a sheet before doing anything with "Sheet1"
    xlnt::worksheet wsOut = outWb.copy_sheet(ws);
    wsOut.title(ANSIToUTF8(pTitle));
    std::vector< std::vector<std::string> > wholeWorksheet;

        
    std::vector<std::string> firstRow;
    firstRow.push_back(ANSIToUTF8("工号"));
    firstRow.push_back(ANSIToUTF8("名字"));
    firstRow.push_back(ANSIToUTF8("总分"));
    firstRow.push_back(ANSIToUTF8("总场数"));
    firstRow.push_back(ANSIToUTF8("平均分"));
    firstRow.push_back(ANSIToUTF8("好人阵营总分"));
    firstRow.push_back(ANSIToUTF8("好人阵营总场数"));
    firstRow.push_back(ANSIToUTF8("好人阵营平均分"));
    firstRow.push_back(ANSIToUTF8("狼人阵营总分"));
    firstRow.push_back(ANSIToUTF8("狼人阵营总场数"));
    firstRow.push_back(ANSIToUTF8("狼人阵营平均分"));

    for (int i = eGameCard_Langr; i < eGameCard_Count; i++) {
        std::string str = GameCardTagStrs[i] + "总分";
        std::string str2 = GameCardTagStrs[i] + "总场数";
        std::string str3 = GameCardTagStrs[i] + "平均分";
        firstRow.push_back(ANSIToUTF8(str));
        firstRow.push_back(ANSIToUTF8(str2));
        firstRow.push_back(ANSIToUTF8(str3));
    }

    wholeWorksheet.push_back(firstRow);

    char tempStr[100];
    const std::map<int, PlayVecData> & mapPlayVecData = CDataMgrInst::singleton()->getPlayVecData();
    for (std::map<int, PlayVecData>::const_iterator iter = mapPlayVecData.begin();
        iter != mapPlayVecData.end();
        iter++) {
        std::vector<std::string> singleRow;
        const PlayVecData &vecDate = iter->second;
        singleRow.push_back(std::to_string(vecDate.vecData[0].jobNum));
        singleRow.push_back(ANSIToUTF8(vecDate.vecData[0].name));


        std::map<eGameCard, ScoreData>::const_iterator iterWin = vecDate.mapScore.find(eGameCard_All);
        if (iterWin != vecDate.mapScore.end()) {
            singleRow.push_back(std::to_string(iterWin->second.totalScore));
            singleRow.push_back(std::to_string(iterWin->second.totalRound));
            
            memset(tempStr, 0, sizeof(tempStr));
            sprintf_s(tempStr, sizeof(tempStr), "%.2f", iterWin->second.averageScore);
            singleRow.push_back(tempStr);
        }
        iterWin = vecDate.mapScore.find(eGameCard_Good);
        if (iterWin != vecDate.mapScore.end()) {
            singleRow.push_back(std::to_string(iterWin->second.totalScore));
            singleRow.push_back(std::to_string(iterWin->second.totalRound));
            memset(tempStr, 0, sizeof(tempStr));
            sprintf_s(tempStr, sizeof(tempStr), "%.2f", iterWin->second.averageScore);
            singleRow.push_back(tempStr);
        }
        iterWin = vecDate.mapScore.find(eGameCard_Bad);
        if (iterWin != vecDate.mapScore.end()) {
            singleRow.push_back(std::to_string(iterWin->second.totalScore));
            singleRow.push_back(std::to_string(iterWin->second.totalRound));
            memset(tempStr, 0, sizeof(tempStr));
            sprintf_s(tempStr, sizeof(tempStr), "%.2f", iterWin->second.averageScore);
            singleRow.push_back(tempStr);
        }


        for (int i = eGameCard_Langr; i < eGameCard_Count; i++) {
            std::string str = "0";
            std::string str2 = "0";
            std::string str3 = "0";
            iterWin = vecDate.mapScore.find((eGameCard)i);
            if (iterWin != vecDate.mapScore.end()) {
                str = std::to_string(iterWin->second.totalScore);
                str2 = std::to_string(iterWin->second.totalRound);
                memset(tempStr, 0, sizeof(tempStr));
                sprintf_s(tempStr, sizeof(tempStr), "%.2f", iterWin->second.averageScore);                
                str3 = tempStr;
            }

            singleRow.push_back(str);
            singleRow.push_back(str2);
            singleRow.push_back(str3);
        }

        wholeWorksheet.push_back(singleRow);
    }

    for (int fOut = 0; fOut < wholeWorksheet.size(); fOut++)
    {
        for (int fIn = 0; fIn < wholeWorksheet.at(fOut).size(); fIn++)
        {
            wsOut.cell(xlnt::cell_reference(fIn + 1, fOut + 1)).value(wholeWorksheet.at(fOut).at(fIn));
        }
    }
}

void CAnalysisTools::SaveRankExcel(xlnt::workbook &outWb, const char* pTitle)
{
    xlnt::worksheet ws = outWb.active_sheet();
    ws.title("Sheet1");
    //xlnt::worksheet wsOut = outWb.create_sheet("Sheet1"); // copy a sheet before doing anything with "Sheet1"
    xlnt::worksheet wsOut = outWb.copy_sheet(ws);
    wsOut.title(ANSIToUTF8(pTitle));
    std::vector< std::vector<std::string> > wholeWorksheet;

    const std::vector<std::string> nullTitleRow;
    this->SaveRankExcelAverageScore("          平均分排行榜          ", wholeWorksheet, eGameCard_All);
    wholeWorksheet.push_back(nullTitleRow);
    wholeWorksheet.push_back(nullTitleRow);
    this->SaveRankExcelTotalScore("          狼人阵营总分排行榜          ", wholeWorksheet, eGameCard_Bad);
    wholeWorksheet.push_back(nullTitleRow);
    wholeWorksheet.push_back(nullTitleRow);
    this->SaveRankExcelTotalScore("          村民总分排行榜          ", wholeWorksheet, eGameCard_Cm);
    wholeWorksheet.push_back(nullTitleRow);
    wholeWorksheet.push_back(nullTitleRow);
    this->SaveRankExcelTotalScore("          女巫总分排行榜          ", wholeWorksheet, eGameCard_Nv);
    wholeWorksheet.push_back(nullTitleRow);
    wholeWorksheet.push_back(nullTitleRow);
    this->SaveRankExcelTotalScore("          猎人总分排行榜          ", wholeWorksheet, eGameCard_Lr);

    for (int fOut = 0; fOut < wholeWorksheet.size(); fOut++)
    {
        for (int fIn = 0; fIn < wholeWorksheet.at(fOut).size(); fIn++)
        {
            wsOut.cell(xlnt::cell_reference(fIn + 1, fOut + 1)).value(wholeWorksheet.at(fOut).at(fIn));
        }
    }
}

void CAnalysisTools::SaveRankExcelAverageScore(const char* pTitle, std::vector< std::vector<std::string> > &wholeWorksheet, eGameCard eType)
{
    std::vector<std::string> titleRow;
    titleRow.push_back(ANSIToUTF8(pTitle));
    wholeWorksheet.push_back(titleRow);


    std::vector<std::string> firstRow;
    firstRow.push_back(ANSIToUTF8("排名"));
    firstRow.push_back(ANSIToUTF8("工号"));
    firstRow.push_back(ANSIToUTF8("名字"));
    firstRow.push_back(ANSIToUTF8(GameCardTagStrs[eType] + "总场数"));
    firstRow.push_back(ANSIToUTF8(GameCardTagStrs[eType] + "平均分"));

    wholeWorksheet.push_back(firstRow);

    std::vector< PlayVecData> rankData;
    const std::map<int, PlayVecData> & mapPlayVecData = CDataMgrInst::singleton()->getPlayVecData();
    for (std::map<int, PlayVecData>::const_iterator iter = mapPlayVecData.begin();
        iter != mapPlayVecData.end();
        iter++) {
        rankData.push_back(iter->second);
    }

    // 平均分
    std::sort(rankData.begin(), rankData.end(),
        [=](const PlayVecData &left, const PlayVecData &right)->bool {
        float leftScore = 0;
        float rightScore = 0;
        std::map<eGameCard, ScoreData>::const_iterator iterLeft = left.mapScore.find(eType);
        if (iterLeft != left.mapScore.end()) {
            leftScore = iterLeft->second.averageScore;
        }
        std::map<eGameCard, ScoreData>::const_iterator iterRight = right.mapScore.find(eType);
        if (iterRight != right.mapScore.end()) {
            rightScore = iterRight->second.averageScore;
        }

        return leftScore > rightScore;
    }
    );

    int rankNum = 0;
    float lastScore = -1;
    char tempStr[100];
    for (unsigned int i = 0; i < rankData.size(); i++) {
        std::vector<std::string> singleRow;
        const PlayVecData &vecDate = rankData[i];

        float averScore = 0;
        std::map<eGameCard, ScoreData>::const_iterator iterRank = vecDate.mapScore.find(eType);
        if (iterRank != vecDate.mapScore.end()) {
            averScore = iterRank->second.averageScore;
        }
        if (averScore != lastScore) {
            rankNum++;
            lastScore = averScore;
        }

        singleRow.push_back(std::to_string(rankNum));
        singleRow.push_back(std::to_string(vecDate.vecData[0].jobNum));
        singleRow.push_back(ANSIToUTF8(vecDate.vecData[0].name));

        std::map<eGameCard, ScoreData>::const_iterator iterWin = vecDate.mapScore.find(eType);
        if (iterWin != vecDate.mapScore.end()) {
            singleRow.push_back(std::to_string(iterWin->second.totalRound));

            memset(tempStr, 0, sizeof(tempStr));
            sprintf_s(tempStr, sizeof(tempStr), "%.2f", iterWin->second.averageScore);
            singleRow.push_back(tempStr);
        }
        else 
        {
            singleRow.push_back("0");
            singleRow.push_back("0");
        }


        wholeWorksheet.push_back(singleRow);
    }
}

void CAnalysisTools::SaveRankExcelTotalScore(const char* pTitle, std::vector< std::vector<std::string> > &wholeWorksheet, eGameCard eType)
{
    std::vector<std::string> titleRow;
    titleRow.push_back(ANSIToUTF8(pTitle));
    wholeWorksheet.push_back(titleRow);

    std::vector<std::string> firstRow;
    firstRow.push_back(ANSIToUTF8("排名"));
    firstRow.push_back(ANSIToUTF8("工号"));
    firstRow.push_back(ANSIToUTF8("名字"));
    firstRow.push_back(ANSIToUTF8(GameCardTagStrs[eType] +"总场数"));
    firstRow.push_back(ANSIToUTF8(GameCardTagStrs[eType] + "总分"));

    wholeWorksheet.push_back(firstRow);

    std::vector< PlayVecData> rankData;
    const std::map<int, PlayVecData> & mapPlayVecData = CDataMgrInst::singleton()->getPlayVecData();
    for (std::map<int, PlayVecData>::const_iterator iter = mapPlayVecData.begin();
        iter != mapPlayVecData.end();
        iter++) {
        rankData.push_back(iter->second);
    }
    // 狼人阵营总分
    std::sort(rankData.begin(), rankData.end(),
        [=](const PlayVecData &left, const PlayVecData &right)->bool {
        int leftScore = 0;
        int rightScore = 0;
        std::map<eGameCard, ScoreData>::const_iterator iterLeft = left.mapScore.find(eType);
        if (iterLeft != left.mapScore.end()) {
            leftScore = iterLeft->second.totalScore;
        }
        std::map<eGameCard, ScoreData>::const_iterator iterRight = right.mapScore.find(eType);
        if (iterRight != right.mapScore.end()) {
            rightScore = iterRight->second.totalScore;
        }

        return leftScore > rightScore;
    }
    );
    int rankNum = 0;
    int lastTotalScore = -1;
    for (unsigned int i = 0; i < rankData.size(); i++) {
        std::vector<std::string> singleRow;
        const PlayVecData &vecDate = rankData[i];

        int totoalScore = 0;
        std::map<eGameCard, ScoreData>::const_iterator iterRank = vecDate.mapScore.find(eType);
        if (iterRank != vecDate.mapScore.end()) {
            totoalScore = iterRank->second.totalScore;
        }
        if (totoalScore != lastTotalScore) {
            rankNum++;
            lastTotalScore = totoalScore;
        }

        singleRow.push_back(std::to_string(rankNum));
        singleRow.push_back(std::to_string(vecDate.vecData[0].jobNum));
        singleRow.push_back(ANSIToUTF8(vecDate.vecData[0].name));

        std::map<eGameCard, ScoreData>::const_iterator iterWin = vecDate.mapScore.find(eType);
        if (iterWin != vecDate.mapScore.end()) {
            singleRow.push_back(std::to_string(iterWin->second.totalRound));
            singleRow.push_back(std::to_string(iterWin->second.totalScore));
        }
        else
        {
            singleRow.push_back("0");
            singleRow.push_back("0");
        }

        wholeWorksheet.push_back(singleRow);
    }
}

void CAnalysisTools::setLuckToDetailTotalScore(bool bVal)
{
    m_bLuckToDetialScore = bVal;
}
