#!/bin/sh

URL="https://github.com/marcandrysco/Errol"

rm -rf errol-upstream
git clone "$URL" errol-upstream
HASH=$(cd errol-upstream && git rev-parse HEAD)
mkdir -p errol
cp errol-upstream/lib/* errol/.
cp errol-upstream/LICENSE.txt errol/.

# Rename things to avoid any potential conflicts with
# users of json-c that also independently use errol.
sed -i \
	-e's/\([^_"]\)errol/\1json_c_errol/g' \
	-e's/^inline char/static inline char/' \
	-e's/^const char/static const char/' \
	-e's/^const struct hp_t/static const struct hp_t/' \
	errol/*.c errol/*.h

# Fix up a few things that the json-c build complains about
sed -i \
	-e's/^static void inline/inline static void/' \
	-e's/^static int inline/inline static int/' \
	-e's/#define LOOKUP_TABLE_LEN /#define LOOKUP_TABLE_LEN (int)/' \
	errol/*.c errol/*.h

cat > errol/README-errol-import.txt <<EOF
Imported from ${URL}/tree/${HASH}
See errol-import.sh
EOF

