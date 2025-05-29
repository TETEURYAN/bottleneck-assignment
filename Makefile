TARGET = bap_solver

SRC = main.cpp src/bap.cpp src/meta-heuristic/ils.cpp src/meta-heuristic/vnd.cpp 
HEADERS = src/bap.hpp src/meta-heuristic/ils.hpp src/meta-heuristic/vnd.hpp

CXX = g++
CXXFLAGS = -Wall -O2 -std=c++17

INSTDIR = instances
OUTDIR = results
SUBDIR = $(shell if [ "$(arg)" -eq 1 ]; then echo "bap"; elif [ "$(arg)" -eq 2 ]; then echo "ils"; elif [ "$(arg)" -eq 3 ]; then echo "vnd"; else echo "unknown"; fi)

INPUTS = $(wildcard $(INSTDIR)/*.in)
OUTPUTS = $(patsubst $(INSTDIR)/%.in, $(OUTDIR)/$(SUBDIR)/%.out, $(INPUTS))

# Compilar
all: $(TARGET)

$(TARGET): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: all $(OUTPUTS)
	@echo "Resultados salvos em $(OUTDIR)/$(SUBDIR)/"

$(OUTDIR)/$(SUBDIR)/%.out: $(INSTDIR)/%.in
	@mkdir -p $(OUTDIR)/$(SUBDIR)
	@echo "Executando $< → $@"
	@./$(TARGET) $(arg) < $< > $@  

clean:
	rm -f $(TARGET)
	rm -rf $(OUTDIR)

.PHONY: all run clean