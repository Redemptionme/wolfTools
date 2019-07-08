#pragma once
#include <string>
#include <vector>
#include <map>

const int ROUND_PlayNum = 12; // 玩家人数

enum eGameCard {  
    eGameCard_None = -1, 
    eGameCard_Langr,//狼人
    eGameCard_Lw,//狼王
    eGameCard_Lx,//狼兄
    eGameCard_Ld,//狼弟    
    eGameCard_Cm,// 村民
    eGameCard_Yyj,// 预言家
    eGameCard_Nv,//女巫
    eGameCard_Lr,//猎人
    eGameCard_Bc,//白痴    
    eGameCard_Sw,//守卫
    eGameCard_Hssr,//黑商商人    

    eGameCard_Count,
    // 目前看应该可以当总类身份卡用
    eGameCard_Good,
    eGameCard_Bad,
    eGameCard_All,
};

// 与excel统一
const std::string GameCardTagStrs[eGameCard_Count] = {
    "狼人"
    ,"狼王"
    ,"狼兄"
    ,"狼弟"
    ,"村民"
    ,"预言家"
    ,"女巫"
    ,"猎人"
    ,"白痴"
    ,"守卫"
    ,"黑商商人"
};

enum eGameCamp {
    eGameCamp_Bad, // 狼人阵营
    eGameCamp_Good,// 好人阵营
};

enum eProPData {
    eProPData_Round = 0, //局数
    eProPData_Num, //号数
    eProPData_Name, //名字
    eProPData_JobNum, //工号
    eProPData_GameCard, //身份
    eProPData_LuckGameCard, //幸运儿身份
    eProPData_DoDay1, //第一天
    eProPData_DoDay2, //第二天
    eProPData_DoDay3, //第三天
    eProPData_DoDay4, //第四天
    eProPData_DoDay5, //第五天
    eProPData_WinScore, //胜负分
    eProPData_exScore, //操作分
    eProPData_TotalScore, //总分

    eProPData_Count, //属性数量
};

// 与excel统一
const std::string NameTagStrs[eProPData_Count] = {
    "局数"
    ,"号数"
    ,"名字"
    ,"工号"
    ,"身份"
    ,"幸运儿身份"
    ,"第一天"
    ,"第二天"
    ,"第三天"
    ,"第四天"
    ,"第五天"
    ,"胜负分"
    ,"额外奖惩"
    ,"总分"
};


struct PlayerData
{
public:
    PlayerData() 
        : day(0)
        , round(0)
        , num(0)
        , name("")
        , jobNum(0)
        , gameCard(eGameCard_None)
        , luckGameCard(eGameCard_None)
       /* , doDay1("")
        , doDay2("")
        , doDay3("")
        , doDay4("")
        , doDay5("")*/
        , winScore(0)
        , exScore(0)
        , totalScore(0)
    {

    }

    void AnalysisData(int nday);

    int day;                    // 日期
    int round ;                 //局数
    int num;                    //号数
    std::string name;           //名字
    int jobNum;                 //工号
    eGameCard gameCard;       //身份
    eGameCard luckGameCard;   //幸运儿身份
    //std::string doDay1;         //第一天
    //std::string doDay2;         //第二天
    //std::string doDay3;         //第三天
    //std::string doDay4;         //第四天
    //std::string doDay5;         //第五天
    int winScore;               //胜负分
    int exScore;                //操作分
    int totalScore;             //总分

    std::string sdata[eProPData_Count];
};


struct RoundData {
public:
    std::vector <PlayerData> vecPlayerData;
    int day;
    int round;
};

struct WinRate
{
public:
    int winRound;
    int allRound;
    float rate;

    WinRate()
        :winRound(0)
        , allRound(0)
        , rate(0.0f)
    {
        
    }
    void Cal() {
        if (allRound != 0) {
            rate = float(winRound) / (float)allRound;
        }
    }
};

struct ScoreData {
public:
    ScoreData()
     :totalScore(0)
     ,averageScore(0.0f)
     ,totalRound(0)
    {

    }

    void Cal() {
        if (totalRound != 0) {
            averageScore = float(totalScore) / (float)totalRound;
        }
    }

    int totalScore;
    float averageScore;
    int totalRound;
};



struct PlayVecData {
public:
    PlayVecData()         
    {

    }
    std::vector<PlayerData> vecData;

    int getRounds(eGameCard eCard);

    void CalAllData();
    void CalWinRateData();
    void CalScoreData();

    std::map<eGameCard, WinRate> mapWinRate;
    std::map<eGameCard, ScoreData> mapScore;
    
};



