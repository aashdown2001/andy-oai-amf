
#!/bin/sh

proc_name="/opt/OpenXG/amf/build/amf/build/amf"       # 进程名
file_name="/opt/OpenXG/amf/stateless/stateless_amf.log"       # 日志文件


date >> /opt/OpenXG/amf/stateless/run_status.log
echo "the shell is running" >> /opt/OpenXG/amf/stateless/run_status.log
while :
do
stillRunning=$(ps -ef |grep $proc_name |grep -v "grep")
if [ -z "$stillRunning" ]
then
date >> /opt/OpenXG/amf/stateless/run_status.log
echo "the amf was closed!!!!!!!!!!!!!!!!!" >> /opt/OpenXG/amf/stateless/run_status.log
/opt/OpenXG/amf/build/amf/build/amf -c /opt/OpenXG/amf/etc/stateless.conf -o | tee /opt/OpenXG/amf/stateless/stateless_amf.log
date >> /opt/OpenXG/amf/stateless/run_status.log
echo "the amf was started!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" >> /opt/OpenXG/amf/stateless/run_status.log
fi
done
