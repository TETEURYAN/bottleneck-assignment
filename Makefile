TARGET = bap_solver

SRC = main.cpp src/bap.cpp
HEADERS = src/bap.hpp

CXX = g++
CXXFLAGS = -Wall -O2 -std=c++17

INSTDIR = instances
OUTDIR = results

INPUTS = $(wildcard $(INSTDIR)/*.in)
OUTPUTS = $(patsubst $(INSTDIR)/%.in, $(OUTDIR)/%.out, $(INPUTS))

# Compilar
all: $(TARGET)

$(TARGET): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: all $(OUTPUTS)

$(OUTDIR)/%.out: $(INSTDIR)/%.in
	@mkdir -p $(OUTDIR)
	@echo "Executando $< → $@"
	@./$(TARGET) < $< > $@

clean:
	rm -f $(TARGET)
	rm -rf $(OUTDIR)
