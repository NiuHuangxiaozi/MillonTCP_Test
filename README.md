# MILLION-TCP-TEST
The project is mainly to test whether a server can build one million connections simultaneously. The result shows that it is unfair to judge a server by only considering its concurrence.

### 1、Set up
1、Modify some configuration settings of your machine.

In client machine:

1.1

```
#vi /etc/sysctl.conf
net.ipv4.ip_local_port_range = 5000 65000
```

1.2

```
#vi /etc/sysctl.conf
fs.file-max=1100000 
fs.nr_open=50000 
```

1.3

```
# vi /etc/security/limits.conf
*  soft  nofile  55000  
*  hard  nofile  55000
```

In server machine:

1.1

```
#vi /etc/sysctl.conf
fs.file-max=1100000 
fs.nr_open=1100000  
```

1.2

```
# vi /etc/security/limits.conf
*  soft  nofile  1010000  
*  hard  nofile  1010000
```

[]: https://zhuanlan.zhihu.com/p/352748959	"you can click here  for more details"

2、You should assure that you machine is in a Local Area Network. And modify the multi_ip.sh to configure 50 IP addresses for your client. You can use "ifconfig -a" to see 50 ip.



### 2、Experiment

Compile and run two ".c" file. And you can see the result by using command:

```
watch "ss -t -a |grep ESTAB |wc -l"
```

