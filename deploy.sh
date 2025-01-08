#!/bin/bash -e

export LC_ALL=en_US.UTF-8
export LANG=en_US.UTF-8

if ! which twine >/dev/null; then
    echo "Aborting: please install twine'"
    exit 1
fi

if [ $# -ne 1 ]; then
	echo "Aborting: missing version tag"
	echo "Usage: deploy.sh <version>"
	exit 1
fi

version=$1

read -p "Deploying version $version; do you want to continue (yN)?  " -n 1 -r
if [[ ! $REPLY =~ ^[Yy]$ ]]
then
	exit 1
fi

./gencl.sh

git tag -s -m "Version $version" v$version

python setup.py sdist bdist_wheel
gpg --detach-sign -a dist/pyusb-$version.tar.gz
gpg --detach-sign -a dist/pyusb-$version-py3-none-any.whl

read -p "Sdist and wheel ready; do you want to push the tag and upload the sdist and wheel (yN)?  " -n 1 -r
if [[ ! $REPLY =~ ^[Yy]$ ]]
then
	exit 1
fi

git push
git push --tags

twine upload -s dist/pyusb-$version{.tar.gz,-py3-none-any.whl}

rm -rf build/

b3sum dist/pyusb-$version* >dist/pyusb-$version.B3SUMS
