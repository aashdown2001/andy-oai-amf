FROM ubuntu:18.04
ADD . /opt/5gc/amf
RUN apt-get update -y && \
    apt-get install --no-install-recommends -y net-tools vim inetutils-ping && \
    mkdir /opt/5gc/log
EXPOSE 8282 38412/sctp
CMD ["/opt/5gc/amf/start.sh"]
