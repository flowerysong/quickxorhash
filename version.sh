#!/bin/sh

if [ -d .git ]; then
    git describe --tags | perl -pe 'chomp; s/-/./; s/-.*//' | tee VERSION
elif [ -s VERSION ]; then
    cat VERSION
else
    echo -n UNKNOWN
fi
