### What is SSH and LXPLUS? 

Google them. 

A link to begin with:  
Beginners Guide to SSH [5:59] : https://www.youtube.com/watch?v=qWKK_PNHnnA

You may also want to consult the following link if you are not experienced in Linux:  
Linux for Ethical Hackers (Kali Linux Tutorial) [2:00:59] : https://www.youtube.com/watch?v=lZAoFs75_cs  (Skip to 30:02 if you don't want to watch installation and overview of Kali Linux part.)


### Login onto LXPLUS

Type the following command to access LXPLUS via SSH.

```bash
ssh username@lxplus.cern.ch
```

Then it will ask you to enter a password. It is your CERN account password.  
Now you have access to the remote server, LXPLUS.

(username is not your CERN email address. username is your CERN account name.)  

### Download a file from LXPLUS  

Open the local folder where you want to download the file from LXPLUS. Open the terminal by right clicking in that folder. (You may also use terminal commands to get that directory.)

Type the following to download the file into the folder you opened terminal in. (`./` refers to the current directory, you may change it as you wish) 

```bash
scp -r username@lxplus.cern.ch:/path_to_file/ ./
```

<!--__OR__ alternatively you may prefer doing in lxplus.
If you are in lxplus, type the following: -->

<!--```bash
scp -r path_to_file/ user@your_ip:path_to_download -->
```

<!--If you don't know what is you IP, then look here: https://itsfoss.com/check-ip-address-ubuntu/ -->

<!--Note: If you want to do upload change the order of the server and local.
Second Note: -r refers to directory. If you want to up/down-load only one file not a directory, then omit -r
-->
