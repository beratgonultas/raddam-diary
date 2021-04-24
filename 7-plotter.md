Since we thought it (6-plotter-issue.md) may be an issue about incompability, I wrote İrem and asked for other files that she used. She sent us a folder called "Files". In future, its link may be available here. 

I tried some other silly things first, which didn't end up with a success. At the end, I tried replacing it with the "Files" folder in my original copy of HFRaddamAnalyzer. The main thing to run in these new "Files" are the runner.sh and run2.sh  
In them, each run number written in them are ran by plotter. So, they don't need to specify a run number to run. I typed ``./runner.sh`` and ``./run2.sh``  
They didn't work. Consistently I got an error saying "Permission denied"  
Idk why I got this error but I noticed another problem. In this .sh files, unlike analyze10TS.sh we don't have ``cmsRun`` line so that we don't generate the NTuples file (files like N_321025.root) for any of the run numbers. That cmsRun part is the essential part for extracting data, I think. What plotter does is plotting data using NTuples files. 
So, I tried running a single run number, of course 321585, by hand. 
1)I created /Plots/321585
2)Of course, don't forget cmsenv
3)Then I typed ``cmsRun hfRaddamAnalyzer_10TS_cfg.py 321585`` in "test" folder, where hfRaddamAnalyzer_10TS_cfg.py is.
4)Then I typed ``mv N_321585.root NTuples``   (This steps what we have in analyze10TS.sh)
5)Then I typed ``./plotter 321585`` 

You will hopefully get the correct result. Check it with İrem's thesis.   
To check by yourself that N_321585.root is necessary, try the procedure by omitting steps 3 and 4. You will get an error concerning NTuples.   
One way or another, code is running thanks to İrem Zöğ.

Note: We need to modify a little bit the ``runner.sh`` to create those NTuples files etc.
