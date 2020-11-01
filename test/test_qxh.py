#!/usr/bin/env python3

import os
import subprocess

import pytest


@pytest.mark.parametrize(
    'testfile',
    [
        ('foo.txt', 'ZnjDGxQAAAAAAAAABAAAAAAAAAA=\n'),
        ('loremipsum.txt', 'ZNqmJyqS9l79QjW7eNx0qjaDpMY=\n'),
        ('binary.bin', 'GgQWCSPUD9bQ/3xxO0VcOoxc4ZM=\n'),
    ]
)
def test_qxh(testfile):
    basepath = os.path.dirname(os.path.realpath(__file__))
    qxh_bin = os.path.realpath(os.path.join(basepath, '..', 'quickxorhash'))

    res = subprocess.run(
        [qxh_bin, os.path.join(basepath, testfile[0])],
        capture_output=True,
        text=True,
    )
    assert res.stdout == testfile[1]
