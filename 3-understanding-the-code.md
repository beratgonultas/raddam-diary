### Comparing the Plots

Previously, I was able to run the code. Next step is to examine its inputs-outputs, what it involves and how it is connected to physics of the project.

There is the original path of the files:

```/afs/cern.ch/work/b/bilki/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/```

And also there is my personal copy in my area:

```~/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/```

So I have two copy of both PulseShapes.pdf and TDCShapes.pdf. I want to compare them since I ran the analyzeTS.sh one additional time for 321025 compared to the original one.

To detect where those pdfs are, I used ```find``` command.

It is used as ```find /dir/path/look/up -name "dir-name-here"```. So in my case, the command was ```find /afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/ -name "PulseShapes.pdf"``` for PulseShapes.pdf

The result is:

```bash
/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/290724/PulseShapes.pdf
/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/290727/PulseShapes.pdf
/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/289904/PulseShapes.pdf
/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/290720/PulseShapes.pdf
/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/291416/PulseShapes.pdf
/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/293304/PulseShapes.pdf
/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/295683/PulseShapes.pdf
/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/301006/PulseShapes.pdf
/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/298355/PulseShapes.pdf
/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/300719/PulseShapes.pdf
/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/300722/PulseShapes.pdf
/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/301007/PulseShapes.pdf
/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/304726/PulseShapes.pdf
/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/312918/PulseShapes.pdf
/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/312912/PulseShapes.pdf
/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/313023/PulseShapes.pdf
/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/313265/PulseShapes.pdf
/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/313526/PulseShapes.pdf
/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/313921/PulseShapes.pdf
/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/321025/PulseShapes.pdf
```

So it turned out to be not only one file. There are PulseShapes.pdf for each run number. Of course, I am interested in the last one, 321025. I dowloaded it by 

```bash
scp -r bgonulta@lxplus.cern.ch:/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/321025/PulseShapes.pdf ./
```

to my current directory. (This command works in terminal without logging in lxplus)

However, interestingly, 

```bash
scp -r bgonulta@lxplus.cern.ch:/afs/cern.ch/work/b/bilki/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/321025/PulseShapes.pdf ./
```

didn't work. The error is

```bash
scp: /afs/cern.ch/work/b/bilki/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/test/Plots/321025/PulseShapes.pdf: Not a directory
```

So, I opened that directory and saw that in Plots we don't have 321025 as a directory as well as some others. But there are some TDCShapes.pdf files with those names. That 321025 file must be what I removed for fixing an error at previous markdown file. 

So, we don't have any folders for 314920, 316800, 318682, 319729 and 321025 in the original directory. 
