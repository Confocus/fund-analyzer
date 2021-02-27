# -*- coding: utf-8 -*-
import requests
import time
import execjs
import struct
import sys

class CManagerInfo:
    name = ""
    star = 0
    work_time = ""
    id = ""
    
    def __init__(self):
        return
    
    def clear(self):
        self.name = ""
        self.star = 0
        self.work_time = ""
        self.id = ""        

class CFundInfo:
    fS_name = ""
    fS_code = ""
    fund_sourceRate = 0.0   
    fund_Rate = 0.0         
    syl_1n = 0.0
    syl_6y = 0.0
    syl_3y = 0.0
    syl_1y = 0.0
    capital_scale = 0.0
    manager_info = CManagerInfo()
    
    def __init__(self):
        return
    
    def clear(self):
        self.fS_name = ""
        self.fS_code = ""     
        fund_sourceRate = 0.0
        fund_Rate = 0.0 
        
        return
    
global_FundInfo = CFundInfo()

def clear():
    global_FundInfo.clear()

def getTest():
    print("Hello python\n");
    

def getUrl(fscode):
    head = 'http://fund.eastmoney.com/pingzhongdata/'
    tail = '.js?v='+ time.strftime("%Y%m%d%H%M%S",time.localtime())
    return head+fscode+tail

def getWorth(fscode):
    content = requests.get(getUrl(fscode))
    jsContent = execjs.compile(content.text)
    name = jsContent.eval('fS_name')
    code = jsContent.eval('fS_code')
    netWorthTrend = jsContent.eval('Data_netWorthTrend')
    ACWorthTrend = jsContent.eval('Data_ACWorthTrend')
    netWorth = []
    ACWorth = []
    for dayWorth in netWorthTrend[::-1]:
        netWorth.append(dayWorth['y'])
    for dayACWorth in ACWorthTrend[::-1]:
        ACWorth.append(dayACWorth[1])
    print(name,code)
    return netWorth, ACWorth

"""
def getFundAllInfo(fS_code):
    content = requests.get(getUrl(fS_code))
    jsContent = execjs.compile(content.text)
    global_FundInfo.fS_code = fS_code
    global_FundInfo.fS_name = jsContent.eval('fS_name')
    global_FundInfo.fund_sourceRate = jsContent.eval('fund_sourceRate')
    global_FundInfo.fund_Rate = jsContent.eval('fund_Rate')
    
    return
"""

def getFundAllInfo(fS_code):
    content = requests.get(getUrl(fS_code))
    jsContent = execjs.compile(content.text)
    global_FundInfo.fS_code = fS_code
    global_FundInfo.fS_name = jsContent.eval('fS_name')
    #print(global_FundInfo.fS_name)
    #global_FundInfo.fS_name = global_FundInfo.fS_name.encode("GBK")
    #print(global_FundInfo.fS_name)
    global_FundInfo.fund_sourceRate = float(jsContent.eval('fund_sourceRate'))
    global_FundInfo.fund_Rate = float(jsContent.eval('fund_Rate'))
    global_FundInfo.syl_1n = float(jsContent.eval('syl_1n'))
    global_FundInfo.syl_6y = float(jsContent.eval('syl_6y'))
    global_FundInfo.syl_3y = float(jsContent.eval('syl_3y'))
    global_FundInfo.syl_1y = float(jsContent.eval('syl_1y'))
    
    capital_scale_dict = dict(jsContent.eval('Data_fluctuationScale'))
    global_FundInfo.capital_scale = capital_scale_dict['series'][-1]['y']
    
    szManager = list(jsContent.eval('Data_currentFundManager'))
    
    global_FundInfo.manager_info.id = szManager[0]['id']
    global_FundInfo.manager_info.name = szManager[0]['name']
    global_FundInfo.manager_info.work_time = szManager[0]['workTime']
    global_FundInfo.manager_info.star = szManager[0]['star']
    
    all_fund_info = (global_FundInfo.fS_name, global_FundInfo.fund_sourceRate, global_FundInfo.fund_Rate,
                     global_FundInfo.syl_1n, global_FundInfo.syl_6y, global_FundInfo.syl_3y, global_FundInfo.syl_1y,
                     global_FundInfo.capital_scale)
    return all_fund_info

def getFundName():
    return global_FundInfo.fS_name

def getFundSourceRate():
    return global_FundInfo.fund_sourceRate

def getFundRate():
    return global_FundInfo.fund_Rate
    
if __name__ == '__main__':
    getFundAllInfo('003511')