#include "pch.h"
#include "CData.h"
#include "CAnalysisTools.h"
#include <utility>


void PlayerData::AnalysisData(int nday)
{
    day = nday;
    round = atoi(sdata[eProPData_Round].c_str());
    num = atoi(sdata[eProPData_Num].c_str());
    name = sdata[eProPData_Name].c_str();
    jobNum = atoi(sdata[eProPData_JobNum].c_str());
    name = sdata[eProPData_Name].c_str();
    gameCard = CAnalysisToolsInst::singleton()->getGameCard(sdata[eProPData_GameCard].c_str());
    luckGameCard = CAnalysisToolsInst::singleton()->getGameCard(sdata[eProPData_LuckGameCard].c_str());
    // 5day 省略
    winScore = atoi(sdata[eProPData_WinScore].c_str());
    exScore = atoi(sdata[eProPData_exScore].c_str());
    totalScore = atoi(sdata[eProPData_TotalScore].c_str());
}

void PlayVecData::CalWinRateData()
{
    mapWinRate.clear();
    WinRate rate;
    mapWinRate.insert(std::make_pair(eGameCard_Good, rate));
    mapWinRate.insert(std::make_pair(eGameCard_Bad, rate));
    mapWinRate.insert(std::make_pair(eGameCard_All, rate));


    for (int i = 0; i < vecData.size(); i++) {
        const PlayerData &data = vecData[i];

        // 先不管幸运儿吧
        std::map<eGameCard, WinRate>::iterator iter = mapWinRate.find(data.gameCard);
        if (iter == mapWinRate.end()) {
            WinRate rate;
            mapWinRate.insert(std::make_pair(data.gameCard, rate));
        }
        WinRate &rate = mapWinRate[data.gameCard];
        rate.allRound++;
        bool bWin = false;
        if (data.winScore > 0) {
            bWin = true;
        }
        if (bWin) {
            rate.winRound++;
        }

        // 如果是狼
        if (data.gameCard < eGameCard_Cm ) {
            if (bWin) {
                mapWinRate[eGameCard_Bad].winRound++;
            }
            mapWinRate[eGameCard_Bad].allRound++;            
        } else if (data.gameCard < eGameCard_Count ) {            
            if (bWin) {
                mapWinRate[eGameCard_Good].winRound++;
            }
            mapWinRate[eGameCard_Good].allRound++;
        }

        mapWinRate[eGameCard_All].allRound++;
        if (bWin) {
            mapWinRate[eGameCard_All].winRound++;
        }
    }

    for (std::map<eGameCard, WinRate>::iterator iterCal = mapWinRate.begin();
        iterCal != mapWinRate.end();
        iterCal++) {
        iterCal->second.Cal();
    }
}

