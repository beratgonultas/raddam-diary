### The Command for Make a Copy in Your Home Folder

The folder called HFRaddamAnalyzer is where the files are located. Its directory is `/afs/cern.ch/work/b/bilki/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/`
First thing you may want to do is copy it to your home folder `(~)`
(You might want to google this at this stage: `"Difference between root (/) and home directory (~) in terminal?")`
To copy it into your public folder, you should use the command below:
```bash
cp -r /afs/cern.ch/work/b/bilki/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/ ~/public
```

### Increasing Your Quota

However, the HFRaddamAnalyzer is 3.2 GB (at least it was so when I was typing these lines) whereas your home folder quota is 2.0 GB by default. (again, at least mine was so) Therefore, you can not copy it entirely to your home folder. If you try, it will only copy 2.0 GB of it. And when you login next time, it will tell you that your space is full and that might cause problems. And it says that you can increase your quota from cern.ch/account 
You should listen this advice and go to your account. Then you should get this page: https://resources.web.cern.ch/resources/Manage/AFS/Settings.aspx
Here you can increase your quota. 

Now you can copy the folder using the command above. When you give the command, it will say nothing on the terminal. Don't worry, it is processing. You can observe it from the link you increased your quota or you can use ``du -sh public/HFRaddamAnalyzer/`` command in a separate terminal after logging in LXPLUS again. This command shows you the size of the folder. In both ways, you will see an increase in the data amount. 

### "Write Failed Broken Pipe" Error

I have kicked out of LXPLUS before finishing the process in my first three attempts. Then I realized that it is the problem described here: https://thecustomizewindows.com/2017/06/fix-write-failed-broken-pipe-packet_write_wait-connection-port-22-broken-pipe/
So, I followed the described solution. It worked. (If your internet connection is faster, you may not encounter this problem.)

### Compiling the Folder

Now you have the folder. However you have to compile it. And to compile it, you will use `scramv1 b` command. Enter the folder (HFRaddamAnalyzer) and try the command. You will get an error. 

```bash
SCRAM fatal: Unable to locate the top of local release. Please run this command from a SCRAM-based area.
```

Yes, you need to run this command from a SCRAM-based area and it turns out that yours is not one of them. They are special folders whose name is in the form of  CMSSW_X_Y_Z , for example CMSSW_11_1_4

We need CMSSW_8_0_16 to make sure that paths look identical with the original folder. (Remember it was `/afs/cern.ch/work/b/bilki/public/CMSSW_8_0_16/src/HCALCommissioning2017/HFRaddamAnalyzer/`)

So, enter ~/public. Then you need to create your SCRAM-based area by following command: 

```bash
cmsrel CMSSW_8_0_16
```

It will give an alert but will do what you wanted. (hopefully) 
Alert looks like this: 

```bash
WARNING:  
Release CMSSW_8_0_16 is not available for architecture slc7_amd64_gcc820.  
         Developer's area is created for available architecture slc7_amd64_gcc530.  
WARNING: Developer's area is created for non-production architecture slc7_amd64_gcc530. Production architecture for this release is slc6_amd64_gcc530.
```

It is fine for now, because we really need this specific version. For example, 11_1_4 gave a long error. 

After that cmsrel command we have ~/public/CMSSW_8_0_16
Then we should obey the original path, and mkdir a folder called HCALCommissioning2017 in src folder. Then use mv or cp command to move or copy HFRaddamAnalyzer to that new folder. Now you can compile with ``scramv1 b`` in HFRaddamAnalyzer. 

Here is the log I got

>> Local Products Rules ..... started
>> Local Products Rules ..... done
>> Entering Package HCALCommissioning2017/HFRaddamAnalyzer
>> Creating project symlinks
  src/HCALCommissioning2017/HFRaddamAnalyzer/python -> python/HCALCommissioning2017/HFRaddamAnalyzer
>> Leaving Package HCALCommissioning2017/HFRaddamAnalyzer
>> Package HCALCommissioning2017/HFRaddamAnalyzer built
gmake[1]: Entering directory '/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16'
>> Local Products Rules ..... started
>> Local Products Rules ..... done
>> Creating project symlinks
  src/HCALCommissioning2017/HFRaddamAnalyzer/python -> python/HCALCommissioning2017/HFRaddamAnalyzer
>> Done python_symlink
>> Compiling python modules python
>> Compiling python modules src/HCALCommissioning2017/HFRaddamAnalyzer/python
>> All python modules compiled
>> Pluging of all type refreshed.
>> Done generating edm plugin poisoned information
gmake[1]: Leaving directory '/afs/cern.ch/user/b/bgonulta/public/CMSSW_8_0_16'
