PAPER = paper
BUILD = build
PDF_TEMPLATE = templates/arxiv-preprint.tex
HTML_CSS = styles/preprint.css

.PHONY: all pdf html clean

all: pdf html

$(BUILD):
	mkdir -p $(BUILD)

pdf: $(BUILD)
	pandoc $(PAPER).md \
		--from markdown+tex_math_dollars+implicit_figures \
		--citeproc \
		--number-sections \
		--template $(PDF_TEMPLATE) \
		--pdf-engine=xelatex \
		--output $(BUILD)/$(PAPER).pdf

html: $(BUILD)
	mkdir -p $(BUILD)/styles
	cp $(HTML_CSS) $(BUILD)/styles/preprint.css
	if [ -d figures ]; then cp -R figures $(BUILD)/; fi
	pandoc $(PAPER).md \
		--from markdown+tex_math_dollars+implicit_figures \
		--citeproc \
		--number-sections \
		--standalone \
		--mathml \
		--css styles/preprint.css \
		--metadata pagetitle="$(PAPER)" \
		--output $(BUILD)/$(PAPER).html

clean:
	rm -rf $(BUILD)
