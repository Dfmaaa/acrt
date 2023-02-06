# Why?
The developer of this project was too lazy to type their complicated password every time they wanted to execute privileged user operations.
# Isn't this unsafe?
That's why the group barrier was added. Only users in a certain group get read/write access to the named pipe. This prevents other users from using privileged user operations without a password.
# Do you realize this program creates a serious vulnerability in your system?
I do, yes. However, I need it badly. whenever I use sudo, I always get the password wrong 7-9 times. I didn't want to resort to this but I am left with no choice.
