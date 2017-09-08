
TEX_SOURCES=equation.tex
OUTPUT:=$(TEX_SOURCES:.tex=.jpg) det.png bhist.png hist.png  grange.spk.png grange.sys.png grange2.spk.png grange2.sys.png rover-sample.png
SCLITE_TO_CLEAN:=lvc\\_hypc.ctm.bhist.plt lvc\\_hypc.ctm.det.plt lvc\\_hypc.ctm.hist.plt \
		 lvc\\_hypc.ctm.bhist.dat1 lvc\\_hypc.ctm.det.dat.00 lvc\\_hypc.ctm.hist.dat \
		 det.tmp.plt bhist.tmp.plt hist.tmp.plt \
		                              \
		 faketest.grange2.spk.dat     \
		 faketest.grange2.spk.mean    \
		 faketest.grange2.spk.median  \
		 faketest.grange2.spk.plt     \
		 faketest.grange2.spk.tmp.plt \
		 faketest.grange2.sys.dat     \
		 faketest.grange2.sys.mean    \
		 faketest.grange2.sys.median  \
		 faketest.grange2.sys.plt     \
		 faketest.grange2.sys.tmp.plt \
		 faketest.grange.spk.dat      \
		 faketest.grange.spk.mean     \
		 faketest.grange.spk.median   \
		 faketest.grange.spk.plt      \
		 faketest.grange.spk.tmp.plt  \
		 faketest.grange.sys.dat      \
		 faketest.grange.sys.mean     \
		 faketest.grange.sys.median   \
		 faketest.grange.sys.plt      \
		 faketest.grange.sys.tmp.plt  \
		 sys1.sgml		      \
		 sys2.sgml		      \
		 sys3.sgml

TO_CLEAN:=$(TEX_SOURCES:.tex=.aux) $(TEX_SOURCES:.tex=.log) $(TEX_SOURCES:.tex=.pdf) $(SCLITE_TO_CLEAN)

all: $(OUTPUT)

install: $(OUTPUT)
	cp $(OUTPUT) ..

%.jpg: %.pdf
	convert -density 300 -seed 0 -strip $< $@

%.png: %.svg
	convert -seed 0 -strip "$<" "$@"

%.pdf: %.tex
	pdflatex $<

SCLITE:=../../bin/sclite
SCSTATS:=../../bin/sc_stats
LVCPATH:=../../src/sclite/testdata

grange.spk.png grange.sys.png grange2.spk.png grange2.sys.png: ref.stm sys1.ctm sys2.ctm sys3.ctm
	$(SCLITE) -r ref.stm stm -h sys1.ctm ctm -n sys1 -o sgml
	$(SCLITE) -r ref.stm stm -h sys2.ctm ctm -n sys2 -o sgml
	$(SCLITE) -r ref.stm stm -h sys3.ctm ctm -n sys3 -o sgml
	cat sys1.sgml sys2.sgml sys3.sgml | $(SCSTATS) -p -g grange -n faketest
	cat sys1.sgml sys2.sgml sys3.sgml | $(SCSTATS) -p -g grange2 -n faketest
	echo "set term pngcairo" > faketest.grange.spk.tmp.plt
	echo "set output \"grange.spk.png\"" >> faketest.grange.spk.tmp.plt
	gnuplot faketest.grange.spk.tmp.plt faketest.grange.spk.plt
	echo "set term pngcairo" > faketest.grange.sys.tmp.plt
	echo "set output \"grange.sys.png\"" >> faketest.grange.sys.tmp.plt
	gnuplot faketest.grange.sys.tmp.plt faketest.grange.sys.plt
	echo "set term pngcairo" > faketest.grange2.spk.tmp.plt
	echo "set output \"grange2.spk.png\"" >> faketest.grange2.spk.tmp.plt
	gnuplot faketest.grange2.spk.tmp.plt faketest.grange2.spk.plt
	echo "set term pngcairo" > faketest.grange2.sys.tmp.plt
	echo "set output \"grange2.sys.png\"" >> faketest.grange2.sys.tmp.plt
	gnuplot faketest.grange2.sys.tmp.plt faketest.grange2.sys.plt

%.png:
	$(SCLITE) -r $(LVCPATH)/lvc_refe.stm stm -h $(LVCPATH)/lvc_hypc.ctm ctm -C $(*) -n "lvc\\_hypc.ctm" -O .
	echo "set term pngcairo" > $(*).tmp.plt
	echo "set output \"$(*).png\"" >> $(*).tmp.plt
	gnuplot $(*).tmp.plt "lvc\\_hypc.ctm".$(*).plt

clean:
	$(RM) $(TO_CLEAN) $(OUTPUT)

.PHONY: clean
.DELETE_ON_ERROR:
