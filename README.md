# ssh-and-lxplus
## Small tutorial on SSH and LXPLUS

### First Step: What is SSH? 

Just google it. 

### Second Step: Login onto LXPLUS

Type the following command to access LXPLUS via SSH.

> ssh username@lxplus.cern.ch

Then it will ask you to enter password. It is your CERN account password.  
Now you have access to remote server, LXPLUS.

(username is not your CERN email address. username is your CERN account.)  

### Third Step: Get files  

Open the local folder where you want to download the file from LXPLUS. Open the terminal by right clicking in that folder. (You may also use terminal commands to get that directory.)

Type the following to download the file into the folder you opened terminal in. (./ refers to current directory) 

> scp -r username@lxplus.cern.ch:/the_path_of_the_file/ ./

