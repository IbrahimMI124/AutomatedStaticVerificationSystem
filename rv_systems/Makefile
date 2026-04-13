SUBDIRS := \
	automotive_system \
	industrial_temp_system \
	control_system \
	medical_system \
	avionics_system \
	robotics_system

.PHONY: all clean run

all:
	@for d in $(SUBDIRS); do \
		$(MAKE) -C $$d all || exit $$?; \
	done

clean:
	@for d in $(SUBDIRS); do \
		$(MAKE) -C $$d clean || exit $$?; \
	done

run:
	@echo "Choose a system: make -C rv_systems/<dir> run"; \
	false
