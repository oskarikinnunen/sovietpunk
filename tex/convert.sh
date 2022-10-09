
srcfolder=input

for filename in $srcfolder/*; do
	bname=$( basename "$filename")
	convert $filename \
		-define png:compression-level=0 \
		-define png:compression-filter=0 \
		-define png:color-type=3 \
		-define png:bit-depth=8 \
		-colors 256 \
	output/$bname
done
