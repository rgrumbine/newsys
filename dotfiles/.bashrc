# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# Uncomment the following line if you don't like systemctl's auto-paging feature:
# export SYSTEMD_PAGER=

# User specific aliases and functions
export PATH=~/bin/:${PATH}

##  Set the number of commands to be maintained in history within a session.
export HISTSIZE=1000
##  Set the number of commands to be maintained in history across logins.
export HISTFILESIZE=1000


