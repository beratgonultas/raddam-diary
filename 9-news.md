Erhan Gülmez told us the latest run for HFRadDam. It is 336149 and taken on 15 July 2020. It can be found on Elog: https://cmsonline.cern.ch/webcenter/portal/cmsonline/pages_common/elog?__adfpwp_action_portlet=623564097&__adfpwp_backurl=https%3A%2F%2Fcmsonline.cern.ch%3A443%2Fwebcenter%2Fportal%2Fcmsonline%2Fpages_common%2Felog%3FMedia-Type%3Dscreen&_piref623564097.strutsAction=%2FviewSubcatMessages.do%3FsubId%3D8%26mode%3Dexpanded%26page%3D110

He expects us to analyze this run number. Here is a summary of his instructions:
1) Firstly, we might need to use a newer version of CMSSW, prefably the latest version.
2) We need to get Q1+Q2/QT graph. This is the graph from which İrem has deducted which TS to use. (See her thesis) However, I think, I and Erdem have never got that graph. So, we need to figure out how to get it.
3) After editing timing if needed, we should check cuts. To do this, we have to learn how İrem put cuts.

__Results:__

I noticed that Q1_Q2overQTotal is the name of TGraph in code for Q1+Q2/QT graph in 1_plotter. I also noticed some part of the code related to this is comment out. I removed comment out. I uploaded this version. Compiled it with make plotter. I run it with 321585. At the end, I didn't get any extra pdf's. Then I tried for 336149. The resulting pdf's are the same with 321585 -there was something wrong. The problem is the same with the problem described in 6-plotter-issue.md, but this time I stuck with 321585 not 321025. So, it seems we couldn't fix it. 

I thought maybe it is related my compiling. Maybe I do something wrong. I used precompiled 0_plotter without any change. The result was the same, I get 321585 instead of 336149. 

Then, I thought, maybe each plotter version is somehow compatible with only a group of run numbers. There is a file called RunList.csv in İrem's Files folder. That file also made me thought this. Therefore, I opened that file and tried for two of the run numbers in it. The result was the same for both of them. 
