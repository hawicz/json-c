#!/bin/bash

set -e

RUNDIR=$(dirname "$0")
RUNDIR=$(realpath "${RUNDIR}")

. "${RUNDIR}/reltools/reltools.subr"
# The 0.17 release is broken
#prev=0.18
#release=0.19

prev=$(get_most_recent_relver)
release=$(get_ver_descr)

ACCDIR=${RUNDIR}/../abi-compliance-checker
INSTBASE=${HOME}/json-c-installs
INSTREL=$(realpath --relative-to="${ACCDIR}" "${INSTBASE}")

if [ ! -d "${ACCDIR}" ]; then
	echo "Cloning abi-compliance-checker into ${ACCDIR}"
	git clone https://github.com/lvc/abi-compliance-checker "${ACCDIR}"
fi

if ! which perl > /dev/null ; then
	echo "perl install needed: apt-get install perl, yum install perl, etc..." 1>&2
	exit 1
fi

if [ "$1" != "--skip-build" ] ; then
	BUILD_DEST=${INSTBASE}/json-c-${release}
	echo "Building $(pwd) into ${BUILD_DEST}"
	mkdir -p build.abi-check
	cd build.abi-check
	CFLAGS=-Og cmake -DCMAKE_INSTALL_PREFIX="${BUILD_DEST}" ..
	make && make test && make install
fi

# Assume the old version has already been built
if [ ! -d "${INSTBASE}/json-c-${prev}" ] ; then
	echo "ERROR: ${INSTBASE}/json-c-${prev} does not exist" 1>&2
	exit 1
fi

mkxml()
{
	ver="$1"
	if [ ! -e "${INSTBASE}/json-c-${ver}/lib64" ] ; then
		ln -s lib "${INSTBASE}/json-c-${ver}/lib64"
	fi
cat <<EOF > json-c-${ver}.xml
<foo>
<version>
   ${ver}
</version>

<headers>
${INSTREL}/json-c-${ver}/include/json-c
</headers>

<libs>
${INSTREL}/json-c-${ver}/lib64/libjson-c.so
</libs>
</foo>
EOF
}

cd "${ACCDIR}"
mkxml ${release}
mkxml ${prev}

perl abi-compliance-checker.pl -lib json-c -dump json-c-${prev}.xml -dump-path ./ABI-${prev}.dump
perl abi-compliance-checker.pl -lib json-c -dump json-c-${release}.xml -dump-path ./ABI-${release}.dump
perl abi-compliance-checker.pl -l json-c -old ABI-${prev}.dump -new ABI-${release}.dump

echo "look in compat_reports/json-c/..."
