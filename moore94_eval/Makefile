DOC = moore94_eval

TEX_FILES = $(wildcard *.tex)
AUX_FILES = $(TEX_FILES:%.tex=%.aux)

TMP_FILES = *.bbl *.blg *.brf *.idx *.ilg *.ind *.lof *.log *.lot *.out *.toc

GPSCRIPT = comparison_cubic.gp
GPDATA = comparison.cubic

PLOTS := compare_kod compare_mdflw compare_nod
PLOTS += compare_qalh compare_rbf compare_vud

PDF_PLOTS = $(PLOTS:%=%.pdf)
EPS_PLOTS = $(PLOTS:%=%.eps)

.PHONY: all clean clobber
.SECONDARY: $(EPS_PLOTS)

all: $(DOC).pdf

$(DOC).pdf: $(TEX_FILES) $(PDF_PLOTS)
	@pdflatex $(DOC)
	@bibtex $(DOC)
	@pdflatex $(DOC)
	@pdflatex $(DOC)

$(EPS_PLOTS): $(GPSCRIPT) $(GPDATA)
	@gnuplot < $(GPSCRIPT)

%.pdf: %.eps
	@epstopdf --autorotate=All $<

clean:
	@rm -f $(EPS_PLOTS) $(AUX_FILES) $(TMP_FILES)

clobber: clean
	@rm -f $(PDF_PLOTS) $(DOC).pdf
