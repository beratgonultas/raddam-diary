The folder called HFRaddamAnalyzer is where the files are located. Its directory is `/afs/cern.ch/work/b/bilki/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/`
First thing you may want to do is copy it to your home folder `(~)`
(You might want to google this at this stage: `"Difference between root (/) and home directory (~) in terminal?")`
To copy it into your public folder, you should use the command below:
```bash
cp -r /afs/cern.ch/work/b/bilki/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/ ~/public
```
However, the HFRaddamAnalyzer is 3.2 GB (at least it was so when I was typing these lines) whereas your home folder quota is 2.0 GB by default. (again, at least mine was so) Therefore, you can not copy it entirely to your home folder. If you try, it will only copy 2.0 GB of it. And when you login next time, it will tell you that your space is full and that might cause problems. And it says that you can increase your quota from cern.ch/account 
You should listen this advice and go to your account. Then you should get this page: https://resources.web.cern.ch/resources/Manage/AFS/Settings.aspx
Here you can increase your quota. 

Now you can copy the folder using the command above. When you give the command, it will say nothing on the terminal. Don't worry, it is processing. You can observe it from the link you increased your quota or you can use ``du -sh public/HFRaddamAnalyzer/`` command in a separate terminal after logging in LXPLUS again. This command shows you the size of the folder. In both ways, you will see an increase in the data amount. 

I have kicked out of LXPLUS before finishing the process in my first three attempts. Then I realized that it is the problem described here: https://thecustomizewindows.com/2017/06/fix-write-failed-broken-pipe-packet_write_wait-connection-port-22-broken-pipe/
So, I followed the described solution. It worked. (If your internet connection is faster, you may not encounter this problem.)
