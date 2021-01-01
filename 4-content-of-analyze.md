analyze10TS.sh is the main code we run. It starts with some lines in the form of 

```bash
if [ ! -d Data ]
then
	mkdir Data
fi
```

These lines check if "Data" directory does "not exist". And if this is the case, i.e. if that directory does not exist, then it creates that directory. Basically first 30 lines of code set up the environment.  
