
#!/bin/sh

proc_name="/home/xgcore/Haiwen/amf/build/amf/build/amf"       # 进程名
file_name="/home/xgcore/Haiwen/amf/stateless/stateless_amf.log"       # 日志文件


date >> /home/xgcore/Haiwen/amf/stateless/run_status.log
echo "the shell is running" >> /home/xgcore/Haiwen/amf/stateless/run_status.log
#while :
#do
#stillRunning=$(ps -ef |grep $proc_name |grep -v "grep")
#if [ -z "$stillRunning" ]
#then
date >> /home/xgcore/Haiwen/amf/stateless/run_status.log
echo "the amf was closed!!!!!!!!!!!!!!!!!" >> /home/xgcore/Haiwen/amf/stateless/run_status.log
/home/xgcore/Haiwen/amf/build/amf/build/amf -c /home/xgcore/Haiwen/amf/etc/register+stateless.conf -o | tee /home/xgcore/Haiwen/amf/stateless/stateless_amf.log
date >> /home/xgcore/Haiwen/amf/stateless/run_status.log
echo "the amf was started!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" >> /home/xgcore/Haiwen/amf/stateless/run_status.log
#fi
#done
