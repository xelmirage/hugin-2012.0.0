# Rules for generating the pano

all: $(REMAPPED_IMAGES)

clean: $(REMAPPED_IMAGES)
	$(RM) $(REMAPPED_IMAGES)

$(REMAPPED_IMAGES): $(INPUT_IMAGES)
	$(NONA) -o $(TARGET_PREFIX) $(PROJECT_FILE)

