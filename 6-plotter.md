At the moment, we noticed that the program we are working on is not the latest version with which the analysis of İrem Zog's thesis is done. So, this version lacks some amount of code, and it is not the best work we've seen regarding tidiness and readability. So, we are now waiting to receive the latest version of the program, hoping that it is easier to understand. 

We received the latest version of "plotter.C", not the whole directory. This version is much more longer (more than twice) than the old one. (Burak Bilki's version)

Also, it is much more clear as it has many comments. We appreciated it. 

Since we only have plotter.C, firstly we hoped to run it by replacing with the plotter.C in the old version. Berat tried it. The way and the result is as follows:

1. In your /public directory, near you already existing "CMSSW_8_0_16" folder, create a folder called "new" or "your favorite folder name"
2. Check if you have enough quota for a second HFRaddamAnalyzer (more than 3 GB) from https://resources.web.cern.ch/resources/Manage/AFS/Settings.aspx
3. If it is okay, copy "CMSSW_8_0_16" inside "new". (We will not go into already mentioned details like how to copy the folder etc. You may look for them in previous parts of this diary if you need. Or just "Google it hocam" as Burçin Ünlü said once upon a time.)
4. Find and delete the old plotter.C. Also delete "Plots" folder, because there were a issue about overwriting.
5. Copy the new plotter.C to lxplus. 
6. Run it with 321585 run number. (Irem's results of this run number are available in her master thesis.) 
7. Download pdfs and compare.

Certainly what Berat got was not the correct one, it was not even similar with what we have in Irem's thesis. However, it was an interesting and particular wrong result. He get exactly same thing with what he got by old code for 321025. 
