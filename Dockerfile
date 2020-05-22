FROM pytorch/pytorch:1.5-cuda10.1-cudnn7-devel

#RUN conda install pyyaml=3.12.*

LABEL maintainer="Hari Subbaraj <hsubbaraj@berkeley.edu>"

#RUN apt-get update -qq

RUN apt-get update \
 && apt-get install -y --no-install-recommends \
        build-essential \
        curl \
        wget \
        software-properties-common \
        python3-pip \
# && add-apt-repository -y ppa:deadsnakes/ppa \
 && apt-get update \
# && apt-get install -y python3.6 python3.6-dev \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/*

RUN python3 -m pip install --no-cache-dir -U pip
RUN python3 -m pip install --no-cache-dir -U setuptools



RUN apt-get update -qq

RUN apt-get install -y libssl-dev libffi-dev

RUN mkdir -p /model \
      && apt-get update -qq \
      && apt-get install -y -qq libzmq5 libzmq5-dev redis-server libsodium-dev

RUN pip install cloudpickle==0.5.* pyopenssl pyzmq==17.0.* prometheus_client \
    jsonschema==2.6.* redis==2.10.* psutil==5.4.* flask==0.12.2 \
    numpy==1.14.* future pyyaml

COPY clipper_admin /clipper_admin/

RUN cd /clipper_admin \
                && pip install .

WORKDIR /container

COPY containers/python/__init__.py containers/python/rpc.py /container/

COPY monitoring/metrics_config.yaml /container/

ENV CLIPPER_MODEL_PATH=/model

HEALTHCHECK --interval=3s --timeout=3s --retries=1 CMD test -f /model_is_ready.check || exit 1

COPY containers/python/pytorch_container.py containers/python/container_entry.sh /container/

CMD ["/container/container_entry.sh", "pytorch-container", "/container/pytorch_container.py"]

# vim: set filetype=dockerfile:
