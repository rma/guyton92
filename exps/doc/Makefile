DOC = experiments

TEX_FILES = $(wildcard *.tex)
AUX_FILES = $(TEX_FILES:%.tex=%.aux)

TMP_FILES = *.bbl *.blg *.brf *.idx *.ilg *.ind *.lof *.log *.lot *.out *.toc

all: $(DOC).pdf

$(DOC).pdf: $(TEX_FILES)
	@pdflatex $(DOC)
	@bibtex $(DOC)
	@pdflatex $(DOC)
	@pdflatex $(DOC)

clean:
	-@rm -f $(AUX_FILES) $(TMP_FILES)

clobber: clean
	-@rm -f $(DOC).pdf
