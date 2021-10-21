FROM ubuntu:18.04
ENV DEBIAN_FRONTEND=noninteractive
RUN export http_proxy=http://10.112.93.158:8123
RUN export https_proxy=http://10.112.93.158:8123
RUN mkdir amf
RUN cd amf
COPY ./amf /amf
RUN apt-get update -y && \
    apt-get install --no-install-recommends -y net-tools vim inetutils-ping git
RUN apt-get install -y update-notifier-common
RUN git config --global http.proxy http://10.112.93.158:8123
RUN git config --global https.proxy http://10.112.93.158:8123
RUN /bin/bash -c "if [[ -v NEEDED_GIT_PROXY ]]; then git config --global http.proxy $NEEDED_GIT_PROXY; fi"
RUN git config --global https.postBuffer 123289600
RUN git config --global http.sslverify false
RUN ./amf/build/scripts/build_amf -I -f
RUN ./amf/build/scripts/build_amf -c -b Debug -j
EXPOSE 8282 38412/sctp
CMD ["/bin/bash"]
