#!/bin/bash -e

export LC_ALL=en_US.UTF-8
export LANG=en_US.UTF-8

if ! which twine; then
    echo "Please run 'sudo pip install twine'"
    exit 1
fi

version=$(python -c 'import usb; print (".".join(str(x) for x in usb.version_info))')

echo "Deploying version $version"

./gencl.sh
git tag -s -m "Version $version" v$version
python setup.py sdist
gpg --detach-sign -a dist/pyusb-$version.tar.gz
git push origin master
git push --tags origin
twine upload -s dist/pyusb-$version.tar.gz dist/pyusb-$version.tar.gz.asc
rm -rf build/ dist/
