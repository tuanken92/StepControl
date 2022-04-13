#pragma once
#ifndef MTICKTIME_H
#define MTICKTIME_H

class MTickTimeAd
{
  public:
    MTickTimeAd();
    ~MTickTimeAd();
    //Funtion
    void StartTimer();
    bool MoreThan(unsigned long time);
    bool LessThan(unsigned long time);
    unsigned long m_ulPreTime ;
};

#endif
