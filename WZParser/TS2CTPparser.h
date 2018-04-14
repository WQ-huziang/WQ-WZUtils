// Author : huziang
// modify copy parse

#ifndef CTPARSER_H_
#define CTPARSER_H_

#include <cstdio>
#include <cstring>
#include "tsdatastruct.h"
#include "ThostFtdcUserApiStruct.h"

inline void parseTo(const TSInputOrderField& req, struct CThostFtdcInputOrderField &rtn){
  switch (req.OrderType){
    case WZ_ORDER_MARKET_ORDER:
      rtn.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
      rtn.TimeCondition = THOST_FTDC_TC_IOC;
      rtn.VolumeCondition = THOST_FTDC_VC_AV;
      rtn.ContingentCondition = THOST_FTDC_CC_Immediately;
      break;
    case WZ_ORDER_LIMIT_ORDER:
      rtn.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
      rtn.TimeCondition = THOST_FTDC_TC_GFD;
      rtn.VolumeCondition = THOST_FTDC_VC_AV;
      rtn.ContingentCondition = THOST_FTDC_CC_Immediately;
      break;

    case WZ_ORDER_FOK_ORDER:
      rtn.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
      rtn.TimeCondition = THOST_FTDC_TC_IOC;
      rtn.VolumeCondition = THOST_FTDC_VC_CV;
      rtn.ContingentCondition = THOST_FTDC_CC_Immediately;
      break;

    case WZ_ORDER_FAK_ORDER:
      rtn.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
      rtn.TimeCondition = THOST_FTDC_TC_IOC;
      rtn.VolumeCondition = THOST_FTDC_VC_MV;
      rtn.ContingentCondition = THOST_FTDC_CC_Immediately;
      break;

    default :
      rtn.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
      rtn.TimeCondition = THOST_FTDC_TC_GFD;
      rtn.VolumeCondition = THOST_FTDC_VC_AV;
      rtn.ContingentCondition = THOST_FTDC_CC_Immediately;
      break;
  }
  strcpy(rtn.InstrumentID, req.InstrumentID);
  rtn.LimitPrice = req.LimitPrice;
  rtn.VolumeTotalOriginal = req.Volume;
  rtn.Direction = req.Direction;
  sprintf(rtn.CombOffsetFlag, "%c", req.OffsetFlag);
  rtn.MinVolume = 1;
  rtn.StopPrice = 0;
  rtn.CombHedgeFlag[0] = '1';
  rtn.IsAutoSuspend = 0;
  rtn.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
}

inline void parseTo(const TSOrderActionField& req, struct CThostFtdcInputOrderActionField &rtn){
  strcpy(rtn.InstrumentID, req.InstrumentID);
  strcpy(rtn.OrderRef, req.OrderRef);
  rtn.ActionFlag = req.ActionFlag;
  rtn.LimitPrice = req.LimitPrice;
  rtn.VolumeChange = req.VolumeChange;
}

inline void parseTo(const TSRtnOrderField& req, struct CThostFtdcOrderField &rtn){
  strcpy(rtn.InstrumentID, req.InstrumentID);
  strcpy(rtn.OrderRef, req.OrderRef);
  rtn.LimitPrice = req.LimitPrice;
  rtn.VolumeTraded = req.VolumeTraded;
  rtn.VolumeTotal = req.VolumeTotal;
  rtn.VolumeTotalOriginal = req.VolumeTotalOriginal;
  rtn.Direction = req.Direction;
  sprintf(rtn.CombOffsetFlag, "%c", req.OffsetFlag);
  rtn.OrderStatus = req.OrderStatus;
}


inline void parseTo(const CThostFtdcDepthMarketDataField &req, struct TSMarketDataField &rtn){
  strcpy(rtn.TradingDay, req.TradingDay);
  strcpy(rtn.InstrumentID, req.InstrumentID);
  rtn.LastPrice = req.LastPrice;
  rtn.PreSettlementPrice = req.PreSettlementPrice;
  rtn.PreClosePrice = req.PreClosePrice;
  rtn.PreOpenInterest = req.PreOpenInterest;
  rtn.OpenPrice = req.OpenPrice;
  rtn.HighestPrice = req.HighestPrice;
  rtn.LowestPrice = req.LowestPrice;
  rtn.Volume = req.Volume;
  rtn.Turnover = req.Turnover;
  rtn.OpenInterest = req.OpenInterest;
  // rtn.ClosePrice = req.ClosePrice;
  // rtn.SettlementPrice = req.SettlementPrice;
  rtn.UpperLimitPrice = req.UpperLimitPrice;
  rtn.LowerLimitPrice = req.LowerLimitPrice;
  strcpy(rtn.UpdateTime, req.UpdateTime);
  rtn.UpdateMillisec = req.UpdateMillisec;
  rtn.BidPrice1 = req.BidPrice1;
  rtn.BidVolume1 = req.BidVolume1;
  rtn.AskPrice1 = req.AskPrice1;
  rtn.AskVolume1 = req.AskVolume1;
}

#endif  // CTPARSER_H_
