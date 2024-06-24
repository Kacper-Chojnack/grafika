GCC = gcc
LIBS = -lpng -lm -largtable2
SOURCES = $(wildcard  *.c)
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = pixels2png
PNGDIR = Output
MAX = 1.50
MIN = -0.50
MED = $(shell echo "scale=2; (${MAX}+${MIN})*0.5" | bc )
Q1 = $(shell echo "scale=2; (${MAX}+${MIN})*0.25" | bc )
Q3 = $(shell echo "scale=2; (${MAX}+${MIN})*0.75" | bc )

all: $(SOURCES) $(EXECUTABLE) images


images:  $(patsubst   %.pixels, %.png,$(addprefix $(PNGDIR)/, $(wildcard   *.pixels)))

$(PNGDIR)/%.png: %x1600title.png gradient.png scale.png
	montage -tile 3x1 $< -label '' scale.png gradient.png -geometry +2+0 $@

gradient.png:
	convert -size 160x2272 gradient:white gradient.png

scale.png:
	convert -size 150x2272 -background white -fill black -interline-spacing 470 -gravity east  -pointsize 72  label:'$(MAX)\n$(Q3)\n$(MED)\n$(Q1)\n$(MIN)'  scale.png

%x1600title.png: %x1600.png
	montage -pointsize 72 -title 'test 2 results' -geometry +16+16 $< $@
	
%x1600.png: %.png
	convert -scale 1600% $< $@

%.png: %.pixels
	./$(EXECUTABLE) -i $< -o $@ -M $(MAX) -m $(MIN)

$(EXECUTABLE): $(OBJECTS) 
	$(GCC) $(OBJECTS) -o $@ $(LIBS)

clean:
	rm -f $(EXECUTABLE) $(OBJECTS) $(PNGDIR)/*.png *.png

