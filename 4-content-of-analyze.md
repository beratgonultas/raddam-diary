analyze10TS.sh is the main code we run. It starts with some lines in the form of 

```bash
if [ ! -d Data ]
then
	mkdir Data
fi
```

These lines check if "Data" directory does "not exist". And if this is the case, i.e. if that directory does not exist, then it creates that directory. Basically first 30 lines of code set up the environment similar to this one.  

$1 in the code refers to the run number we provided. 
(Remember the command to run the analyze10TS.sh: ``./analyze10TS.sh RunNumber``)

After setting up the environment, we have:

```bash
cmsRun hfRaddamAnalyzer_10TS_cfg.py $1
```

This is the configuration file. (See: https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookConfigFileIntro)

I guess, this file create some .root files in the form 'N_'+runNumber+'.root'  (See: line 52 of the cfg file)

Then if we come back to analyze10TS.sh, we see this .root files are moved to NTuples directory. 

After that, plotter runs with the run number. (line 39 of analyze10TS.sh)

