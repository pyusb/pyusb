#!/bin/sh

changelog_file=ChangeLog
additional_options=''

if [ "$1" != "" ]; then
    additional_options="--since=$1"
fi

git log $additional_options --pretty='format:Author: %an%n%w(0,4,4)%B' > $changelog_file
