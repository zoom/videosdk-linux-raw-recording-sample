# Use the official Ubuntu image as the base image
FROM ubuntu:22.04

# Install necessary dependencies
RUN apt-get update && \
    apt-get install -y build-essential cmake

RUN apt-get install -y pkgconf

RUN apt-get install -y gtkmm-3.0



RUN apt-get update && apt-get install -y --no-install-recommends --no-install-suggests \
    libx11-xcb1 \
    libxcb-xfixes0 \
    libxcb-shape0 \
    libxcb-shm0 \
    libxcb-randr0 \
    libxcb-image0 \
    libxcb-keysyms1 \
    libxcb-xtest0 

# these are copied from msdk, not sure if needed

# RUN apt-get update && apt-get install -y --no-install-recommends --no-install-suggests \
    #libdbus-1-3 \
    #libglib2.0-0 \
    #libgbm1 \
    #libxfixes3 \
    #libgl1 \
    #libdrm2 \
    #libgssapi-krb5-2 \
    #openssl \
    #ca-certificates \
    #pkg-config \
    #libegl-mesa0 \
    #libsdl2-dev \
    #g++-multilib 

    # these are needed to start join the session, need to test it out
 RUN apt-get install -y libglib2.0-dev \
    liblzma-dev \
    libxcb-xkb1 \
    libgbm1 \
    libxtst6 \
    libgl1 \
    libnss3 \ 
    libasound2\ 
    libpulse0

RUN apt-get install -y pulseaudio

RUN apt-get install -y libcurl4-openssl-dev

# Set the working directory
WORKDIR /app

# Copy your application files to the container

COPY / /app/

# Execute additional commands
RUN rm -rf bin && rm -rf build  && cmake -B build && cd build && make

# Set the working directory to the binary folder
WORKDIR /app/bin


# Define a shell script to run multiple commands
RUN echo '#!/bin/bash' > /app/bin/run.sh \
    && echo '/app/src/setup-pulseaudio.sh' >> /app/bin/run.sh \
    && echo './zoom_v-sdk_linux_bot' >> /app/bin/run.sh

# Make the run script executable
RUN chmod +x /app/bin/run.sh

# Run the meetingSDKDemo binary when the container starts
CMD ["/app/bin/run.sh"]
#CMD ["/bin/bash"]
#CMD ["./zoom_v-sdk_linux_bot"]
