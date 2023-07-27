#!/bin/bash

set -ex

ffmpeg -f v4l2 -framerate 25 -video_size 640x480 -i /dev/video0 -t 10 video.mp4
