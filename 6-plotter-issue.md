At the moment, we noticed that the program we are working on is not the latest version with which the analysis of İrem Zöğ's thesis is done. So, this version lacks some amount of code, and it is not the best work we've seen regarding tidiness and readability. So, we are now waiting to receive the latest version of the program, hoping that it is easier to understand.   
(Some days later)  
From Erhan Gülmez, we received the latest version of "plotter.C", not the whole directory. This version is much more longer (more than twice) than the old one. (Burak Bilki's version)

Also, it is much more clear as it has many comments. We appreciated it. 

Since we only have plotter.C, firstly we hoped to run it by replacing with the plotter.C in the old version. I tried it. The way and the result is as follows:

1. In your /public directory, near you already existing "CMSSW_8_0_16" folder, create a folder called "new" or "your favorite folder name"
2. Check if you have enough quota for a second HFRaddamAnalyzer (more than 3 GB) from https://resources.web.cern.ch/resources/Manage/AFS/Settings.aspx
3. If it is okay, copy "CMSSW_8_0_16" inside "new". (We will not go into already mentioned details like how to copy the folder etc. You may look for them in previous parts of this diary if you need. Or just "Google it hocam" as Burçin Ünlü said once upon a time.)
4. Find and delete the old plotter.C. Also delete "Plots" folder, because there were a issue about overwriting.
5. Copy the new plotter.C to lxplus. 
6. Run it with 321585 run number. (Irem's results of this run number are available in her master thesis.) 
7. Download pdfs and compare.

Certainly what I got was not the correct one, it was not even similar with what we have in İrem's thesis. However, it was an interesting and particular wrong result: I got exactly same thing with what I got by old code for 321025. 

A few days later, I noticed that I forgot re-compile the plotter.C previous time. So plotter, which is run by analyze10TS.sh, is of older version. I deleted it and recompiled plotter.C  
Then I ran the analyze10TS.sh with 321585, again. This time, I got not only PulseShapes.pdf and TDCShapes.pdf, but also many more like Q2.pdf  
This is something good, so we are now really running the new program. However, this pdfs are also all belong to run number 321025, not 321585.  
__Note__: Notice that that problem (getting 321025 while expecting 321585) seems unrelated with the new code, because I got that problem even before compiling the new plotter.

After this, I was curious about what if I try for some other run number. I tried for 304726. This time, there was no PulseShapes.pdf or TDCShapes.pdf, and the other plots were existing but all empty.  
On the other hand, Erdem tried 321585 on his own pc and he didn't also get PulseShapes.pdf and TDCShapes.pdf. He got other plots. 

Therefore, this was very strange problem. There was no pattern. We couldn't figure out why this was happening and we assumed it may be caused by an incompatibility between İrem's plotter and the other files in HFRaddamAnalyzer. However, due to Note above, I think it is much like a problem about 321585 and the old code. Idk.
