#pragma once
#include <string>
#include <vector>
#include <map>

const int ROUND_PlayNum = 12; // �������

enum eGameCard {  
    eGameCard_None = -1, 
    eGameCard_Langr,//����
    eGameCard_Lw,//����
    eGameCard_Lx,//����
    eGameCard_Ld,//�ǵ�    
    eGameCard_Cm,// ����
    eGameCard_Yyj,// Ԥ�Լ�
    eGameCard_Nv,//Ů��
    eGameCard_Lr,//����
    eGameCard_Bc,//�׳�    
    eGameCard_Sw,//����
    eGameCard_Hssr,//��������    

    eGameCard_Count,
    // Ŀǰ��Ӧ�ÿ��Ե�������ݿ���
    eGameCard_Good,
    eGameCard_Bad,
    eGameCard_All,
};

// ��excelͳһ
const std::string GameCardTagStrs[eGameCard_Count] = {
    "����"
    ,"����"
    ,"����"
    ,"�ǵ�"
    ,"����"
    ,"Ԥ�Լ�"
    ,"Ů��"
    ,"����"
    ,"�׳�"
    ,"����"
    ,"��������"
};

enum eGameCamp {
    eGameCamp_Bad, // ������Ӫ
    eGameCamp_Good,// ������Ӫ
};

enum eProPData {
    eProPData_Round = 0, //����
    eProPData_Num, //����
    eProPData_Name, //����
    eProPData_JobNum, //����
    eProPData_GameCard, //���
    eProPData_LuckGameCard, //���˶����
    eProPData_DoDay1, //��һ��
    eProPData_DoDay2, //�ڶ���
    eProPData_DoDay3, //������
    eProPData_DoDay4, //������
    eProPData_DoDay5, //������
    eProPData_WinScore, //ʤ����
    eProPData_exScore, //������
    eProPData_TotalScore, //�ܷ�

    eProPData_Count, //��������
};

// ��excelͳһ
const std::string NameTagStrs[eProPData_Count] = {
    "����"
    ,"����"
    ,"����"
    ,"����"
    ,"���"
    ,"���˶����"
    ,"��һ��"
    ,"�ڶ���"
    ,"������"
    ,"������"
    ,"������"
    ,"ʤ����"
    ,"���⽱��"
    ,"�ܷ�"
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

    int day;                    // ����
    int round ;                 //����
    int num;                    //����
    std::string name;           //����
    int jobNum;                 //����
    eGameCard gameCard;       //���
    eGameCard luckGameCard;   //���˶����
    //std::string doDay1;         //��һ��
    //std::string doDay2;         //�ڶ���
    //std::string doDay3;         //������
    //std::string doDay4;         //������
    //std::string doDay5;         //������
    int winScore;               //ʤ����
    int exScore;                //������
    int totalScore;             //�ܷ�

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



