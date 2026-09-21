#!/bin/bash
#
# Generate docs on the gh-pages branch in ~/json-c-pages
# Does NOT push changes
#

set -e -o pipefail

RUNDIR=$(dirname "$0")
. "${RUNDIR}/reltools.subr"

release="$1"
if [ -z "${release}" ] ; then
	echo "Usage: $0 <relver>" 1>&2
	exit 1
fi

cd "${HOME}"

if [ ! -d "json-c-${release}" ] ; then
	git clone https://github.com/json-c/json-c "json-c-${release}"
fi

cd json-c-${release}
git checkout json-c-${release}
cd ..

git clone -b gh-pages https://github.com/json-c/json-c json-c-pages
cd json-c-pages
mkdir json-c-${release}
cp -R ../json-c-${release}/doc json-c-${release}/.
git add json-c-${release}
rm json-c-current-release
ln -s json-c-${release} json-c-current-release
git commit -a -m "Add the ${release} docs."

vi index.html
# Add/change links to current release.

git commit -a -m "Update the doc links to point at ${release}"

cat <<EOF
cd $(pwd)
git log -n1 --name-status
git log -n1 -p


# If all looks ok:
git push
EOF
